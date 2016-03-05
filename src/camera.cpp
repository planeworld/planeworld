////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
CCamera::CCamera() : m_fViewportWidth(m_Graphics.getViewPort().right-m_Graphics.getViewPort().left),
                     m_fViewportHeight(m_Graphics.getViewPort().top-m_Graphics.getViewPort().bottom)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
    m_vecCell.setZero();
    
//     IHooker::m_strName += ": Camera";
    
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras bounding box
///
/// The bounding box of the camera is returned. It surrounds the viewport and
/// can be used for culling.
///
/// \return The bounding box
///
////////////////////////////////////////////////////////////////////////////////
const CBoundingBox CCamera::getBoundingBox()
{
    METHOD_ENTRY("CCamera::getBoundingBox")
    this->updateWithHook();
    return m_BoundingBox;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns radius of cameras bounding circle
///
/// The radius of the bounding circle of the camera is returned. The bounding
/// circle surrounds the viewport and can be used for culling like the bbox.
///
/// \return The bounding circle's radius
///
////////////////////////////////////////////////////////////////////////////////
const double& CCamera::getBoundingCircleRadius() 
{
    METHOD_ENTRY("CCamera::BoundingCircleRadius")
    this->updateWithHook();
    return m_fBoundingCircleRadius;
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

    m_KinematicsState.setOrigin(Vector2d(_fX, _fY));
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

    if ((_fW <= m_Graphics.getViewPort().right - m_Graphics.getViewPort().left) &&
        (_fH <= m_Graphics.getViewPort().top   - m_Graphics.getViewPort().bottom))
    {
      m_fViewportWidth  = _fW*0.5;
      m_fViewportHeight = _fH*0.5;
    }
    else
    {
      m_fViewportWidth  = (m_Graphics.getViewPort().right - m_Graphics.getViewPort().left)*0.5;
      m_fViewportHeight = (m_Graphics.getViewPort().top   - m_Graphics.getViewPort().bottom)*0.5;
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
    m_Graphics.rotCamTo(m_KinematicsState.getAngle());
//     if (!m_bIsHooked)
//     {
//         updateWithoutHook();        
//         m_Graphics.rotCamTo(m_KinematicsState.getLocalAngle());
//     }
//     else
    {
        updateWithHook();
//         m_Graphics.rotCamTo(m_KinematicsState.getAngleReferredTo(m_KinematicsStateReference));
    }
    
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

//     m_KinematicsState.getPosition().setZero();
    m_vecCell.setZero();
    m_vecCenter.setZero();
    m_KinematicsState.setAngle(0.0);
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
    m_KinematicsState.increaseAngle(_fAngle);
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
    m_KinematicsState.setAngle(_fAngle);
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
    m_KinematicsState.setOrigin(m_KinematicsState.getLocalPosition( _vecV));
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

    m_KinematicsState.setOrigin(_vecV);
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
    
    std::array<Vector2d,4> vecFrame;
    
    m_vecCenter = m_KinematicsState.getOrigin();// + IUniverseScaled::cellToDouble(m_KinematicsStateReference.getCell());

    // m_vecCenter is in absolute coordinates while m_vecCell is zero. Thus, they
    // have to be separated:
    IUniverseScaled::separateCenterCell(m_vecCenter,m_vecCenter,m_vecCell);

    // The frame doesn't need to care about the grid. If it is large, the camera is zoomed out.
    // Hence, accuracy is low, so it can stay with the double value.
    vecFrame[0] = m_KinematicsState.getPosition(m_vecFrame0[0]/m_fZoom);// + IUniverseScaled::cellToDouble(m_KinematicsStateReference.getCell());
    vecFrame[1] = m_KinematicsState.getPosition(m_vecFrame0[1]/m_fZoom);// + IUniverseScaled::cellToDouble(m_KinematicsStateReference.getCell());
    vecFrame[2] = m_KinematicsState.getPosition(m_vecFrame0[2]/m_fZoom);// + IUniverseScaled::cellToDouble(m_KinematicsStateReference.getCell());
    vecFrame[3] = m_KinematicsState.getPosition(m_vecFrame0[3]/m_fZoom);// + IUniverseScaled::cellToDouble(m_KinematicsStateReference.getCell());
    
    m_BoundingBox.setLowerLeft(vecFrame[0]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.setUpperRight(vecFrame[0]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[1]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[2]-IUniverseScaled::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[3]-IUniverseScaled::cellToDouble(m_vecCell));
    
    m_fBoundingCircleRadius = sqrt(m_fViewportWidth*m_fViewportWidth + 
                                   m_fViewportHeight*m_fViewportHeight)/m_fZoom;
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
    
    std::array<Vector2d,4>  vecFrame;
    Vector2i                vecCell;
    
    IUniverseScaled::separateCenterCell(m_KinematicsState.getLocalOrigin(), m_KinematicsState.Origin(), vecCell);
    m_vecCenter = m_KinematicsState.getLocalOrigin();
    m_vecCell  += vecCell;
    
    // The frame doesn't need to care about the grid. If it is large, the camera is zoomed out.
    // Hence, accuracy is low, so it can stay with the double value.
    vecFrame[0] = m_KinematicsState.getPosition(m_vecFrame0[0]/m_fZoom);
    vecFrame[1] = m_KinematicsState.getPosition(m_vecFrame0[1]/m_fZoom);
    vecFrame[2] = m_KinematicsState.getPosition(m_vecFrame0[2]/m_fZoom);
    vecFrame[3] = m_KinematicsState.getPosition(m_vecFrame0[3]/m_fZoom);
    
    m_BoundingBox.setLowerLeft(vecFrame[0]);
    m_BoundingBox.setUpperRight(vecFrame[0]);
    m_BoundingBox.update(vecFrame[1]);
    m_BoundingBox.update(vecFrame[2]);
    m_BoundingBox.update(vecFrame[3]);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pCam CCamera instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CCamera* const _pCam)
{
    METHOD_ENTRY("CCamera::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IKinematicsStateUser
    _is >> _pCam->m_KinematicsState;
    
    for (auto i=0u; i<4u; ++i)
    {
        _is >> _pCam->m_vecFrame0[i][0] >>
               _pCam->m_vecFrame0[i][1];
    }
    _is >> _pCam->m_BoundingBox;
    _is >> _pCam->m_vecCenter[0] >> _pCam->m_vecCenter[1];
    _is >> _pCam->m_fBoundingCircleRadius;
    _is >> _pCam->m_fViewportWidth;
    _is >> _pCam->m_fViewportHeight;
    _is >> _pCam->m_fZoom
    ;
    /// \todo Stream IUniverseScaled information
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _UID CCamera instance to stream
///
/// \return Stream with game state information of CCamera instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CCamera* const _pCam)
{
    METHOD_ENTRY("CCamera::operator<<")
    
    _os << "Camera:" << std::endl;
    
    // From IKinematicsStateUser
    _os << _pCam->m_KinematicsState << std::endl;
    
    for (auto i=0u; i<4u; ++i)
    {
        _os << _pCam->m_vecFrame0[i][0] << " " <<
               _pCam->m_vecFrame0[i][1] << std::endl;
    }
    _os << _pCam->m_BoundingBox << std::endl;
    _os << _pCam->m_vecCenter[0] << " " <<
           _pCam->m_vecCenter[1] << std::endl;
    _os << _pCam->m_fBoundingCircleRadius << std::endl;
    _os << _pCam->m_fViewportWidth << std::endl;
    _os << _pCam->m_fViewportHeight << std::endl;
    _os << _pCam->m_fZoom << std::endl;
    
    /// \todo Stream IUniverseScaled information
    
    return _os;
}
