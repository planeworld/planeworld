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
/// \file       camera.cpp
/// \brief      Implementation of class "CCamera"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-14
///
////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

//--- Program header ---------------------------------------------------------//
#include "body.h"
// #include "object.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera() : m_fViewportWidth(320.0), m_fViewportHeight(200.0),
                     m_fHookAng(0.0), m_pHook(0)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
    m_vecFrame0.resize(4);
    m_vecFrame.resize(4);
    m_vecHook.setZero();
    
    this->reset();
    
    METHOD_EXIT("CCamera::CCamera");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CCamera::~CCamera()
{
    METHOD_ENTRY("CCamera::~CCamera")
    DTOR_CALL("CCamera::~CCamera")
    
    METHOD_EXIT("CCamera::~CCamera")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the object the camera is hooked to
///
/// \param _pHook Object to be hooked on
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::setHook(IObject* _pHook)
{
    METHOD_ENTRY("CCamera::setHook")

    m_pHook = _pHook;
    m_vecHook = m_pHook->getCOM();

    METHOD_EXIT("CCamera::setHook")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the local position of the camera
///
/// \param _fX X coordinate
/// \param _fY Y coordinate
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::setPosition(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CCamera::setPosition")

    m_vecPosition[0] = _fX;
    m_vecPosition[1] = _fY;

    METHOD_EXIT("CCamera::setPosition")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the viewport of the camera
///
/// \param _fW Width
/// \param _fH Height
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::setViewport(const double& _fW, const double& _fH)
{
    METHOD_ENTRY("CCamera::setViewport")

    m_fViewportWidth  = _fW*0.5;
    m_fViewportHeight = _fH*0.5;
        
    m_vecFrame0[0][0] = -m_fViewportWidth;
    m_vecFrame0[0][1] = -m_fViewportHeight;
    m_vecFrame0[1][0] =  m_fViewportWidth;
    m_vecFrame0[1][1] = -m_fViewportHeight;
    m_vecFrame0[2][0] =  m_fViewportWidth;
    m_vecFrame0[2][1] =  m_fViewportHeight;
    m_vecFrame0[3][0] = -m_fViewportWidth;
    m_vecFrame0[3][1] =  m_fViewportHeight;
    
    m_BoundingBox.setLowerLeft (Vector2d(-m_fViewportWidth, 
                                         -m_fViewportHeight));
    m_BoundingBox.setUpperRight(Vector2d( m_fViewportWidth, 
                                          m_fViewportHeight));

    METHOD_EXIT("CCamera::setViewport")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update of the bounding box, frame and position
///
/// \todo Replace dynamic cast and catch bad hooks on point masses.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::update()
{
    METHOD_ENTRY("CCamera::update")

    if (m_pHook != 0)
    {
        m_vecHook = m_pHook->getCOM();
        m_fHookAng = dynamic_cast<CBody*>(m_pHook)->getAngle();
    }
    
    m_Graphics.rotCamTo(m_fHookAng+m_fAngle);
    m_Graphics.zoomCamTo(m_fZoom);

    Rotation2Dd CameraRotation(m_fAngle);
    Rotation2Dd HookRotation(m_fHookAng);
    
    m_vecFrame[0] = HookRotation * (CameraRotation * m_vecFrame0[0]/m_fZoom+
                                       Vector2d(m_vecPosition[0],-m_vecPosition[1]))
                                      +m_vecHook;
    m_vecFrame[1] = HookRotation * (CameraRotation * m_vecFrame0[1]/m_fZoom+
                                       Vector2d(m_vecPosition[0],-m_vecPosition[1]))
                                      +m_vecHook;
    m_vecFrame[2] = HookRotation * (CameraRotation * m_vecFrame0[2]/m_fZoom+
                                       Vector2d(m_vecPosition[0],-m_vecPosition[1]))
                                      +m_vecHook;
    m_vecFrame[3] = HookRotation * (CameraRotation * m_vecFrame0[3]/m_fZoom+
                                       Vector2d(m_vecPosition[0],-m_vecPosition[1]))
                                      +m_vecHook;
    
    m_BoundingBox.setLowerLeft( m_vecFrame[0]);
    m_BoundingBox.setUpperRight(m_vecFrame[0]);
    m_BoundingBox.update(m_vecFrame[1]);
    m_BoundingBox.update(m_vecFrame[2]);
    m_BoundingBox.update(m_vecFrame[3]);
    
    m_fBoundingCircleRadius = sqrt(m_fViewportWidth*m_fViewportWidth + 
                                   m_fViewportHeight*m_fViewportHeight)/m_fZoom;
    m_vecCenter = HookRotation * (Vector2d(m_vecPosition[0], -m_vecPosition[1]))
                   + m_vecHook;
    
    METHOD_EXIT("CCamera::update")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets the camera to default values
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::reset()
{
    METHOD_ENTRY("CCamera::reset")

    m_vecPosition.setZero();
    m_vecCenter.setZero();
    m_fAngle = 0.0;
    m_fZoom  = 1.0;
    m_vecFrame0[0][0] = -m_fViewportWidth;
    m_vecFrame0[0][1] = -m_fViewportHeight;
    m_vecFrame0[1][0] =  m_fViewportWidth;
    m_vecFrame0[1][1] = -m_fViewportHeight;
    m_vecFrame0[2][0] =  m_fViewportWidth;
    m_vecFrame0[2][1] =  m_fViewportHeight;
    m_vecFrame0[3][0] = -m_fViewportWidth;
    m_vecFrame0[3][1] =  m_fViewportHeight;
    
    METHOD_EXIT("CCamera::reset")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera by a given angle
///
/// \param _fAngle Angle to rotate camera by
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::rotateBy(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateBy")

    m_fAngle += _fAngle;
    
    METHOD_EXIT("CCamera::rotateBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera to a given angle
///
/// \param _fAngle Angle to rotate camera to
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::rotateTo(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateTo")

    m_fAngle = _fAngle;
    
    METHOD_EXIT("CCamera::rotateTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Translates the camera by a given position
///
/// \param _vecV Vector to move camera
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::translateBy(const Vector2d& _vecV)
{
    METHOD_ENTRY("CCamera::translateBy")

    Rotation2Dd Rotation(-m_fAngle);

    m_vecPosition += Rotation * _vecV;
    
    METHOD_EXIT("CCamera::translateBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Translates the camera to a given position
///
/// \param _vecV Vector to move camera by
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::translateTo(const Vector2d& _vecV)
{
    METHOD_ENTRY("CCamera::translateTo")

    m_vecPosition = _vecV;
    
    METHOD_EXIT("CCamera::translateTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zooms the camera by a given factor
///
/// \param _fZoom Zoom factor
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::zoomBy(const double& _fZoom)
{
    METHOD_ENTRY("CCamera::zoomBy")

    m_fZoom *= _fZoom;
    
    METHOD_EXIT("CCamera::zoomBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zooms the camera to a given factor
///
/// \param _fZoom Zoom factor
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::zoomTo(const double& _fZoom)
{
    METHOD_ENTRY("CCamera::zoomTo")

    m_fZoom = _fZoom;
    
    METHOD_EXIT("CCamera::zoomTo")
}
