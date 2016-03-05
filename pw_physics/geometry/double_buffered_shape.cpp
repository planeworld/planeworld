////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2013-2016 Torsten Büschenfeld
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
/// \file       double_buffered_shape.cpp
/// \brief      Implementation of class "CDoubleBufferedShape"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2013-01-15
///
////////////////////////////////////////////////////////////////////////////////

#include "double_buffered_shape.h"

#include "circle.h"
#include "planet.h"
#include "polyline.h"
#include "terrain.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CDoubleBufferedShape::CDoubleBufferedShape() : m_pShapeBuf(0),m_pShapeCur(0)
{
    METHOD_ENTRY ( "CDoubleBufferedShape::CDoubleBufferedShape" )
    CTOR_CALL ( "CDoubleBufferedShape::CDoubleBufferedShape" )
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
CDoubleBufferedShape::~CDoubleBufferedShape()
{
    METHOD_ENTRY ( "CDoubleBufferedShape::~CDoubleBufferedShape" )
    DTOR_CALL ( "CDoubleBufferedShape::~CDoubleBufferedShape" )
    
    if (m_pShapeBuf != nullptr)
    {
        delete m_pShapeBuf;
        MEM_FREED("IShape")
        m_pShapeBuf = nullptr;
    }
    if (m_pShapeCur != nullptr)
    {
        delete m_pShapeCur;
        MEM_FREED("IShape")
        m_pShapeCur = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones a double buffered shape
///
/// \return Pointer to cloned double buffered shape
///
////////////////////////////////////////////////////////////////////////////////
CDoubleBufferedShape* CDoubleBufferedShape::clone() const
{
    METHOD_ENTRY("CDoubleBufferedShape::clone")
    
    CDoubleBufferedShape* pClone = new CDoubleBufferedShape();
    MEM_ALLOC("CDoubleBufferedShape")
    
    pClone->m_pShapeCur = m_pShapeCur->clone();
    pClone->m_pShapeBuf = m_pShapeBuf->clone();
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Buffer a given shape
///
/// \param _pShape Shape to buffer
///
///////////////////////////////////////////////////////////////////////////////
void CDoubleBufferedShape::buffer(IShape* const _pShape)
{
    METHOD_ENTRY ( "CDoubleBufferedShape::buffer")
    
    if (m_pShapeCur != nullptr)
    {
        delete m_pShapeBuf;
        MEM_FREED("IShape");
        m_pShapeBuf = nullptr;
        delete m_pShapeCur;
        MEM_FREED("IShape");
        m_pShapeCur = nullptr;
        NOTICE_MSG("Double buffered shape", "Shape already buffered. Deleting old shapes.")
    }
    
    m_pShapeCur = _pShape;
    m_pShapeBuf = _pShape->clone();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _DBS CDoubleBufferedShape instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CDoubleBufferedShape& _DBS)
{
    METHOD_ENTRY("CDoubleBufferedShape::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IUniqueIDUser
    _is >> _DBS.m_UID;
    
    if (_DBS.m_pShapeBuf != nullptr)
    {
        delete _DBS.m_pShapeBuf;
        MEM_FREED("IShape")
        _DBS.m_pShapeBuf = nullptr;
    }
    if (_DBS.m_pShapeCur != nullptr)
    {
        delete _DBS.m_pShapeCur;
        MEM_FREED("IShape")
        _DBS.m_pShapeCur = nullptr;
    }
    
    // Cast streamable basetype to strongly typed enum ShapeType
    std::underlying_type<ShapeType>::type nShapeType;
    _is >> nShapeType;
    ShapeType CurrentShapeType = static_cast<ShapeType>(nShapeType);

    switch (CurrentShapeType)
    {
        case ShapeType::SHAPE_CIRCLE:
            _DBS.m_pShapeBuf = new CCircle;
            _DBS.m_pShapeCur = new CCircle;
            MEM_ALLOC("IShape")
            MEM_ALLOC("IShape")
            break;
        case ShapeType::SHAPE_PLANET:
            _DBS.m_pShapeBuf = new CPlanet;
            _DBS.m_pShapeCur = new CPlanet;
            MEM_ALLOC("IShape")
            MEM_ALLOC("IShape")
            break;
        case ShapeType::SHAPE_POLYLINE:
            _DBS.m_pShapeBuf = new CPolyLine;
            _DBS.m_pShapeCur = new CPolyLine;
            MEM_ALLOC("IShape")
            MEM_ALLOC("IShape")
            break;
        case ShapeType::SHAPE_TERRAIN:
            _DBS.m_pShapeBuf = new CTerrain;
            _DBS.m_pShapeCur = new CTerrain;
            MEM_ALLOC("IShape")
            MEM_ALLOC("IShape")
            break;
        case ShapeType::SHAPE_NONE:
            break;
    }
    
    _is >> _DBS.m_pShapeCur >> nShapeType;
    _is >> _DBS.m_pShapeBuf;
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _DBS CDoubleBufferedShape instance to stream
///
/// \return Stream with game state information of CDoubleBufferedShape instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CDoubleBufferedShape& _DBS)
{
    METHOD_ENTRY("CDoubleBufferedShape::operator<<")
    
    _os << "DoubleBufferedShape:" << std::endl;
    
    // From IUniqueIDUser
    _os << _DBS.m_UID << std::endl;
    
    _os << _DBS.m_pShapeCur << std::endl;
    _os << _DBS.m_pShapeBuf << std::endl;
    
    return _os;
}
