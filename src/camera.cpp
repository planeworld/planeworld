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
#include "rigidbody.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera() : m_fViewportWidth(GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT),
                     m_fViewportHeight(GRAPHICS_TOP_DEFAULT-GRAPHICS_BOTTOM_DEFAULT)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
    m_vecCell.setZero();
    m_vecFrame0.resize(4);
    
    IHooker::m_strName += ": Camera";
    
    this->reset();
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

    if ((_fW < GRAPHICS_RIGHT_DEFAULT - GRAPHICS_LEFT_DEFAULT) &&
        (_fH < GRAPHICS_TOP_DEFAULT - GRAPHICS_BOTTOM_DEFAULT))
    {
      m_fViewportWidth  = _fW*0.5;
      m_fViewportHeight = _fH*0.5;
    }
    else
    {
      m_fViewportWidth  = (GRAPHICS_RIGHT_DEFAULT - GRAPHICS_LEFT_DEFAULT)*0.5;
      m_fViewportHeight = (GRAPHICS_TOP_DEFAULT - GRAPHICS_BOTTOM_DEFAULT)*0.5;
      NOTICE_MSG("Camera", "Given viewport is larger than actual screen, resizing to screen size.")
      NOTICE(
        std::cout << "  Viewport: " << _fW << "m x " << _fH << "m" << std::endl;
        std::cout << "  Screen  : " << m_fViewportWidth*2.0 << "m x " <<
                     m_fViewportHeight*2.0 << "m" << std::endl;
      )
    }
        
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update of the bounding box, frame and position
///
/// \todo Hook handling must be improved. At the moment, the camera doesn't
///       use cells but absolute coordinates because of the hook. The cell data
///       is just post-calculated and thus, lacks precesion. Best would be to
///       only allow for hooked movement within double precision.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::update()
{
    METHOD_ENTRY("CCamera::update")
    
    // If method UpdateFromHooked is not called, m_bIsHooked is false. In this
    // case, camera update has to be done without hook
    if (!m_bIsHooked)
    {
        updateWithoutHook();        
        m_Graphics.rotCamTo(m_fAngle);
    }
    else
    {
        updateWithHook();
        m_Graphics.rotCamTo(m_fAngle+m_fHookAngle);
    }
    
    m_fBoundingCircleRadius = sqrt(m_fViewportWidth*m_fViewportWidth + 
                                   m_fViewportHeight*m_fViewportHeight)/m_fZoom;
    m_BoundingBox.setCell(m_vecCell);
    
    m_Graphics.zoomCamTo(m_fZoom);
    m_Graphics.applyCamMovement();
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
    m_vecCell.setZero();
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera by a given angle
///
/// \note Angles are interpreted mathematically positive which means
///       counter clockwise!
///
/// \param _fAngle Angle to rotate camera by
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::rotateBy(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateBy")
    m_fAngle += _fAngle;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera to a given angle
///
/// \note Angles are interpreted mathematically positive which means
///       counter clockwise!
///
/// \param _fAngle Angle to rotate camera to
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::rotateTo(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateTo")
    m_fAngle = _fAngle;
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

    Rotation2Dd Rotation(m_fAngle);

    m_vecPosition += Rotation * _vecV;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Translates the camera to a given position
///
/// \param _vecV Vector to move camera to
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::translateTo(const Vector2d& _vecV)
{
    METHOD_ENTRY("CCamera::translateTo")

    m_vecPosition = _vecV;
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update of the bounding box, frame and position, triggered by entity
///        it is hooked on.
///
/// The graphics class relevant movement will be updated within another update
/// method (above) which is triggered by the visuals manager to avoid conflicts
/// due to different threads (like race conditions) and to sync graphics
/// appropriately.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::myUpdateFromHookable()
{
    METHOD_ENTRY("CCamera::myUpdateFromHookable")
    
    updateWithHook();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Helper method. Update of the bounding box, frame and position.
///
/// Update of bounding box etc is called from updateFromHooked as well as
/// from internal update method to synchronise camera and object graphics.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::updateWithHook()
{
    METHOD_ENTRY("CCamera::updateWithHook")
    
    std::vector<Vector2d>  vecFrame(4);
    Rotation2Dd            CameraRotation(m_fAngle);
    
    Rotation2Dd HookRotation(m_fHookAngle);

    m_vecCenter = HookRotation * m_vecPosition + m_vecHookOrigin + IUniverseScaled::cellToDouble(m_vecHookCell);

    // m_vecCenter is in absolute coordinates while m_vecCell is zero. Thus, they
    // have to be separated:
    IUniverseScaled::separateCenterCell(m_vecCenter,m_vecCenter,m_vecCell);

    // The frame doesn't need to care about the grid. If it is large, the camera is zoomed out.
    // Hence, accuracy is low, so it can stay with the double value.
    vecFrame[0] = HookRotation * (CameraRotation * m_vecFrame0[0]/m_fZoom + m_vecPosition) +
                m_vecHookOrigin + IUniverseScaled::cellToDouble(m_vecHookCell);
    vecFrame[1] = HookRotation * (CameraRotation * m_vecFrame0[1]/m_fZoom + m_vecPosition) +
                m_vecHookOrigin + IUniverseScaled::cellToDouble(m_vecHookCell);
    vecFrame[2] = HookRotation * (CameraRotation * m_vecFrame0[2]/m_fZoom + m_vecPosition) +
                m_vecHookOrigin + IUniverseScaled::cellToDouble(m_vecHookCell);
    vecFrame[3] = HookRotation * (CameraRotation * m_vecFrame0[3]/m_fZoom + m_vecPosition) +
                m_vecHookOrigin + IUniverseScaled::cellToDouble(m_vecHookCell);
                
    m_BoundingBox.setLowerLeft(vecFrame[0]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.setUpperRight(vecFrame[0]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[1]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[2]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[3]-IUniverseScaled::cellToDouble(m_vecCell));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Helper method. Update of the bounding box, frame and position.
///
/// Update of bounding box etc is called from from internal update method if
/// no hook is given.
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::updateWithoutHook()
{
    METHOD_ENTRY("CCamera::updateWithoutHook")
    
    std::vector<Vector2d>   vecFrame(4);
    Rotation2Dd             CameraRotation(m_fAngle);
    Vector2i                vecCell;
    
    IUniverseScaled::separateCenterCell(m_vecPosition, m_vecPosition, vecCell);
    m_vecCenter = m_vecPosition;
    m_vecCell  += vecCell;
    
    // The frame doesn't need to care about the grid. If it is large, the camera is zoomed out.
    // Hence, accuracy is low, so it can stay with the double value.
    vecFrame[0] = CameraRotation * m_vecFrame0[0]/m_fZoom + m_vecPosition;
    vecFrame[1] = CameraRotation * m_vecFrame0[1]/m_fZoom + m_vecPosition;
    vecFrame[2] = CameraRotation * m_vecFrame0[2]/m_fZoom + m_vecPosition;
    vecFrame[3] = CameraRotation * m_vecFrame0[3]/m_fZoom + m_vecPosition;
    
    m_BoundingBox.setLowerLeft(vecFrame[0]);
    m_BoundingBox.setUpperRight(vecFrame[0]);
    m_BoundingBox.update(vecFrame[1]);
    m_BoundingBox.update(vecFrame[2]);
    m_BoundingBox.update(vecFrame[3]);
}