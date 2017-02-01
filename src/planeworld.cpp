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
#include "debris_emitter.h"
#include "game_state_manager.h"
#include "input_manager.h"
#include "lua_manager.h"
#include "physics_manager.h"
#include "objects_emitter.h"
#include "thruster.h"
#include "xfig_loader.h"
#include "xml_importer.h"
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
    if (pInputThread != nullptr) \
    { \
        delete pInputThread; \
        Log.log("Memory freed", "std::thread", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pInputThread = nullptr; \
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
    std::cout << "Usage: planeworld <OPTIONS> <UNIVERSE_DATA_FILE>" << std::endl;
    std::cout << "\nOptions: " << std::endl;
    std::cout << "--no graphics: Start simulation without graphical output." << std::endl;
    std::cout << "\nExamples: " << std::endl;
    std::cout << "planeworld path/to/scene.xml" << std::endl;
    std::cout << "planeworld --no-graphics path/to/scene.xml" << std::endl;
    
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
    CComConsole         ComConsole;
    CGameStateManager   GameStateManager;
    CXFigLoader         XFigLoader;
    CUniverse           Universe;
    CVisualsDataStorage VisualsDataStorage;
    CWorldDataStorage   WorldDataStorage;
    CThruster           Thruster;

    CInputManager*      pInputManager   = nullptr;
    CLuaManager*        pLuaManager     = nullptr;
    CPhysicsManager*    pPhysicsManager = nullptr;
    CVisualsManager*    pVisualsManager = nullptr;
    
    std::thread*        pInputThread = nullptr;
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
    
    /// Frequency of main program (writer commands) and input module
    const double PLANEWORLD_INPUT_FREQUENCY = 100.0;
    
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
    
    pLuaManager->initComInterface(&ComInterface, "lua");
    pPhysicsManager->initComInterface(&ComInterface, "physics");
    if (bGraphics)
    {
        ComConsole.setComInterface(&ComInterface);
        
        pInputManager->setComConsole(&ComConsole);
        pVisualsManager->setComConsole(&ComConsole);
        
        pInputManager->initComInterface(&ComInterface, "input");
        pVisualsManager->initComInterface(&ComInterface, "visuals");
        
    }
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 4. Initialise storage access
    //
    ////////////////////////////////////////////////////////////////////////////
    GameStateManager.setWorldDataStorage(&WorldDataStorage);
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    if (bGraphics)
    {
        pVisualsManager->setWorldDataStorage(&WorldDataStorage);
        pVisualsManager->setVisualsDataStorage(&VisualsDataStorage);
    }
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 5. Import simulation data
    //
    ////////////////////////////////////////////////////////////////////////////
    {
        // XML-Importer is initialised in its own scope to free memory after
        // objects are handed over to managers. However, this only applies for
        // the list structure of pointers.
        CXMLImporter        XMLImporter;
        
        XMLImporter.setWorldDataStorage(&WorldDataStorage);
        XMLImporter.setVisualsDataStorage(&VisualsDataStorage);
        if (!XMLImporter.import(strArgData)) {CLEAN_UP; return EXIT_FAILURE;}
        
        pLuaManager->setPhysicsInterface(XMLImporter.getPhysicsInterface());
        pLuaManager->setFrequency(XMLImporter.getFrequencyLua());
        
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        pPhysicsManager->addComponents(XMLImporter.getComponents());
        pPhysicsManager->addEmitters(XMLImporter.getEmitters());
        pPhysicsManager->setFrequency(XMLImporter.getPhysicsFrequency());
        pPhysicsManager->setFrequencyDebris(XMLImporter.getFrequencyDebris());
        if (bGraphics)
        {
            pVisualsManager->setFrequency(XMLImporter.getVisualsFrequency());
            pVisualsManager->setFont(XMLImporter.getFont());
        }
        Universe.clone(XMLImporter.getUniverse());
    }
    
    if (bGraphics)
    {
        pVisualsManager->setPhysicsManager(pPhysicsManager);
        pVisualsManager->setUniverse(&Universe);
    }
    pPhysicsManager->setUniverse(&Universe);

    //////////////////////////////////////////////////////////////////////////// 
    //
    // 6. Start physics
    //
    ////////////////////////////////////////////////////////////////////////////
    pPhysicsManager->initObjects();
    pPhysicsManager->initEmitters();
    pPhysicsManager->initComponents();
    
    #ifdef PW_MULTITHREADING    
        pPhysicsThread = new std::thread(&CPhysicsManager::run, pPhysicsManager);
        MEM_ALLOC("std::thread")
    #endif
        
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 7. Start lua
    //
    ////////////////////////////////////////////////////////////////////////////
    if (!pLuaManager->init()) {CLEAN_UP; return EXIT_FAILURE;}
    
    #ifdef PW_MULTITHREADING    
        pLuaThread = new std::thread(&CLuaManager::run, pLuaManager);
        MEM_ALLOC("std::thread")
    #endif
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 8. Start graphcis
    //
    ////////////////////////////////////////////////////////////////////////////
    #ifndef PW_MULTITHREADING
        auto nFrame = 0u;
    #endif

    CTimer Timer;
    Timer.start();
    if (bGraphics)
    {
        // X11 need special care when for threaded graphics
        #ifdef __linux__
            XInitThreads();
        #endif
            
        //--------------------------------------------------------------------------
        // Initialize window and graphics
        //--------------------------------------------------------------------------
        pWindow = new WindowHandleType(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()), "Planeworld", sf::Style::Default);
        
        pVisualsManager->setWindow(pWindow);
        pVisualsManager->initGraphics();
        
        #ifdef PW_MULTITHREADING    
            pVisualsThread = new std::thread(&CVisualsManager::run, pVisualsManager);
            MEM_ALLOC("std::thread")
        #endif
        
        //////////////////////////////////////////////////////////////////////// 
        //
        // 9. Start input
        //
        ////////////////////////////////////////////////////////////////////////
        pInputManager->setFrequency(100.0);
        pInputManager->setWindow(pWindow);
    
        #ifdef PW_MULTITHREADING    
            pInputThread = new std::thread(&CInputManager::run, pInputManager);
            MEM_ALLOC("std::thread")
        #endif

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
                Timer.sleepRemaining(PLANEWORLD_INPUT_FREQUENCY);
            #endif
                
            //--- Call Commands from com interface ---//
            ComInterface.callWriters("main");
        }
        #ifdef PW_MULTITHREADING
            pInputManager->terminate();
            pInputThread->join();
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
    #endif

     CLEAN_UP; return EXIT_SUCCESS;
}
