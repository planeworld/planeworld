////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2017 Torsten Büschenfeld
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

//--- Global variables -------------------------------------------------------//
bool g_bDone = false;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
////////////////////////////////////////////////////////////////////////////////
void usage()
{
    METHOD_ENTRY("usage")
    std::cout << "Usage: planeworld <OPTIONS> <LUA_FILE>" << std::endl;
    std::cout << "\nOptions: " << std::endl;
    std::cout << "--no graphics: Start simulation without graphical output." << std::endl;
    std::cout << "\nExamples: " << std::endl;
    std::cout << "planeworld path/to/scene.lua" << std::endl;
    std::cout << "planeworld --no-graphics path/to/scene.lua" << std::endl;
    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Quit program
///
////////////////////////////////////////////////////////////////////////////////
void quit()
{
    METHOD_ENTRY("quit")
    g_bDone = true;
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
        
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 1. Check for given arguments
    //
    //////////////////////////////////////////////////////////////////////////// 
    bool bGraphics = true;
    std::string strArgOptions("");
    std::string strArgData("");
        
    if (argc < 2 || argc > 3)
    {
        usage();
        return EXIT_FAILURE;
    }
    if (argc == 3)
    {
        strArgOptions = argv[1];
        strArgData    = argv[2];
        if (strArgOptions == "--no-graphics")
        {
            bGraphics = false;
            INFO_MSG("Main", "Starting simulation without graphical interface.")
        }
        else
        {
            WARNING_MSG("Main", "Unknown option " << strArgOptions << ", ignoring.")
        }
    }
    else
    {
        strArgData = argv[1];
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
    
    pLuaManager     = new CLuaManager;
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("CLuaManager")
    MEM_ALLOC("CPhysicsManager")
    if (bGraphics)
    {
        pInputManager   = new CInputManager;
        pVisualsManager = new CVisualsManager;
        MEM_ALLOC("CInputManager")
        MEM_ALLOC("CVisualsManager")
    }
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 3. Initialise com interface
    //
    ////////////////////////////////////////////////////////////////////////////
    CComInterface ComInterface;
    
    ComInterface.registerWriterDomain("main");
    
    ComInterface.registerFunction("exit",
                                    CCommand<void>(quit),
                                    "Exit processing, clean up and end simulation. Same as <quit>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("quit",
                                    CCommand<void>(quit),
                                    "Quit processing, clean up and end simulation. Same as <exit>",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "main"
    );
    ComInterface.registerFunction("init_physics",
                                    CCommand<void>([&]()
                                    {
                                        GameStateManager.setWorldDataStorage(&WorldDataStorage);
                                        pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
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
                                        pVisualsManager->setWorldDataStorage(&WorldDataStorage);
                                        pVisualsManager->setVisualsDataStorage(&VisualsDataStorage);
                                        
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
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 4. Prepare engine managers
    //
    ////////////////////////////////////////////////////////////////////////////
    
    
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
        auto nFrame = 0u;
    #endif

    CTimer Timer;
    Timer.start();
    if (bGraphics)
    {
        
        //////////////////////////////////////////////////////////////////////// 
        //
        // 9. Start input
        //
        ////////////////////////////////////////////////////////////////////////
        

        while (!g_bDone)
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
                if (++nFrame == 1000u) nFrame = 0u;
            #else
                pInputManager->processFrame();
                Timer.sleepRemaining(pInputManager->getFrequency());
            #endif
                
            //--- Call Commands from com interface ---//
            ComInterface.callWriters("main");
        }
        #ifdef PW_MULTITHREADING
            pVisualsManager->terminate();
            pVisualsThread->join();
        #endif
    }
    else // if (!bGraphics)
    {
        #ifndef PW_MULTITHREADING
            while (!g_bDone)
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
                if (++nFrame == 1000u) nFrame = 0u;
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
        DOM_STATS(DEBUG_MSG("main", "Spinlock sleeps: " << CSpinlock::getSleeps() << " * 500us"))
    #endif

    CLEAN_UP; return EXIT_SUCCESS;
}
