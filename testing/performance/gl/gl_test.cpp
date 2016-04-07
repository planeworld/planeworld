////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \date       2016-03-31
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       gl_test.cpp
/// \brief      Main program for OpenGL performance tests
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-31
///
////////////////////////////////////////////////////////////////////////////////

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <iostream>

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"
#include "shader.h"

//--- Misc-Header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    std::cout << "Usage: gl_test" << std::endl;
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
    if (argc != 1)
    {
        usage();
        return EXIT_FAILURE;
    }
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    
    CGraphics& Graphics = CGraphics::getInstance();
    
    Graphics.init();
    
    float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
    float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)

    // Setup triangle vertices
    fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
    fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
    fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
  
    // Setup quad vertices
    fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
    fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
    fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
    fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;
    
    // Create VAO
    GLuint unVBA;
    
    glGenVertexArrays(1, &unVBA);
    glBindVertexArray(unVBA);

    // Now we create two VBOs
    GLuint unVBO[2]; 
    glGenBuffers(2, unVBO);
  
    glBindBuffer(GL_ARRAY_BUFFER, unVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, unVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_DYNAMIC_DRAW);     
    
    // We just clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    
    std::uint64_t nCount = 0u;
    
    CShader VertexShader;
    CShader FragmentShader;
    VertexShader.load("shader.vert", GL_VERTEX_SHADER);
    FragmentShader.load("shader.frag", GL_FRAGMENT_SHADER);
    
    while (nCount++ < 1000)
    {
        glEnableVertexAttribArray(0);
        
        // Triangle render
        glBindBuffer(GL_ARRAY_BUFFER, unVBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Quad render using triangle strip
        glBindBuffer(GL_ARRAY_BUFFER, unVBO[1]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
      
//         glDisableVertexAttribArray(0);
        
        Graphics.swapBuffers();
    }
}
