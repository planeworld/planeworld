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
    m_vecIndicesLines.reserve(m_unIndexMax);
    m_vecIndicesLineStrip.reserve(m_unIndexMax);
    m_vecIndicesLineLoop.reserve(m_unIndexMax);
    m_vecIndicesPoints.reserve(m_unIndexMax);
    m_vecIndicesTriangles.reserve(m_unIndexMax);
    
    m_vecColours.reserve(m_unIndexMax);
    m_vecVertices.reserve(m_unIndexMax);
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
/// \brief Clear and swap videobuffers
///
/// This method swaps videobuffers and clears offscreen buffers afterwards.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::swapBuffers() 
{
    METHOD_ENTRY("CGraphics::swapBuffers")
    
    glBindVertexArray(m_unVAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLines.size()*sizeof(GLuint), &m_vecIndicesLines.front(), GL_STREAM_DRAW);
    glDrawElements(GL_LINES, m_vecIndicesLines.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineStrip);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineStrip.size()*sizeof(GLuint), &m_vecIndicesLineStrip.front(), GL_STREAM_DRAW);
    glDrawElements(GL_LINE_STRIP, m_vecIndicesLineStrip.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineLoop);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineLoop.size()*sizeof(GLuint), &m_vecIndicesLineLoop.front(), GL_STREAM_DRAW);
    glDrawElements(GL_LINE_LOOP, m_vecIndicesLineLoop.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesPoints.size()*sizeof(GLuint), &m_vecIndicesPoints.front(), GL_STREAM_DRAW);
    glDrawElements(GL_POINTS, m_vecIndicesPoints.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesTriangles.size()*sizeof(GLuint), &m_vecIndicesTriangles.front(), GL_STREAM_DRAW);
    glDrawElements(GL_TRIANGLES, m_vecIndicesTriangles.size(), GL_UNSIGNED_INT, 0);
    
    m_pWindow->display();

    // Reserve gpu memory for all relevant buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_unVAO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferData(GL_ARRAY_BUFFER, m_unIndexMax * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLines.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineStrip);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineStrip.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineLoop);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineLoop.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesPoints.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesTriangles.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    m_unIndex = 0u;
    m_unIndexStart = 0u;
    m_vecIndicesLines.clear();
    m_vecIndicesLineLoop.clear();
    m_vecIndicesLineStrip.clear();
    m_vecIndicesPoints.clear();
    m_vecIndicesTriangles.clear();
    
    // clear offscreen buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
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
    glEnable(GL_DEPTH_TEST);
    if (!glIsEnabled(GL_DEPTH_TEST))
    {
        WARNING_MSG("OpenGL", "Could not enable depthbuffer.")
    }
    else
    {
        INFO_MSG("OpenGL", "Enabling depthbuffer.")
    }
    
    //--------------------------------------------------------------------------
    // Prepare OpenGL buffers (VBO, VAO, IBO, FBO)
    //--------------------------------------------------------------------------
    glGenBuffers(1, &m_unVBO);
    glGenBuffers(1, &m_unVBOColours);
    glGenBuffers(1, &m_unIBOLines);
    glGenBuffers(1, &m_unIBOLineLoop);
    glGenBuffers(1, &m_unIBOLineStrip);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLines.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineStrip);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineStrip.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    #ifdef PW_MULTITHREADING
        m_pWindow->setActive(false);
    #endif
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOLineLoop);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesLineLoop.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesPoints.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unIBOTriangles);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vecIndicesTriangles.size()*sizeof(GLuint), nullptr, GL_STREAM_DRAW);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(m_unIndexMax);
    
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

//     glTranslated(-m_vecCamPos[0], -m_vecCamPos[1], 0.0);
    
    m_matScale = glm::scale(glm::vec3(GLfloat(m_fCamZoom), GLfloat(m_fCamZoom), 1.0f));
    m_matRotate = glm::rotate(float(-m_fCamAng), glm::vec3(0.0f, 0.0f, 1.0f));
    m_matTransform = m_matProjection * m_matScale * m_matRotate;

    GLint nTransMatLoc=glGetUniformLocation(m_ShaderProgram.getID(), "matTransform");
    glUniformMatrix4fv(nTransMatLoc, 1, GL_FALSE, glm::value_ptr(m_matTransform));
