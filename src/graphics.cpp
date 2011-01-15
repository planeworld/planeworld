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
/// \file       graphics.cpp
/// \brief      implementation of class "CGraphics"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "graphics.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief constructor, initialising members.
///
/// the constructor is private, this is because of the implementation as a
/// meyers-singleton.
///
///////////////////////////////////////////////////////////////////////////////
CGraphics::CGraphics() : m_fCamAng(0.0),
                        m_fCamZoom(1.0),
                        m_fDepthMax(GRAPHICS_FAR_DEFAULT),
                        m_fDepthMin(GRAPHICS_NEAR_DEFAULT),
                        m_fDynPelSize(GRAPHICS_DYN_PEL_SIZE_DEFAULT),
                        m_nVideoFlags(0),
                        m_unWidthScr(GRAPHICS_WIDTH_DEFAULT),
                        m_unHeightScr(GRAPHICS_HEIGHT_DEFAULT),
                        m_unNrOfLines(0u),
                        m_fDepth(1.0)
{
    METHOD_ENTRY("CGraphics::CGraphics")
    CTOR_CALL("CGraphics::CGraphics")
    METHOD_EXIT("CGraphics::CGraphics")
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
        
    fX = ((GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT) / m_unWidthScr * _vecV[0]+
                    GRAPHICS_LEFT_DEFAULT) / m_fCamZoom;
    fY = ((GRAPHICS_TOP_DEFAULT-GRAPHICS_BOTTOM_DEFAULT) / m_unHeightScr * _vecV[1]+
                    GRAPHICS_BOTTOM_DEFAULT) /  m_fCamZoom;
    
    fL = sqrt(fX*fX+fY*fY);
    fAtan = atan2(fX,fY);
    
    vecResult[0] = fL*cos(fAtan - (GRAPHICS_PI/2.0-m_fCamAng))+ m_vecCamPos[0];
    vecResult[1] = fL*sin(fAtan - (GRAPHICS_PI/2.0-m_fCamAng))- m_vecCamPos[1];

    METHOD_EXIT("CGraphics::screen2World")
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
        
    fX = ((GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT) / m_unWidthScr * _fX+
                    GRAPHICS_LEFT_DEFAULT) / m_fCamZoom;
    fY = ((GRAPHICS_TOP_DEFAULT-GRAPHICS_BOTTOM_DEFAULT) / m_unHeightScr * _fY+
                    GRAPHICS_BOTTOM_DEFAULT) /  m_fCamZoom;
    
    fL = sqrt(fX*fX+fY*fY);
    fAtan = atan2(fX,fY);
    
    vecResult[0] = fL*cos(fAtan - (GRAPHICS_PI/2.0-m_fCamAng))+ m_vecCamPos[0];
    vecResult[1] = fL*sin(fAtan - (GRAPHICS_PI/2.0-m_fCamAng))- m_vecCamPos[1];

    METHOD_EXIT("CGraphics::screen2World")
    return vecResult;
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
    
    SDL_GL_SwapBuffers();

    //INFO_MSG("Graphics", "Lines: " << m_unNrOfLines, LOG_DOMAIN_NONE);
    m_unNrOfLines = 0u;
    
    // clear offscreen buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    // set default to matrixmode modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    METHOD_EXIT("CGraphics::swapBuffers")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes graphics
///
/// This method initialises the graphics by calling some SDL- and
/// OpenGL-routines
///
/// \note With SDL 1.2.10: SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
///         aktivates the VSYNC.
///
/// \return Success
///
///////////////////////////////////////////////////////////////////////////////
bool CGraphics::init()
{
    METHOD_ENTRY("CGraphics::init")
    
    const   SDL_VideoInfo*  pVideoInfo;
    
    //--------------------------------------------------------------------------
    // Initialize window and graphics
    //--------------------------------------------------------------------------
    
    // Enable vsync
//  putenv("__GL_SYNC_TO_VBLANK=1");
//  INFO_MSG("Graphics", "Enabling vsync.", LOG_DOMAIN_NONE);

    // Initialize SDL-video and -timer
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        ERROR_MSG("Graphics/SDL", "Could not initialise SDL.")
    }
    else
    {
        INFO_MSG("Graphics/SDL", "SDL initialised.")
    }
    
    // Set video-flags
    pVideoInfo = SDL_GetVideoInfo();
    
    if (0 == pVideoInfo)
    {
        ERROR_MSG("Graphics/SDL", "Could not get video information.")
        METHOD_EXIT("CGraphics::init")
        return (false);
    }
    m_nVideoFlags  = SDL_OPENGL;
    m_nVideoFlags |= SDL_GL_DOUBLEBUFFER;
    m_nVideoFlags |= SDL_RESIZABLE;
//  m_nVideoFlags |= SDL_FULLSCREEN;

    // Testing for SDL hardware acceleration is meaningless when only using OpenGL
    //  // Test for hardware acceleration
    //     if (pVideoInfo->hw_available)
    //  {
    //      m_nVideoFlags |= SDL_HWSURFACE;
    //      INFO_MSG("Graphics/SDL", "Enabling hardware accelerated surfaces.", LOG_DOMAIN_NONE);
    //  }
    //     else
    //  {
    //      m_nVideoFlags |= SDL_SWSURFACE;
    //      NOTICE_MSG("Graphics/SDL", "Hardware accelerated surfaces not supported.", LOG_DOMAIN_NONE);
    //      NOTICE_MSG("Graphics/SDL", "-> Linux doesn't allow acceleration without DGA.", LOG_DOMAIN_NONE);
    //  }
    // 
    //     // Test for hardware blitting
    //     if (pVideoInfo->blit_hw)
    //  {
    //      m_nVideoFlags |= SDL_HWACCEL;
    //      INFO_MSG("Graphics/SDL", "Enabling hardware accelerated blitting.", LOG_DOMAIN_NONE);
    //  }
    //     else
    //  {
    //      NOTICE_MSG("Graphics/SDL", "Hardware accelerated blitting not supported.", LOG_DOMAIN_NONE);
    //      NOTICE_MSG("Graphics/SDL", "-> Linux doesn't allow acceleration without DGA.", LOG_DOMAIN_NONE);
    //  }
    
    // Enable double buffering
    if (0 == SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ))
    {
        INFO_MSG("Graphics/SDL_GL", "Enabling double-buffering.")
    }
    else
    {
        NOTICE_MSG("Graphics/SDL_GL", "Double-buffering not supported.")
    }
    
    // Enable cursor
    SDL_ShowCursor(SDL_ENABLE);
    
    // Initialize graphics
    if (SDL_SetVideoMode(m_unWidthScr, m_unHeightScr, 0, m_nVideoFlags) != 0)
    {
        INFO_MSG("Graphics/SDL", "Videomode set (" << m_unWidthScr << "x" << m_unHeightScr << ").")
    }
    else
    {
        ERROR_MSG("Graphics/SDL", "Could not set videomode.")
        METHOD_EXIT("CGraphics::init")
        return(false);
    }
    

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
    glOrtho(GRAPHICS_LEFT_DEFAULT, GRAPHICS_RIGHT_DEFAULT,
            GRAPHICS_BOTTOM_DEFAULT, GRAPHICS_TOP_DEFAULT,
            GRAPHICS_NEAR_DEFAULT, GRAPHICS_FAR_DEFAULT);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable anti-aliasing
