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
/// \file       pw_gl_test_buffers.cpp
/// \brief      Main program for OpenGL performance tests
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
#include "timer.h"

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
    std::cout << "Usage: pw_gl_test_buffers" << std::endl;
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
    
    INFO(
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
    
    INFO(
        std::cout << "Time [s]:         " << fTime <<  std::endl;
        std::cout << "Frequency [fps]:  " << "\033[1;32m" << _nNrOfFrames/fTime
                                          << "\033[0;37m" << std::endl;
    )
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
    std::uniform_real_distribution<float>  UniformDistribution(-1.0f, 1.0f);
    
    std::vector<float> vecShape(9);
    std::vector<float> vecColor(9);
    
    GLuint* punVBO = new GLuint[_nNrOfShapes*2];
    GLuint* punVAO = new GLuint[_nNrOfShapes];
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")

    outputTestParameters(_nNrOfShapes, 1u, _nNrOfFrames, _BufferUsage, _Mode);
    
    glGenBuffers(_nNrOfShapes*2, punVBO);
    glGenVertexArrays(_nNrOfShapes, punVAO);
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
        for (auto j=0u; j<_nNrOfShapes; ++j)
        {
            createShape(&vecShape, Generator, UniformDistribution, 0.5f, 20.0f);
            createColor(&vecColor, Generator, UniformDistribution);
          
            glBindVertexArray(punVAO[j]);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecShape.size()*sizeof(float), &vecShape.front(), _BufferUsage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecColor.size()*sizeof(float), &vecColor.front(), _BufferUsage);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindVertexArray(punVAO[j]);
            glDrawArrays(_Mode, 0, 3);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecShape.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ARRAY_BUFFER, punVBO[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecColor.size()*sizeof(float), nullptr, _BufferUsage);
        }
        
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
    return Timer.getTime();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Test performance by using one buffered vbo per shape
