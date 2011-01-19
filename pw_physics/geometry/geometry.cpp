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

    for (std::list< IShape* >::iterator it = m_ShapeList.begin();
        it != m_ShapeList.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IShape*")
        }
    }
    
    for (std::list< IShape* >::iterator it = m_PrevShapeList.begin();
        it != m_PrevShapeList.end(); ++it)
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

    m_ShapeList.push_back(_pShape);
    
    switch (_pShape->getShapeType())
    {
        case SHAPE_CIRCLE:
        {
            CCircle* pS = new CCircle();
            MEM_ALLOC("pS");
            *pS = *(static_cast<CCircle*>(_pShape));
            m_PrevShapeList.push_back(pS);
            break;
        }
        case SHAPE_RECTANGLE:
        {
            CRectangle* pS = new CRectangle();
            MEM_ALLOC("pS");
            *pS = *(static_cast<CRectangle*>(_pShape));
            m_PrevShapeList.push_back(pS);
            break;
        }
        case SHAPE_POLYLINE:
        {
            CPolyLine* pS = new CPolyLine();
            MEM_ALLOC("pS");
            *pS = *(static_cast<CPolyLine*>(_pShape));
            m_PrevShapeList.push_back(pS);
            break;
        }
        case SHAPE_NONE:
            break;
    }

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

    m_ShapeList = _ShapeList;
    
    std::list<IShape*>::const_iterator ci = m_ShapeList.begin();
    
    while (ci != m_ShapeList.end())
    {
        switch ((*ci)->getShapeType())
        {
            case SHAPE_CIRCLE:
            {
                CCircle* pS = new CCircle();
                MEM_ALLOC("pS");
                *pS = *(static_cast<CCircle*>((*ci)));
                m_PrevShapeList.push_back(pS);
                break;
            }
            case SHAPE_RECTANGLE:
            {
                CRectangle* pS = new CRectangle();
                MEM_ALLOC("pS");
                *pS = *(static_cast<CRectangle*>((*ci)));
                m_PrevShapeList.push_back(pS);
                break;
            }
            case SHAPE_POLYLINE:
            {
                CPolyLine* pS = new CPolyLine();
                MEM_ALLOC("pS");
                *pS = *(static_cast<CPolyLine*>((*ci)));
                m_PrevShapeList.push_back(pS);
                break;
            }
            case SHAPE_NONE:
                break;
        }
        ++ci;
    }
    
    METHOD_EXIT("CGeometry::setShapes")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy current shapelist as shapes of previous time step
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::update()
{
    METHOD_ENTRY("CGeometry::update")

    std::list<IShape*>::const_iterator ci = m_ShapeList.begin();
    std::list<IShape*>::iterator it = m_PrevShapeList.begin();
    while (ci != m_ShapeList.end())
    {
        switch ((*ci)->getShapeType())
        {
            case SHAPE_CIRCLE:
            {
                *(static_cast<CCircle*>((*it))) = *(static_cast<CCircle*>((*ci)));
                break;
            }
            case SHAPE_RECTANGLE:
            {
                *(static_cast<CRectangle*>((*it))) = *(static_cast<CRectangle*>((*ci)));
                break;
            }
            case SHAPE_POLYLINE:
            {
                *(static_cast<CPolyLine*>((*it))) = *(static_cast<CPolyLine*>((*ci)));
                break;
            }
            case SHAPE_NONE:
                break;
        }
        ++ci;
        ++it;
    }
    
    METHOD_EXIT("CGeometry::update")
}