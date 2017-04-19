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
    m_CosCache.resize(GRAPHICS_MAX_CACHE_SIZE);
    m_SinCache.resize(GRAPHICS_MAX_CACHE_SIZE);
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
    
    m_pWindow->display();

    // clear offscreen buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    DOM_VAR(INFO_MSG("Graphics", "Antialiasing level:" << m_pWindow->getSettings().antialiasingLevel))

    //--------------------------------------------------------------------------
    // Setup OpenGL
    //--------------------------------------------------------------------------

    // Setup viewport
    glMatrixMode(GL_VIEWPORT);
    glLoadIdentity();
    glViewport(0, 0, m_unWidthScr, m_unHeightScr);
    
    // Setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_ViewPort.leftplane, m_ViewPort.rightplane,
            m_ViewPort.bottomplane, m_ViewPort.topplane,
            m_ViewPort.nearplane, m_ViewPort.farplane);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
//     glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

//     // Test for depthbuffer and enable if possible
//     glEnable(GL_DEPTH_TEST);
//     if (!glIsEnabled(GL_DEPTH_TEST))
//     {
//         WARNING_MSG("OpenGL", "Could not enable depthbuffer.")
//     }
//     else
//     {
//         INFO_MSG("OpenGL", "Enabling depthbuffer.")
//     }
    
    // clear buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    // set default to matrixmode modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
//     GLfloat global_ambient[] = {0.0f,0.0f,0.0f,1.0f};
//     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
//     
//     GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f };
//     GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
//     GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
//     GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0e1f, 1.0f };
//     
//     glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
//     glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
//     glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
//     glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
//     glEnable(GL_LIGHT1);
//     glEnable(GL_LIGHTING);
//     glEnable(GL_RESCALE_NORMAL);
//     glEnable(GL_NORMALIZE);
//     glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
//     glEnable ( GL_COLOR_MATERIAL );
    
