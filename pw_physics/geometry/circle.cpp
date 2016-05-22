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
/// \file       circle.cpp
/// \brief      Implementation of class "CCircle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "circle.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CCircle::CCircle() : m_fAngle(0.0),m_fRadius(1.0)
{
    METHOD_ENTRY("CCircle::CCircle");
    CTOR_CALL ("CCircle::CCircle");
    
    m_vecCenter0.setZero();
    m_vecCenter.setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CCircle::~CCircle()
{
    METHOD_ENTRY("CCircle::~CCircle");
    DTOR_CALL("CCircle::~CCircle");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones circle
///
/// \return Pointer to cloned circle
///
////////////////////////////////////////////////////////////////////////////////
CCircle* CCircle::clone() const
{
    METHOD_ENTRY("CCircle::clone");
    
    CCircle* pClone = new CCircle();
    MEM_ALLOC("CCircle")
        
    pClone->copy(this);
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CCircle::transform( const double& _fAngle, const Vector2d& _vecV )
{
    METHOD_ENTRY("CCircle::transform");

    Rotation2Dd Rotation(_fAngle);

    m_vecCenter = Rotation * m_vecCenter0 + _vecV;

    m_fAngle = _fAngle;

    // Update bounding box
    m_AABB.setLowerLeft( m_vecCenter - Vector2d(m_fRadius,m_fRadius));
    m_AABB.setUpperRight(m_vecCenter + Vector2d(m_fRadius,m_fRadius));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _vecC Center of the circle
///
////////////////////////////////////////////////////////////////////////////////
void CCircle::setCenter(const Vector2d& _vecC)
{
    METHOD_ENTRY("CCircle::setCenter")

    m_vecCenter0 = _vecC;
    m_vecCenter = _vecC;
    
    // Calculate COM, shape no longer valid
    m_vecCentroid = m_vecCenter0;
    m_fArea = M_PI * m_fRadius * m_fRadius;
    m_fInertia = m_fMass * m_fRadius * m_fRadius; // *0.5 for solid disc
    m_bIsValid = false;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _fX Center-x of the circle
/// \param _fY Center-y of the circle
///
////////////////////////////////////////////////////////////////////////////////
void CCircle::setCenter(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CCircle::setCenter")

    m_vecCenter0[0] = _fX;
    m_vecCenter0[1] = _fY;

    m_vecCenter[0] = _fX;
    m_vecCenter[1] = _fY;
    
    // Calculate COM, shape no longer valid
    m_vecCentroid = m_vecCenter0;
    m_fArea = M_PI * m_fRadius * m_fRadius;
    m_fInertia = m_fMass * m_fRadius * m_fRadius; // *0.5 for solid disc
    m_bIsValid = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _fRadius Radius of the circle
///
////////////////////////////////////////////////////////////////////////////////
void CCircle::setRadius(const double& _fRadius)
{
    METHOD_ENTRY("CCircle::setRadius")

    m_fRadius = _fRadius;
    
    // Calculate COM, shape no longer valid
    m_vecCentroid = m_vecCenter0;
    m_fArea = M_PI * m_fRadius * m_fRadius;
    m_fInertia = m_fMass * m_fRadius * m_fRadius; // *0.5 for solid disc
    m_bIsValid = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& CCircle::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CCircle::myStreamIn")
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> m_vecCenter[0];
    _is >> m_vecCenter[1];
    _is >> m_vecCenter0[0];
    _is >> m_vecCenter0[1];
    _is >> m_fAngle;
    _is >> m_fRadius;
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CCircle instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CCircle::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CCircle::myStreamOut")
    
    _os << "Circle" << std::endl;
    
    _os << m_vecCenter[0] << " " <<
           m_vecCenter[1] << std::endl;
    _os << m_vecCenter0[0] << " " <<
           m_vecCenter0[1] << std::endl;
    _os << m_fAngle << std::endl;
    _os << m_fRadius << std::endl;
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies information of a given circle
///
/// This method does not create a new circle, use clone in that case!
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CCircle::myCopy(const IShape* const _pShape)
{
    METHOD_ENTRY("CCircle::myCopy");
    
    const CCircle* const pCircle = static_cast<const CCircle* const>(_pShape);
        
    m_vecCenter    = pCircle->m_vecCenter;
    m_vecCenter0   = pCircle->m_vecCenter0;
    m_fAngle       = pCircle->m_fAngle;
    m_fRadius      = pCircle->m_fRadius;
}
