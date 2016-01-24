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
/// \file       shape.cpp
/// \brief      Implementation of interface "IShape"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-16
///
////////////////////////////////////////////////////////////////////////////////

#include "shape.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pShape IShape instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, IShape* const _pShape)
{
    METHOD_ENTRY("IShape::operator>>")
    
    _is >> _pShape->m_AABB;
    _is >> _pShape->m_nDepthlayers;
    
    return _pShape->myStreamIn(_is);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pShape IShape instance to stream
///
/// \return Stream with game state information of IShape instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, IShape* const _pShape)
{
    METHOD_ENTRY("IShape::operator<<")
    
    _os << _pShape->m_AABB;
    _os << _pShape->m_nDepthlayers;
    
    return _pShape->myStreamOut(_os);
}
