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
#include <random>

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"
#include "shader_program.h"
#include "timer.h"

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
    METHOD_ENTRY("usage")
    std::cout << "Usage: gl_test" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Test performance by using one vbo per Shape
///
/// \param _nNrOfShapes Number of shapes to be drawn per frame
/// \param _nNrOfFrames Number of frames to be drawn
/// \param _BufferUsage How the buffer is used (STATIC, DYNAMIC, STREAM)
/// \param _Mode        Which draw mode (TRIANGLE, LINE_LOOP)
///
/// \return Time in seconds for this test
///
///////////////////////////////////////////////////////////////////////////////
const double testOneVBOPerShape(const std::uint32_t _nNrOfShapes,
                                const std::uint32_t _nNrOfFrames,
                                const GLenum        _BufferUsage,
                                const GLenum        _Mode
                               )
{
    METHOD_ENTRY("testOneVBOPerShape")
    
    CGraphics& Graphics=CGraphics::getInstance();
    CTimer Timer;
    
    std::mt19937 Generator;
    Generator.seed(23479);
    std::uniform_real_distribution<float>  UniformDistribution(-0.3f, 0.3f);
    
    std::vector<float> vecShape(16);
    std::vector<float> vecColor(16);
    
    vecShape[0] = -0.4f; vecShape[1] = 0.1f; vecShape[2] = 0.0f;
    vecShape[3] =  0.4f; vecShape[4] = 0.1f; vecShape[5] = 0.0f;
    vecShape[6] =  0.0f; vecShape[7] = 0.7f; vecShape[8] = 0.0f;
    
    vecColor[0] = 1.0f; vecColor[1] = 0.0f; vecColor[2] = 0.0f;
    vecColor[3] = 0.0f; vecColor[4] = 1.0f; vecColor[5] = 0.0f;
    vecColor[6] = 0.0f; vecColor[7] = 0.0f; vecColor[8] = 1.0f;
    
    GLuint* punVBO = new GLuint[_nNrOfShapes*2];
    GLuint* punVAO = new GLuint[_nNrOfShapes];
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")

    INFO_MSG("GL Test", "Starting test with one VBO per shape")
    INFO(
        std::cout << "Draw mode:    ";
        if (_Mode == GL_TRIANGLES)
            std::cout << "GL_TRIANGLES" << std::endl;
        else if (_Mode == GL_LINE_LOOP)
            std::cout << "GL_LINE_LOOP" << std::endl;
        std::cout << "Buffer usage: ";
        if (_BufferUsage == GL_STATIC_DRAW)
            std::cout << "GL_STATIC_DRAW" << std::endl;
        else if (_BufferUsage == GL_DYNAMIC_DRAW)
            std::cout << "GL_DYNAMIC_DRAW" << std::endl;
        else if (_BufferUsage == GL_STREAM_DRAW)
            std::cout << "GL_STREAM_DRAW" << std::endl;
    )
    
    glGenBuffers(_nNrOfShapes*2, punVBO);
    glGenVertexArrays(_nNrOfShapes, punVAO);
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
        for (auto j=0u; j<_nNrOfShapes; ++j)
        {
            glBindVertexArray(punVAO[j]);
            
            vecShape[0] = -0.4f+UniformDistribution(Generator); vecShape[1] = -0.4f+UniformDistribution(Generator); vecShape[2] = 0.0f;
            vecShape[3] =  0.4f+UniformDistribution(Generator); vecShape[4] = -0.4f+UniformDistribution(Generator); vecShape[5] = 0.0f;
            vecShape[6] =  0.0f+UniformDistribution(Generator); vecShape[7] =  0.4f+UniformDistribution(Generator); vecShape[8] = 0.0f;
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecShape.size()*sizeof(float), &vecShape.front(), _BufferUsage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            vecColor[0] = 1.0f+UniformDistribution(Generator); vecColor[1] = 0.0f; vecColor[2] = 0.0f;
            vecColor[3] = 0.0f; vecColor[4] = 1.0f+UniformDistribution(Generator); vecColor[5] = 0.0f;
            vecColor[6] = 0.0f; vecColor[7] = 0.0f; vecColor[8] = 1.0f+UniformDistribution(Generator);

            glBindBuffer(GL_ARRAY_BUFFER, punVBO[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecColor.size()*sizeof(float), &vecColor.front(), _BufferUsage);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindVertexArray(punVAO[j]);
            glDrawArrays(_Mode, 0, 3);
        }
        Graphics.swapBuffers();
    }
    Timer.stop();
    double fTime = Timer.getTime();
    INFO_MSG("GL Test", "Drawn " << _nNrOfShapes << " triangles in " <<
                        _nNrOfFrames << " frames. Time: " << fTime << "s; " <<
                        _nNrOfFrames/fTime << "fps")
    
    // Clean up
    if (punVBO != nullptr)
    {
        delete[] punVBO;
        MEM_FREED("GLuint")
        punVBO = nullptr;
    }
    if (punVAO != nullptr)
    {
        delete[] punVAO;
        MEM_FREED("GLuint")
        punVAO = nullptr;
    }
    return Timer.getTime();
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

    testOneVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_LINE_LOOP);
}
