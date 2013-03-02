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
/// \file       geometry.cpp
/// \brief      Implementation of class "CGeometry"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-18
///
////////////////////////////////////////////////////////////////////////////////

#include "geometry.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CGeometry::CGeometry()
{
    METHOD_ENTRY("CGeometry::CGeometry")
    CTOR_CALL("CGeometry::CGeometry")
    
    m_pShapes = new std::list<CDoubleBufferedShape*>();
    MEM_ALLOC("m_pShapes")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
CGeometry::~CGeometry()
{
    METHOD_ENTRY("CGeometry::~CGeometry")
    DTOR_CALL("CGeometry::~CGeometry")

    for (std::list< CDoubleBufferedShape* >::iterator it = m_pShapes->begin();
        it != m_pShapes->end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("CDoubleBufferedShape*")
        }
    }
    
    // Free memory if pointer is still existent
    if (m_pShapes != 0)
    {
        delete m_pShapes;
        m_pShapes = 0;
        MEM_FREED("m_pShapes")
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a shape to the current list of shapes
///
/// \param _pShape Shape, that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::addShape(CDoubleBufferedShape* const _pShape)
{
    METHOD_ENTRY("CGeometry::addShape")
    m_pShapes->push_back(_pShape);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy the given shapelist
///
/// \param _pShapeList Shapelist to be set
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::setShapes(std::list<CDoubleBufferedShape*>* _pShapeList)
{
    METHOD_ENTRY("CGeometry::setShapes")

    m_pShapes->clear();
    m_pShapes = _pShapeList;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap current shapelist with shapes of previous time step
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::update()
{
    METHOD_ENTRY("CGeometry::update")
    
    for (std::list<CDoubleBufferedShape*>::iterator it  = m_pShapes->begin();
                                                    it != m_pShapes->end(); ++it)
    {
        (*it)->swapBuffer();
    }
}