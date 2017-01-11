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
#include "com_console.h"
#include "com_interface.h"
#include "debris_emitter.h"
#include "game_state_manager.h"
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

#define CLEAN_UP_AND_EXIT_FAILURE { \
    if (pPhysicsManager != nullptr) \
    { \
        delete pPhysicsManager;\
        Log.log("Memory freed", "CPhysicsManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pPhysicsManager = nullptr; \
    } \
    if (pVisualsManager != nullptr) \
    { \
        delete pVisualsManager; \
        Log.log("Memory freed", "CVisualsManager", LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED); \
        pVisualsManager = nullptr; \
    } \
    return EXIT_FAILURE; \
}

#define CLEAN_UP_AND_EXIT_SUCCESS { \
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
    return EXIT_SUCCESS; \
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
    // 2. Initialise com interface
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

    //////////////////////////////////////////////////////////////////////////// 
    //
    // 3. Initialise major objects
    //
    ////////////////////////////////////////////////////////////////////////////
    CGraphics&          Graphics = CGraphics::getInstance();
    CCamera*            pCamera = nullptr;
    CComConsole         ComConsole;
    CGameStateManager   GameStateManager;
    CPhysicsManager*    pPhysicsManager = nullptr;
    CVisualsManager*    pVisualsManager = nullptr;
    CXFigLoader         XFigLoader;
    CUniverse           Universe;
    CVisualsDataStorage VisualsDataStorage;
    CWorldDataStorage   WorldDataStorage;
    CThruster           Thruster;
    CLuaManager         LuaManager;
    
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("CPhysicsManager")
    if (bGraphics)
    {
        pVisualsManager = new CVisualsManager;
        MEM_ALLOC("CVisualsManager")
    }
    
    /// Frequency of main program, polling input events
    const double PLANEWORLD_INPUT_FREQUENCY = 100.0;
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 4. Initialise storage access
    //
    ////////////////////////////////////////////////////////////////////////////
    GameStateManager.setWorldDataStorage(&WorldDataStorage);
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    if (bGraphics)
    {
        pVisualsManager->setComConsole(&ComConsole);
        pVisualsManager->setWorldDataStorage(&WorldDataStorage);
        pVisualsManager->setVisualsDataStorage(&VisualsDataStorage);
        pVisualsManager->initComInterface(&ComInterface, "visuals");
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
        if (!XMLImporter.import(strArgData)) CLEAN_UP_AND_EXIT_FAILURE;
//         WorldDataStorage.setCamera(XMLImporter.getCamera());
        
        LuaManager.setPhysicsInterface(XMLImporter.getPhysicsInterface());
        LuaManager.setFrequency(XMLImporter.getFrequencyLua());
        
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        pPhysicsManager->addComponents(XMLImporter.getComponents());
        pPhysicsManager->addEmitters(XMLImporter.getEmitters());
        pPhysicsManager->setFrequency(XMLImporter.getPhysicsFrequency());
        pPhysicsManager->setFrequencyDebris(XMLImporter.getFrequencyDebris());
        if (bGraphics)
        {
            pVisualsManager->setFrequency(XMLImporter.getVisualsFrequency());
            pVisualsManager->setFont(XMLImporter.getFont());
            pCamera=pVisualsManager->getCurrentCamera();
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
    pPhysicsManager->initComInterface(&ComInterface, "physics");
    
    #ifdef PW_MULTITHREADING    
        std::thread PhysicsThread(&CPhysicsManager::run, pPhysicsManager);
    #endif
    
    //////////////////////////////////////////////////////////////////////////// 
    //
    // 7. Start lua
    //
    ////////////////////////////////////////////////////////////////////////////
    LuaManager.initComInterface(&ComInterface, "lua");
    LuaManager.init();
    
    #ifdef PW_MULTITHREADING    
        std::thread LuaThread(&CLuaManager::run, &LuaManager);
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
            
        ComConsole.setComInterface(&ComInterface);
        
        //--------------------------------------------------------------------------
        // Initialize window and graphics
        //--------------------------------------------------------------------------
        WindowHandleType Window(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()), "Planeworld", sf::Style::Default);
        
        pVisualsManager->setWindow(&Window);
        pVisualsManager->initGraphics();
        
        bool bGraphicsOn = true;
        bool bMouseCursorVisible = false;
        bool bConsoleMode = false;
        std::string strConsoleCommand("");
        
        #ifdef PW_MULTITHREADING    
            std::thread VisualsThread(&CVisualsManager::run, pVisualsManager);
        #endif
        
        //////////////////////////////////////////////////////////////////////// 
        //
        // 8. Main loop
        //
        ////////////////////////////////////////////////////////////////////////
            
        //--- Prepare for querying relative mouse movement -----------------------//
        sf::Vector2i vecMouse;
        sf::Vector2i vecMouseCenter(sf::Vector2i(Window.getSize().x >> 1, Window.getSize().y >> 1));
        sf::Mouse::setPosition(vecMouseCenter,Window);
        
        while (!g_bDone)
        {
            #ifndef PW_MULTITHREADING
            if (nFrame++ % static_cast<int>(pPhysicsManager->getFrequency()*
                                            pPhysicsManager->getTimeAccel()/
                                            PLANEWORLD_INPUT_FREQUENCY) == 0)
            {
            #endif
                vecMouse = vecMouseCenter-sf::Mouse::getPosition(Window);
                vecMouse.x = -vecMouse.x; // Horizontal movements to the left should be negative
                if (!bMouseCursorVisible)
                    sf::Mouse::setPosition(vecMouseCenter,Window);

                //--- Handle events ---//
                sf::Event Event;
                while (Window.pollEvent(Event))
                {
                    switch (Event.type)
                    {
                        case sf::Event::Closed:
                        {
                            // End the program
                            ComInterface.call<void>("quit");
                            break;
                        }
                        case sf::Event::Resized:
                        {
                            // Adjust the viewport when the window is resized
                            vecMouseCenter = sf::Vector2i(Window.getSize().x >> 1, Window.getSize().y >> 1);
                            Graphics.resizeWindow(Event.size.width, Event.size.height);
                            pCamera->setViewport(Graphics.getViewPort().rightplane-Graphics.getViewPort().leftplane - 20.0,
                                                Graphics.getViewPort().topplane  -Graphics.getViewPort().bottomplane - 20.0);
                            break;
                        }
                        case sf::Event::KeyPressed:
                        {
                            if (Event.key.code == sf::Keyboard::Home)
                            {
                                strConsoleCommand = "";
                                bConsoleMode ^= true;
                                ComInterface.call<void>("toggle_console_mode");
                                INFO_MSG("Main","Console mode toggled")
                                break;
                            }
                            if (bConsoleMode)
                            {
                                switch (Event.key.code)
                                {
                                    case sf::Keyboard::BackSpace:
                                    {
                                        if (!strConsoleCommand.empty())
                                        {
                                            strConsoleCommand.pop_back();
                                        }
                                        ComConsole.setCurrentCommand(strConsoleCommand);
                                        break;
                                    }
                                    case sf::Keyboard::Home:
                                    {
                                        strConsoleCommand = "";
                                        bConsoleMode ^= true;
                                        ComInterface.call<void>("toggle_console_mode");
                                        INFO_MSG("Main","Console mode toggled")
                                        break;
                                    }
                                    case sf::Keyboard::Up:
                                    {
                                        ComConsole.prevCommand();
                                        strConsoleCommand = ComConsole.getCurrentCommand();
                                        break;
                                    }
                                    case sf::Keyboard::Down:
                                    {
                                        ComConsole.nextCommand();
                                        strConsoleCommand = ComConsole.getCurrentCommand();
                                        break;
                                    }
                                    case sf::Keyboard::Return:
                                    {
                                        strConsoleCommand = "";
                                        ComConsole.execute();
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                            else
                            {
                                switch (Event.key.code)
                                {
                                    case sf::Keyboard::Escape:
                                    {
                                        ComInterface.call<void>("quit");
                                        break;
                                    }
                                    case sf::Keyboard::Num0:
                                    {
                                        ComInterface.call("toggle_timers");
                                        break;
                                    }
                                    case sf::Keyboard::Num1:
                                    {
                                        ComInterface.call<void,int>("toggle_timer", 0);
                                        break;
                                    }
                                    case sf::Keyboard::Num2:
                                    {
                                        ComInterface.call<void,int>("toggle_timer", 1);
                                        break;
                                    }
                                    case sf::Keyboard::Num3:
                                    {
                                        ComInterface.call<void,int>("toggle_timer", 2);
                                        break;
                                    }
                                    case sf::Keyboard::Add:
                                    case sf::Keyboard::A:
                                    {
                                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                                            sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                                            ComInterface.call<void, bool>("accelerate_time", PHYSICS_ALLOW_STEP_SIZE_INC);
                                        else
                                            ComInterface.call<void, bool>("accelerate_time", PHYSICS_FORBID_STEP_SIZE_INC);
                                        break;
                                    }
                                    case sf::Keyboard::Subtract:
                                    case sf::Keyboard::Dash:
                                    case sf::Keyboard::D:
                                    {
                                        ComInterface.call<void>("decelerate_time");
                                        break;
                                    }
                                    case sf::Keyboard::Return:
                                    {
                                        ComInterface.call<void>("reset_time");
                                        break;
                                    }
                                    case sf::Keyboard::C:
                                    {
                                        ComInterface.call<void>("cycle_camera");
                                        pCamera=ComInterface.call<CCamera*>("get_current_camera");
                                        break;
                                    }
                                    case sf::Keyboard::B:
                                    {
                                        ComInterface.call<void>("toggle_bboxes");
                                        break;
                                    }
                                    case sf::Keyboard::G:
                                    {
                                        ComInterface.call<void>("toggle_grid");
                                        break;
                                    }
                                    case sf::Keyboard::K:
                                    {
                                        pVisualsManager->toggleVisualisations(VISUALS_KINEMATICS_STATES);
                                        break;
                                    }
                                    case sf::Keyboard::L:
                                    {
                                        /// \todo Really check if physics thread is paused before saving the simulation
                                        ComInterface.call<void>("pause");
                                        GameStateManager.load();
                                        pCamera=(*VisualsDataStorage.getCamerasByName().cbegin()).second;
                                        ComInterface.call<void>("toggle_pause");
                                        break;
                                    }
                                    case sf::Keyboard::N:
                                    {
                                        ComInterface.call<void>("toggle_names");
                                        break;
                                    }
                                    case sf::Keyboard::P:
                                    {
                                        ComInterface.call<void>("toggle_pause");
                                        break;
                                    }
                                    case sf::Keyboard::S:
                                    {
                                        /// \todo Really check if physics thread is paused before saving the simulation
                                        ComInterface.call<void>("pause");
                                        GameStateManager.save();
                                        ComInterface.call<void>("toggle_pause");
                                        break;
                                    }
                                    case sf::Keyboard::Space:
                                    {
                                        ComInterface.call<void>("process_one_frame");
                                        break;
                                    }
                                    case sf::Keyboard::T:
                                    {
                                        pVisualsManager->toggleVisualisations(VISUALS_OBJECT_TRAJECTORIES);
                                        break;
                                    }
                                    case sf::Keyboard::V:
                                    {
                                        bGraphicsOn ^= 1;
                                        bMouseCursorVisible ^= 1;
                                        Window.setMouseCursorVisible(bMouseCursorVisible);
                                        vecMouse.x=0;
                                        vecMouse.y=0;
                                        
                                        if (bGraphicsOn)
                                        {
                                            INFO_MSG("Main", "Graphics reactivated.")
                                        }
                                        else
                                        {
                                            INFO_MSG("Main", "Graphics deactivated, simulation still running...")
                                        }
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                            break;
                        }
                        case sf::Event::MouseMoved:
                        {
                            if (bGraphicsOn)
                            {
                                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                {
                                    double fZoom = ComInterface.call<double>("get_camera_zoom");
                                    ComInterface.call<void,double,double>("translate_camera_by",
                                                    0.2/GRAPHICS_PX_PER_METER*double(vecMouse.x)/fZoom,
                                                    0.2/GRAPHICS_PX_PER_METER*double(vecMouse.y)/fZoom);
                                }
                                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                                {
                                    ComInterface.call<void, double>("rotate_camera_by", -double(vecMouse.x)*0.001);
                                    ComInterface.call<void, double>("zoom_camera_by",1.0+double(vecMouse.y)*0.001);
                                    if (pCamera->getZoom() < 1.0e-18)
                                        ComInterface.call<void,double>("zoom_camera_to",1.0e-18);
                                    else if (pCamera->getZoom() > 1.0e3)
                                        ComInterface.call<void,double>("zoom_camera_to",1.0e3);
                                }
                                break;
                            }
                        }
                        case sf::Event::MouseWheelMoved:
                            if (bGraphicsOn)
                            {
                                ComInterface.call<void, double>("zoom_camera_by",1.0+double(Event.mouseWheel.delta)*0.1);
                                if (pCamera->getZoom() < 1.0e-18)
                                    ComInterface.call<void,double>("zoom_camera_to",1.0e-18);
                                else if (pCamera->getZoom() > 1.0e3)
                                    ComInterface.call<void,double>("zoom_camera_to",1.0e3);
                            }
                        case sf::Event::TextEntered:
                        {
                            if (bConsoleMode)
                            {
                                if (Event.text.unicode > 31 && Event.text.unicode < 127)
                                {
                                    strConsoleCommand += static_cast<char>(Event.text.unicode);
                                    ComConsole.setCurrentCommand(strConsoleCommand);
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                //--- Call Commands from com interface ---//
                ComInterface.callWriters("main");
                
            #ifndef PW_MULTITHREADING
            }
                //--- Run Physics ---//
                pPhysicsManager->processFrame();
                
                if (bGraphicsOn)
                {
                    if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                                  pPhysicsManager->getTimeAccel() /
                                                  pVisualsManager->getFrequency()) == 0)
                    {
                        pVisualsManager->processFrame();
                    }
                }
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              LuaManager.getFrequency()) == 0)
                {
                    LuaManager.processFrame();
                }
                pPhysicsManager->setTimeSlept(
                    Timer.sleepRemaining(pPhysicsManager->getFrequency() *
                                         pPhysicsManager->getTimeAccel()));
                if (nFrame % 1000u == 0u) nFrame = 0u;
            #else
                Timer.sleepRemaining(PLANEWORLD_INPUT_FREQUENCY);
            #endif
        }
        #ifdef PW_MULTITHREADING
            pVisualsManager->terminate();
            INFO_MSG("Main", "Visuals thread stopped.")
            VisualsThread.join();
        #endif
    }
    else // if (!bGraphics)
    {
        #ifndef PW_MULTITHREADING
        while (!g_bDone)
        {
            if (nFrame % static_cast<int>(pPhysicsManager->getFrequency() *
                                              pPhysicsManager->getTimeAccel() /
                                              LuaManager.getFrequency()) == 0)
            {
                LuaManager.processFrame();
            }
            
            //--- Run Physics ---//
            pPhysicsManager->processFrame();
            pPhysicsManager->setTimeSlept(
              Timer.sleepRemaining(pPhysicsManager->getFrequency() * 
                                   pPhysicsManager->getTimeAccel())
            );
        }
        #else
        Timer.start();
        while (!g_bDone)
        {
            Timer.sleepRemaining(0.1);
        }
        #endif
    }
      
    #ifdef PW_MULTITHREADING
        LuaManager.terminate();
        INFO_MSG("Main", "Lua thread stopped.")
        LuaThread.join();
        pPhysicsManager->terminate();
        INFO_MSG("Main", "Physics thread stopped.")
        PhysicsThread.join();
    #endif

    CLEAN_UP_AND_EXIT_SUCCESS;
}