//     float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
//     glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
//     glFrontFace(GL_CCW);
    #ifdef PW_MULTITHREADING
        m_pWindow->setActive(false);
    #endif
    
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
    
    // Setup viewport
    glMatrixMode(GL_VIEWPORT);
    glLoadIdentity();
    glViewport(0, 0, _unWidthScr, _unHeightScr);
    
    // Setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_ViewPort.leftplane, m_ViewPort.rightplane,
            m_ViewPort.bottomplane, m_ViewPort.topplane,
            m_ViewPort.nearplane, m_ViewPort.farplane);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
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

    glScaled(m_fCamZoom, m_fCamZoom, 1.0);
    glRotated(-m_fCamAng*GRAPHICS_RAD2DEG, 0.0, 0.0, 1.0);
    glTranslated(-m_vecCamPos[0], -m_vecCamPos[1], 0.0);
    
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
/// \param _nNrOfSeg Number of segments
/// \param _bCache  Flag if sine/cosine cache should be used
///                 (call \ref cacheSinCos before).
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::circle(const Vector2d& _vecC, const double& _fR,
                                              const int _nNrOfSeg,
                                              const bool _bCache) const
{
    METHOD_ENTRY("CGraphics::circle")

    double fAng = 0.0;
    double fFac = MATH_2PI/_nNrOfSeg;

    if (_bCache)
    {
        glBegin(GL_LINE_LOOP);
            for (int i=0; i<_nNrOfSeg; ++i)
            {
                glVertex3d( _vecC[0]+m_SinCache[i]*_fR,
                            _vecC[1]+m_CosCache[i]*_fR,
                            -10.0);
            }
        glEnd();
    }
    else
    {
        double fAng = 0.0;
        double fFac = MATH_2PI /_nNrOfSeg;

        glBegin(GL_LINE_LOOP);
            while (fAng < MATH_2PI)
            {
                glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                            _vecC[1]+std::cos(fAng)*_fR,
                            -10.0);
                fAng += fFac;
            }
        glEnd();
    }
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
/// \brief Draw dots
///
/// \param _Dots List of dots to be drawn
/// \param _vecOffset Offset for drawing, used e.g. when existing list should
///                   be shifted.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::dots(CCircularBuffer<Vector2d>& _Dots,
                     const Vector2d& _vecOffset) const
{
    METHOD_ENTRY("CGraphics::dots")
    
//     glPointSize(1.0);
    glBegin(GL_POINTS);
        if (_vecOffset.isZero())
        {
            for (auto i=0u; i<_Dots.size(); ++i)
            {
                glVertex3d( _Dots[i][0], _Dots[i][1], -10.0);
            }
        }
        else
        {
            for (auto i=0u; i<_Dots.size(); ++i)
            {
                glVertex3d( _Dots[i][0]+_vecOffset[0], _Dots[i][1]+_vecOffset[1], -10.0);
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
/// \param _nNrOfSeg Number of segments
/// \param _bCache  Flag if sine/cosine cache should be used
///                 (call \ref cacheSinCos before).
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::filledCircle(const Vector2d& _vecC, const double& _fR,
                                                    const int _nNrOfSeg,
                                                    const bool _bCache) const
{
    METHOD_ENTRY("CGraphics::filledCircle")

    if (_bCache)
    {
        glBegin(GL_TRIANGLE_FAN);
            glVertex3d( _vecC[0], _vecC[1],-10.0);
            for (int i=0; i<_nNrOfSeg+1; ++i)
            {
                glVertex3d( _vecC[0]+m_SinCache[i]*_fR,
                            _vecC[1]+m_CosCache[i]*_fR,
                            -10.0);
            }
        glEnd();
    }
    else
    {
        double fAng = 0.0;
        double fFac = MATH_2PI /_nNrOfSeg;

        glBegin(GL_TRIANGLE_FAN);
            glVertex3d( _vecC[0], _vecC[1],-10.0);
            while (fAng < MATH_2PI)
            {
                glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                            _vecC[1]+std::cos(fAng)*_fR,
                            -10.0);
                fAng += fFac;
            }
            glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                        _vecC[1]+std::cos(fAng)*_fR,
                        -10.0);
        glEnd();
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
void CGraphics::filledRect(const Vector2d& _vecLL, const Vector2d& _vecUR) const
{
    METHOD_ENTRY("CGraphics::filledRect")

    glBegin(GL_QUADS);
        glVertex3d(_vecLL[0], _vecLL[1], -15.0);
        glVertex3d(_vecUR[0], _vecLL[1], -15.0);
        glVertex3d(_vecUR[0], _vecUR[1], -15.0);
        glVertex3d(_vecLL[0], _vecUR[1], -15.0);
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

    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Setup perspective matrix for OpenGL
///
/// This method does the same as gluPerspective. It sets up the perspective
/// matrix using fov and aspect ratio. In this case one can directly pass
/// the resolution instead of aspect ratio.
///
/// \param _fFov    fov, viewing angle
/// \param _fWidthScr   x-resolution of window
/// \param _fHeightScr  y-resolution of window
/// \param _fZNear  near clipping plane
/// \param _fZFar   far clipping plane
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::glSetPerspective(const GLdouble& _fFov, const GLdouble& _fWidthScr,
                                const GLdouble& _fHeightScr, const GLdouble& _fZNear,
                                const GLdouble& _fZFar) const
{
    METHOD_ENTRY("CGraphics::glSetPerspective")

    GLdouble fXMin, fXMax, fYMin, fYMax;
    
    fYMax = _fZNear * std::tan(_fFov * M_PI / 360.0);
    fYMin = -fYMax;
    fXMin = fYMin * _fWidthScr/_fHeightScr;
    fXMax = fYMax * _fWidthScr/_fHeightScr;
    glFrustum(fXMin, fXMax, fYMin, fYMax, _fZNear, _fZFar);
}

