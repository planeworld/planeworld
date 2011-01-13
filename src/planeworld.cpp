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
#include "circle_visuals.h"
#include "engine_manager.h"
#include "pointmass.h"
#include "polyline_visuals.h"
#include "random_line.h"
#include "rectangle_visuals.h"
#include "rigidbody.h"
#include "spring_visuals.h"
#include "xfig_loader.h"

//--- Misc-Header ------------------------------------------------------------//
#include "SDL/SDL.h"

#include <boost/thread.hpp> 

// 
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
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
    CCircle*            pCircle;
    CCircleVisuals*     pCircleVisuals;
//  CPointMass*         pPointMass;
    CRigidBody*         pPointMass;
    CPolyLine*          pPolyLine;
    CPolylineVisuals*   pPolylineVisuals;
    CRandomLine*        pRandomLine;
    CRectangle*         pRect;
    CRectangleVisuals*  pRectVisuals;
    CRigidBody*         pBody1;
    CRigidBody*         pBody2;
    CRigidBody*         pBody3;
    CRigidBody*         pBody4;
    CEngineManager      EngineManager;
    CPhysicsManager*    pPhysicsManager;
    CVisualsManager*    pVisualsManager;
    CXFigLoader         XFigLoader;
    CSpring*            pSpring;
    CSpringVisuals*     pSpringVisuals;

    long nX;
    long nY;
    srand(SDL_GetTicks());

    //--- Initialisation -----------------------------------------------------//
//     pEngineManager = new CEngineManager;
//     MEM_ALLOC("pEngineManager")
    pPhysicsManager = new CPhysicsManager;
    MEM_ALLOC("pPhysicsManager")
    pVisualsManager = new CVisualsManager;
    MEM_ALLOC("pVisualsManager")
    
    int nNumberOfBoxes = 0;
    
    lua_State *L = lua_open();
    luaL_openlibs(L);
    int s = luaL_loadfile(L, (PLANEWORLD_DATA_DIR+"test.lua").c_str());
    if ( s==0 ) {
        // Execute Lua program
        s = lua_pcall(L, 0, LUA_MULTRET, 0);
    }
//  std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
//  lua_pop(L, 1);
    lua_getglobal(L,"NumberOfBoxes");
    if(lua_isnumber(L,-1))
        nNumberOfBoxes = lua_tonumber(L,-1);
    
    lua_close(L);

    //--- Initialise particles -----------------------------------------------//
    for (int i=0; i<nNumberOfBoxes; ++i)
    {
        pPointMass = new CRigidBody;
        MEM_ALLOC("pPointMass")

        DEBUG_MSG("Main", "Starting randomization...")
        nX = (rand()%200) - 100;
        nY = (rand()%200) - 100;
//         double fRad = (rand()%2) + 0.5;
        DEBUG_MSG("Main", "Stopping randomization.")
        pPointMass->setMass(1.0e12);
        pPointMass->setOrigin(nX, nY);
        pPointMass->setInertia(2000.0);

        std::stringstream test;
        test << "PointMass_" << i;
        pPointMass->setName(test.str());

//         pPointMass->disableGravitation();
//         pPointMass->setTimeFac(1e-10);
        pPointMass->setDepths(SHAPE_DEPTH_ALL);
        
//         pRect = new CRectangle();
        pCircle = new CCircle();
        MEM_ALLOC("pRectangle")
//         pRectVisuals = new CRectangleVisuals(pRect);
        pCircleVisuals = new CCircleVisuals(pCircle);
        MEM_ALLOC("pRectangleVisuals")


//         pRect->setDepths(SHAPE_DEPTH_ALL);
//         pRect->setUL(Vector2d(-1.0, +1.0));
//         pRect->setLR(Vector2d(+1.0, -1.0));
        pCircle->setDepths(SHAPE_DEPTH_ALL);
        pCircle->setCenter(1.0, 0.0);
        pCircle->setRadius(1.0);

        pPointMass->getGeometry().addShape(pCircle);
        
//         pPointMass->getGeometry().addShape(pRect);
//         pPointMass->addVisuals(pRectVisuals);
        
//         pVisualsManager->addVisuals(pRectVisuals);
        pPointMass->addVisualsID(pVisualsManager->addVisuals(pCircleVisuals));
        pPhysicsManager->addObject(pPointMass);
    }

    //--- Initialize Rigidbody ----------------------------------------------//
    pBody1 = new CRigidBody;
    MEM_ALLOC("pBody1")

    XFigLoader.load(PLANEWORLD_DATA_DIR + "test.fig");
    pBody1->getGeometry().setShapes(XFigLoader.getShapes());
    pBody1->setMass(150.0);
    pBody1->setInertia(2000.0);
    pBody1->setOrigin(Vector2d(3.0, 5.0));
    pBody1->setName("Rigidbody from .fig");
//  pBody1->disableDynamics();
//     pBody1->disableGravitation();

    pBody1->addVisualsIDList(pVisualsManager->addVisualsList(XFigLoader.getVisuals()));
    pPhysicsManager->addObject(pBody1);
    
    //--- Initialize Rigidbody ----------------------------------------------//
    pBody2 = new CRigidBody;
    MEM_ALLOC("pBody2")

    pBody2->setMass(100.0);
    pBody2->setInertia(2000.0);
    pBody2->setOrigin(Vector2d(-20.0,-10.0));
