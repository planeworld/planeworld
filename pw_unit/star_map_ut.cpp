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
/// \file       star_map_ut.cpp
/// \brief      Unit test which draws a star map.
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-17
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "universe.h"
#include "time.h"

//--- Misc-Header ------------------------------------------------------------//
#include "SDL/SDL.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << "Usage: starmap_ut <seed>" << std::endl;
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
    CTimer              Timer;
    double              fZoom = 1.0;
    Vector2d            vecTranslation;

    //--- Demonstration Variables ---------------------------------------------//
    int             nMouseX;
    int             nMouseY;
    
    //--- Major instances ----------------------------------------------------//
    CGraphics&          Graphics = CGraphics::getInstance();
    CUniverse           Universe;
    
    long nX;
    long nY;

    //--- Initialisation -----------------------------------------------------//
    Universe.generate(atoi(argv[1]));
    std::vector<CStarSystem*> StarSystems = Universe.getStarSystems();
    Graphics.init();
    
    //--- Initialize graphics ------------------------------------------------//
//     SDL_WM_GrabInput(SDL_GRAB_ON);
    Timer.start();
    while (false == bDone)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        bDone = true;
                        std::cout << "QUIT" << std::endl;
                        break;
                    }
                case SDL_ACTIVEEVENT:
                    if (0 == event.active.gain)
                        bIsActive = false;
                    else
                        bIsActive = true;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state == SDL_BUTTON(1))
                    {
                        vecTranslation += Vector2d(double(event.motion.xrel)/
                                                    fZoom,
                                                    double(event.motion.yrel)/
                                                    fZoom);
                    }
                    if (event.motion.state == SDL_BUTTON(3))
                    {
                        fZoom *= 1.0-double(event.motion.yrel)/100;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_MIDDLE)
                    {
                        fZoom = 1.0;
                        vecTranslation.setZero();
                    }
                    break;
                case SDL_VIDEORESIZE:
                    Graphics.resizeWindow(event.resize.w, event.resize.h);
                    break;
                case SDL_QUIT:
                    std::cout << "QUIT" << std::endl;
                    bDone = true;
                    break;
                default:
                    break;
            }
            
        }
        Graphics.zoomCamTo(fZoom);
        Graphics.transCamTo(vecTranslation);
        for (int i=0; i<StarSystems.size(); ++i)
        {
            double fColor = 0.1*StarSystems[i]->getStarType()+0.3;
            Graphics.setColor(0.8,fColor,0.3);
            Graphics.setPointSize(StarSystems[i]->getNumberOfPlanets());
            Graphics.dot(StarSystems[i]->getCenter());
        }
        Graphics.swapBuffers();
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
