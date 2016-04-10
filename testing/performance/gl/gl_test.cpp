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
/// Code is partly based on the tutorial #3 of Michal Bubnar, see
/// http://mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=6
/// and official OpenGL Tutorial 1, see
/// https://www.opengl.org/wiki/Tutorial1:_Rendering_shapes_with_glDrawRangeElements,_VAO,_VBO,_shaders_%28C%2B%2B_/_freeGLUT%29
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
#include "shader_program.h"

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
    
    float fTriangle[9];
    float fQuad[12];
    float fTriangleColor[9];
    float fQuadColor[12];

    GLuint unVBO[4];
    GLuint unVAO[2];

    // Setup triangle vertices
    fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
    fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
    fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

    // Setup triangle color

    fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
    fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
    fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;
 
    // Setup quad vertices
 
    fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
    fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
    fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
    fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

    // Setup quad color

    fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
    fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
    fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
    fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

    glGenVertexArrays(2, unVAO); // Generate two VAOs, one for triangle and one for quad
    glGenBuffers(4, unVBO); // And four VBOs

    glBindVertexArray(unVAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, unVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, unVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(unVAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, unVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, unVBO[3]);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    std::uint64_t nCount = 0u;
    
    CShader VertexShader;
    CShader FragmentShader;
    VertexShader.load("shader.vert", GL_VERTEX_SHADER);
    FragmentShader.load("shader.frag", GL_FRAGMENT_SHADER);
    
    CShaderProgram ShaderProgram;
    
    ShaderProgram.create();
    ShaderProgram.addShader(VertexShader);
    ShaderProgram.addShader(FragmentShader);
    ShaderProgram.link();
    ShaderProgram.use();

    while (nCount++ < 1000)
    {
        // Triangle
        glBindVertexArray(unVAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, unVBO[0]);
        glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, unVBO[1]);
        glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Quad
        glBindVertexArray(unVAO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, unVBO[2]);
        glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, unVBO[3]);
        glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindVertexArray(unVAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(unVAO[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        Graphics.swapBuffers();
    }
}
