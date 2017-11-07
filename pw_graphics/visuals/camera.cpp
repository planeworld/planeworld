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
#include "math_constants.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera() : IGridUser(),
                     IKinematicsStateUser(),
                     IUIDUser(),
                     m_fViewportWidth(m_Graphics.getViewPort().rightplane-m_Graphics.getViewPort().leftplane),
                     m_fViewportHeight(m_Graphics.getViewPort().topplane-m_Graphics.getViewPort().bottomplane),
                     m_fZoom(1.0)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
    m_vecCell.setZero();
    
    m_UID.setName("Cam_"+m_UID.getName());
    
    this->reset();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor
///
/// \param _Camera Camera to be constructed from
///
///////////////////////////////////////////////////////////////////////////////
CCamera::CCamera(const CCamera& _Camera) : CGraphicsBase(_Camera),
                                           IGridUser(_Camera),
                                           IKinematicsStateUser(_Camera),
                                           IUIDUser(_Camera)
{
    METHOD_ENTRY("CCamera::CCamera");
    CTOR_CALL("CCamera::CCamera");
    
    this->copy(_Camera);
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
/// \brief Copy assignment operator
///
/// \param _Camera Camera to be copied and assigned data from
///
///////////////////////////////////////////////////////////////////////////////
CCamera& CCamera::operator=(const CCamera& _Camera)
{
    METHOD_ENTRY("CCamera::operator=")
    
    if (this != &_Camera)
    {
        IKinematicsStateUser::operator=(_Camera);
        IUIDUser::operator=(_Camera);
        IGridUser::operator=(_Camera);
        this->copy(_Camera);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones camera
///
/// \return Pointer to cloned camera
///
////////////////////////////////////////////////////////////////////////////////
CCamera* CCamera::clone() const
{
    METHOD_ENTRY("CCamera::clone")
    
    CCamera* pClone = new CCamera(*this);
    MEM_ALLOC("CCamera")
    
    return pClone;
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
//     this->updateWithHook();
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
//     this->updateWithHook();
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

    if ((_fW <= m_Graphics.getViewPort().rightplane - m_Graphics.getViewPort().leftplane) &&
        (_fH <= m_Graphics.getViewPort().topplane   - m_Graphics.getViewPort().bottomplane))
    {
      m_fViewportWidth  = _fW*0.5;
      m_fViewportHeight = _fH*0.5;
    }
    else
    {
      m_fViewportWidth  = (m_Graphics.getViewPort().rightplane - m_Graphics.getViewPort().leftplane)*0.5;
      m_fViewportHeight = (m_Graphics.getViewPort().topplane   - m_Graphics.getViewPort().bottomplane)*0.5;
      NOTICE_MSG("Camera", "Given viewport is larger than actual screen, resizing to screen size.")
      NOTICE_BLK(
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
    
    m_Graphics.rotCamTo(m_KinematicsState.getAngle());

    std::array<Vector2d,4> vecFrame;
    
    m_vecCenter = m_KinematicsState.getOrigin();

    // m_vecCenter is in absolute coordinates while m_vecCell is zero. Thus, they
    // have to be separated:
    IGridUser::separateCenterCell(m_vecCenter,m_vecCenter,m_vecCell);

    // The frame doesn't need to care about the grid. If it is large, the camera is zoomed out.
    // Hence, accuracy is low, so it can stay with the double value.
    m_KinematicsState.getPositions<4>(m_vecFrame0, vecFrame, m_fZoom);
        
    m_BoundingBox.setLowerLeft(vecFrame[0]-IGridUser::cellToDouble(m_vecCell));
    m_BoundingBox.setUpperRight(vecFrame[0]-IGridUser::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[1]-IGridUser::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[2]-IGridUser::cellToDouble(m_vecCell));
    m_BoundingBox.update(vecFrame[3]-IGridUser::cellToDouble(m_vecCell));
    
    m_fBoundingCircleRadius = std::sqrt(m_fViewportWidth*m_fViewportWidth + 
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
    
    // Limit zoom to universe limits, worst viewport case (=> sqrt(2))
    double fLimit = CKinematicsState::getWorldLimitX();
    if (fLimit > CKinematicsState::getWorldLimitY()) fLimit = CKinematicsState::getWorldLimitY();
    if (MATH_SQRT2*m_fViewportWidth/m_fZoom  > fLimit) m_fZoom = MATH_SQRT2*m_fViewportWidth/fLimit;
    if (MATH_SQRT2*m_fViewportHeight/m_fZoom > fLimit) m_fZoom = MATH_SQRT2*m_fViewportHeight/fLimit;
        
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
    
    if (_fZoom > 0.0)
    {
        m_fZoom = _fZoom;
        
        // Limit zoom to universe limits, worst viewport case (=> sqrt(2))
        double fLimit = CKinematicsState::getWorldLimitX();
        if (fLimit > CKinematicsState::getWorldLimitY()) fLimit = CKinematicsState::getWorldLimitY();
        if (MATH_SQRT2*m_fViewportWidth/m_fZoom  > fLimit) m_fZoom = MATH_SQRT2*m_fViewportWidth/fLimit;
        if (MATH_SQRT2*m_fViewportHeight/m_fZoom > fLimit) m_fZoom = MATH_SQRT2*m_fViewportHeight/fLimit;
    }
    else
    {
        NOTICE_MSG("Camera", "Invalid zoom factor " << _fZoom << ".")
    }
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
    _is >> _pCam->m_fZoom;
    /// \todo Stream IGridUser information
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pCam CCamera instance to stream
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
    
    /// \todo Stream IGridUser information
    
    return _os;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy data from given Camera
///
/// \param _Camera Camera to copy data from
///
///////////////////////////////////////////////////////////////////////////////
void CCamera::copy(const CCamera& _Camera)
{
    METHOD_ENTRY("CCamera::copy")
    
    // Copy CCamera data
    m_vecFrame0             = _Camera.m_vecFrame0;
    m_BoundingBox           = _Camera.m_BoundingBox;
    m_vecCenter             = _Camera.m_vecCenter;
    m_fBoundingCircleRadius = _Camera.m_fBoundingCircleRadius;
    m_fViewportWidth        = _Camera.m_fViewportWidth;
    m_fViewportHeight       = _Camera.m_fViewportHeight;
    m_fZoom                 = _Camera.m_fZoom;    
}
