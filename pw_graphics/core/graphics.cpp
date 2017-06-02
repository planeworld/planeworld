////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       graphics.cpp
/// \brief      Implementation of class "CGraphics"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "graphics.h"

#include "conf_pw.h"
#include "math_constants.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief constructor, initialising members.
///
/// the constructor is private, this is because of the implementation as a
/// meyers-singleton.
///
///////////////////////////////////////////////////////////////////////////////
CGraphics::CGraphics() : m_pWindow(nullptr),
                        m_aColour({{1.0, 1.0, 1.0, 1.0}}),
                        m_fCamAng(0.0),
                        m_fCamZoom(1.0),
                        m_fDepth(GRAPHICS_DEPTH_DEFAULT),
                        m_fDynPelSize(GRAPHICS_DYN_PEL_SIZE_DEFAULT),
                        m_nVideoFlags(0),
                        m_unWidthScr(GRAPHICS_WIDTH_DEFAULT),
                        m_unHeightScr(GRAPHICS_HEIGHT_DEFAULT)
{
    METHOD_ENTRY("CGraphics::CGraphics")
    CTOR_CALL("CGraphics::CGraphics")
    
    m_vecCamPos.setZero();
    m_CosCache.resize(GRAPHICS_MAX_CACHE_SIZE);
    m_SinCache.resize(GRAPHICS_MAX_CACHE_SIZE);
    m_vecIndicesLines.resize(m_unIndexMax);
    m_vecIndicesPoints.resize(m_unIndexMax);
    m_vecIndicesTriangles.resize(m_unIndexMax);
    
    m_vecColours.resize(m_unIndexMax);
    m_vecVertices.resize(m_unIndexMax);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, frees memory.
///
/// The destructor is private, this is because of the implementation as a
/// meyers-singleton.
///
///////////////////////////////////////////////////////////////////////////////
CGraphics::~CGraphics()
{
    METHOD_ENTRY("CGraphics::CGraphics")
    DTOR_CALL("CGraphics::CGraphics")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reprojection of coordinate to world
///
/// \param _vecV Coordinate on screen
///
/// \return Position in world coordinates
///
///////////////////////////////////////////////////////////////////////////////
Vector2d CGraphics::screen2World(const Vector2d& _vecV) const
{
    METHOD_ENTRY("CGraphics::screen2World")

    Vector2d vecResult;
    
    double fAtan;
    double fL;
    double fX;
    double fY;
        
    fX = ((m_ViewPort.rightplane-m_ViewPort.leftplane) / m_unWidthScr * _vecV[0]+
                    m_ViewPort.leftplane) / m_fCamZoom;
    fY = ((m_ViewPort.topplane-m_ViewPort.bottomplane) / m_unHeightScr * _vecV[1]+
                    m_ViewPort.bottomplane) /  m_fCamZoom;
    
    fL = sqrt(fX*fX+fY*fY);
    fAtan = atan2(fX,fY);
    
    vecResult[0] = fL*cos(fAtan - (MATH_PI2-m_fCamAng))+ m_vecCamPos[0];
    vecResult[1] = fL*sin(fAtan - (MATH_PI2-m_fCamAng))- m_vecCamPos[1];

    return vecResult;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reprojection of coordinate to world
///
/// \param _fX X-coordinate on screen
/// \param _fY Y-coordinate on screen
///
/// \return Position in world coordinates
///
///////////////////////////////////////////////////////////////////////////////
Vector2d CGraphics::screen2World(const double& _fX, const double& _fY) const
{
    METHOD_ENTRY("CGraphics::screen2World")

    Vector2d vecResult;
    
    double fAtan;
    double fL;
    double fX;
    double fY;
        
    fX = ((m_ViewPort.rightplane-m_ViewPort.leftplane) / m_unWidthScr * _fX +
           m_ViewPort.leftplane) / m_fCamZoom;
    fY = ((m_ViewPort.topplane-m_ViewPort.bottomplane) / m_unHeightScr * _fY +
           m_ViewPort.bottomplane) /  m_fCamZoom;
    
    fL = sqrt(fX*fX+fY*fY);
    fAtan = atan2(fX,fY);
    
    vecResult[0] = fL*cos(fAtan - (MATH_PI2-m_fCamAng))+ m_vecCamPos[0];
    vecResult[1] = fL*sin(fAtan - (MATH_PI2-m_fCamAng))- m_vecCamPos[1];

    return vecResult;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reprojection of world coordinate to a pixel position
///
/// \param _vecV Position in world coordinates
///
/// \return Position in screen coordinates
///
///////////////////////////////////////////////////////////////////////////////
Vector2d CGraphics::world2Screen(const Vector2d& _vecV) const
{
    METHOD_ENTRY("CGraphics::world2Screen")

    Rotation2Dd Rot(m_fCamAng);
    
    return (Rot*Vector2d(_vecV[0],-_vecV[1])*m_fCamZoom-Vector2d(m_ViewPort.leftplane,-m_ViewPort.topplane))
            *m_unWidthScr/(m_ViewPort.rightplane-m_ViewPort.leftplane);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Cache sine and cosine values for circle calculations
///
/// \param _nSeg Number of segment per circle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::cacheSinCos(const int _nSeg) 
{
    METHOD_ENTRY("CGraphics::cacheSinCos")
    
    for (int i=0; i<_nSeg+1; ++i)
    {
        m_CosCache[i] =  std::cos(double(i)*MATH_2PI/_nSeg);
        m_SinCache[i] =  std::sin(double(i)*MATH_2PI/_nSeg);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Clear and swap videobuffers
///
/// This method swaps videobuffers and clears offscreen buffers afterwards.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::swapBuffers() 
{
    METHOD_ENTRY("CGraphics::swapBuffers")
    
    glBindVertexArray(m_unVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexVerts*sizeof(GLfloat), &(m_vecVertices.front()), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexCol*sizeof(GLfloat), &(m_vecColours.front()), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexLines*sizeof(GLuint), &m_vecIndicesLines.front(), GL_STREAM_DRAW);
    glDrawElements(GL_LINES, m_vecIndicesLines.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexPoints*sizeof(GLuint), &m_vecIndicesPoints.front(), GL_STREAM_DRAW);
    glDrawElements(GL_POINTS, m_vecIndicesPoints.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexTriangles*sizeof(GLuint), &m_vecIndicesTriangles.front(), GL_STREAM_DRAW);
    glDrawElements(GL_TRIANGLES, m_vecIndicesTriangles.size(), GL_UNSIGNED_INT, 0);
    
    m_pWindow->display();

    // Reserve gpu memory for all relevant buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_unVAO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexLines*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexPoints*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexTriangles*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    m_unIndex = 0u;
    m_unIndexVerts = 0u;
    m_unIndexCol = 0u;
    m_unIndexLines = 0u;
    m_unIndexPoints = 0u;
    m_unIndexTriangles = 0u;

    // clear offscreen buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets projection matrix to screen space 
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::switchToScreenSpace() 
{
    METHOD_ENTRY("CGraphics::switchToScreenSpace")
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_unWidthScr, m_unHeightScr, 0,
            m_ViewPort.nearplane, m_ViewPort.farplane);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets projection matrix to screen space 
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::switchToWorldSpace() 
{
    METHOD_ENTRY("CGraphics::switchToWorldSpace")
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_ViewPort.leftplane, m_ViewPort.rightplane,
            m_ViewPort.bottomplane, m_ViewPort.topplane,
            m_ViewPort.nearplane, m_ViewPort.farplane);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes graphics
///
/// This method initialises the graphics by calling some OpenGL-routines
///
/// \return Success
///
///////////////////////////////////////////////////////////////////////////////
bool CGraphics::init()
{
    METHOD_ENTRY("CGraphics::init")
    
    //--------------------------------------------------------------------------
    // Initialize window and graphics
    //--------------------------------------------------------------------------
    m_pWindow->setMouseCursorVisible(false);
    m_pWindow->setVerticalSyncEnabled(false);
    DOM_VAR(INFO_MSG("Graphics", "Found OpenGL version: " << m_pWindow->getSettings().majorVersion << "." << m_pWindow->getSettings().minorVersion))
    DOM_VAR(INFO_MSG("Graphics", "Antialiasing level: " << m_pWindow->getSettings().antialiasingLevel))
    DOM_VAR(INFO_MSG("Graphics", "Depth Buffer Bits: " << m_pWindow->getSettings().depthBits))
    DOM_VAR(INFO_MSG("Graphics", "Stencil Buffer Bits: " << m_pWindow->getSettings().stencilBits))
    DOM_VAR(INFO_MSG("Graphics", "Core Profile (1): " << m_pWindow->getSettings().attributeFlags))

    //--------------------------------------------------------------------------
    // Setup OpenGL
    //--------------------------------------------------------------------------
    CShader VertexShader;
    CShader FragmentShader;
    VertexShader.load("shader.vert", GL_VERTEX_SHADER);
    FragmentShader.load("shader.frag", GL_FRAGMENT_SHADER);
    
    m_ShaderProgram.create();
    m_ShaderProgram.addShader(VertexShader);
    m_ShaderProgram.addShader(FragmentShader);
    m_ShaderProgram.link();
    m_ShaderProgram.use();
    
    m_matProjection = glm::ortho<float>(m_ViewPort.leftplane, m_ViewPort.rightplane,
            m_ViewPort.bottomplane, m_ViewPort.topplane,
            m_ViewPort.nearplane, m_ViewPort.farplane);
    GLint nProjMatLoc=glGetUniformLocation(m_ShaderProgram.getID(), "matProjection");
    glUniformMatrix4fv(nProjMatLoc, 1, GL_FALSE, glm::value_ptr(m_matProjection));
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
//     glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    // Test for depthbuffer and enable if possible
//     glEnable(GL_DEPTH_TEST);
//     if (!glIsEnabled(GL_DEPTH_TEST))
//     {
//         WARNING_MSG("OpenGL", "Could not enable depthbuffer.")
//     }
//     else
//     {
//         INFO_MSG("OpenGL", "Enabling depthbuffer.")
//     }
    
    //--------------------------------------------------------------------------
    // Prepare OpenGL buffers (VBO, VAO, IBO, FBO)
    //--------------------------------------------------------------------------
    glGenBuffers(1, &m_unVBO);
    glGenBuffers(1, &m_unVBOColours);
    glGenBuffers(1, &m_unIBOLines);
    glGenBuffers(1, &m_unIBOPoints);
    glGenBuffers(1, &m_unIBOTriangles);
    glGenVertexArrays(1, &m_unVAO);
    
    // Reserve GPU memory for all relevant buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_unVAO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexLines*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    #ifdef PW_MULTITHREADING
        m_pWindow->setActive(false);
    #endif
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexPoints*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_unIndexTriangles*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    // Clear buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Prepare graphics for new resolution
///
/// This method reinitialises the OpenGL-matrices for the new resolution.
///
/// \param _unWidthScr New x-resolution
/// \param _unHeightScr New y-resolution
///
/// \return Success
///
///////////////////////////////////////////////////////////////////////////////
bool CGraphics::resizeWindow(unsigned short _unWidthScr, unsigned short _unHeightScr)
{
    METHOD_ENTRY("CGraphics::resizeWindow")
    
    sf::FloatRect View(0,0,_unWidthScr,_unHeightScr);
    m_pWindow->setView(sf::View(View));
    m_pWindow->setSize(sf::Vector2u(_unWidthScr, _unHeightScr));

    m_ViewPort.rightplane = double(_unWidthScr  * (0.5 / GRAPHICS_PX_PER_METER));
    m_ViewPort.topplane   = double(_unHeightScr * (0.5 / GRAPHICS_PX_PER_METER));
    m_ViewPort.leftplane   = -m_ViewPort.rightplane;
    m_ViewPort.bottomplane = -m_ViewPort.topplane;
    
    m_matProjection = glm::ortho<float>(m_ViewPort.leftplane, m_ViewPort.rightplane,
            m_ViewPort.bottomplane, m_ViewPort.topplane,
            m_ViewPort.nearplane, m_ViewPort.farplane);
    GLint nProjMatLoc=glGetUniformLocation(m_ShaderProgram.getID(), "matProjection");
    glUniformMatrix4fv(nProjMatLoc, 1, GL_FALSE, glm::value_ptr(m_matProjection));
    
    // Store resolution
    m_unWidthScr = _unWidthScr;
    m_unHeightScr = _unHeightScr;
    
    INFO_MSG("Graphics", "Viewport changed to " << m_ViewPort.rightplane - m_ViewPort.leftplane << "m x " <<
                                                   m_ViewPort.topplane   - m_ViewPort.bottomplane << "m (" <<
                                                   _unWidthScr << "x" << _unHeightScr << ").")
    
    return (true);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Apply the camera movement
///
/// This method applies the movement of the camera, that is rotation and
/// translation, to OpenGL. If camera is rotated clockwise, the world must be
/// rotated counter clockwise.
/// The matrices are multiplied, so the first matrix will be the last operation.
/// This means, a vertex is translated, rotated and scaled in the end.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::applyCamMovement()
{
    METHOD_ENTRY("CGraphics::applyCamMovement")

    m_matScale = glm::scale(glm::vec3(GLfloat(m_fCamZoom), GLfloat(m_fCamZoom), 1.0f));
    m_matRotate = glm::rotate(float(-m_fCamAng), glm::vec3(0.0f, 0.0f, 1.0f));
    m_matTransform = m_matProjection * m_matScale * m_matRotate;

    GLint nTransMatLoc=glGetUniformLocation(m_ShaderProgram.getID(), "matTransform");
    glUniformMatrix4fv(nTransMatLoc, 1, GL_FALSE, glm::value_ptr(m_matTransform));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief reset camera position and orientation
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::resetCam()
{
    METHOD_ENTRY("CGraphics::resetCam")

    m_fCamZoom = 1.0;
    m_fCamAng = 0.0;
    m_vecCamPos.setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotate camera clockwise by given angle
///
/// \note Angles are interpreted mathematically positive which means
///       counter clockwise!
///
/// \param _fInc incremental camera angle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rotCamBy(const double& _fInc)
{
    METHOD_ENTRY("CGraphics::rotCamBy")

    m_fCamAng += _fInc;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotate camera clockwise to given angle
///
/// \note Angles are interpreted mathematically positive which means
///       counter clockwise!
///
/// \param _fAng camera angle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rotCamTo(const double& _fAng)
{
    METHOD_ENTRY("CGraphics::rotCamTo")

    m_fCamAng = _fAng;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Move camera position
///
/// The world is translated and then rotated with negative camera angle.
/// Therefore, further translations must be made with respect to positive
/// camera angle.
///
/// \param _vecInc incremental movement in x-,y- and z-direction
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::transCamBy(const Vector2d& _vecInc)
{
    METHOD_ENTRY("CGraphics::transCamBy")

    Rotation2Dd Rotation(m_fCamAng);

    m_vecCamPos.segment<2>(0) += Rotation * _vecInc;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Move camera to given position
///
/// \param _vecPos position in x-,y- and z-direction
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::transCamTo(const Vector2d& _vecPos)
{
    METHOD_ENTRY("CGraphics::transCamTo")

    Rotation2Dd Rotation(m_fCamAng);
    
    m_vecCamPos.segment<2>(0) = Rotation * _vecPos;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zoom camera
///
/// \param _fFac multiplicational camera zoom
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::zoomCamBy(const double& _fFac)
{
    METHOD_ENTRY("CGraphics::zoomCamBy")

    m_fCamZoom *= _fFac;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zoom camera to given factor
///
/// \param _fFac Camera zoom-factor
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::zoomCamTo(const double& _fFac)
{
    METHOD_ENTRY("CGraphics::zoomCamTo")

    m_fCamZoom = _fFac;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a circle
///
/// \param _vecC    Center of circle
/// \param _fR      Radius of circle
/// \param _nNrOfSeg Number of segments
/// \param _bCache  Flag if sine/cosine cache should be used
///                 (call \ref cacheSinCos before).
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::circle(const Vector2d& _vecC, const double& _fR,
                                              const int _nNrOfSeg,
                                              const bool _bCache)
{
    METHOD_ENTRY("CGraphics::circle")

    if (_bCache)
    {
        m_vecVertices[m_unIndexVerts++] = _vecC[0]+m_SinCache[0]*_fR;
        m_vecVertices[m_unIndexVerts++] = _vecC[1]+m_CosCache[0]*_fR;
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        m_unIndex++;
        
        for (int i=1; i<_nNrOfSeg; ++i)
        {
            m_vecVertices[m_unIndexVerts++] = _vecC[0]+m_SinCache[i]*_fR;
            m_vecVertices[m_unIndexVerts++] = _vecC[1]+m_CosCache[i]*_fR;
            m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
            m_vecColours[m_unIndexCol++] = m_aColour[0];
            m_vecColours[m_unIndexCol++] = m_aColour[1];
            m_vecColours[m_unIndexCol++] = m_aColour[2];
            m_vecColours[m_unIndexCol++] = m_aColour[3];
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex-1;
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;
        }
        
        m_vecIndicesLines[m_unIndexLines++] = m_unIndex-1;
        m_vecIndicesLines[m_unIndexLines++] = m_unIndex-_nNrOfSeg;
    }
    else
    {
        double fAng = 0.0;
        double fFac = MATH_2PI /_nNrOfSeg;
 
        m_vecVertices[m_unIndexVerts++] = _vecC[0]+std::sin(fAng)*_fR;
        m_vecVertices[m_unIndexVerts++] = _vecC[1]+std::cos(fAng)*_fR;
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        m_unIndex++;
        fAng *= fFac;
        
        while (fAng < MATH_2PI)
        {
            m_vecVertices[m_unIndexVerts++] = _vecC[0]+std::sin(fAng)*_fR;
            m_vecVertices[m_unIndexVerts++] = _vecC[1]+std::cos(fAng)*_fR;
            m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
            m_vecColours[m_unIndexCol++] = m_aColour[0];
            m_vecColours[m_unIndexCol++] = m_aColour[1];
            m_vecColours[m_unIndexCol++] = m_aColour[2];
            m_vecColours[m_unIndexCol++] = m_aColour[3];
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex-1;
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;
            fAng += fFac;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Show the given vector
///
/// \param _vecV Vector to be shown
/// \param _vecPos Position of vector
///
/// \todo atan replacement should be written
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::showVec(const Vector2d& _vecV, const Vector2d& _vecPos) const
{
    METHOD_ENTRY("CGraphics::showVec")

    // Catch nullvectors at first
    if (_vecV.norm() != 0.0)
    {
        Vector2d vecFront   = _vecPos+_vecV;
        Vector2d vecDir     = _vecV.normalized();
        Vector2d vecFrontT  = vecFront - vecDir * 5.0/m_fCamZoom;
        Vector2d vecFrontOL = vecFrontT + Vector2d(-vecDir[1],  vecDir[0]) * 2.0/m_fCamZoom;
        Vector2d vecFrontOR = vecFrontT + Vector2d( vecDir[1], -vecDir[0]) * 2.0/m_fCamZoom;

        glBegin(GL_LINE_STRIP);
            glVertex3d( _vecPos[0],_vecPos[1], -20.0);
            glVertex3d( vecFrontT[0], vecFrontT[1], -20.0);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3d( vecFrontOL[0], vecFrontOL[1], -10.0);
            glVertex3d( vecFront[0], vecFront[1], -10.0);
            glVertex3d( vecFrontOR[0], vecFrontOR[1], -10.0);
        glEnd();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add vertex to linelist
///
/// The vertex is added at the current position in the linelist, specified by
/// the size-variable
///
/// \param _vecV Vertex
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::addVertex(const Vector2d& _vecV)
{
    METHOD_ENTRY("CGraphics::addVertex(const Vector2d&)");

    m_vecVertices[m_unIndexVerts++] = _vecV[0];
    m_vecVertices[m_unIndexVerts++] = _vecV[1];
    m_vecVertices[m_unIndexVerts++] = m_fDepth;
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_nLineNrOfVerts++;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add vertex to linelist
///
/// The vertex is added at the current position in the linelist, specified by
/// the size-variable
///
/// \param _fX Vertex x-position
/// \param _fY Vertex y-position
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::addVertex(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CGraphics::addVertex(const double&, const double&)");

    m_vecVertices[m_unIndexVerts++] = _fX;
    m_vecVertices[m_unIndexVerts++] = _fY;
    m_vecVertices[m_unIndexVerts++] = m_fDepth;
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_nLineNrOfVerts++;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a dot
///
/// \param _vecV Position of the dot that should be drawn
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::dot(const Vector2d& _vecV)
{
    METHOD_ENTRY("CGraphics::dot")

    m_vecVertices[m_unIndexVerts++] = _vecV[0];
    m_vecVertices[m_unIndexVerts++] = _vecV[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecIndicesPoints[m_unIndexPoints++] = m_unIndex++;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw dots
///
/// \param _Dots List of dots to be drawn
/// \param _vecOffset Offset for drawing, used e.g. when existing list should
///                   be shifted.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::dots(CCircularBuffer<Vector2d>& _Dots,
                     const Vector2d& _vecOffset)
{
    METHOD_ENTRY("CGraphics::dots")
    
    for (auto i=0u; i<_Dots.size(); ++i)
    {
        m_vecVertices[m_unIndexVerts++] = _Dots[i][0]+_vecOffset[0];
        m_vecVertices[m_unIndexVerts++] = _Dots[i][1]+_vecOffset[1];
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        m_vecIndicesPoints[m_unIndexPoints++] = m_unIndex++;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a circle
///
/// \param _vecC    Center of circle
/// \param _fR      Radius of circle
/// \param _nNrOfSeg Number of segments
/// \param _bCache  Flag if sine/cosine cache should be used
///                 (call \ref cacheSinCos before).
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::filledCircle(const Vector2d& _vecC, const double& _fR,
                                                    const int _nNrOfSeg,
                                                    const bool _bCache)
{
    METHOD_ENTRY("CGraphics::filledCircle")

    if (_bCache)
    {
        m_vecVertices[m_unIndexVerts++] = _vecC[0];
        m_vecVertices[m_unIndexVerts++] = _vecC[1];
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        auto m_unCenterIndex = m_unIndex;
        
        m_vecVertices[m_unIndexVerts++] = _vecC[0]+m_SinCache[0]*_fR;
        m_vecVertices[m_unIndexVerts++] = _vecC[1]+m_CosCache[0]*_fR;
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        m_unIndex += 2u;
        
        for (int i=1; i<_nNrOfSeg; ++i)
        {
            m_vecVertices[m_unIndexVerts++] = _vecC[0]+m_SinCache[i]*_fR;
            m_vecVertices[m_unIndexVerts++] = _vecC[1]+m_CosCache[i]*_fR;
            m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
            m_vecColours[m_unIndexCol++] = m_aColour[0];
            m_vecColours[m_unIndexCol++] = m_aColour[1];
            m_vecColours[m_unIndexCol++] = m_aColour[2];
            m_vecColours[m_unIndexCol++] = m_aColour[3];
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unCenterIndex;
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex-1;
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex++;
        }
        
        m_vecIndicesTriangles[m_unIndexTriangles++] = m_unCenterIndex;
        m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex-1;
        m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex-_nNrOfSeg;
    }
    else
    {
        double fAng = 0.0;
        double fFac = MATH_2PI /_nNrOfSeg;
        
        m_vecVertices[m_unIndexVerts++] = _vecC[0];
        m_vecVertices[m_unIndexVerts++] = _vecC[1];
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        auto m_unCenterIndex = m_unIndex;
        
        m_vecVertices[m_unIndexVerts++] = _vecC[0]+std::sin(fAng)*_fR;
        m_vecVertices[m_unIndexVerts++] = _vecC[1]+std::cos(fAng)*_fR;
        m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
        
        m_unIndex += 2u;
        fAng *= fFac;

        while (fAng < MATH_2PI)
        {
            m_vecVertices[m_unIndexVerts++] = _vecC[0]+std::sin(fAng)*_fR;
            m_vecVertices[m_unIndexVerts++] = _vecC[1]+std::cos(fAng)*_fR;
            m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
            m_vecColours[m_unIndexCol++] = m_aColour[0];
            m_vecColours[m_unIndexCol++] = m_aColour[1];
            m_vecColours[m_unIndexCol++] = m_aColour[2];
            m_vecColours[m_unIndexCol++] = m_aColour[3];
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unCenterIndex;
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex-1;
            m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex++;
            fAng += fFac;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a filled rectangle
///
/// \param _vecLL   Lower left corner
/// \param _vecUR   Upper right corner
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::filledRect(const Vector2d& _vecLL, const Vector2d& _vecUR)
{
    METHOD_ENTRY("CGraphics::filledRect")

    m_vecVertices[m_unIndexVerts++] = _vecLL[0];
    m_vecVertices[m_unIndexVerts++] = _vecLL[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecUR[0];
    m_vecVertices[m_unIndexVerts++] = _vecLL[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecLL[0];
    m_vecVertices[m_unIndexVerts++] = _vecUR[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecUR[0];
    m_vecVertices[m_unIndexVerts++] = _vecUR[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex++;   // 1
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex++;   // 2
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex;     // 3
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex;     // 3
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex-1u;  // 2
    m_vecIndicesTriangles[m_unIndexTriangles++] = m_unIndex+1u;  // 4
    m_unIndex += 2u;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a polygon line
///
/// \param _Vertices List of vertices
/// \param _PolygonType PolygonType
/// \param _vecOffset Offset for drawing, used e.g. when existing list should
///                   be shifted.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::polygon(const VertexListType& _Vertices,
                        const PolygonType& _PolygonType,
                        const Vector2d& _vecOffset)
{
    METHOD_ENTRY("CGraphics::polygon")

    this->beginLine();
    if (_vecOffset.isZero())
    {
        for (const auto Vertex : _Vertices)
        {
            m_vecVertices[m_unIndexVerts++] = Vertex[0];
            m_vecVertices[m_unIndexVerts++] = Vertex[1];
            m_vecVertices[m_unIndexVerts++] = m_fDepth;
        }
    }
    else
    {
        for (const auto Vertex : _Vertices)
        {
            m_vecVertices[m_unIndexVerts++] = Vertex[0]+_vecOffset[0];
            m_vecVertices[m_unIndexVerts++] = Vertex[1]+_vecOffset[1];;
            m_vecVertices[m_unIndexVerts++] = m_fDepth;
        }
    }
    for (auto i=0u; i < _Vertices.size(); ++i)
    {
        m_vecColours[m_unIndexCol++] = m_aColour[0];
        m_vecColours[m_unIndexCol++] = m_aColour[1];
        m_vecColours[m_unIndexCol++] = m_aColour[2];
        m_vecColours[m_unIndexCol++] = m_aColour[3];
    }
    
    m_nLineNrOfVerts += _Vertices.size();
    this->endLine(_PolygonType);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a rectangle
///
/// \param _vecLL   Lower left corner
/// \param _vecUR   Upper right corner
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rect(const Vector2d& _vecLL, const Vector2d& _vecUR)
{
    METHOD_ENTRY("CGraphics::rect")
    
    m_vecVertices[m_unIndexVerts++] = _vecLL[0];
    m_vecVertices[m_unIndexVerts++] = _vecLL[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecUR[0];
    m_vecVertices[m_unIndexVerts++] = _vecLL[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecUR[0];
    m_vecVertices[m_unIndexVerts++] = _vecUR[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecVertices[m_unIndexVerts++] = _vecLL[0];
    m_vecVertices[m_unIndexVerts++] = _vecUR[1];
    m_vecVertices[m_unIndexVerts++] = float(m_fDepth);
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecColours[m_unIndexCol++] = m_aColour[0];
    m_vecColours[m_unIndexCol++] = m_aColour[1];
    m_vecColours[m_unIndexCol++] = m_aColour[2];
    m_vecColours[m_unIndexCol++] = m_aColour[3];
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;   // 1
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex;     // 2
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;   // 2
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex;     // 3
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;   // 3
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex;     // 4
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex;     // 4
    m_vecIndicesLines[m_unIndexLines++] = m_unIndex-3;   // 1
    m_unIndex++;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Define beginning of line to be drawn
///
/// This method just specifies the start of a line list. The type of the list
/// defines if it is a closed line loop, a single line etc. This concept is
/// directly related to OpenGL-syntax.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::beginLine()
{
    METHOD_ENTRY("CGraphics::beginLine")
    m_nLineNrOfVerts = 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Define end of line to be drawn
///
/// This method just specifies the end of a line list. This concept is
/// directly related to OpenGL-syntax.
///
/// \param _PType Type of line, specifying howto end drawing
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::endLine(const PolygonType& _PType)
{
    METHOD_ENTRY("CGraphics::endLine")
    
    for (auto i=0u; i<m_nLineNrOfVerts-1; ++i)
    {
        m_vecIndicesLines[m_unIndexLines++] = m_unIndex++;
        m_vecIndicesLines[m_unIndexLines++] = m_unIndex;
    }
    
    if (_PType == PolygonType::LINE_LOOP || _PType == PolygonType::FILLED)
    {
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex;
            m_vecIndicesLines[m_unIndexLines++] = m_unIndex+1-m_nLineNrOfVerts;
    }
    m_unIndex++;
}
