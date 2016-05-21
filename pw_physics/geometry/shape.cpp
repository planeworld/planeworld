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
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
IShape::IShape() : m_nDepthlayers(SHAPE_DEPTH_ALL)
{
    METHOD_ENTRY("IShape::IShape")
    CTOR_CALL("IShape::IShape")
    
    m_vecCentroid.setZero();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies shape information and calls copy for inherited shapes
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void IShape::copy(const IShape* const _pShape)
{
    METHOD_ENTRY("IShape::copy")

    m_AABB            = _pShape->m_AABB;
    m_bIsValid        = _pShape->m_bIsValid;
    m_fMass = _pShape->m_fMass;
    m_fArea           = _pShape->m_fArea;
    m_nDepthlayers    = _pShape->m_nDepthlayers;
    m_vecCentroid     = _pShape->m_vecCentroid;
    
    this->myCopy(_pShape);
}

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
    
    // From IUniqueIDUser
    _is >> _pShape->m_UID;
    
    _is >> _pShape->m_AABB;
    _is >> _pShape->m_bIsValid;
    _is >> _pShape->m_fMass;
    _is >> _pShape->m_fArea;
    _is >> _pShape->m_nDepthlayers;
    _is >> _pShape->m_vecCentroid[0];
    _is >> _pShape->m_vecCentroid[1];
    
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
    _os << _pShape->m_bIsValid << std::endl;
    _os << _pShape->m_fMass << std::endl;
    _os << _pShape->m_fArea << std::endl;
    _os << _pShape->m_nDepthlayers << std::endl;
    _os << _pShape->m_vecCentroid[0] << " " << 
           _pShape->m_vecCentroid[1] << std::endl;
    
    return _pShape->myStreamOut(_os);
}