///
/// \param _nNrOfShapes Number of shapes to be drawn per frame
/// \param _nNrOfFrames Number of frames to be drawn
/// \param _BufferUsage How the buffer is used (STATIC, DYNAMIC, STREAM)
/// \param _Mode        Which draw mode (TRIANGLE, LINE_LOOP)
///
/// \return Time in seconds for this test
///
///////////////////////////////////////////////////////////////////////////////
const double testOneBufferedVBOPerShape(const std::uint32_t _nNrOfShapes,
                                const std::uint32_t _nNrOfFrames,
                                const GLenum        _BufferUsage,
                                const GLenum        _Mode
                               )
{
    METHOD_ENTRY("testOneBufferedVBOPerShape")
    
    CGraphics& Graphics=CGraphics::getInstance();
    CTimer Timer;
    
    std::mt19937 Generator;
    Generator.seed(23479);
    std::uniform_real_distribution<float>  UniformDistribution(-1.0f, 1.0f);
    
    std::vector<float> vecShape(9);
    std::vector<float> vecColor(9);
    
    GLuint* punVBO1 = new GLuint[_nNrOfShapes*2];
    GLuint* punVAO1 = new GLuint[_nNrOfShapes];
    GLuint* punVBO2 = new GLuint[_nNrOfShapes*2];
    GLuint* punVAO2 = new GLuint[_nNrOfShapes];
    GLuint* punVBOFront = new GLuint[_nNrOfShapes*2];
    GLuint* punVBOBack = new GLuint[_nNrOfShapes*2];
    GLuint* punVAOFront = new GLuint[_nNrOfShapes];
    GLuint* punVAOBack = new GLuint[_nNrOfShapes];
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")

    outputTestParameters(_nNrOfShapes, 1u, _nNrOfFrames, _BufferUsage, _Mode);
    
    glGenBuffers(_nNrOfShapes*2, punVBO1);
    glGenVertexArrays(_nNrOfShapes, punVAO1);
    glGenBuffers(_nNrOfShapes*2, punVBO2);
    glGenVertexArrays(_nNrOfShapes, punVAO2);
    
    for (auto i=0u; i<_nNrOfShapes; ++i)
    {
        punVAOFront[i] = punVAO1[i];
        punVAOBack[i] = punVAO2[i];
        punVBOFront[0+i*2] = punVBO1[0+i*2];
        punVBOFront[1+i*2] = punVBO1[1+i*2];
        punVBOBack[0+i*2] = punVBO2[0+i*2];
        punVBOBack[1+i*2] = punVBO2[1+i*2];
    }
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
        for (auto j=0u; j<_nNrOfShapes; ++j)
        {
            createShape(&vecShape, Generator, UniformDistribution, 0.5f, 20.0f);
            createColor(&vecColor, Generator, UniformDistribution);
          
            glBindVertexArray(punVAOBack[j]);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecShape.size()*sizeof(float), &vecShape.front(), _BufferUsage);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecColor.size()*sizeof(float), &vecColor.front(), _BufferUsage);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindVertexArray(punVAOFront[j]);
            glDrawArrays(_Mode, 0, 3);
            
            std::swap(punVAOFront[j], punVAOBack[j]);
            std::swap(punVBOFront[0+j*2], punVBOBack[0+j*2]);
            std::swap(punVBOFront[1+j*2], punVBOBack[1+j*2]);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecShape.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, vecColor.size()*sizeof(float), nullptr, _BufferUsage);
        }
        Graphics.swapBuffers();
    }
    Timer.stop();
    double fTime = Timer.getTime();
    outputTestResult(_nNrOfFrames, fTime);
    
    // Clean up
    if (punVBO1 != nullptr)
    {
        delete[] punVBO1;
        MEM_FREED("GLuint")
        punVBO1 = nullptr;
    }
    if (punVAO1 != nullptr)
    {
        delete[] punVAO1;
        MEM_FREED("GLuint")
        punVAO1 = nullptr;
    }
    if (punVBO2 != nullptr)
    {
        delete[] punVBO2;
        MEM_FREED("GLuint")
        punVBO2 = nullptr;
    }
    if (punVAO2 != nullptr)
    {
        delete[] punVAO2;
        MEM_FREED("GLuint")
        punVAO2 = nullptr;
    }
    if (punVBOFront != nullptr)
    {
        delete[] punVBOFront;
        MEM_FREED("GLuint")
        punVBOFront = nullptr;
    }
    if (punVAOFront != nullptr)
    {
        delete[] punVAOFront;
        MEM_FREED("GLuint")
        punVAOFront = nullptr;
    }
    if (punVBOBack != nullptr)
    {
        delete[] punVBOBack;
        MEM_FREED("GLuint")
        punVBOBack = nullptr;
    }
    if (punVAOBack != nullptr)
    {
        delete[] punVAOBack;
        MEM_FREED("GLuint")
        punVAOBack = nullptr;
    }
    return Timer.getTime();
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
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
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
                createShape(&vecShape, Generator, UniformDistribution, 0.5f, 20.0f);
                
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
/// \brief Test performance by using one buffered vbo per shape
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
const double testOneBufferedVBOPerMultipleShapes(const std::uint32_t _nNrOfShapes,
                                const std::uint32_t _nNrOfShapesPerGroup,
                                const std::uint32_t _nNrOfFrames,
                                const GLenum        _BufferUsage,
                                const GLenum        _Mode
                               )
{
    METHOD_ENTRY("testOneBufferedVBOPerMultipleShapes")
    
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
    
    GLuint* punVBO1 = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup*2];
    GLuint* punVAO1 = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup];
    GLuint* punVBO2 = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup*2];
    GLuint* punVAO2 = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup];
    GLuint* punVBOFront = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup*2];
    GLuint* punVBOBack = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup*2];
    GLuint* punVAOFront = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup];
    GLuint* punVAOBack = new GLuint[_nNrOfShapes/_nNrOfShapesPerGroup];
    GLuint* punIBO = new GLuint[_nNrOfShapes / _nNrOfShapesPerGroup];
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")
    MEM_ALLOC("GLuint")

    outputTestParameters(_nNrOfShapes, _nNrOfShapesPerGroup, _nNrOfFrames, _BufferUsage, _Mode);
    
    glGenBuffers(_nNrOfShapes/_nNrOfShapesPerGroup*2, punVBO1);
    glGenVertexArrays(_nNrOfShapes/_nNrOfShapesPerGroup, punVAO1);
    glGenBuffers(_nNrOfShapes/_nNrOfShapesPerGroup*2, punVBO2);
    glGenVertexArrays(_nNrOfShapes/_nNrOfShapesPerGroup, punVAO2);
    glGenBuffers(_nNrOfShapes/_nNrOfShapesPerGroup, punIBO);
    
    for (auto i=0u; i<_nNrOfShapes/_nNrOfShapesPerGroup; ++i)
    {
        punVAOFront[i] = punVAO1[i];
        punVAOBack[i] = punVAO2[i];
        punVBOFront[0+i*2] = punVBO1[0+i*2];
        punVBOFront[1+i*2] = punVBO1[1+i*2];
        punVBOBack[0+i*2] = punVBO2[0+i*2];
        punVBOBack[1+i*2] = punVBO2[1+i*2];
    }
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
    
    Timer.start();
    for (auto i=0u; i<_nNrOfFrames; ++i)
    {
        for (auto j=0u; j<_nNrOfShapes/_nNrOfShapesPerGroup; ++j)
        {
            glBindVertexArray(punVAOBack[j]);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[0+j*2]);
            glBufferData(GL_ARRAY_BUFFER, _nNrOfShapesPerGroup * vecShape.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[1+j*2]);
            glBufferData(GL_ARRAY_BUFFER, _nNrOfShapesPerGroup * vecColor.size()*sizeof(float), nullptr, _BufferUsage);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, punIBO[j]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nNrOfShapesPerGroup*sizeof(GLushort), nullptr, _BufferUsage);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[0+j*2]);
            for (auto k=0u; k<_nNrOfShapesPerGroup; ++k)
            {
                createShape(&vecShape, Generator, UniformDistribution, 0.5f, 20.0f);

                glBufferSubData(GL_ARRAY_BUFFER, k*vecShape.size()*sizeof(float),
                                                   vecShape.size()*sizeof(float), &vecShape.front());
            }
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, punVBOBack[1+j*2]);
            for (auto k=0u; k<_nNrOfShapesPerGroup; ++k)
            {
                createColor(&vecColor, Generator, UniformDistribution);
                
                glBufferSubData(GL_ARRAY_BUFFER, k*vecColor.size()*sizeof(float),
                                                   vecColor.size()*sizeof(float), &vecColor.front());
            }
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindVertexArray(punVAOBack[j]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, punIBO[j]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vecIndices.size()*sizeof(GLushort), &vecIndices.front(), _BufferUsage);
            glDrawElements(_Mode, vecIndices.size(), GL_UNSIGNED_SHORT, 0);
            
            std::swap(punVAOFront[j], punVAOBack[j]);
            std::swap(punVBOFront[0+j*2], punVBOBack[0+j*2]);
            std::swap(punVBOFront[1+j*2], punVBOBack[1+j*2]);
        }
        Graphics.swapBuffers();
    }
    Timer.stop();
    double fTime = Timer.getTime();
    outputTestResult(_nNrOfFrames, fTime);
    
    // Clean up
    if (punVBO1 != nullptr)
    {
        delete[] punVBO1;
        MEM_FREED("GLuint")
        punVBO1 = nullptr;
    }
    if (punVAO1 != nullptr)
    {
        delete[] punVAO1;
        MEM_FREED("GLuint")
        punVAO1 = nullptr;
    }
    if (punVBO2 != nullptr)
    {
        delete[] punVBO2;
        MEM_FREED("GLuint")
        punVBO2 = nullptr;
    }
    if (punVAO2 != nullptr)
    {
        delete[] punVAO2;
        MEM_FREED("GLuint")
        punVAO2 = nullptr;
    }
    if (punVBOFront != nullptr)
    {
        delete[] punVBOFront;
        MEM_FREED("GLuint")
        punVBOFront = nullptr;
    }
    if (punVAOFront != nullptr)
    {
        delete[] punVAOFront;
        MEM_FREED("GLuint")
        punVAOFront = nullptr;
    }
    if (punVBOBack != nullptr)
    {
        delete[] punVBOBack;
        MEM_FREED("GLuint")
        punVBOBack = nullptr;
    }
    if (punVAOBack != nullptr)
    {
        delete[] punVAOBack;
        MEM_FREED("GLuint")
        punVAOBack = nullptr;
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
    
    INFO_MSG("GL Test", "Starting test with one VBO per shape")
    testOneVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_LINE_LOOP);
    INFO_MSG("GL Test", "Starting test with one buffered VBO per shape")
    testOneBufferedVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerShape(1000, 100, GL_STATIC_DRAW, GL_LINE_LOOP);
    testOneBufferedVBOPerShape(1000, 100, GL_DYNAMIC_DRAW, GL_LINE_LOOP);
    testOneBufferedVBOPerShape(1000, 100, GL_STREAM_DRAW, GL_LINE_LOOP);
    INFO_MSG("GL Test", "Starting test with one VBO per multiple shapes")
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_STATIC_DRAW, GL_TRIANGLES);
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_DYNAMIC_DRAW, GL_TRIANGLES);
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_STATIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_DYNAMIC_DRAW, GL_LINE_LOOP);
    testOneVBOPerMultipleShapes(1000, 100, 100, GL_STREAM_DRAW, GL_LINE_LOOP);
    INFO_MSG("GL Test", "Starting test with one VBO per multiple buffered shapes")
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_STATIC_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_DYNAMIC_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_STREAM_DRAW, GL_TRIANGLES);
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_STATIC_DRAW, GL_LINE_LOOP);
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_DYNAMIC_DRAW, GL_LINE_LOOP);
    testOneBufferedVBOPerMultipleShapes(1000, 100, 100, GL_STREAM_DRAW, GL_LINE_LOOP);
    
    if (pWindow != nullptr)
    {
        delete pWindow;
        MEM_FREED("WindowHandleType")
        pWindow = nullptr;
    }
}
