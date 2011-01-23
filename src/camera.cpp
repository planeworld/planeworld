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
    
    m_vecFrame0.resize(4);
    m_vecFrame.resize(4);
    
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
/// \brief Resets the camera to default values
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::reset()
{
    METHOD_ENTRY("CCamera::reset")

    m_vecPosition.setZero();
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
    
    this->update();

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
    
    this->update();

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
    
    this->update();

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
    
    this->update();

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
    
    this->update();

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
    
    this->update();

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
    
    this->update();
    
    METHOD_EXIT("CCamera::zoomTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update of the bounding box and frame with respect to position, angle and zoom.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::update()
{
    METHOD_ENTRY("CCamera::update")

    Rotation2Dd Rotation(m_fAngle);

    m_vecFrame[0] = (Rotation * m_vecFrame0[0])/m_fZoom+m_vecPosition;
    m_vecFrame[1] = (Rotation * m_vecFrame0[1])/m_fZoom+m_vecPosition;
    m_vecFrame[2] = (Rotation * m_vecFrame0[2])/m_fZoom+m_vecPosition;
    m_vecFrame[3] = (Rotation * m_vecFrame0[3])/m_fZoom+m_vecPosition;
    
    m_BoundingBox.setLowerLeft( m_vecFrame[0]);
    m_BoundingBox.setUpperRight(m_vecFrame[0]);
    m_BoundingBox.update(m_vecFrame[1]);
    m_BoundingBox.update(m_vecFrame[2]);
    m_BoundingBox.update(m_vecFrame[3]);
    
    m_fBoundingCircleRadius = sqrt(m_fViewportWidth*m_fViewportWidth + 
                                   m_fViewportHeight*m_fViewportHeight)/m_fZoom;
    
    METHOD_EXIT("CCamera::update")
}
