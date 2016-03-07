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
#include "conf_pw.h"
#include "circular_buffer.h"
#include "debris_emitter.h"
#include "game_state_manager.h"
#include "physics_manager.h"
#include "planet_visuals.h"
#include "objects_emitter.h"
#include "pointmass.h"
#include "spring_visuals.h"
#include "thruster.h"
#include "xfig_loader.h"
#include "xml_importer.h"
#include "visuals_manager.h"

//--- Misc-Header ------------------------------------------------------------//

//--- Global Variables -------------------------------------------------------//
bool g_bPhysicsPaused = false;

#ifdef PW_MULTITHREADING
    ////////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief Runs the physics engine, called as a thread.
    ///
    /// \param _pPhysicsManager Pointer to physics manager
    /// \param _pbDone Pointer to bool, indicating if program is stopped
    ///
    ///////////////////////////////////////////////////////////////////////////////
    void runPhysics(CPhysicsManager* const _pPhysicsManager, bool* const _pbDone)
    {
        METHOD_ENTRY("runPhysics")
        
        INFO_MSG("Main", "Physics thread started.")
        CTimer PhysicsTimer;
        
        PhysicsTimer.start();
        while (!(*_pbDone))
        {
            if (!g_bPhysicsPaused)
            {
                _pPhysicsManager->processFrame();
            }
            PhysicsTimer.sleepRemaining(_pPhysicsManager->getFrequency());
        }
        INFO_MSG("Main", "Physics thread stopped.")
    }
#endif

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << "Usage: planeworld <UNIVERSE_DATA_FILE>" << std::endl;
    std::cout << "\nExamples: " << std::endl;
    std::cout << "Within scene directory call: planeworld scene.xml" << std::endl;
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
    if (argc != 2)
    {
        usage();
        return EXIT_FAILURE;
    }
    
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    
    bool                bDone = false;
    CTimer              Timer;

    //--- Major instances ----------------------------------------------------//
    CGraphics&          Graphics = CGraphics::getInstance();
    CCamera*            pCamera;
    CGameStateManager   GameStateManager;
    CPhysicsManager*    pPhysicsManager;
    CVisualsManager*    pVisualsManager;
    CXFigLoader         XFigLoader;
    CUniverse           Universe;
    CWorldDataStorage   WorldDataStorage;
    CThruster           Thruster;
    
    //--- Initialisation -----------------------------------------------------//
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("CPhysicsManager")
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("CVisualsManager")
    
    //--- Initialize storage access for engine managers ----------------------//
    GameStateManager.setWorldDataStorage(&WorldDataStorage);
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    pVisualsManager->setWorldDataStorage(&WorldDataStorage);
    
    //--- Import from xml file ----------------------------------------------//
    {
        // XML-Importer is initialised in its own scope to free memory after
        // objects are handed over to managers. However, this only applies for
        // the list structure of pointers.
        CXMLImporter        XMLImporter;
        
        XMLImporter.setWorldDataStorage(&WorldDataStorage);
        XMLImporter.import(argv[1]);
        WorldDataStorage.setCamera(XMLImporter.getCamera());
//         pPhysicsManager->addObjects(XMLImporter.getObjects());
//         pVisualsManager->addVisualsList(XMLImporter.getVisuals());
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        pPhysicsManager->addComponents(XMLImporter.getComponents());
        pPhysicsManager->addEmitters(XMLImporter.getEmitters());
        pPhysicsManager->setPhysicsInterface(XMLImporter.getPhysicsInterface());
        pPhysicsManager->setFrequency(XMLImporter.getPhysicsFrequency());
        pPhysicsManager->setFrequencyDebris(XMLImporter.getFrequencyDebris());
        pPhysicsManager->setFrequencyLua(XMLImporter.getFrequencyLua());
        pVisualsManager->setFrequency(XMLImporter.getVisualsFrequency());
        pVisualsManager->setFont(XMLImporter.getFont());
        pCamera=XMLImporter.getCamera();
        Universe.clone(XMLImporter.getUniverse());
    }