//     
//     GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
//     glLightf (GL_LIGHT1, GL_SPOT_CUTOFFB, 15.f);
//     glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
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
/// \param _fNrOfSeg Number of segments
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::circle(const Vector2d& _vecC, const double& _fR, const double& _fNrOfSeg) const
{
    METHOD_ENTRY("CGraphics::circle")

    double fAng = 0.0;
    double fFac = 2.0/_fNrOfSeg;

    glBegin(GL_LINE_LOOP);
        while (fAng < 2.0*M_PI)
        {
            glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                        _vecC[1]+std::cos(fAng)*_fR,
                        -10.0);
            fAng += M_PI * fFac;
        }
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a dot
///
/// \param _vecV Position of the dot that should be drawn
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::dot(const Vector2d& _vecV) const
{
    METHOD_ENTRY("CGraphics::dot")

    glBegin(GL_POINTS);
        glVertex3d( _vecV[0], _vecV[1], -10.0);
    glEnd();
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
void CGraphics::dots(const std::vector<Vector2d>& _Dots,
                     const Vector2d& _vecOffset) const
{
    METHOD_ENTRY("CGraphics::dots")
    
    std::vector<Vector2d>::const_iterator ci = _Dots.begin();

//     glPointSize(1.0);
    glBegin(GL_POINTS);
        if (_vecOffset.isZero())
        {
            while (ci != _Dots.end())
            {
                glVertex3d( (*ci)[0], (*ci)[1], -10.0);
                ++ci;
            }
        }
        else
        {
            while (ci != _Dots.end())
            {
                glVertex3d( (*ci)[0]+_vecOffset[0], (*ci)[1]+_vecOffset[1], -10.0);
                ++ci;
            }
        }   
    glEnd();
//     glPointSize(1.0);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a circle
///
/// \param _vecC    Center of circle
/// \param _fR      Radius of circle
/// \param _fNrOfSeg Number of segments
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::filledCircle(const Vector2d& _vecC, const double& _fR, const double& _fNrOfSeg) const
{
    METHOD_ENTRY("CGraphics::filledCircle")

    double fAng = 0.0;
    double fFac = 2.0/_fNrOfSeg;

    glBegin(GL_TRIANGLE_FAN);
        glVertex3d( _vecC[0], _vecC[1],-10.0);
        while (fAng < 2.0*M_PI)
        {
            glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                        _vecC[1]+std::cos(fAng)*_fR,
                        -10.0);
            fAng += M_PI * fFac;
        }
        glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                    _vecC[1]+std::cos(fAng)*_fR,
                    -10.0);
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a filled rectangle in screen space
///
/// \param _vecLL   Lower left corner
/// \param _vecUR   Upper right corner
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::filledRectSS(const Vector2d& _vecLL, const Vector2d& _vecUR) const
{
    METHOD_ENTRY("CGraphics::filledRectSS")
    
    Vector2d vecLL = screen2World(_vecLL[0], _vecLL[1]);
    Vector2d vecLR = screen2World(_vecUR[0], _vecLL[1]);
    Vector2d vecUL = screen2World(_vecLL[0], _vecUR[1]);
    Vector2d vecUR = screen2World(_vecUR[0], _vecUR[1]);
    
    glBegin(GL_QUADS);
        glVertex3d(vecLL[0], vecLL[1], -15.0);
        glVertex3d(vecLR[0], vecLR[1], -15.0);
        glVertex3d(vecUR[0], vecUR[1], -15.0);
        glVertex3d(vecUL[0], vecUL[1], -15.0);
    glEnd();
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
                         const Vector2d& _vecOffset) const
{
    METHOD_ENTRY("CGraphics::polygon")

    switch(_PolygonType)
    {
        case PolygonType::FILLED:
            glBegin(GL_LINE_LOOP);  
            break;
        case PolygonType::LINE_SINGLE:
            glBegin(GL_LINES);  
            break;
        case PolygonType::LINE_LOOP:
            glBegin(GL_LINE_LOOP);
            break;
        case PolygonType::LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            break;
    }
    if (_vecOffset.isZero())
    {
        for (VertexListType::const_iterator ci = _Vertices.begin();
            ci != _Vertices.end(); ++ci)
        {
            glVertex3d((*ci)[0], (*ci)[1], -15.0);
        }
    }
    else
    {
        for (VertexListType::const_iterator ci = _Vertices.begin();
            ci != _Vertices.end(); ++ci)
        {
            glVertex3d((*ci)[0]+_vecOffset[0], (*ci)[1]+_vecOffset[1], -15.0);
        }
    }
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a rectangle
///
/// \param _vecLL   Lower left corner
/// \param _vecUR   Upper right corner
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rect(const Vector2d& _vecLL, const Vector2d& _vecUR) const
{
    METHOD_ENTRY("CGraphics::rect")
    
    glBegin(GL_LINE_LOOP);
        glVertex3d(_vecLL[0], _vecLL[1], -15.0);
        glVertex3d(_vecUR[0], _vecLL[1], -15.0);
        glVertex3d(_vecUR[0], _vecUR[1], -15.0);
        glVertex3d(_vecLL[0], _vecUR[1], -15.0);
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a rectangle in screen space
///
/// \param _vecLL   Lower left corner
/// \param _vecUR   Upper right corner
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rectSS(const Vector2d& _vecLL, const Vector2d& _vecUR) const
{
    METHOD_ENTRY("CGraphics::rectSS")
    
    Vector2d vecLL = screen2World(_vecLL[0], _vecLL[1]);
    Vector2d vecLR = screen2World(_vecUR[0], _vecLL[1]);
    Vector2d vecUL = screen2World(_vecLL[0], _vecUR[1]);
    Vector2d vecUR = screen2World(_vecUR[0], _vecUR[1]);
    
    glBegin(GL_LINE_LOOP);
        glVertex3d(vecLL[0], vecLL[1], -15.0);
        glVertex3d(vecLR[0], vecLR[1], -15.0);
        glVertex3d(vecUR[0], vecUR[1], -15.0);
        glVertex3d(vecUL[0], vecUL[1], -15.0);
    glEnd();
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

    m_vecVertices.push_back(_vecV[0]);
    m_vecVertices.push_back(_vecV[1]);
    m_vecVertices.push_back(m_fDepth);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_pvecIndex->push_back(m_unIndex++);
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

    m_vecVertices.push_back(_fX);
    m_vecVertices.push_back(_fY);
    m_vecVertices.push_back(m_fDepth);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_pvecIndex->push_back(m_unIndex++);
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
        m_vecVertices.push_back(_Dots[i][0]+_vecOffset[0]);
        m_vecVertices.push_back(_Dots[i][1]+_vecOffset[1]);
        m_vecVertices.push_back(float(m_fDepth));
        m_vecColours.push_back(1.0f);
        m_vecColours.push_back(1.0f);
        m_vecColours.push_back(1.0f);
        m_vecIndicesPoints.push_back(m_unIndex++);
    }

    glBindVertexArray(m_unVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecVertices.size()*sizeof(float),
                                     &(m_vecVertices.front()));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecColours.size()*sizeof(float),
                                     &(m_vecColours.front()));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    m_unIndexStart += m_vecVertices.size();
    
    m_vecColours.clear();
    m_vecVertices.clear();
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

    m_vecVertices.push_back(_vecLL[0]);
    m_vecVertices.push_back(_vecLL[1]);
    m_vecVertices.push_back(float(m_fDepth));
    m_vecVertices.push_back(_vecUR[0]);
    m_vecVertices.push_back(_vecLL[1]);
    m_vecVertices.push_back(float(m_fDepth));
    m_vecVertices.push_back(_vecLL[0]);
    m_vecVertices.push_back(_vecUR[1]);
    m_vecVertices.push_back(float(m_fDepth));
    m_vecVertices.push_back(_vecUR[0]);
    m_vecVertices.push_back(_vecUR[1]);
    m_vecVertices.push_back(float(m_fDepth));
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecColours.push_back(1.0f);
    m_vecIndicesTriangles.push_back(m_unIndex++);   // 1
    m_vecIndicesTriangles.push_back(m_unIndex++);   // 2
    m_vecIndicesTriangles.push_back(m_unIndex);     // 3
    m_vecIndicesTriangles.push_back(m_unIndex);     // 3
    m_vecIndicesTriangles.push_back(m_unIndex-1u);  // 2
    m_vecIndicesTriangles.push_back(m_unIndex+1u);  // 4
    m_unIndex += 2u;
    
    m_vecIndicesTriangles.push_back(m_unIndexMax);

    glBindVertexArray(m_unVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecVertices.size()*sizeof(float),
                                     &(m_vecVertices.front()));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecColours.size()*sizeof(float),
                                     &(m_vecColours.front()));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    m_unIndexStart += m_vecVertices.size();
    
    m_vecColours.clear();
    m_vecVertices.clear();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Define beginning of line to be drawn
///
/// This method just specifies the start of a line list. The type of the list
/// defines if it is a closed line loop, a single line etc. This concept is
/// directly related to OpenGL-syntax.
///
/// \param _PType Linetype
/// \param _fDepth Depth of line
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::beginLine(const PolygonType& _PType, const double& _fDepth)
{
    METHOD_ENTRY("CGraphics::beginLine")

    m_fDepth = _fDepth;

    switch(_PType)
    {
        case PolygonType::FILLED:
            /// \todo Implement filled polygon type
            break;
        case PolygonType::LINE_SINGLE:
            m_pvecIndex = &m_vecIndicesLines;
            break;
        case PolygonType::LINE_LOOP:
            m_pvecIndex = &m_vecIndicesLineLoop;
            break;
        case PolygonType::LINE_STRIP:
            m_pvecIndex = &m_vecIndicesLineStrip;
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Define end of line to be drawn
///
/// This method just specifies the end of a line list. This concept is
/// directly related to OpenGL-syntax.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::endLine()
{
    METHOD_ENTRY("CGraphics::endLine")
    
    m_pvecIndex->push_back(m_unIndexMax);
    
    glBindVertexArray(m_unVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecVertices.size()*sizeof(float),
                                     &(m_vecVertices.front()));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                     m_vecColours.size()*sizeof(float),
                                     &(m_vecColours.front()));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    m_unIndexStart += m_vecVertices.size();

    m_vecColours.clear();
    m_vecVertices.clear();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Buffers Vertex data for OpenGL rendering
///
/// \param _Mode OpenGL draw mode for vertices
/// \param _pvecVertices Vector of vertices to be buffered
/// \param _pvecColours  Vector of vertex colours to be buffered
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::bufferGL(const GLenum _Mode,
                         const std::vector<GLfloat>* const _pvecVertices,
                         const std::vector<GLfloat>* const _pvecColours)
{
    METHOD_ENTRY("CGraphics::bufferGL")

    glBindVertexArray(m_unVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                    _pvecVertices->size()*sizeof(float),
                                     &(_pvecVertices->front()));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_unVBOColours);
    glBufferSubData(GL_ARRAY_BUFFER, m_unIndexStart*sizeof(float),
                                    _pvecColours->size()*sizeof(float),
                                     &(_pvecColours->front()));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    switch(_Mode)
    {
        case GL_LINES:
            for (auto i=0u; i<_pvecVertices->size(); ++i)
            {
                m_vecIndicesLines.push_back(m_unIndexStart+i);
            }
            break;
        case GL_LINE_LOOP:
            for (auto i=0u; i<_pvecVertices->size(); ++i)
            {
                m_vecIndicesLineStrip.push_back(m_unIndexStart+i);
            }
            break;
        case GL_LINE_STRIP:
            for (auto i=0u; i<_pvecVertices->size(); ++i)
            {
                m_vecIndicesLineStrip.push_back(m_unIndexStart+i);
            }
            break;
    }
    
    m_unIndexStart += _pvecVertices->size();
}
