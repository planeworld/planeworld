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
CGeometry::CGeometry() : m_pShapesCurrent(&m_Shapes1),
                         m_pShapesPrevious(&m_Shapes2)
{
    METHOD_ENTRY("CGeometry::CGeometry")
    CTOR_CALL("CGeometry::CGeometry")

    METHOD_EXIT("CGeometry::CGeometry")
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

    for (std::list< IShape* >::iterator it = m_Shapes1.begin();
        it != m_Shapes1.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IShape*")
        }
    }
    
    for (std::list< IShape* >::iterator it = m_Shapes2.begin();
        it != m_Shapes2.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IShape*")
        }
    }
    
    METHOD_EXIT("CGeometry::~CGeometry")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a shape to the current list of shapes
///
/// \param _pShape Shape, that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::addShape(IShape* _pShape)
{
    METHOD_ENTRY("CGeometry::addShape")

    m_pShapesCurrent->push_back(_pShape);
    m_pShapesPrevious->push_back(_pShape->clone());
    
    METHOD_EXIT("CGeometry::addShape")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy the given shapelist
///
/// \param _ShapeList Shapelist to be set
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::setShapes(const std::list<IShape*> _ShapeList)
{
    METHOD_ENTRY("CGeometry::setShapes")

    m_pShapesPrevious->clear();
    m_pShapesCurrent->clear();
    
    *m_pShapesCurrent = _ShapeList;
    
    std::list<IShape*>::const_iterator ci = m_pShapesCurrent->begin();
    
    while (ci != m_pShapesCurrent->end())
    {
        m_pShapesPrevious->push_back((*ci)->clone());
        ++ci;
    }
    
    METHOD_EXIT("CGeometry::setShapes")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap current shapelist with shapes of previous time step
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::update()
{
    METHOD_ENTRY("CGeometry::update")
    
//     std::list<IShape*>::iterator it = m_pShapesPrevious->begin();
//     for (std::list<IShape*>::const_iterator ci  = m_pShapesCurrent->begin();
//                                             ci != m_pShapesCurrent->end(); ++ci, ++it)
//          {
//              *(*it) = *(*ci);
//          }
    
    std::swap(m_pShapesCurrent,m_pShapesPrevious);
    
    METHOD_EXIT("CGeometry::update")
}