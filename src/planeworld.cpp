/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
#include "debris_emitter.h"
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
        
        int nCC=0;

        while (!(*_pbDone))
        {
            if (!g_bPhysicsPaused)
            {
                _pPhysicsManager->addGlobalForces();
                _pPhysicsManager->moveMasses(nCC);
                _pPhysicsManager->collisionDetection();
                _pPhysicsManager->runCellUpdate();
            }
            PhysicsTimer.sleepRemaining(_pPhysicsManager->getFrequency());
            if (++nCC == 10000) nCC = 0;
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
    std::cout << "Usage: planeworld <DATA_DIRECTORY> <UNIVERSE_DATA_FILE>" << std::endl;
    std::cout << "\nNotice: At the moment, planeworld must be called from within the "
                 "scene directory, where the UNIVERSE_DATA_FILE is located. Otherwise, "
                 "some files might not be found. "<< std::endl;
    std::cout << "\nDATA_DIRECTORY must be given to find global resources like font "
                 "files, which are not depending on the currently loaded scene." << std::endl;
    std::cout << "\nExamples: " << std::endl;
    std::cout << "Within scene directory call: planeworld .. scene.xml" << std::endl;
    std::cout << "Within scene directory call: planeworld /home/user/planeworld/data scene.xml" << std::endl;
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
    if (argc != 3)
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
    CCircle*            pCircle;
    CCircleVisuals*     pCircleVisuals;
    IObjectVisuals*     pObjectVisuals;
    CDebris*            pDebris;
    CDebrisVisuals*     pDebrisVisuals;
    CDebrisEmitter*     pDebrisEmitter;
    CObjectEmitter*     pObjectsEmitter;
    CRigidBody*         pPointMass;
    CPlanet*            pPlanet;
    CPlanetVisuals*     pPlanetVisuals;
    CPolyLine*          pPolyLine;
    CPolylineVisuals*   pPolylineVisuals;
    CRigidBody*         pBody1;
    CRigidBody*         pBody2;
    CRigidBody*         pBody3;
    CRigidBody*         pBody4;
    CPhysicsManager*    pPhysicsManager;
    CVisualsManager*    pVisualsManager;
    CXFigLoader         XFigLoader;
    CSpring*            pSpring;
    CSpringVisuals*     pSpringVisuals;
    CUniverse           Universe;
    CWorldDataStorage   WorldDataStorage;
    CThruster           Thruster;
    
    //--- Initialisation -----------------------------------------------------//
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("CPhysicsManager")
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("CVisualsManager")
    
    //--- Initialize storage access for engine managers ----------------------//
    pPhysicsManager->setWorldDataStorage(&WorldDataStorage);
    pVisualsManager->setWorldDataStorage(&WorldDataStorage);
    
    //--- Initialize Debris --------------------------------------------------//
//     pDebrisEmitter = new CDebrisEmitter;
//     MEM_ALLOC("CDebrisEmitter")
//     pDebrisEmitter->setMode(EMITTER_MODE_TIMED);
//     pDebrisEmitter->setDistribution(EMITTER_DISTRIBUTION_POINT_SOURCE);
//     pDebrisEmitter->setAngle(0.0);
//     pDebrisEmitter->setAngleVariance(0.2);
//     pDebrisEmitter->setVelocity(20.0);
//     pDebrisEmitter->setVelocityVariance(1.0);
//     pDebrisEmitter->setFrequency(50.0);
//     pDebrisEmitter->setCell(0, 0);
//     pDebrisEmitter->setNumber(1000);
//     pPhysicsManager->addEmitter(pDebrisEmitter);
//     
//     pDebrisEmitter = new CDebrisEmitter;
//     MEM_ALLOC("CDebrisEmitter")
//     pDebrisEmitter->setMode(EMITTER_MODE_TIMED);
//     pDebrisEmitter->setDistribution(EMITTER_DISTRIBUTION_RECTANGULAR_FIELD);
//     pDebrisEmitter->setFrequency(50.0);
//     pDebrisEmitter->setLimits(-200.0, 200.0, 150.0, 200.0);
//     pDebrisEmitter->setCell(0, 0);
//     pDebrisEmitter->setNumber(1000);
//     pPhysicsManager->addEmitter(pDebrisEmitter);
//     
//         CRigidBody* pTemplate = new CRigidBody;
//         pCircle = new CCircle;
//         MEM_ALLOC("CRigidBody")
//         MEM_ALLOC("CCircle")
//         
//         pCircle->setDepths(SHAPE_DEPTH_ALL);
//         pCircle->setCenter(0.0, 0.0);
//         pCircle->setRadius(2.0);
// 
//         CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
//         MEM_ALLOC("CDoubleBufferedShape")
//         pShape->buffer(pCircle);
//         pTemplate->getGeometry()->addShape(pShape);
//     
//         pCircleVisuals = new CCircleVisuals(pShape);
//         MEM_ALLOC("CCircleVisuals")
//         
//         IObjectVisuals* pTemplateVisuals = new IObjectVisuals(pTemplate);
//         MEM_ALLOC("IObjectVisuals")
//         
//         pTemplateVisuals->addVisuals(pCircleVisuals);
//     
//     pObjectsEmitter = new CObjectEmitter;
//     MEM_ALLOC("CObjectsEmitter")
//     pObjectsEmitter->setMode(EMITTER_MODE_TIMED);
//     pObjectsEmitter->setDistribution(EMITTER_DISTRIBUTION_POINT_SOURCE);
//     pObjectsEmitter->setAngle(M_PI);
//     pObjectsEmitter->setAngleVariance(0.2);
//     pObjectsEmitter->setVelocity(20.0);
//     pObjectsEmitter->setVelocityVariance(1.0);
//     pObjectsEmitter->setFrequency(2.0);
//     pObjectsEmitter->setCell(0, 0);
//     pObjectsEmitter->setTemplate(pTemplate, pTemplateVisuals);
//     pPhysicsManager->addEmitter(pObjectsEmitter);
//     
//     pObjectsEmitter = new CObjectEmitter;
//     MEM_ALLOC("CObjectsEmitter")
//     pObjectsEmitter->setMode(EMITTER_MODE_EMIT_ONCE);
//     pObjectsEmitter->setDistribution(EMITTER_DISTRIBUTION_RECTANGULAR_FIELD);
//     pObjectsEmitter->setFrequency(2.0);
//     pObjectsEmitter->setLimits(-100.0, 100.0, -100.0, 100.0);
//     pObjectsEmitter->setCell(0, 0);
//     pObjectsEmitter->setNumber(100);
//     pPhysicsManager->addEmitter(pObjectsEmitter);
    
    //--- Import from xml file ----------------------------------------------//
    {
        // XML-Importer is initialised in its own scope to free memory after
        // objects are handed over to managers. However, this only applies for
        // the list structure of pointers.
        CXMLImporter        XMLImporter;
        
        XMLImporter.setWorldDataStorage(&WorldDataStorage);
        XMLImporter.import(argv[2]);
        pVisualsManager->setCamera(XMLImporter.getCamera());
//         pPhysicsManager->addObjects(XMLImporter.getObjects());
//         pVisualsManager->addVisualsList(XMLImporter.getVisuals());
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        pPhysicsManager->addEmitters(XMLImporter.getEmitters());
        pPhysicsManager->setPhysicsInterface(XMLImporter.getPhysicsInterface());
        pPhysicsManager->setFrequency(XMLImporter.getPhysicsFrequency());
        pPhysicsManager->setFrequencyDebris(XMLImporter.getFrequencyDebris());
        pPhysicsManager->setFrequencyLua(XMLImporter.getFrequencyLua());
        pVisualsManager->setFrequency(XMLImporter.getVisualsFrequency());
        pCamera=XMLImporter.getCamera();
        Universe.clone(XMLImporter.getUniverse());
    }
    if (WorldDataStorage.getDynamicObjects().find("Ball") != WorldDataStorage.getDynamicObjects().end())
    {
      IHookable* pBallObject = (*(WorldDataStorage.getDynamicObjects().find("Ball"))).second;
      Thruster.hook(pBallObject);
      if (Thruster.init(pBallObject))
      {
          pPhysicsManager->registerComponent(&Thruster);
      }
      else
      {
          WARNING_MSG("Main", "Failed to hook thruster.")
      }
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

    //--- Set initialisation state of all objects ----------------------------//
    pPhysicsManager->initEmitters();
    pPhysicsManager->initObjects();
    if (!pPhysicsManager->initLua()) return EXIT_FAILURE;
    
    //--- Initialise graphics ------------------------------------------------//
    pVisualsManager->setDataPath(argv[1]);
    pVisualsManager->initGraphics();
    
    WindowHandleType* pWindow = pVisualsManager->getWindowHandle();
    
    #ifdef PW_MULTITHREADING    
        std::thread PhysicsThread(runPhysics, pPhysicsManager, &bDone);
    #endif
    
    //--- Prepare for querying relative mouse movement -----------------------//
    sf::Vector2i vecMouse;
    sf::Vector2i vecMouseCenter(sf::Vector2i(pWindow->getSize().x >> 1, pWindow->getSize().y >> 1));
    sf::Mouse::setPosition(vecMouseCenter,*pWindow);
    
    //--- Run the main loop --------------------------------------------------//
    Timer.start();
    while (!bDone)
    {
        vecMouse = vecMouseCenter-sf::Mouse::getPosition(*pWindow);
        vecMouse.x = -vecMouse.x; // Horizontal movements to the left should be negative
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
                    // adjust the viewport when the window is resized
                    Graphics.resizeWindow(Event.size.width, Event.size.height);
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
                        case sf::Keyboard::F:
                        {
                            Thruster.fire();
                            break;
                        }
                        case sf::Keyboard::G:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_UNIVERSE_GRID);
                            break;
                        }
                        case sf::Keyboard::N:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_NAMES);
                            break;
                        }
                        case sf::Keyboard::P:
                        {
                            g_bPhysicsPaused ? g_bPhysicsPaused=false : g_bPhysicsPaused=true;
                            break;
                        }
                        case sf::Keyboard::T:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_OBJECT_TRAJECTORIES);
                            break;
                        }
                    }
                    break;
                }
                case sf::Event::MouseMoved:
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
                case sf::Event::MouseWheelMoved:
                    pCamera->zoomBy(1.0+double(Event.mouseWheel.delta)*0.1);
                    if      (pCamera->getZoom() < 1.0e-18) pCamera->zoomTo(1.0e-18);
                    else if (pCamera->getZoom() > 1.0e3) pCamera->zoomTo(1.0e3);
                default:
                    break;
            }
        }
        #ifndef PW_MULTITHREADING
            //--- Run Physics ---//
            static int nCC=0;
            if (!g_bPhysicsPaused)
            {
                pPhysicsManager->addGlobalForces();
                pPhysicsManager->moveMasses(nCC);
                pPhysicsManager->collisionDetection();
                pPhysicsManager->runCellUpdate();
            }
            if (++nCC == 10000) nCC = 0;
        #endif
        
        //--- Draw visuals ---//
        #ifndef PW_MULTITHREADING
            if (nCC % static_cast<int>(pPhysicsManager->getFrequency()/
                                       pVisualsManager->getFrequency()) == 0)
            {
        #endif
        pVisualsManager->drawGrid();
        pVisualsManager->drawTrajectories();
        pVisualsManager->drawWorld();
        pVisualsManager->drawBoundingBoxes();
        pVisualsManager->drawGridHUD();
        pVisualsManager->finishFrame();
        #ifndef PW_MULTITHREADING
            }
            Timer.sleepRemaining(pPhysicsManager->getFrequency());
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
    
    return EXIT_SUCCESS;
}
