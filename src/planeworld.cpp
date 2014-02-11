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

//--- Program header ---------------------------------------------------------//
#include "physics_manager.h"
#include "planet.h"
#include "planet_visuals.h"
#include "pointmass.h"
#include "rigidbody.h"
#include "spring_visuals.h"
#include "xfig_loader.h"
#include "xml_importer.h"
#include "visuals_manager.h"

//--- Misc-Header ------------------------------------------------------------//

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
        _pPhysicsManager->addGlobalForces();
        _pPhysicsManager->moveMasses(nCC);
        _pPhysicsManager->collisionDetection();
        PhysicsTimer.sleepRemaining(_pPhysicsManager->getFrequency());
        ++nCC;
        if (nCC == 10000) nCC = 0;
    }
    INFO_MSG("Main", "Physics thread stopped.")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << "Usage: planeworld UNIVERSE_DATA_FILE" << std::endl;
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
    CCircle*            pCircle;
    CCircleVisuals*     pCircleVisuals;
    IObjectVisuals*     pObjectVisuals;
    CDebris*            pDebris;
    CDebrisVisuals*     pDebrisVisuals;
//     CPointMass*         pPointMass;
    CRigidBody*         pPointMass;
    CPlanet*            pPlanet;
    CPlanetVisuals*     pPlanetVisuals;
    CPolyLine*          pPolyLine;
    CPolylineVisuals*   pPolylineVisuals;
    CRigidBody*         pBody1;
    CRigidBody*         pBody2;
    CRigidBody*         pBody3;
    CRigidBody*         pBody4;
//     CEngineManager      EngineManager;
    CPhysicsManager*    pPhysicsManager;
    CVisualsManager*    pVisualsManager;
    CXFigLoader         XFigLoader;
    CSpring*            pSpring;
    CSpringVisuals*     pSpringVisuals;
    CUniverse           Universe;
    
    //--- Initialisation -----------------------------------------------------//
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("pPhysicsManager")
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("pVisualsManager")
    
    int nNumberOfBoxes = 100;
    
    //--- Initialise particles -----------------------------------------------//
    long nX;
    long nY;
    srand(234);
    for (int i=0; i<nNumberOfBoxes; ++i)
    {
        pPointMass = new CRigidBody;
        MEM_ALLOC("pPointMass")

        DEBUG_MSG("Main", "Starting randomization...")
        nX = (rand()%200) - 100;
        nY = (rand()%200) - 100;
//         nX = (rand()%2000000) - 1000000;
//         nY = (rand()%2000000) - 1000000;
//         double fRad = (rand()%2) + 0.5;
        DEBUG_MSG("Main", "Stopping randomization.")
        pPointMass->setMass(10.0);
        pPointMass->setOrigin(nX, nY);
        pPointMass->setInertia(2000.0);

        std::stringstream test;
        test << "PointMass_" << i;
        pPointMass->setName(test.str());

//         pPointMass->disableGravitation();
//         pPointMass->disableDynamics();
//         pPointMass->setTimeFac(1e-10);
        pPointMass->setDepths(SHAPE_DEPTH_ALL);
        
        pCircle = new CCircle();
//         pPlanet = new CPlanet();
        MEM_ALLOC("pCircle")

//         pRect->setDepths(SHAPE_DEPTH_ALL);
//         pRect->setUL(Vector2d(-1.0, +1.0));
//         pRect->setLR(Vector2d(+1.0, -1.0));
        pCircle->setDepths(SHAPE_DEPTH_ALL);
        pCircle->setCenter(1.0, 0.0);
        pCircle->setRadius(1.0);
//         pPlanet->setDepths(SHAPE_DEPTH_ALL);
//         pPlanet->setCenter(0.0, 0.0);
//         pPlanet->setRadius(100.0);
//         pPlanet->setHeight(1.0);
//         pPlanet->initTerrain();

        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("pShape")
        pShape->buffer(pCircle);
        pPointMass->getGeometry()->addShape(pShape);
        
        pCircleVisuals = new CCircleVisuals(pShape);
//         pPlanetVisuals = new CPlanetVisuals(pPlanet);
        MEM_ALLOC("pCircleVisuals")
        
        pObjectVisuals = new IObjectVisuals(pPointMass);
//         pPlanetVisuals = new CPlanetVisuals(pPlanet);
        MEM_ALLOC("pObjectVisuals")
        
//         pPointMass->getGeometry().addShape(pPlanet);
        
//         pPointMass->getGeometry().addShape(pRect);
//         pPointMass->addVisuals(pRectVisuals);
        
//         pVisualsManager->addVisuals(pRectVisuals);
        pObjectVisuals->addVisuals(pCircleVisuals);
        pVisualsManager->addVisuals(pObjectVisuals);
//         pPlanet->setVisualsID(pVisualsManager->addVisuals(pPlanetVisuals));
        pPhysicsManager->addObject(pPointMass);
    }

    //--- Initialize Debris -------------------------------------------------//
    pDebris = new CDebris;
    MEM_ALLOC("pDebris")
    pDebrisVisuals = new CDebrisVisuals(pDebris);
    MEM_ALLOC("pDebrisVisuals")

    pDebris->setNumber(5000);
    pVisualsManager->addVisuals(pDebrisVisuals);
    pPhysicsManager->addDebris(pDebris);

    //--- Initialize Rigidbody ----------------------------------------------//
