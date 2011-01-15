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
/// \file       rectangle.cpp
/// \brief      Implementation of class "CRectangle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "rectangle.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CRectangle::CRectangle()
{
    METHOD_ENTRY("CRectangle::CRectangle()");
    CTOR_CALL("CRectangle::CRectangle()");
    
    m_vecLL0.setZero();
    m_vecLR0.setZero();
    m_vecUL0.setZero();
    m_vecUR0.setZero();
    
    METHOD_EXIT("CRectangle::CRectangle()");
}
///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CRectangle::~CRectangle()
{
    METHOD_ENTRY("CRectangle::~CRectangle()");
    DTOR_CALL("CRectangle::~CRectangle()");

    METHOD_EXIT("CRectangle::~CRectangle()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CRectangle::transform(const double& _fAngle, const Vector2d& _vecV)
{
    METHOD_ENTRY("CRectangle::transform(const double&, const Vector2d&)");

    Rotation2Dd Rotation(_fAngle);

    m_vecVerts[0] = Rotation * m_vecLL0 + _vecV;
    m_vecVerts[1] = Rotation * m_vecUL0 + _vecV;
    m_vecVerts[2] = Rotation * m_vecUR0 + _vecV;
    m_vecVerts[3] = Rotation * m_vecLR0 + _vecV;

    // Reinitialise bounding box
    m_AABB.setLowerLeft( m_vecVerts[0]);
    m_AABB.setUpperRight(m_vecVerts[0]);

    // Update bounding box
    for (int i=0; i<4; ++i)
    {
        m_AABB.update(m_vecVerts[i]);
    }

    METHOD_EXIT("CRectangle::transform(const double&, const Vector2d&)");
}
