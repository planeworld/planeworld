////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
#include "physics_manager.h"
#include "objects_emitter.h"
#include "thruster.h"
#include "xfig_loader.h"
#include "xml_importer.h"
#include "visuals_manager.h"

//--- Misc-Header ------------------------------------------------------------//

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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << "Usage: planeworld <OPTIONS> <UNIVERSE_DATA_FILE>" << std::endl;
    std::cout << "\nOptions: " << std::endl;
    std::cout << "--no graphics: Start simulation without graphical output." << std::endl;
    std::cout << "\nExamples: " << std::endl;
    std::cout << "planeworld path/to/scene.xml" << std::endl;
    std::cout << "planeworld --no-graphics path/to/scene.xml" << std::endl;
    
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
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
  
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

    bool                bDone = false;
    CTimer              Timer;

    //--- Major instances ----------------------------------------------------//
    CGraphics&          Graphics = CGraphics::getInstance();
    CCamera*            pCamera = nullptr;
    CGameStateManager   GameStateManager;
    CPhysicsManager*    pPhysicsManager = nullptr;
    CVisualsManager*    pVisualsManager = nullptr;
    CXFigLoader         XFigLoader;
    CUniverse           Universe;
    CVisualsDataStorage VisualsDataStorage;
    CWorldDataStorage   WorldDataStorage;
    CThruster           Thruster;
    
    //--- Initialisation -----------------------------------------------------//
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("CPhysicsManager")
    if (bGraphics)
    {
        pVisualsManager = new CVisualsManager;
        MEM_ALLOC("CVisualsManager")
    }
    
    //--- Initialize storage access for engine managers ----------------------//
    GameStateManager.setWorldDataStorage(&WorldDataStorage);
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    if (bGraphics)
    {
        pVisualsManager->setWorldDataStorage(&WorldDataStorage);
        pVisualsManager->setVisualsDataStorage(&VisualsDataStorage);
    }
    
    //--- Import from xml file ----------------------------------------------//
    {
        // XML-Importer is initialised in its own scope to free memory after
        // objects are handed over to managers. However, this only applies for
        // the list structure of pointers.
        CXMLImporter        XMLImporter;
        
        XMLImporter.setWorldDataStorage(&WorldDataStorage);
        XMLImporter.setVisualsDataStorage(&VisualsDataStorage);
        if (!XMLImporter.import(strArgData)) CLEAN_UP_AND_EXIT_FAILURE;
//         WorldDataStorage.setCamera(XMLImporter.getCamera());
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        pPhysicsManager->addComponents(XMLImporter.getComponents());
        pPhysicsManager->addEmitters(XMLImporter.getEmitters());
        pPhysicsManager->setPhysicsInterface(XMLImporter.getPhysicsInterface());
        pPhysicsManager->setFrequency(XMLImporter.getPhysicsFrequency());
        pPhysicsManager->setFrequencyDebris(XMLImporter.getFrequencyDebris());
        pPhysicsManager->setFrequencyLua(XMLImporter.getFrequencyLua());
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

    //--- Initialise physics -------------------------------------------------//
    pPhysicsManager->initObjects();
    pPhysicsManager->initEmitters();
    pPhysicsManager->initComponents();
    if (!pPhysicsManager->initLua()) CLEAN_UP_AND_EXIT_FAILURE;
    
    #ifdef PW_MULTITHREADING    
        std::thread PhysicsThread(&CPhysicsManager::run, pPhysicsManager);
    #endif
    
    //--- Initialise graphics ------------------------------------------------//
    WindowHandleType* pWindow = nullptr;

    if (bGraphics)
    {
        pVisualsManager->initGraphics();
        pWindow=pVisualsManager->getWindowHandle();
        
        bool bGraphicsOn = true;
        bool bMouseCursorVisible = false;
        
        //--- Prepare for querying relative mouse movement -----------------------//
        sf::Vector2i vecMouse;
        sf::Vector2i vecMouseCenter(sf::Vector2i(pWindow->getSize().x >> 1, pWindow->getSize().y >> 1));
        sf::Mouse::setPosition(vecMouseCenter,*pWindow);
        
        //--- Run the main loop --------------------------------------------------//
        #ifndef PW_MULTITHREADING
            auto nFrame = 0u;
        #endif
        Timer.start();
        while (!bDone)
        {
            #ifndef PW_MULTITHREADING
            if (nFrame++ % static_cast<int>(pPhysicsManager->getFrequency()/
                                        pVisualsManager->getFrequency()) == 0)
            {
            #endif
                vecMouse = vecMouseCenter-sf::Mouse::getPosition(*pWindow);
                vecMouse.x = -vecMouse.x; // Horizontal movements to the left should be negative
                if (!bMouseCursorVisible)
                    sf::Mouse::setPosition(vecMouseCenter,*pWindow);
                
                //--- Handle events ---//
                sf::Event Event;
                while (pWindow->pollEvent(Event))
                {
                    switch (Event.type)
                    {
                        case sf::Event::Closed:
                        {
                            // End the program
                            bDone = true;
                            break;
                        }
                        case sf::Event::Resized:
                        {
                            // Adjust the viewport when the window is resized
                            vecMouseCenter = sf::Vector2i(pWindow->getSize().x >> 1, pWindow->getSize().y >> 1);
                            Graphics.resizeWindow(Event.size.width, Event.size.height);
                            pCamera->setViewport(Graphics.getViewPort().right-Graphics.getViewPort().left - 20.0,
                                                Graphics.getViewPort().top  -Graphics.getViewPort().bottom - 20.0);
                            break;
                        }
                        case sf::Event::KeyPressed:
                        {
                            switch (Event.key.code)
                            {
                                case sf::Keyboard::Escape:
                                {
                                    bDone = true;
                                    break;
                                }
                                case sf::Keyboard::Num0:
                                {
                                    pVisualsManager->toggleVisualisations(VISUALS_TIMERS);
                                    break;
                                }
                                case sf::Keyboard::Num1:
                                {
                                    pPhysicsManager->getSimTimerLocal()[0].toggle();
                                    break;
                                }
                                case sf::Keyboard::Num2:
                                {
                                    pPhysicsManager->getSimTimerLocal()[1].toggle();
                                    break;
                                }
                                case sf::Keyboard::Num3:
                                {
                                    pPhysicsManager->getSimTimerLocal()[2].toggle();
                                    break;
                                }
                                case sf::Keyboard::Add:
                                case sf::Keyboard::A:
                                {
                                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                                        sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                                        pPhysicsManager->accelerateTime(PHYSICS_ALLOW_STEP_SIZE_INC);
                                    else
                                        pPhysicsManager->accelerateTime();
                                    break;
                                }
                                case sf::Keyboard::Subtract:
                                case sf::Keyboard::Dash:
                                case sf::Keyboard::D:
                                {
                                    pPhysicsManager->decelerateTime();
                                    break;
                                }
                                case sf::Keyboard::Return:
                                {
                                    pPhysicsManager->resetTime();
                                    break;
                                }
                                case sf::Keyboard::C:
                                {
                                    pVisualsManager->cycleCamera();
                                    pCamera=pVisualsManager->getCurrentCamera();
                                    break;
                                }
                                case sf::Keyboard::B:
                                {
                                    pVisualsManager->toggleVisualisations(VISUALS_OBJECT_BBOXES);
                                    break;
                                }
                                case sf::Keyboard::G:
                                {
                                    pVisualsManager->toggleVisualisations(VISUALS_UNIVERSE_GRID);
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
                                    pPhysicsManager->pause();
                                    GameStateManager.load();
                                    pCamera=(*VisualsDataStorage.getCamerasByName().cbegin()).second;
                                    pPhysicsManager->togglePause();
                                    break;
                                }
                                case sf::Keyboard::N:
                                {
                                    pVisualsManager->toggleVisualisations(VISUALS_NAMES);
                                    break;
                                }
                                case sf::Keyboard::P:
                                {
                                    pPhysicsManager->togglePause();
                                    break;
                                }
                                case sf::Keyboard::S:
                                {
                                    /// \todo Really check if physics thread is paused before saving the simulation
                                    pPhysicsManager->pause();
                                    GameStateManager.save();
                                    pPhysicsManager->togglePause();
                                    break;
                                }
                                case sf::Keyboard::Space:
                                {
                                    pPhysicsManager->processOneFrame();
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
                                    pWindow->setMouseCursorVisible(bMouseCursorVisible);
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
                            break;
                        }
                        case sf::Event::MouseMoved:
                        {
                            if (bGraphicsOn)
                            {
                                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                                {
                                    pCamera->translateBy(0.2/GRAPHICS_PX_PER_METER * Vector2d(double(vecMouse.x)/pCamera->getZoom(),double(vecMouse.y)/pCamera->getZoom()));
                                }
                                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                                {
                                    pCamera->rotateBy(-double(vecMouse.x)*0.001); // Rotate clockwise for right mouse movement
                                    pCamera->zoomBy(1.0+double(vecMouse.y)*0.001);
                                    if      (pCamera->getZoom() < 1.0e-18) pCamera->zoomTo(1.0e-18);
                                    else if (pCamera->getZoom() > 1.0e3) pCamera->zoomTo(1.0e3);
                                }
                                break;
                            }
                        }
                        case sf::Event::MouseWheelMoved:
                            if (bGraphicsOn)
                            {
                                pCamera->zoomBy(1.0+double(Event.mouseWheel.delta)*0.1);
                                if      (pCamera->getZoom() < 1.0e-18) pCamera->zoomTo(1.0e-18);
                                else if (pCamera->getZoom() > 1.0e3) pCamera->zoomTo(1.0e3);
                            }
                        default:
                            break;
                    }
                }
            #ifndef PW_MULTITHREADING
            }
                //--- Run Physics ---//
                pPhysicsManager->processFrame();
                if (nFrame % static_cast<int>(pPhysicsManager->getFrequency()/
                                          pVisualsManager->getFrequency()) == 0)
                {
            #endif
                    //--- Draw visuals if requested ---//
                    if (bGraphicsOn)
                    {
                        pVisualsManager->processFrame();
                    }
            #ifndef PW_MULTITHREADING
                }
                pPhysicsManager->setTimeSlept(
                  Timer.sleepRemaining(pPhysicsManager->getFrequency() * 
                                      pPhysicsManager->getTimeAccel())
                );
                
                if (nFrame % 1000u == 0u) nFrame = 0u;
            #else
                Timer.sleepRemaining(pVisualsManager->getFrequency());
            #endif
        }
    }
    else // if (!bGraphics)
    {
        #ifndef PW_MULTITHREADING
        while (!bDone)
        {
            //--- Run Physics ---//
            pPhysicsManager->processFrame();
            pPhysicsManager->setTimeSlept(
              Timer.sleepRemaining(pPhysicsManager->getFrequency() * 
                                  pPhysicsManager->getTimeAccel())
            );
        }
        #else
        Timer.start();
        while (!bDone)
        {
            Timer.sleepRemaining(0.1);
        }
        #endif
    }
      
    #ifdef PW_MULTITHREADING
        pPhysicsManager->terminate();
        INFO_MSG("Main", "Physics thread stopped.")
        PhysicsThread.join();
    #endif

    CLEAN_UP_AND_EXIT_SUCCESS;
}
