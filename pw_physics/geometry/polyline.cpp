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

#include <type_traits>

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
    MEM_ALLOC("CPolyLine")
    
    pClone->m_LineType     = m_LineType;
    pClone->m_VertList     = m_VertList;
    pClone->m_VertList0    = m_VertList0;
    pClone->m_AABB         = m_AABB;
    pClone->m_nDepthlayers = m_nDepthlayers;
    
    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies information of a given polyline
///
/// This method does not create a new polyline, use clone in that case!
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CPolyLine::copy(const IShape* const _pShape)
{
    METHOD_ENTRY("CPolyLine::copy");
    
    const CPolyLine* const pPolyLine = static_cast<const CPolyLine* const>(_pShape);
        
    m_LineType     = pPolyLine->m_LineType;
    m_VertList     = pPolyLine->m_VertList;
    m_VertList0    = pPolyLine->m_VertList0;
    m_AABB         = pPolyLine->m_AABB;
    m_nDepthlayers = pPolyLine->m_nDepthlayers;
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
std::istream& CPolyLine::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CPolyLine::myStreamIn")
    
    // Cast streamable basetype to strongly typed enum LineType
    std::underlying_type<LineType>::type nLinetype;
    _is >> nLinetype; m_LineType = static_cast<LineType>(nLinetype);
    
//      _os << m_VertList.size();
//     for (auto ci : m_VertList)
//         _os << ci;
//     for (auto ci : m_VertList0)
//         _os << ci;
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CPolyLine instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CPolyLine::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CPolyLine::myStreamOut")

    // Cast strongly typed enum LineType to streamable base type
    auto nLineType = static_cast<std::underlying_type<LineType>::type>(m_LineType);
    
    _os << nLineType;
    _os << m_VertList.size();
    for (auto ci : m_VertList)
        _os << ci;
    for (auto ci : m_VertList0)
        _os << ci;
    
    _os << "CPolyLine::streamOut TEST";
    
    return _os;
}