//     pBody1 = new CRigidBody;
//     MEM_ALLOC("pBody1")
// 
//     XFigLoader.load(PLANEWORLD_DATA_DIR + "test.fig");
//     pBody1->getGeometry()->setShapes(XFigLoader.getShapes());
//     pBody1->setMass(150.0);
//     pBody1->setInertia(2000.0);
//     pBody1->setOrigin(Vector2d(3.0, 5.0));
//     pBody1->setName("Rigidbody from .fig");
// //  pBody1->disableDynamics();
// //     pBody1->disableGravitation();
// 
//     pVisualsManager->addVisualsList(XFigLoader.getVisuals());
//     pPhysicsManager->addObject(pBody1);
//     
//     //--- Initialize Rigidbody ----------------------------------------------//
//     pBody2 = new CRigidBody;
//     MEM_ALLOC("pBody2")
// 
//     pBody2->setMass(100.0);
//     pBody2->setInertia(2000.0);
//     pBody2->setOrigin(Vector2d(-20.0,-10.0));
// //     pBody2->disableGravitation();
// //  pBody2->setTimeFac(5.0);
//     pBody2->setName("Rigidbody");
// 
//     pCircle = new CCircle();
//     MEM_ALLOC("pCircle")
//     pCircleVisuals = new CCircleVisuals(pCircle);
//     MEM_ALLOC("pCircleVisuals")
//     pCircle->setDepths(SHAPE_DEPTH_ALL);
//     pCircle->setCenter( Vector2d(-2.0,0.0) );
//     pCircle->setRadius(3.0);
// 
//     pBody2->getGeometry()->addShape(pCircle);
//     pCircle->setVisualsID(pVisualsManager->addVisuals(pCircleVisuals));
// 
//     pCircle = new CCircle();
//     MEM_ALLOC("pCircle")
//     pCircleVisuals = new CCircleVisuals(pCircle);
//     MEM_ALLOC("pCircleVisuals")
//     pCircle->setDepths(SHAPE_DEPTH_ALL);
//     pCircle->setCenter( Vector2d(2.0,0.0) );
//     pCircle->setRadius(2.0);
// 
//     pBody2->getGeometry()->addShape(pCircle);
//     pCircle->setVisualsID(pVisualsManager->addVisuals(pCircleVisuals));
// 
//     pPolyLine = new CPolyLine;
//     MEM_ALLOC("pPolyLine")
//     pPolylineVisuals = new CPolylineVisuals(pPolyLine);
//     MEM_ALLOC("pPolylineVisuals")
//     pPolyLine->setLineType(GRAPHICS_LINETYPE_LOOP);
//     pPolyLine->setDepths(SHAPE_DEPTH_ALL);
//     pPolyLine->addVertex(0.0, -5.0);
//     pPolyLine->addVertex(5.0, 5.0);
//     pPolyLine->addVertex(-5.0, 5.0);
// 
//     pBody2->getGeometry()->addShape(pPolyLine);
//     pPolyLine->setVisualsID(pVisualsManager->addVisuals(pPolylineVisuals));
// 
//     pPhysicsManager->addObject(pBody2);
// 
//     //--- Initialize Rigidbody ----------------------------------------------//
//     pPolyLine = new CPolyLine();
//     MEM_ALLOC("pPolyLine")
//     pPolylineVisuals = new CPolylineVisuals(pPolyLine);
//     MEM_ALLOC("pPolylineVisuals")
//     pPolyLine->setLineType(GRAPHICS_LINETYPE_LOOP);
//     pPolyLine->setDepths(SHAPE_DEPTH_ALL);
//     pPolyLine->addVertex(0.0, -5.0);
//     pPolyLine->addVertex(5.0, 5.0);
//     pPolyLine->addVertex(-3.0, 5.0);
//     pPolyLine->addVertex(-7.0, 0.0);
// 
//     pBody3 = new CRigidBody;
//     MEM_ALLOC("pBody3")
// 
//     pBody3->setMass(250.0);
//     pBody3->setInertia(3000.0);
//     pBody3->setOrigin(Vector2d(15.0,-8.0));
// //     pBody3->disableGravitation();
//     pBody3->setName("Box");
//     
//     pBody3->getGeometry()->addShape(pPolyLine);
//     pPolyLine->setVisualsID(pVisualsManager->addVisuals(pPolylineVisuals));
// 
//     pPhysicsManager->addObject(pBody3);
//     
    //--- Import from xml file ----------------------------------------------//
    {
        // XML-Importer is initialised in its own scope to free memory after
        // objects are handed over to managers. However, this only applies for
        // the list structure of pointers.
        CXMLImporter        XMLImporter;
        
        XMLImporter.import(argv[1]);
        pVisualsManager->setCamera(XMLImporter.getCamera());
        pPhysicsManager->addObjects(XMLImporter.getObjects());
        pVisualsManager->addVisualsList(XMLImporter.getVisuals());
        pPhysicsManager->setConstantGravity(XMLImporter.getGravity());
        
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
//     pPhysicsManager->setUniverse(&Universe);
    pPhysicsManager->addObjects(Universe.getObjects());
    pVisualsManager->addVisualsList(Universe.getVisuals());
    
    // Set initialisation state of all objects
    pPhysicsManager->initObjects();
    
//     pEarth->setVelocity(Vector2d(29.78e3, 0.0));
//     pEarth->setAngleVelocity(0.001);
//     pMoon->setVelocity(Vector2d(29.78e3+1.023e3, 0.0));
    
//     pCamera->setHook(pBody3);
    
    //--- Initialise graphics ------------------------------------------------//
//     sf::RenderWindow Window(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),
//                             "Planeworld", sf::Style::Default, sf::ContextSettings(32));
    sf::RenderWindow Window(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),
                            "Planeworld");
    
    pVisualsManager->setWindow(&Window);
    pVisualsManager->initGraphics();
    
    sf::Thread PhysicsThread(std::bind(&runPhysics, pPhysicsManager, &bDone));
    sf::Thread CellUpdater(std::bind(&CPhysicsManager::runCellUpdate,pPhysicsManager));
    PhysicsThread.launch();
    CellUpdater.launch();
    INFO_MSG("Main", "Physics thread for cell update started.")
    
    
    
    //--- Prepare for querying relative mouse movement -----------------------//
    sf::Vector2i vecMouse;
    sf::Vector2i vecMouseCenter(sf::Vector2i(Window.getSize().x >> 1, Window.getSize().y >> 1));
    sf::Mouse::setPosition(vecMouseCenter,Window);
    
    //--- Run the main loop --------------------------------------------------//
    Timer.start();
    while (!bDone)
    {
        vecMouse = vecMouseCenter-sf::Mouse::getPosition(Window);
        vecMouse.x = -vecMouse.x; // Horizontal movements to the left should be negative
        sf::Mouse::setPosition(vecMouseCenter,Window);
        
        // Handle events
        sf::Event Event;
        while (Window.pollEvent(Event))
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
                        case sf::Keyboard::I:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_OBJECT_INTERSECTIONS);
                            break;
                        }
                        case sf::Keyboard::G:
                        {
                            pVisualsManager->toggleVisualisations(VISUALS_UNIVERSE_GRID);
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
                default:
                    break;
            }
        }
        pVisualsManager->drawGrid();
        pVisualsManager->drawWorld();
        pVisualsManager->drawBoundingBoxes();
        pVisualsManager->drawGridHUD();
        pVisualsManager->finishFrame();
        Timer.sleepRemaining(pVisualsManager->getFrequency());
    }

    CellUpdater.terminate();
    CellUpdater.wait();
    INFO_MSG("Main", "Physics thread for cell update stopped.")
    PhysicsThread.wait();

    if (pPhysicsManager != 0)
    {
        delete pPhysicsManager;
        MEM_FREED("pPhysicsManager")
    }
    if (pVisualsManager != 0)
    {
        delete pVisualsManager;
        MEM_FREED("pVisualsManager")
    }
    
    return 0;
}