//     pBody2->disableGravitation();
//  pBody2->setTimeFac(5.0);
    pBody2->setName("Rigidbody");

    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircleVisuals = new CCircleVisuals(pCircle);
    MEM_ALLOC("pCircleVisuals")
    pCircle->setDepths(SHAPE_DEPTH_ALL);
    pCircle->setCenter( Vector2d(-2.0,0.0) );
    pCircle->setRadius(3.0);

    pBody2->getGeometry().addShape(pCircle);
    pBody2->addVisualsID(pVisualsManager->addVisuals(pCircleVisuals));

    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircleVisuals = new CCircleVisuals(pCircle);
    MEM_ALLOC("pCircleVisuals")
    pCircle->setDepths(SHAPE_DEPTH_ALL);
    pCircle->setCenter( Vector2d(2.0,0.0) );
    pCircle->setRadius(2.0);

    pBody2->getGeometry().addShape(pCircle);
    pBody2->addVisualsID(pVisualsManager->addVisuals(pCircleVisuals));

    pPolyLine = new CPolyLine;
    MEM_ALLOC("pPolyLine")
    pPolylineVisuals = new CPolylineVisuals(pPolyLine);
    MEM_ALLOC("pPolylineVisuals")
    pPolyLine->setLineType(GRAPHICS_LINETYPE_LOOP);
    pPolyLine->setDepths(SHAPE_DEPTH_ALL);
    pPolyLine->addVertex(0.0, -5.0);
    pPolyLine->addVertex(5.0, 5.0);
    pPolyLine->addVertex(-5.0, 5.0);

    pBody2->getGeometry().addShape(pPolyLine);
    pBody2->addVisualsID(pVisualsManager->addVisuals(pPolylineVisuals));

//  pRandomLine = new CRandomLine;
//  MEM_ALLOC("pRandomLine")
//  pRandomLine->init(-160.0, 160.0, -100.0, -100.0, 0.8, 6, 4);
//  pRandomLine->setDepths(SHAPE_DEPTH_1);
// 
//  pBody2->getGeometry().addShape(pRandomLine);

    pRect = new CRectangle();
    MEM_ALLOC("pRectangle")
    pRectVisuals = new CRectangleVisuals(pRect);
    MEM_ALLOC("pRectVisuals")
    pRect->setDepths(SHAPE_DEPTH_ALL);
    pRect->setUL(Vector2d(-3.0, -5.5));
    pRect->setLR(Vector2d(0.4, 2.0));

    pBody2->getGeometry().addShape(pRect);
    pBody2->addVisualsID(pVisualsManager->addVisuals(pRectVisuals));

    pPhysicsManager->addObject(pBody2);

    //--- Initialize Rigidbody ----------------------------------------------//
    pPolyLine = new CPolyLine();
    MEM_ALLOC("pPolyLine")
    pPolylineVisuals = new CPolylineVisuals(pPolyLine);
    MEM_ALLOC("pPolylineVisuals")
    pPolyLine->setLineType(GRAPHICS_LINETYPE_LOOP);
    pPolyLine->setDepths(SHAPE_DEPTH_ALL);
    pPolyLine->addVertex(0.0, -5.0);
    pPolyLine->addVertex(5.0, 5.0);
    pPolyLine->addVertex(-3.0, 5.0);
    pPolyLine->addVertex(-7.0, 0.0);

    pBody3 = new CRigidBody;
    MEM_ALLOC("pBody3")

    pBody3->setMass(250.0);
    pBody3->setInertia(3000.0);
    pBody3->setOrigin(Vector2d(15.0,-8.0));
