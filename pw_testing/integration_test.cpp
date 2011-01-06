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
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       integration_test.cpp
/// \brief      Unit test for different integrators
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <sstream>
#include <unistd.h>

//--- Program header ---------------------------------------------------------//
#include "circle.h"
#include "engine_manager.h"
#include "log.h"
#include "polyline.h"
#include "rectangle.h"
#include "rigidbody.h"
#include "spring.h"
#include "timer.h"

//--- Misc-Header ------------------------------------------------------------//
#include "SDL/SDL.h"

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
    bool                bIsActive = true;
    bool                bDone = false;
    SDL_Event           event;
    int                 nT0 = 0;            // initial value of timer
    CTimer              Timer;

    //--- Major instances ----------------------------------------------------//
    CGraphics&      Graphics = CGraphics::getInstance();
    CEngineManager* pEngineManager;
    CSpring*        pSpring;
    CCircle*        pCircle;
    CRectangle*     pRect;
    CRigidBody*     pBody1;
    CRigidBody*     pBody2;
    CRigidBody*     pBody3;

    //--- Initialisation -----------------------------------------------------//
    pEngineManager = new CEngineManager;
    MEM_ALLOC("pEngineManager")

    //--- Initialise Rigidbody ----------------------------------------------//
    pBody1 = new CRigidBody;
    MEM_ALLOC("pBody1")

//  pBody1->setMass(150.0);
//  pBody1->setInertia(2000.0);
    pBody1->setCOM(Vector2d(0.0, 30.0));
    pBody1->setName("Ceiling");
    pBody1->disableDynamics();
    pBody1->disableGravitation();

    pRect = new CRectangle();
    MEM_ALLOC("pRectangle")
    pRect->setDepths(SHAPE_DEPTH_1);
    pRect->setUL(Vector2d(50.0, 5.0));
    pRect->setLR(Vector2d(-50.0, -5.0));

    pBody1->getGeometry().addShape(pRect);

    pEngineManager->addObject(pBody1);

    //--- Initialise Rigidbody ----------------------------------------------//
    pBody2 = new CRigidBody;
    MEM_ALLOC("pBody2")

    pBody2->setMass(100.0);
    pBody2->setInertia(2000.0);
    pBody2->setCOM(Vector2d(-40.0,10.0));
    pBody2->setName("Mass 1");

    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircle->setDepths(SHAPE_DEPTH_1);
    pCircle->setCenter(Vector2d(0.0,0.0));
    pCircle->setRadius(3.0);

    pBody2->getGeometry().addShape(pCircle);
    pBody2->disableGravitation();
    
    pEngineManager->addObject(pBody2);

    //--- Initialise Rigidbody ----------------------------------------------//
    pBody3 = new CRigidBody;
    MEM_ALLOC("pBody3")

    pBody3->setMass(100.0);
    pBody3->setInertia(2000.0);
    pBody3->setCOM(Vector2d(40.0, 10.0));
    pBody3->setName("Mass 2");

    pCircle = new CCircle();
    MEM_ALLOC("pCircle")
    pCircle->setDepths(SHAPE_DEPTH_1);
    pCircle->setCenter(Vector2d(0.0,0.0));
    pCircle->setRadius(3.0);

    pBody3->getGeometry().addShape(pCircle);
    pBody3->disableGravitation();

    pEngineManager->addObject(pBody3);

    //--- Initialise Spring -------------------------------------------------//
    pSpring = new CSpring();
    MEM_ALLOC("pSpring")

    pSpring->attachObjectA(pBody1, pBody1->addAnchor(Vector2d(-40.0,0.0)));
    pSpring->attachObjectB(pBody2, pBody2->addAnchor(Vector2d(0.0,0.0)));
    pSpring->setC(150000.0);
    pSpring->setLength(15.0);

    pEngineManager->addJoint(pSpring);

    //--- Initialise Spring -------------------------------------------------//
    pSpring = new CSpring();
    MEM_ALLOC("pSpring")

    pSpring->attachObjectA(pBody1, pBody1->addAnchor(Vector2d(40.0,0.0)));
    pSpring->attachObjectB(pBody3, pBody3->addAnchor(Vector2d(0.0,0.0)));
    pSpring->setC(50000.0);
    pSpring->setLength(15.0);

    pEngineManager->addJoint(pSpring);

    //--- Initialize graphics ------------------------------------------------//
    Graphics.init();
    // SDL_WM_GrabInput(SDL_GRAB_ON);

    // Set initialisation state of all objects
    pEngineManager->initObjects();
    pEngineManager->setConstantGravitation(Vector2d(0.0, -9.81));
    
    Timer.start();
    int nFrames = 0;
    while (false == bDone)
    {

        Graphics.applyCamMovement();
        pEngineManager->addGlobalForces();
        pEngineManager->moveMasses();
        pEngineManager->collisionDetection();
        pEngineManager->drawWorld();
        
        Graphics.swapBuffers();

        while (SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_r)
                        pEngineManager->initObjects();
                    
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        bDone = true;
                    }
                    //break;
                case SDL_ACTIVEEVENT:
                    if (0 == event.active.gain)
                        bIsActive = false;
                    else
                        bIsActive = true;
                    break;
                case SDL_VIDEORESIZE:
                    Graphics.resizeWindow(event.resize.w, event.resize.h);
                    break;
                case SDL_QUIT:
                    bDone = true;
                    break;
                default:
                    break;
            }
        }
        //if (bIsActive)
        {
        // Use Fixed Framerate
        Timer.stop();
        double fFrametime = 1.0/pEngineManager->getFrequency()-Timer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        Timer.start();
        ++nFrames;
        }
    }
    if (pEngineManager != 0)
    {
        delete pEngineManager;
        pEngineManager = 0;
        MEM_FREED("pEngineManager")
    }

    SDL_Quit();
    return 0;
}