//  glEnable(GL_LINE_SMOOTH);
//  glShadeModel(GL_SMOOTH);
//  glEnable(GL_POLYGON_SMOOTH);
//  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    // Test for depthbuffer and enable if possible
//  glEnable(GL_DEPTH_TEST);
//  if (!glIsEnabled(GL_DEPTH_TEST))
//  {
//      WARNING_MSG("OpenGL", "Could not enable depthbuffer.")
//  }
//  else
//  {
//      INFO_MSG("OpenGL", "Enabling depthbuffer.")
//  }
    
    // clear buffers
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    
    // set default to matrixmode modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLfloat LightAmbient[]= { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
//     glEnable(GL_LIGHT1);
//     glEnable(GL_LIGHTING);
//     glEnable(GL_RESCALE_NORMAL);
//     glEnable(GL_NORMALIZE);
    glEnable(GL_LINE_SMOOTH);
    
    METHOD_EXIT("CGraphics::init")
    return(true);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Prepare graphics for new resolution
///
/// This method reinitialises the OpenGL-matrices for the new resolution.
///
/// \param _unX New x-resolution
/// \param _unY New y-resolution
///
/// \return Success
///
///////////////////////////////////////////////////////////////////////////////
bool CGraphics::resizeWindow(unsigned short _unWidthScr, unsigned short _unHeightScr)
{
    METHOD_ENTRY("CGraphics::resizeWindow")

    if (SDL_SetVideoMode(_unWidthScr, _unHeightScr, 0, m_nVideoFlags) != 0)
    {
        INFO_MSG("Graphics/SDL", "Windowsize changed (" << m_unWidthScr << "x" << m_unHeightScr << ").")
    }
    else
    {
        ERROR_MSG("Graphics/SDL", "Could not set videomode.")
        METHOD_EXIT("CGraphics::resizeWindow")
        return(false);
    }

    // Setup viewport
    glMatrixMode(GL_VIEWPORT);
    glLoadIdentity();
    glViewport(0, 0, _unWidthScr, _unHeightScr);
    
    // Setup projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(GRAPHICS_LEFT_DEFAULT, GRAPHICS_RIGHT_DEFAULT,
            GRAPHICS_BOTTOM_DEFAULT, GRAPHICS_TOP_DEFAULT,
            GRAPHICS_NEAR_DEFAULT, GRAPHICS_FAR_DEFAULT);
    
    // Set default to matrixmode modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Store resolution
    m_unWidthScr = _unWidthScr;
    m_unHeightScr = _unHeightScr;
    
    METHOD_EXIT("CGraphics::resizeWindow")
    return(true);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief apply the camera movement
