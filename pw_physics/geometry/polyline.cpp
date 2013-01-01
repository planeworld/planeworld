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
/// \file       polyline.cpp
/// \brief      Implementation of class "CPolyLine"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "polyline.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CPolyLine::CPolyLine() : m_LineType(GRAPHICS_LINETYPE_STRIP)
{
    METHOD_ENTRY("CPolyLine::CPolyLine()");
    CTOR_CALL("CPolyLine::CPolyLine()");
    
    METHOD_EXIT("CPolyLine::CPolyLine()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPolyLine::~CPolyLine()
{
    METHOD_ENTRY("CPolyLine::~CPolyLine()");
    DTOR_CALL("CPolyLine::~CPolyLine()");

    METHOD_EXIT("CPolyLine::~CPolyLine()");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones polyline
///
/// \return Pointer to cloned polyline
///
////////////////////////////////////////////////////////////////////////////////
CPolyLine* CPolyLine::clone() const
{
    METHOD_ENTRY("CPolyLine::clone")
    
    CPolyLine* pClone = new CPolyLine();
    MEM_ALLOC("pClone")
    
    pClone->m_LineType     = m_LineType;
    pClone->m_VertList     = m_VertList;
    pClone->m_VertList0    = m_VertList0;
    pClone->m_AABB         = m_AABB;
    pClone->m_nDepthlayers = m_nDepthlayers;
    
    METHOD_EXIT("CPolyLine::clone")
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
void CPolyLine::transform(const double& _fAngle, const Vector2d& _vecV)
{
    METHOD_ENTRY("CPolyLine::transform(const double&, const Vector2d&)");

    Rotation2Dd Rotation(_fAngle);

    std::list<Vector2d>::const_iterator ci = m_VertList0.begin();
    std::list<Vector2d>::iterator it = m_VertList.begin();
    
    m_AABB.setLowerLeft( Rotation * (*ci) + _vecV);
    m_AABB.setUpperRight(Rotation * (*ci) + _vecV);
    
    (*it) = Rotation * (*ci) + _vecV;
    ++it;
    ++ci;
    
    while (ci != m_VertList0.end())
    {
        (*it) = Rotation * (*ci) + _vecV;

        // Update bounding box
        m_AABB.update((*it));
        ++it;
        ++ci;
    }

    METHOD_EXIT("CPolyLine::transform(const double&, const Vector2d&)");
}
