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
        MEM_FREED("m_pShapeBuf")
        m_pShapeBuf = 0;
    }
    if (m_pShapeCur != 0)
    {
        delete m_pShapeCur;
        MEM_FREED("m_pShapeCur")
        m_pShapeCur = 0;
    }
}
