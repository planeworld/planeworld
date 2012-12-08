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
#include <iostream>

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "universe.h"

//--- Misc-Header ------------------------------------------------------------//

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
    double              fZoom = 1.0;
    double              fAngle = 0.0;
    Vector2d            vecTranslation;
    CTimer              Timer;

    int                 nMouseX = 0;
    int                 nMouseY = 0;
    
    //--- Major instances ----------------------------------------------------//
    CGraphics&          Graphics = CGraphics::getInstance();
    CUniverse           Universe;
    
    //--- Initialisation of universe------------------------------------------//
    Universe.generate(atoi(argv[1]));
    std::vector<CStarSystem*> StarSystems = Universe.getStarSystems();

    //--- Initialisation of graphics -----------------------------------------//
    sf::Window Window(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),
                      "Starmap Unit Test", sf::Style::Default, sf::ContextSettings(32));
    
    Graphics.setWindow(&Window);
    Graphics.init();
    
    //--- Prepare for querying relative mouse movement -----------------------//
    sf::Vector2i vecMouse;
    sf::Vector2i vecMouseCenter(sf::Vector2i(Window.getSize().x >> 1,Window.getSize().y >> 1));
    
    //--- Run the main loop --------------------------------------------------//
    while (!bDone)
    {
        vecMouse = vecMouseCenter-sf::Mouse::getPosition();
        vecMouse.x = -vecMouse.x; // Horizontal movements to the left should be negative
        sf::Mouse::setPosition(vecMouseCenter);
        
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
                    }
                    break;
                }
                case sf::Event::MouseMoved:
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        Graphics.transCamBy(0.2*Vector2d(double(vecMouse.x)/Graphics.getCamZoom(),double(vecMouse.y)/Graphics.getCamZoom()));
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        Graphics.zoomCamBy(1.0+double(vecMouse.y)*0.001);
                        Graphics.rotCamBy(-double(vecMouse.x)*0.001); // Clockwise for right mouse movement
                    }
                    break;
                }
                default:
                    break;
            }
        }
        
        
        // Draw
        Graphics.applyCamMovement();
        
        for (int i=0; i<StarSystems.size(); ++i)
        {
            double fColor = 0.1*StarSystems[i]->getStarType()+0.3;
            Graphics.setColor(0.8,fColor,0.3);
            Graphics.setPointSize(StarSystems[i]->getNumberOfPlanets());
            Graphics.dot(StarSystems[i]->getCenter());
        }

        Graphics.swapBuffers();
        Timer.sleepRemaining(30.0);
    }

    return 0;
}
