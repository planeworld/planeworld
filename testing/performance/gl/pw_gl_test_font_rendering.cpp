////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
/// \mainpage   Project Planeworld - OpenGL Performance Tests
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-02
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_gl_test_font_rendering.cpp
/// \brief      Main program for OpenGL font rendering tests
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-02
///
////////////////////////////////////////////////////////////////////////////////

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <fstream>
#include <iostream>
#include <random>

//--- Program header ---------------------------------------------------------//
#include "font_manager.h"
#include "graphics.h"
#include "log.h"
#include "timer.h"
#include "render_target.h"

//--- Misc-Header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//-- Constants ---------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
////////////////////////////////////////////////////////////////////////////////
void usage()
{
    METHOD_ENTRY("usage")
    std::cout << "Usage: pw_gl_test_font_rendering" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Main function
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
    if (argc != 1)
    {
        usage();
        return EXIT_FAILURE;
    }
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    
    //--- Init Graphics ------------------------------------------------------//
    CGraphics& Graphics = CGraphics::getInstance();
    WindowHandleType* pWindow = new WindowHandleType(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),
                                                    "Planeworld - GL Font Rendering Test", sf::Style::Default,
                                                    sf::ContextSettings(24,8,4,4,5,sf::ContextSettings::Core));
    MEM_ALLOC("WindowHandleType")
    
    Graphics.setWindow(pWindow);
    
    //--- Init Font ----------------------------------------------------------//
    CFontManager FontManager;
        
    FontManager.addFont("anka_c87_r", "fonts/AnkaCoder-C87-r.ttf");
    FontManager.addFont("anka_r", "fonts/AnkaCoder-r.ttf");
    FontManager.addFont("anka_c87_bi", "fonts/AnkaCoder-C87-bi.ttf");
    FontManager.addFont("anka_bi", "fonts/AnkaCoder-bi.ttf");
    FontManager.setFont("anka_c87_r");
    
    Graphics.init();
    Graphics.setupScreenSpace();
    
    const std::vector<float> vecUVs =
    {
        0, 1,
        1, 1,
        0, 0,
        1, 0
    };
    
    //--- Main Event Loop ----------------------------------------------------//
    CTimer TimerMain; TimerMain.start();
    bool bDone = false;
    sf::Event Event;
    
    Graphics.setColor({{1.0, 1.0, 1.0, 1.0}});
    
    while (!bDone)
    {
        while (pWindow->pollEvent(Event))
        {
            switch (Event.type)
            {
                case sf::Event::KeyPressed:
                    if (Event.key.code == sf::Keyboard::Escape)
                        bDone = true;
                    break;
                case sf::Event::Closed:
                {
                    bDone = true;
                    break;
                }
                default:
                    break;
            }
        }
        
        Graphics.beginRenderBatch("font");
            FontManager.setFont("anka_c87_bi");
            Graphics.setColor({{0.0, 0.0, 1.0, 0.75}});
            Graphics.texturedRect(Vector2d(0.0, FONT_MGR_ATLAS_SIZE_DEFAULT*FONT_MGR_SCALE),
                                    Vector2d(FONT_MGR_ATLAS_SIZE_DEFAULT*FONT_MGR_SCALE, 0.0),
                                    &vecUVs);
            Graphics.setColor({{0.0, 1.0, 0.0, 0.5}});
            FontManager.drawText("Testing font rendering", 300.0f, 200.0f);
        Graphics.endRenderBatch();
        
        Graphics.swapBuffers();
        TimerMain.sleepRemaining(60.0);
    }

    //--- Clean up -----------------------------------------------------------//
    if (pWindow != nullptr)
    {
        delete pWindow;
        MEM_FREED("WindowHandleType")
        pWindow = nullptr;
    }
    return EXIT_SUCCESS;
}
