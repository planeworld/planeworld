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
    MEM_ALLOC("pClone")
        
    pClone->m_vecCenter    = m_vecCenter;
    pClone->m_vecCenter0   = m_vecCenter0;
    pClone->m_fAngle       = m_fAngle;
    pClone->m_fRadius      = m_fRadius;
    pClone->m_AABB         = m_AABB;
    pClone->m_nDepthlayers = m_nDepthlayers;
    
    return pClone;
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
void CCircle::copy(const IShape* const _pShape)
{
    METHOD_ENTRY("CCircle::copy");
    
    const CCircle* const pCircle = static_cast<const CCircle* const>(_pShape);
        
    m_vecCenter    = pCircle->m_vecCenter;
    m_vecCenter0   = pCircle->m_vecCenter0;
    m_fAngle       = pCircle->m_fAngle;
    m_fRadius      = pCircle->m_fRadius;
    m_AABB         = pCircle->m_AABB;
    m_nDepthlayers = pCircle->m_nDepthlayers;
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
