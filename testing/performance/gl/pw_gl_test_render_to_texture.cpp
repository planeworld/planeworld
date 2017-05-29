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
/// \date       2016-04-25
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_gl_test_render_to_texture.cpp
/// \brief      Main program for OpenGL render to texture tests
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-25
///
////////////////////////////////////////////////////////////////////////////////

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <iostream>
#include <random>

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"
#include "timer.h"
#include "render_target.h"

//--- Misc-Header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Usage to call program
///
///////////////////////////////////////////////////////////////////////////////
void usage()
{
    METHOD_ENTRY("usage")
    std::cout << "Usage: pw_gl_test_render_to_texture" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates random color for a shape of given size.
///
/// \param _pvecColor Color of shape to be created
/// \param _Generator Generator for random numbers
/// \param _Distribution Distribution of generated random numbers
///
///////////////////////////////////////////////////////////////////////////////
void createColor(std::vector<float>* const _pvecColor,
                 std::mt19937& _Generator, std::uniform_real_distribution<float>& _Distribution
)
{
    METHOD_ENTRY("createColor")
  
    (*_pvecColor)[0] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[1] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[2] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[3] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[4] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[5] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[6] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[7] = _Distribution(_Generator)*0.5f+0.5f;
    (*_pvecColor)[8] = _Distribution(_Generator)*0.5f+0.5f;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a shape of given size with random position.
///
/// \param _pvecShape Shape to be created
/// \param _Generator Generator for random numbers
/// \param _Distribution Distribution of generated random numbers
/// \param _fSizeMin  Minimum size of shapes
/// \param _fSizeMax  Maximum size of shapes
///
///////////////////////////////////////////////////////////////////////////////
void createShape(std::vector<float>* const _pvecShape,
                 std::mt19937& _Generator, std::uniform_real_distribution<float>& _Distribution,
                 float _fSizeMin, float _fSizeMax
)
{
    METHOD_ENTRY("createShape")
  
    float fPosX = _Distribution(_Generator)*300.0f;
    float fPosY = _Distribution(_Generator)*200.0f;
    
    (*_pvecShape)[0] = fPosX+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[1] = fPosY+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[2] = -10.0f;
    (*_pvecShape)[3] = fPosX+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[4] = fPosY+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[5] = -10.0f;
    (*_pvecShape)[6] = fPosX+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[7] = fPosY+_Distribution(_Generator)*((_fSizeMax-_fSizeMin)+_fSizeMin);
    (*_pvecShape)[8] = -10.0f;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output of test parameters
///
/// \param _nNrOfShapes Number of shapes to be drawn per frame
/// \param _nNrOfShapesPerGroup Number of shapes that share one VBO
/// \param _nNrOfFrames Number of frames to be drawn
/// \param _BufferUsage How the buffer is used (STATIC, DYNAMIC, STREAM)
/// \param _Mode        Which draw mode (TRIANGLE, LINE_LOOP)
///
///////////////////////////////////////////////////////////////////////////////
void outputTestParameters(const std::uint32_t _nNrOfShapes,
                          const std::uint32_t _nNrOfShapesPerGroup,
                          const std::uint32_t _nNrOfFrames,
                          const GLenum _BufferUsage,
                          const GLenum _Mode)
{
    METHOD_ENTRY("outputTestParameters")
    
    INFO_BLK(
        std::cout << "Draw mode:        ";
        if (_Mode == GL_TRIANGLES)
            std::cout << "GL_TRIANGLES" << std::endl;
        else if (_Mode == GL_LINE_LOOP)
            std::cout << "GL_LINE_LOOP" << std::endl;
        else if (_Mode == GL_TRIANGLE_STRIP)
            std::cout << "GL_TRIANGLE_STRIP" << std::endl;
        else if (_Mode == GL_TRIANGLE_FAN)
            std::cout << "GL_TRIANGLE_FAN" << std::endl;
        std::cout << "Buffer usage:     ";
        if (_BufferUsage == GL_STATIC_DRAW)
            std::cout << "GL_STATIC_DRAW" << std::endl;
        else if (_BufferUsage == GL_DYNAMIC_DRAW)
            std::cout << "GL_DYNAMIC_DRAW" << std::endl;
        else if (_BufferUsage == GL_STREAM_DRAW)
            std::cout << "GL_STREAM_DRAW" << std::endl;
        std::cout << "Number of frames: " << _nNrOfFrames << std::endl;
        std::cout << "Number of shapes: " << _nNrOfShapes << std::endl;
        std::cout << "  - per group:    " << _nNrOfShapesPerGroup << std::endl;
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output of test parameters
///
/// \param _nNrOfFrames Number of frames
/// \param _fTime       Processing time
///
///////////////////////////////////////////////////////////////////////////////
void outputTestResult(const std::uint32_t _nNrOfFrames,
                      const double fTime)
{
    METHOD_ENTRY("outputTestResult")
    
    INFO_BLK(
        std::cout << "Time [s]:         " << fTime <<  std::endl;
        std::cout << "Frequency [fps]:  " << "\033[1;32m" << _nNrOfFrames/fTime
                                          << "\033[0;37m" << std::endl;
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Test performance by using one vbo per for multiple shapes
///
/// \param _nNrOfShapes Number of shapes to be drawn per frame
/// \param _nNrOfShapesPerGroup Number of shapes that share one VBO
/// \param _nNrOfFrames Number of frames to be drawn
/// \param _BufferUsage How the buffer is used (STATIC, DYNAMIC, STREAM)
/// \param _Mode        Which draw mode (TRIANGLE, LINE_LOOP)
///
/// \return Time in seconds for this test
///
///////////////////////////////////////////////////////////////////////////////
const double testOneVBOPerMultipleShapes(const std::uint32_t _nNrOfShapes,
                                         const std::uint32_t _nNrOfShapesPerGroup,
                                         const std::uint32_t _nNrOfFrames,
                                         const GLenum        _BufferUsage,
                                         const GLenum        _Mode
                                        )
{
    METHOD_ENTRY("testOneVBOPerMultipleShapes")
    
    CGraphics& Graphics=CGraphics::getInstance();
    CTimer Timer;
    
    std::mt19937 Generator;
    Generator.seed(23479);
    std::uniform_real_distribution<float>  UniformDistribution(-1.0f, 1.0f);
    
    std::vector<float> vecShape(9);
    std::vector<float> vecColor(9);
    
    if (_nNrOfShapes % _nNrOfShapesPerGroup != 0)
    {
        ERROR_MSG("GL Test", "Number of shapes per group does not fit in number of shapes")
        return 0.0;
    }
    
    GLuint* punVBO = new GLuint[_nNrOfShapes / _nNrOfShapesPerGroup * 2];
    GLuint* punVAO = new GLuint[_nNrOfShapes / _nNrOfShapesPerGroup];
    GLuint* punIBO = new GLuint[_nNrOfShapes / _nNrOfShapesPerGroup];
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")

    outputTestParameters(_nNrOfShapes, _nNrOfShapesPerGroup, _nNrOfFrames, _BufferUsage, _Mode);
    
    glGenBuffers(_nNrOfShapes / _nNrOfShapesPerGroup, punIBO);
    glGenBuffers(_nNrOfShapes / _nNrOfShapesPerGroup * 2, punVBO);
    glGenVertexArrays(_nNrOfShapes / _nNrOfShapesPerGroup, punVAO);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(_nNrOfShapesPerGroup*3);

    std::vector<GLushort> vecIndices(_nNrOfShapesPerGroup*3+_nNrOfShapesPerGroup-1);
    {
        auto i=0u;
        auto k=0u;
        while (i<vecIndices.size())
        {
            for (auto j=0u; j<3; ++j)
            {
                vecIndices[i++] = k*3+j;
            }
            ++k;
            vecIndices[i++] = _nNrOfShapesPerGroup*3;
        }
    }
    
    CShader VertexShader;
    CShader FragmentShader;
    CShaderProgram ShaderProgram;
    
    VertexShader.load("shader.vert", GL_VERTEX_SHADER);
    FragmentShader.load("shader.frag", GL_FRAGMENT_SHADER);
    ShaderProgram.create();
    ShaderProgram.addShader(VertexShader);
    ShaderProgram.addShader(FragmentShader);
    ShaderProgram.link();
    ShaderProgram.use();
    
    glm::mat4 matProjection = glm::ortho<float>(
        Graphics.getViewPort().leftplane, Graphics.getViewPort().rightplane,
        Graphics.getViewPort().bottomplane, Graphics.getViewPort().topplane,
        Graphics.getViewPort().nearplane, Graphics.getViewPort().farplane);
    GLint nProjMatLoc=glGetUniformLocation(ShaderProgram.getID(), "matTransform");
    glUniformMatrix4fv(nProjMatLoc, 1, GL_FALSE, glm::value_ptr(matProjection));
    
    CShader         TextureToScreenShader;
    CShader         PassTroughShader;
    CShaderProgram  TextureToScreenShaderProgram;
    
    TextureToScreenShader.load("render_to_screen.frag", GL_FRAGMENT_SHADER);
    PassTroughShader.load("render_to_screen.vert", GL_VERTEX_SHADER);
    TextureToScreenShaderProgram.create();
    TextureToScreenShaderProgram.addShader(TextureToScreenShader);
    TextureToScreenShaderProgram.addShader(PassTroughShader);
    TextureToScreenShaderProgram.link();
    TextureToScreenShaderProgram.use();
    
    glm::mat4 matProjection2 = glm::perspective(90.0f, float(Graphics.getWidthScr()/Graphics.getHeightScr()), 10.0f, 1000.0f) *
                               glm::lookAt(glm::vec3(0,0,750), glm::vec3(0,0,0), glm::vec3(0,1,0));
    GLuint nProjMatLoc2=glGetUniformLocation(TextureToScreenShaderProgram.getID(), "matProjection");
    glUniformMatrix4fv(nProjMatLoc2, 1, GL_FALSE, glm::value_ptr(matProjection2));
    GLuint texID = glGetUniformLocation(TextureToScreenShaderProgram.getID(), "RenderedTexture");
    
    CRenderTarget RenderTarget;
    RenderTarget.init(Graphics.getWidthScr(), Graphics.getHeightScr());
    RenderTarget.setTarget(Graphics.getViewPort().leftplane,  Graphics.getViewPort().bottomplane,
                           Graphics.getViewPort().rightplane, Graphics.getViewPort().bottomplane,
                           Graphics.getViewPort().rightplane, Graphics.getViewPort().topplane,
                           Graphics.getViewPort().leftplane,  Graphics.getViewPort().topplane);
    
    GLuint unUVBuffer;
    glGenBuffers(1, &unUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, unUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, RenderTarget.getTexUV().size() * sizeof(GLfloat), &(RenderTarget.getTexUV().front()), GL_STATIC_DRAW);
    
    GLuint unVBOScreenQuad;
    glGenBuffers(1, &unVBOScreenQuad);
    glBindBuffer(GL_ARRAY_BUFFER, unVBOScreenQuad);
    glBufferData(GL_ARRAY_BUFFER, RenderTarget.getQuad().size()*sizeof(GLfloat), &(RenderTarget.getQuad().front()), GL_STATIC_DRAW);
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
        ShaderProgram.use();
        
        RenderTarget.bind();
        glViewport(0, 0, Graphics.getWidthScr(), Graphics.getHeightScr());
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto k=0u; k< _nNrOfShapes / _nNrOfShapesPerGroup; ++k)
        {
            glBindVertexArray(punVAO[k]);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[0+k*2]);
            glBufferData(GL_ARRAY_BUFFER, _nNrOfShapesPerGroup * vecShape.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[1+k*2]);
            glBufferData(GL_ARRAY_BUFFER, _nNrOfShapesPerGroup * vecColor.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, punIBO[k]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size()*sizeof(GLushort), nullptr, _BufferUsage);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[0+k*2]);
            for (auto j=0u; j<_nNrOfShapesPerGroup; ++j)
            {
                createShape(&vecShape, Generator, UniformDistribution, 5.0f, 20.0f);
                
                glBufferSubData(GL_ARRAY_BUFFER, j*vecShape.size()*sizeof(float),
                                                   vecShape.size()*sizeof(float), &vecShape.front());
            }
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[1+k*2]);
            for (auto j=0u; j<_nNrOfShapesPerGroup; ++j)
            {
                createColor(&vecColor, Generator, UniformDistribution);
                
                glBufferSubData(GL_ARRAY_BUFFER, j*vecColor.size()*sizeof(float),
                                                   vecColor.size()*sizeof(float), &vecColor.front());
            }
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindVertexArray(punVAO[k]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, punIBO[k]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size()*sizeof(GLushort), &vecIndices.front(), _BufferUsage);
            glDrawElements(_Mode, vecIndices.size(), GL_UNSIGNED_SHORT, 0);
        }
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        RenderTarget.unbind();
        
        glViewport(0, 0, Graphics.getWidthScr(), Graphics.getHeightScr());
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        TextureToScreenShaderProgram.use();
        
        // Bind texture in texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, RenderTarget.getIDTex());
        glUniform1i(texID, 0);
        
        // Draw quad
        glBindBuffer(GL_ARRAY_BUFFER, unVBOScreenQuad);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, unUVBuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        Graphics.swapBuffers();
       

    }
    Timer.stop();
    double fTime = Timer.getTime();
    outputTestResult(_nNrOfFrames, fTime);
    
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
    if (punIBO != nullptr)
    {
        delete[] punIBO;
        MEM_FREED("GLuint")
        punIBO = nullptr;
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
 
    WindowHandleType* pWindow = new WindowHandleType(sf::VideoMode(Graphics.getWidthScr(), Graphics.getHeightScr()),
                                                    "Planeworld", sf::Style::Default,
                                                    sf::ContextSettings(24,8,4,3,3,sf::ContextSettings::Core));
    MEM_ALLOC("WindowHandleType")
    
    Graphics.setWindow(pWindow);
    
    INFO_MSG("GL Test", "Starting test with one VBO per multiple shapes")
    testOneVBOPerMultipleShapes(1000, 10, 1000, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneVBOPerMultipleShapes(1000, 10, 1000, GL_STREAM_DRAW, GL_LINE_LOOP);

    if (pWindow != nullptr)
    {
        delete pWindow;
        MEM_FREED("WindowHandleType")
        pWindow = nullptr;
    }
}
