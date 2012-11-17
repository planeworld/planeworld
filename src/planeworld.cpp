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

//--- Program header ---------------------------------------------------------//
#include "debris_visuals.h"
#include "engine_manager.h"
#include "pointmass.h"
#include "xml_importer.h"
#include "rectangle_visuals.h"
#include "rigidbody.h"
#include "spring_visuals.h"
#include "universe.h"
#include "xfig_loader.h"

//--- Misc-Header ------------------------------------------------------------//
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

#include <iostream>

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
    
    bool                bIsActive = true;
    bool                bDone = false;
    SDL_Event           event;
    int                 nT0 = 0;            // initial value of timer
    const std::string   PLANEWORLD_DATA_DIR="/home/bfeld/tmp/local/share/planeworld/data/";
    CTimer              Timer;

    //--- Demonstration Variables ---------------------------------------------//
    int             nMouseX;
    int             nMouseY;
    
    //--- Major instances ----------------------------------------------------//
    CGraphics&          Graphics = CGraphics::getInstance();
    CCamera*            pCamera;
    CCircle*            pCircle;
    CCircleVisuals*     pCircleVisuals;
    CDebris*            pDebris;
    CDebrisVisuals*     pDebrisVisuals;
//     CPointMass*         pPointMass;
    CRigidBody*         pPointMass;
    CPlanet*            pPlanet;
    CPlanetVisuals*     pPlanetVisuals;
    CPolyLine*          pPolyLine;
    CPolylineVisuals*   pPolylineVisuals;
    CRectangle*         pRect;
    CRectangleVisuals*  pRectVisuals;
    CRigidBody*         pBody1;
    CRigidBody*         pBody2;
    CRigidBody*         pBody3;
    CRigidBody*         pBody4;
    CRigidBody*         pEarth;
    CRigidBody*         pMoon;
    CRigidBody*         pSun;
    CEngineManager      EngineManager;
    CPhysicsManager*    pPhysicsManager;
    CVisualsManager*    pVisualsManager;
    CXFigLoader         XFigLoader;
    CSpring*            pSpring;
    CSpringVisuals*     pSpringVisuals;
    CUniverse           Universe;
    
    long nX;
    long nY;
    srand(SDL_GetTicks());

    //--- Initialisation -----------------------------------------------------//
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("pPhysicsManager")
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("pVisualsManager")
    
    int nNumberOfBoxes = 100;
    
    Universe.generate(100);
    
    //--- Initialise particles -----------------------------------------------//
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
        pPointMass->setMass(1.0e12);
        pPointMass->setOrigin(nX, nY);
        pPointMass->setInertia(2000.0);

        std::stringstream test;
        test << "PointMass_" << i;
        pPointMass->setName(test.str());

//         pPointMass->disableGravitation();
//         pPointMass->disableDynamics();
//         pPointMass->setTimeFac(1e-10);
        pPointMass->setDepths(SHAPE_DEPTH_ALL);
        
//         pRect = new CRectangle();
        pCircle = new CCircle();
//         pPlanet = new CPlanet();
        MEM_ALLOC("pRectangle")
//         pRectVisuals = new CRectangleVisuals(pRect);
        pCircleVisuals = new CCircleVisuals(pCircle);
//         pPlanetVisuals = new CPlanetVisuals(pPlanet);
        MEM_ALLOC("pRectangleVisuals")


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

        pPointMass->getGeometry()->addShape(pCircle);
//         pPointMass->getGeometry().addShape(pPlanet);
        
//         pPointMass->getGeometry().addShape(pRect);
//         pPointMass->addVisuals(pRectVisuals);
        
//         pVisualsManager->addVisuals(pRectVisuals);
        pCircle->setVisualsID(pVisualsManager->addVisuals(pCircleVisuals));