///
/// This method applies the movement of the camera, that is rotation and
/// translation, to OpenGL.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::applyCamMovement()
{
    METHOD_ENTRY("CGraphics::applyCamMovement")

    glScaled(m_fCamZoom, m_fCamZoom, 1.0);
    glRotated(-m_fCamAng*GRAPHICS_RAD2DEG, 0.0, 0.0, 1.0);
    glTranslated(-m_vecCamPos[0], m_vecCamPos[1], 0.0);
    
//     GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
//     glLightf (GL_LIGHT1, GL_SPOT_CUTOFFB, 15.f);
//     glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);

    METHOD_EXIT("CGraphics::applyCamMovement")
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
    

    METHOD_EXIT("CGraphics::resetCam")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief rotate camera
///
/// \param _fInc incremental camera angle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rotCamBy(const double& _fInc)
{
    METHOD_ENTRY("CGraphics::rotCamBy")

    m_fCamAng += _fInc;

    METHOD_EXIT("CGraphics::rotCamBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief rotate camera to given angle
///
/// \param _fAng camera angle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::rotCamTo(const double& _fAng)
{
    METHOD_ENTRY("CGraphics::rotCamTo")

    m_fCamAng = _fAng;

    METHOD_EXIT("CGraphics::rotCamTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief move camera position
///
/// \param _vecInc incremental movement in x-,y- and z-direction
///
/// \note This one was a little bit tricky. Camera movement has to be applied
///         with respect to the angle. I first tried to do this within the
///         method "applyCamMovement" but that's not easily possible.
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::transCamBy(const Vector3d& _vecInc)
{
    METHOD_ENTRY("CGraphics::transCamBy")

    Rotation2Dd Rotation(-m_fCamAng);

    m_vecCamPos.segment<2>(0) += Rotation * _vecInc.segment<2>(0);
    
    METHOD_EXIT("CGraphics::transCamBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief move camera to given position
///
/// \param _vecPos position in x-,y- and z-direction
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::transCamTo(const Vector3d& _vecPos)
{
    METHOD_ENTRY("CGraphics::transCamTo")

    m_vecCamPos = _vecPos;

    METHOD_EXIT("CGraphics::transCamTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief zoom camera
///
/// \param _fFac multiplicational camera zoom
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::zoomCamBy(const double& _fFac)
{
    METHOD_ENTRY("CGraphics::zoomCamBy")

    m_fCamZoom *= _fFac;

    METHOD_EXIT("CGraphics::zoomCamBy")
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

    METHOD_EXIT("CGraphics::zoomCamTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw a circle
///
/// \param _vecC    Center of circle
/// \param _fR      Radius of circle
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::circle(const Vector3d& _vecC, const double& _fR) const
{
    METHOD_ENTRY("CGraphics::circle")

    double fAng = 0.0;

    glBegin(GL_LINE_LOOP);
        while (fAng < 2*GRAPHICS_PI)
        {
            glVertex3d( _vecC[0]+std::sin(fAng)*_fR,
                        _vecC[1]+std::cos(fAng)*_fR,
                        _vecC[2]);
            fAng += 2*GRAPHICS_PI / 100;
        }
    glEnd();

    METHOD_EXIT("CGraphics::circle")
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

    glPointSize(7.0);
    glBegin(GL_POINTS);
        glVertex3d( _vecV[0], _vecV[1], -10.0);
    glEnd();
    glPointSize(1.0);

    METHOD_EXIT("CGraphics::dot")
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

    METHOD_EXIT("CGraphics::filledRect")
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

    METHOD_EXIT("CGraphics::rect")
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
        // Define temporary varible
        double fAtan;

        // Use red for displaying forces
//         glLineWidth(m_fCamZoom);
        glBegin(GL_LINE_STRIP);
            glVertex3d( _vecPos[0],_vecPos[1], -20.0);
            glVertex3d( _vecV[0]+_vecPos[0],
                        _vecV[1]+_vecPos[1],
                        -20.0);
        glEnd();
        
//      glPointSize(3.0);
//      glBegin(GL_POINTS);
//          glVertex3d( _vecPos[0],_vecPos[1], -50.0);
//      glEnd();
//      glPointSize(1.0);

        fAtan = atan2(_vecV[1],_vecV[0]);

        glBegin(GL_LINE_STRIP);
            glVertex3d( sin(fAtan)*0.1*_vecV.norm() + _vecPos[0]+0.75*_vecV[0], 
                        -cos(fAtan)*0.1*_vecV.norm() + _vecPos[1]+0.75*_vecV[1],
                        -10.0);
            glVertex3d( _vecV[0]+_vecPos[0],
                        _vecV[1]+_vecPos[1],
                        -10.0);
            glVertex3d( -sin(fAtan)*0.1*_vecV.norm() + _vecPos[0]+0.75*_vecV[0], 
                        cos(fAtan)*0.1*_vecV.norm() + _vecPos[1]+0.75*_vecV[1],
                        -10.0);
        glEnd();

//         glLineWidth(1.0);
    }

    METHOD_EXIT("CGraphics::showVec")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Define beginning of line to be drawn
///
/// This method just specifies the start of a line list. The type of the list
/// defines if it is a closed line loop, a single line etc. This concept is
/// directly related to OpenGL-syntax.
///
/// \param _LType Linetype
/// \param _fDepth Depth of line
///
///////////////////////////////////////////////////////////////////////////////
void CGraphics::beginLine(const LineType& _LType, const double& _fDepth)
{
    METHOD_ENTRY("CGraphics::beginLine")

    m_fDepth = _fDepth;

    switch(_LType)
    {
        case GRAPHICS_LINETYPE_SINGLE:
            glBegin(GL_LINES);  
            break;
        case GRAPHICS_LINETYPE_LOOP:
            glBegin(GL_LINE_LOOP);
            break;
        case GRAPHICS_LINETYPE_STRIP:
            glBegin(GL_LINE_STRIP);
            break;
    }

    METHOD_EXIT("CGraphics::beginLine")
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

    for (std::list<Vector2d>::const_iterator ci = m_VertList.begin();
        ci != m_VertList.end(); ++ci)
    {
        glVertex3d((*ci)[0], (*ci)[1], m_fDepth);
    }
    glEnd();
    m_VertList.clear();

    METHOD_EXIT("CGraphics::endLine")
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

    METHOD_EXIT("CGraphics::glSetPerspective")
}

