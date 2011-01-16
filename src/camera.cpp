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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera() : m_fViewportWidth(320.0), m_fViewportHeight(200.0)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
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
    
    m_vecCorners[0][0] = -m_fViewportWidth;
    m_vecCorners[0][1] = -m_fViewportHeight;
    m_vecCorners[1][0] =  m_fViewportWidth;
    m_vecCorners[1][1] = -m_fViewportHeight;
    m_vecCorners[2][0] =  m_fViewportWidth;
    m_vecCorners[2][1] =  m_fViewportHeight;
    m_vecCorners[3][0] = -m_fViewportWidth;
    m_vecCorners[3][1] =  m_fViewportHeight;
    
    m_BoundingBox.setLowerLeft (Vector2d(-m_fViewportWidth, 
                                         -m_fViewportHeight));
    m_BoundingBox.setUpperRight(Vector2d( m_fViewportWidth, 
                                          m_fViewportHeight));

    METHOD_EXIT("CCamera::setViewport")
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
    m_fAngle = 0.0;
    m_fZoom  = 1.0;
    m_vecCorners[0][0] = -m_fViewportWidth;
    m_vecCorners[0][1] = -m_fViewportHeight;
    m_vecCorners[1][0] =  m_fViewportWidth;
    m_vecCorners[1][1] = -m_fViewportHeight;
    m_vecCorners[2][0] =  m_fViewportWidth;
    m_vecCorners[2][1] =  m_fViewportHeight;
    m_vecCorners[3][0] = -m_fViewportWidth;
    m_vecCorners[3][1] =  m_fViewportHeight;
    
    m_BoundingBox.setLowerLeft (Vector2d(-m_fViewportWidth, 
                                         -m_fViewportHeight));
    m_BoundingBox.setUpperRight(Vector2d( m_fViewportWidth, 
                                          m_fViewportHeight));

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
    
    this->updateBoundingBox();

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
    
    this->updateBoundingBox();

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

    m_vecPosition += Vector2d( (Rotation * _vecV)[0],
                              -(Rotation * _vecV)[1]);
    
    this->updateBoundingBox();

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
    
    this->updateBoundingBox();

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
    
    this->updateBoundingBox();

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
    
    this->updateBoundingBox();
    
    METHOD_EXIT("CCamera::zoomTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update of the bounding box with respect to position, angle and zoom.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::updateBoundingBox()
{
    METHOD_ENTRY("CCamera::updateBoundingBox")

    Rotation2Dd Rotation(-m_fAngle);
    Vector2d vecPosition(m_vecPosition[0], m_vecPosition[1]);
    
    m_BoundingBox.setLowerLeft(( Rotation * m_vecCorners[0])/m_fZoom+vecPosition);
    m_BoundingBox.setUpperRight((Rotation * m_vecCorners[0])/m_fZoom+vecPosition);
    m_BoundingBox.update((Rotation * m_vecCorners[1])/m_fZoom+vecPosition);
    m_BoundingBox.update((Rotation * m_vecCorners[2])/m_fZoom+vecPosition);
    m_BoundingBox.update((Rotation * m_vecCorners[3])/m_fZoom+vecPosition);

    METHOD_EXIT("CCamera::updateBoundingBox")
}
