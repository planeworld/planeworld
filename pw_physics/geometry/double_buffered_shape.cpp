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
/// \file       double_buffered_shape.cpp
/// \brief      Implementation of class "CDoubleBufferedShape"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2013-01-15
///
////////////////////////////////////////////////////////////////////////////////

#include "double_buffered_shape.h"

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
    
    if (m_pShapeBuf != 0)
    {
        delete m_pShapeBuf;
        MEM_FREED("IShape")
        m_pShapeBuf = 0;
    }
    if (m_pShapeCur != 0)
    {
        delete m_pShapeCur;
        MEM_FREED("IShape")
        m_pShapeCur = 0;
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
    
    if (m_pShapeCur != 0)
    {
        delete m_pShapeBuf;
        MEM_FREED("IShape");
        delete m_pShapeCur;
        MEM_FREED("IShape");
        NOTICE_MSG("Double buffered shape", "Shape already buffered. Deleting old shapes.")
    }
    
    m_pShapeCur = _pShape;
    m_pShapeBuf = _pShape->clone();
}
