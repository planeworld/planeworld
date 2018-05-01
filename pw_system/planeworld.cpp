////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2018 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \mainpage   Project Planeworld
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       planeworld.cpp
/// \brief      main program
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <functional>
#include <thread>

//--- Program header ---------------------------------------------------------//
#include "log.h"

#include "conf_pw.h"
#include "circular_buffer.h"
#include "particle_emitter.h"
#include "game_state_manager.h"
#include "input_manager.h"
#include "lua_manager.h"
#include "physics_manager.h"
#include "objects_emitter.h"
#include "thruster.h"
#include "visuals_manager.h"

//--- Misc-Header ------------------------------------------------------------//

#ifdef __linux__
    #include "X11/Xlib.h"
#endif

#define CLEAN_UP { \
    if (pInputManager != nullptr) \
    { \
        delete pInputManager;\
        Log.log("Memory freed", "CInputManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pInputManager = nullptr; \
    } \
    if (pLuaManager != nullptr) \
    { \
        delete pLuaManager;\
        Log.log("Memory freed", "CLuaManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pLuaManager = nullptr; \
    } \
    if (pPhysicsManager != nullptr) \
    { \
        delete pPhysicsManager; \
        Log.log("Memory freed", "CPhysicsManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pPhysicsManager = nullptr; \
    } \
    if (pVisualsManager != nullptr) \
    { \
        delete pVisualsManager; \
        Log.log("Memory freed", "CVisualsManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pVisualsManager = nullptr; \
    } \
    if (pLuaThread != nullptr) \
    { \
        delete pLuaThread; \
        Log.log("Memory freed", "std::thread", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pLuaThread = nullptr; \
    } \
    if (pPhysicsThread != nullptr) \
    { \
        delete pPhysicsThread; \
        Log.log("Memory freed", "std::thread", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pPhysicsThread = nullptr; \
    } \
    if (pVisualsThread != nullptr) \
    { \
        delete pVisualsThread; \
        Log.log("Memory freed", "std::thread", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pVisualsThread = nullptr; \
    } \
    if (pWindow != nullptr) \
    { \
        delete pWindow; \
        Log.log("Memory freed", "WindowHandleType", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pWindow = nullptr; \
    } \
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
////////////////////////////////////////////////////////////////////////////////
void usage()
{
    METHOD_ENTRY("usage")
    std::cout << "Usage: planeworld <LUA_FILE>" << std::endl;
    std::cout << "\nExample: " << std::endl;
    std::cout << "planeworld path/to/scene.lua" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief main function
///
/// This is the entrance point for program startup.
///
/// \param  argc number of given arguments
/// \param  argv array, storing the arguments
/// \return exit code
///
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    #ifdef WIN32
        Log.setColourScheme(LOG_COLOUR_SCHEME_DEFAULT);
    #else
        Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    #endif
        
    INFO_MSG("Planeworld", "Version " << PW_VERSION_FULL)
        
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 1. Check for given arguments
    //
    //////////////////////////////////////////////////////////////////////////// 
    bool bGraphics = false;
    std::string strArgData("");
        
    if (argc == 2)
    {
        strArgData = argv[1];
    }
    else
    {
        usage();
        return EXIT_FAILURE;
    }
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 2. Initialise major objects
    //
    ////////////////////////////////////////////////////////////////////////////
    CGraphics&          Graphics = CGraphics::getInstance();
    CGameStateManager   GameStateManager;
    CVisualsDataStorage VisualsDataStorage;
    CWorldDataStorage   WorldDataStorage;

    CInputManager*      pInputManager   = nullptr;
    CLuaManager*        pLuaManager     = nullptr;
    CPhysicsManager*    pPhysicsManager = nullptr;
    CVisualsManager*    pVisualsManager = nullptr;
    
    std::thread*        pLuaThread = nullptr;
    std::thread*        pPhysicsThread = nullptr;
    std::thread*        pVisualsThread = nullptr;
        
    WindowHandleType*   pWindow = nullptr;
    
    pInputManager   = new CInputManager;
    pLuaManager     = new CLuaManager;
    pPhysicsManager = new CPhysicsManager;
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("CInputManager")
    MEM_ALLOC("CLuaManager")
    MEM_ALLOC("CPhysicsManager")
    MEM_ALLOC("CVisualsManager")
    
    bool bExit = false; ///< Exit simulation
    bool bExitError = false; ///< Exit with error
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 3. Initialise com interface
    //
    ////////////////////////////////////////////////////////////////////////////
    CComInterface ComInterface;
    
    ComInterface.registerWriterDomain("main");
    
    ComInterface.registerFunction("exit",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = false;
                                    }),
                                    "Exit processing, clean up and end simulation. Same as <quit>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("exit_success",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = false;
                                    }),
                                    "Exit processing without error, clean up and end simulation. Same as <exit>/<quit>/<quit_success>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("exit_error",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = true;
                                    }),
                                    "Exit processing with error, clean up and end simulation. Same as <quit_error>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("quit",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = false;
                                    }),
                                    "Quit processing, clean up and end simulation. Same as <exit>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    
    ComInterface.registerFunction("quit_success",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = false;
                                    }),
                                    "Quit processing without error, clean up and end simulation. Same as <quit>/<exit>/<exit_success>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("quit_error",
                                    CCommand<void>([&]()
                                    {
                                        bExit = true;
                                        bExitError = true;
                                    }),
                                    "Quit processing with error, clean up and end simulation. Same as <exit_error>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("init_physics",
                                    CCommand<void>([&]()
                                    {
                                        #ifdef PW_MULTITHREADING    
                                            pPhysicsThread = new std::thread(&CPhysicsManager::run, pPhysicsManager);
                                            MEM_ALLOC("std::thread")
                                        #endif
                                    }
                                    ),
                                    "Initialise physics module, start thread if in multithreading mode.",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system"
    );
    ComInterface.registerFunction("init_visuals",
                                    CCommand<void>([&]()
                                    {
                                        //--------------------------------------------------------------------------
                                        // Initialize window and graphics
                                        //--------------------------------------------------------------------------
                                        
                                        // X11 need special care when for threaded graphics
                                        #ifdef __linux__
                                            XInitThreads();
                                        #endif
                                            
                                        // Window initialisation has to be done before lua scripting starts, since
                                        // GL-based window functions rely on an existing window.
                                        pWindow = new WindowHandleType(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),"Planeworld", sf::Style::Default,
                                                                    sf::ContextSettings(24,8,4,4,2,sf::ContextSettings::Core)
                                                                    );
                                        MEM_ALLOC("WindowHandleType")
                                        
                                        pInputManager->setWindow(pWindow);
                                        pVisualsManager->setWindow(pWindow);
                                        
                                        pVisualsManager->init();
                                        #ifdef PW_MULTITHREADING
                                            pWindow->setActive(false);
                                            pVisualsThread = new std::thread(&CVisualsManager::run, pVisualsManager);
                                            MEM_ALLOC("std::thread")
                                        #endif
                                        bGraphics = true;
                                    }
                                    ),
                                    "Initialise visuals module, start thread if in multithreading mode.",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system"
    );
    pInputManager->initComInterface(&ComInterface, "input");
    pLuaManager->initComInterface(&ComInterface, "lua");
    pPhysicsManager->initComInterface(&ComInterface, "physics");
    pVisualsManager->initComInterface(&ComInterface, "visuals");
    GameStateManager.initComInterface(&ComInterface, "gamestate");
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 4. Prepare engine managers
    //
    ////////////////////////////////////////////////////////////////////////////
    GameStateManager.setWorldDataStorage(&WorldDataStorage);
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    pVisualsManager->setWorldDataStorage(&WorldDataStorage);
    pVisualsManager->setVisualsDataStorage(&VisualsDataStorage);
    
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 5. Initialise Lua
    //
    ////////////////////////////////////////////////////////////////////////////
    pLuaManager->setScript(strArgData);
    if (!pLuaManager->init())
    {
        CLEAN_UP; return EXIT_FAILURE;
    }
    #ifdef PW_MULTITHREADING    
        pLuaThread = new std::thread(&CLuaManager::run, pLuaManager);
        MEM_ALLOC("std::thread")
    #endif
    
    #ifndef PW_MULTITHREADING
        std::uint64_t nFrame = 0u;
    #endif

    CTimer Timer;
    Timer.start();
    if (bGraphics)
    {
        
        //////////////////////////////////////////////////////////////////////// 
        //
        // 6. Start input
        //
        ////////////////////////////////////////////////////////////////////////
        while (!bExit)
        {
            #ifndef PW_MULTITHREADING
                //--- Run Physics ---//
                pPhysicsManager->processFrame();
                
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              pInputManager->getFrequency()) == 0)
                {
                    pInputManager->processFrame();
                }
                
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              pVisualsManager->getFrequency()) == 0)
                {
                    pVisualsManager->processFrame();
                }
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              pLuaManager->getFrequency()) == 0)
                {
                    pLuaManager->processFrame();
                }
                pPhysicsManager->setTimeSlept(
                    Timer.sleepRemaining(pPhysicsManager->getFrequency() *
                                         pPhysicsManager->getTimeAccel()));
            #else
                pInputManager->processFrame();
                Timer.sleepRemaining(pInputManager->getFrequency());
            #endif
                
            //--- Call Commands from com interface ---//
            ComInterface.callWriters("main");
            ComInterface.callWriters("gamestate");
        }
        #ifdef PW_MULTITHREADING
            pVisualsManager->terminate();
            pVisualsThread->join();
        #endif
    }
    else // if (!bGraphics)
    {
        #ifndef PW_MULTITHREADING
            while (!bExit)
            {
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              pLuaManager->getFrequency()) == 0)
                {
                    pLuaManager->processFrame();
                }
                
                //--- Run Physics ---//
                pPhysicsManager->processFrame();
                pPhysicsManager->setTimeSlept(
                Timer.sleepRemaining(pPhysicsManager->getFrequency() * 
                                     pPhysicsManager->getTimeAccel())
                );
            }
        #endif
    }
    
      
    #ifdef PW_MULTITHREADING
        pLuaManager->terminate();
        pLuaThread->join();
        pPhysicsManager->terminate();
        pPhysicsThread->join();
        DOM_STATS(DEBUG_MSG("main", "Spinlock waits: " << CSpinlock::getWaits()))
        DOM_STATS(DEBUG_MSG("main", "Spinlock yields: " << CSpinlock::getYields()))
        DOM_STATS(DEBUG_MSG("main", "Spinlock sleeps: " << CSpinlock::getSleeps()*0.5 << " ms"))
    #endif

    CLEAN_UP;
    
    if (bExitError)
    {
        ERROR_MSG("Planeworld", "Exiting with failure")
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}
