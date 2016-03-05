////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> _pShape->m_UID;
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

    // ShapeType has to be the first information, since object creation when
    // loading depends on it.
    // Cast strongly typed enum ShapeType to streamable base type
    auto nShapeType = static_cast<std::underlying_type<ShapeType>::type>(_pShape->getShapeType());
    _os << nShapeType << std::endl;

    _os << "Shape:" << std::endl;
    
    // From IUniqueIDUser
    _os << _pShape->m_UID << std::endl;
    
    _os << _pShape->m_AABB << std::endl;
    _os << _pShape->m_nDepthlayers << std::endl;
    
    return _pShape->myStreamOut(_os);
}