//         pPlanet->setVisualsID(pVisualsManager->addVisuals(pPlanetVisuals));
        pPhysicsManager->addObject(pPointMass);
    }

    //--- Initialize Debris -------------------------------------------------//
    pDebris = new CDebris;
    MEM_ALLOC("pDebris")
    pDebrisVisuals = new CDebrisVisuals(pDebris);
    MEM_ALLOC("pDebrisVisuals")

    pDebris->setNumber(1000);
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
//     pRect = new CRectangle();
//     MEM_ALLOC("pRectangle")
//     pRectVisuals = new CRectangleVisuals(pRect);
//     MEM_ALLOC("pRectVisuals")
//     pRect->setDepths(SHAPE_DEPTH_ALL);
//     pRect->setUL(Vector2d(-3.0, -5.5));
//     pRect->setLR(Vector2d(0.4, 2.0));
// 
//     pBody2->getGeometry()->addShape(pRect);
//     pRect->setVisualsID(pVisualsManager->addVisuals(pRectVisuals));
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
    
    //--- Initialize graphics ------------------------------------------------//
//     SDL_WM_GrabInput(SDL_GRAB_ON);

    // Set initialisation state of all objects
    pPhysicsManager->initObjects();

//     pEarth->setVelocity(Vector2d(29.78e3, 0.0));
//     pEarth->setAngleVelocity(0.001);
//     pMoon->setVelocity(Vector2d(29.78e3+1.023e3, 0.0));
    
//     pCamera->setHook(pBody3);
    
    EngineManager.setPhysicsManager(pPhysicsManager);
    EngineManager.setVisualsManager(pVisualsManager);
    EngineManager.runPhysicsThread();
    EngineManager.runGraphicsThread();
    Timer.start();
    while (false == bDone)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_KP_PLUS || event.key.keysym.sym == SDLK_PLUS)
                    {
                        pPhysicsManager->accelerateTime();
                    }
                    if (event.key.keysym.sym == SDLK_KP_MINUS || event.key.keysym.sym == SDLK_MINUS)
                    {
                        pPhysicsManager->decelerateTime();
                    }
                    if (event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
                    {
                        pPhysicsManager->resetTime();
                    }
                    if (event.key.keysym.sym == SDLK_b)
                    {
                        pVisualsManager->toggleVisualisations(VISUALS_OBJECT_BBOXES);
                    }
                    if (event.key.keysym.sym == SDLK_i)
                    {
                        pVisualsManager->toggleVisualisations(VISUALS_OBJECT_INTERSECTIONS);
                    }
                    if (event.key.keysym.sym == SDLK_g)
                    {
                        pVisualsManager->toggleVisualisations(VISUALS_UNIVERSE_GRID);
                    }
                    if (event.key.keysym.sym == SDLK_r)
                        pPhysicsManager->initObjects();
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        bDone = true;
                        std::cout << "QUIT" << std::endl;
                        EngineManager.stop();
                    }
//                     break;
                case SDL_ACTIVEEVENT:
                    if (0 == event.active.gain)
                        bIsActive = false;
                    else
                        bIsActive = true;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state == SDL_BUTTON(1))
                    {
                        pCamera->translateBy(Vector2d(double(event.motion.xrel)/
                                              pCamera->getZoom(),
                                              double(event.motion.yrel)/
                                              pCamera->getZoom()));
                    }
                    if (event.motion.state == SDL_BUTTON(3))
                    {
                        pCamera->rotateBy(-double(event.motion.xrel)*0.01);
                        pCamera->zoomBy(1.0-double(event.motion.yrel)/100);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_MIDDLE)
                    {
                        pCamera->reset();
                    }
                    break;
                case SDL_VIDEORESIZE:
                    Graphics.resizeWindow(event.resize.w, event.resize.h);
                    break;
                case SDL_QUIT:
                    std::cout << "QUIT" << std::endl;
                    EngineManager.stop();
                    bDone = true;
                    break;
                default:
                    break;
            }
            
        }
        Timer.stop();
        double fFrametime = 1.0/30.0-Timer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        Timer.start();
    }

    SDL_Quit();
    return 0;
}