//     //--- Initialize Springs ------------------------------------------------//
//     pSpring = new CSpring();
//     MEM_ALLOC("pSpring")
//     pSpringVisuals = new CSpringVisuals(pSpring);
//     MEM_ALLOC("pSpringVisuals")
// 
//     pSpring->attachObjectA(pBody1, pBody1->addAnchor(Vector2d(4.0,5.0)));
//     pSpring->attachObjectB(pBody2, pBody2->addAnchor(Vector2d(4.0,3.0)));
//     pSpring->setC(150.0);
//     pSpring->setLength(20.0);
// 
//     pPhysicsManager->addJoint(pSpring);
//     pSpring->setVisualsID(pVisualsManager->addVisuals(pSpringVisuals));
// 
//     pSpring = new CSpring();
//     MEM_ALLOC("pSpring")
//     pSpringVisuals = new CSpringVisuals(pSpring);
//     MEM_ALLOC("pSpringVisuals")
// 
//     pSpring->attachObjectA(pBody2, pBody2->addAnchor(Vector2d(-3.0,2.0)));
//     pSpring->attachObjectB(pBody3, pBody3->addAnchor(Vector2d(2.0,-2.3)));
//     pSpring->setC(125.0);
//     pSpring->setLength(15.0);
// 
//     pPhysicsManager->addJoint(pSpring);
//     pSpring->setVisualsID(pVisualsManager->addVisuals(pSpringVisuals));
// 
//     pSpring = new CSpring();
//     MEM_ALLOC("pSpring")
//     pSpringVisuals = new CSpringVisuals(pSpring);
//     MEM_ALLOC("pSpringVisuals")
// 
//     pSpring->attachObjectA(pBody3, pBody3->addAnchor(Vector2d(0.5,0.5)));
//     pSpring->attachObjectB(pBody1, pBody1->addAnchor(Vector2d(-3.0,-1.5)));
//     pSpring->setC(120.0);
//     pSpring->setLength(18.0);
// 
//     pPhysicsManager->addJoint(pSpring);
//     pSpring->setVisualsID(pVisualsManager->addVisuals(pSpringVisuals));
    
    pVisualsManager->setUniverse(&Universe);
    pPhysicsManager->setUniverse(&Universe);

    //--- Initialise physics -------------------------------------------------//
    pPhysicsManager->initObjects();
    pPhysicsManager->initEmitters();
    pPhysicsManager->initComponents();
    if (!pPhysicsManager->initLua()) return EXIT_FAILURE;
    
    #ifdef PW_MULTITHREADING    
        std::thread PhysicsThread(runPhysics, pPhysicsManager, &bDone);
    #endif
    
    //--- Initialise graphics ------------------------------------------------//
    pVisualsManager->initGraphics();
    
    WindowHandleType* pWindow = pVisualsManager->getWindowHandle();
        
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
                        case sf::Keyboard::Add:
                        {
                            pPhysicsManager->accelerateTime();
                            break;
                        }
                        case sf::Keyboard::Subtract:
                        case sf::Keyboard::Dash:
                        {
                            pPhysicsManager->decelerateTime();
                            break;
                        }
                        case sf::Keyboard::Return:
                        {
                            pPhysicsManager->resetTime();
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
                            g_bPhysicsPaused=true;
                            GameStateManager.load();
                            pCamera=WorldDataStorage.getCamera();
                            g_bPhysicsPaused=false;
                            break;
                        }
                        case sf::Keyboard::N:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_NAMES);
                            break;
                        }
                        case sf::Keyboard::P:
                        {
                            g_bPhysicsPaused ^= 1;
                            break;
                        }
                        case sf::Keyboard::S:
                        {
                            /// \todo Really check if physics thread is paused before saving the simulation
                            g_bPhysicsPaused=true;
                            GameStateManager.save();
                            g_bPhysicsPaused=false;
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
            //--- Run Physics ---//
            if (!g_bPhysicsPaused)
            {
                pPhysicsManager->processFrame();
            }
            if (nFrame++ % static_cast<int>(pPhysicsManager->getFrequency()/
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
            Timer.sleepRemaining(pPhysicsManager->getFrequency());
            if (nFrame % 1000u == 0u) nFrame = 0u;
        #else
            Timer.sleepRemaining(pVisualsManager->getFrequency());
        #endif
    }
    
    #ifdef PW_MULTITHREADING
        INFO_MSG("Main", "Physics thread stopped.")
        PhysicsThread.join();
    #endif

    if (pPhysicsManager != 0)
    {
        delete pPhysicsManager;
        MEM_FREED("CPhysicsManager")
    }
    if (pVisualsManager != 0)
    {
        delete pVisualsManager;
        MEM_FREED("CVisualsManager")
    }
    
    GameStateManager.save();
    
    return EXIT_SUCCESS;
}