//     pBody3->disableGravitation();
    pBody3->setName("Box");
    
    pBody3->getGeometry().addShape(pPolyLine);
    pBody3->addVisualsID(pVisualsManager->addVisuals(pPolylineVisuals));

    pPhysicsManager->addObject(pBody3);
    
    //--- Initialise Rigidbody ----------------------------------------------//
    pBody4 = new CRigidBody;
    MEM_ALLOC("pBody4")

    pBody4->setMass(5.974e24);
    pBody4->setInertia(3000.0);
    pBody4->setOrigin(Vector2d(0.0,-6378.27e3));
    pBody4->disableGravitation();
    pBody4->disableDynamics();
    pBody4->setName("Earth");
    
    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircleVisuals = new CCircleVisuals(pCircle);
    MEM_ALLOC("pCircleVisuals")
    pCircle->setDepths(SHAPE_DEPTH_ALL);
    pCircle->setCenter( Vector2d(0.0,0.0) );
    pCircle->setRadius(6378.16e3);
    
    pBody4->getGeometry().addShape(pCircle);
    pBody3->addVisualsID(pVisualsManager->addVisuals(pCircleVisuals));

    pPhysicsManager->addObject(pBody4);
    
    //--- Initialise Rigidbody ----------------------------------------------//
    pBody4 = new CRigidBody;
    MEM_ALLOC("pBody4")
    
    pBody4->setMass(7.349e22);
    pBody4->setInertia(3000.0);
    pBody4->setOrigin(Vector2d(0.0, -384.4e6));
    pBody4->setTimeFac(1e5);
    pBody4->disableGravitation();
    pBody4->disableDynamics();
    pBody4->setName("Moon");
    
    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircleVisuals = new CCircleVisuals(pCircle);
    MEM_ALLOC("pCircleVisuals")
    pCircle->setDepths(SHAPE_DEPTH_ALL);
    pCircle->setCenter( Vector2d(0.0,0.0) );
    pCircle->setRadius(1738.0e3);
    
    pBody4->getGeometry().addShape(pCircle);
    pBody4->addVisualsID(pVisualsManager->addVisuals(pCircleVisuals));

    pPhysicsManager->addObject(pBody4);

    //--- Initialize Spring -------------------------------------------------//
    pSpring = new CSpring();
    MEM_ALLOC("pSpring")
    pSpringVisuals = new CSpringVisuals(pSpring);
    MEM_ALLOC("pSpringVisuals")

    pSpring->attachObjectA(pBody1, pBody1->addAnchor(Vector2d(4.0,5.0)));
    pSpring->attachObjectB(pBody2, pBody2->addAnchor(Vector2d(4.0,3.0)));
    pSpring->setC(150.0);
    pSpring->setLength(20.0);

    pPhysicsManager->addJoint(pSpring);
    pSpring->addVisualsID(pVisualsManager->addVisuals(pSpringVisuals));

    pSpring = new CSpring();
    MEM_ALLOC("pSpring")
    pSpringVisuals = new CSpringVisuals(pSpring);
    MEM_ALLOC("pSpringVisuals")

    pSpring->attachObjectA(pBody2, pBody2->addAnchor(Vector2d(-3.0,2.0)));
    pSpring->attachObjectB(pBody3, pBody3->addAnchor(Vector2d(2.0,-2.3)));
    pSpring->setC(125.0);
    pSpring->setLength(15.0);

    pPhysicsManager->addJoint(pSpring);
    pSpring->addVisualsID(pVisualsManager->addVisuals(pSpringVisuals));

    pSpring = new CSpring();
    MEM_ALLOC("pSpring")
    pSpringVisuals = new CSpringVisuals(pSpring);
    MEM_ALLOC("pSpringVisuals")

    pSpring->attachObjectA(pBody3, pBody3->addAnchor(Vector2d(0.5,0.5)));
    pSpring->attachObjectB(pBody1, pBody1->addAnchor(Vector2d(-3.0,-1.5)));
    pSpring->setC(120.0);
    pSpring->setLength(18.0);

    pPhysicsManager->addJoint(pSpring);
    pSpring->addVisualsID(pVisualsManager->addVisuals(pSpringVisuals));
    
    //--- Initialize graphics ------------------------------------------------//
//     SDL_WM_GrabInput(SDL_GRAB_ON);

    // Set initialisation state of all objects
    pPhysicsManager->initObjects();
    pPhysicsManager->setConstantGravitation(Vector2d(0.0, -9.81));
    
    EngineManager.setPhysicsManager(pPhysicsManager);
    EngineManager.setVisualsManager(pVisualsManager);
    pBody4->setVelocity(Vector2d(1023.0, 0.0));
    
    // Test threading
    boost::thread PhysicsThread(&CEngineManager::runPhysics, &EngineManager);
    boost::thread VisualsThread(&CEngineManager::runGraphics, &EngineManager);
    
    sleep(1);
    Timer.start();
    while (false == bDone)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_KP_PLUS)
                    {
                        pPhysicsManager->accelerateTime();
                    }
                    if (event.key.keysym.sym == SDLK_KP_MINUS)
                    {
                        pPhysicsManager->decelerateTime();
                    }
                    if (event.key.keysym.sym == SDLK_KP_ENTER)
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
                        Graphics.transCamBy(Vector3d(double(event.motion.xrel)/
                                            (Graphics.getCamZoom()),
                                            double(event.motion.yrel)/
                                            (Graphics.getCamZoom()), 0.0));
                    }
                    if (event.motion.state == SDL_BUTTON(3))
                    {
                        Graphics.rotCamBy(-double(event.motion.xrel)/2);
                        Graphics.zoomCamBy(1.0-double(event.motion.yrel)/100);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_MIDDLE)
                    {
                        Graphics.resetCam();
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
    
    PhysicsThread.join();
    VisualsThread.join();
    
//     if (pEngineManager != 0)
//     {
//         delete pEngineManager;
//         pEngineManager = 0;
//         MEM_FREED("pEngineManager")
//     }
    
    if (pPhysicsManager != 0)
    {
        delete pPhysicsManager;
        pPhysicsManager = 0;
        MEM_FREED("pPhysicsManager")
    }
    if (pVisualsManager != 0)
    {
        delete pVisualsManager;
        pVisualsManager = 0;
        MEM_FREED("pVisualsManager")
    }

    SDL_Quit();
    return 0;
}
