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
/// \file       circle_visuals.cpp
/// \brief      Implementation of class "CPolylineVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "polyline_visuals.h"

#include "polyline.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPolyline Polyline to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CPolylineVisuals::CPolylineVisuals(CDoubleBufferedShape* const _pPolyline)
{
    METHOD_ENTRY("CPolylineVisuals::CPolylineVisuals")
    CTOR_CALL("CPolylineVisuals::CPolylineVisuals")
    
    this->attach(_pPolyline);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPolylineVisuals::~CPolylineVisuals()
{
    METHOD_ENTRY("CPolylineVisuals::~CPolylineVisuals")
    DTOR_CALL("CPolylineVisuals::~CPolylineVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone a polyline shape visual
///
/// \param _pShape Shape the visuals are attached to
///
/// \return Clone of polyline shape visuals
///
///////////////////////////////////////////////////////////////////////////////
IVisuals* CPolylineVisuals::clone(CDoubleBufferedShape* const _pShape) const
{
    METHOD_ENTRY("CPolylineVisuals::clone");
    
    CPolylineVisuals* pClone = new CPolylineVisuals(_pShape);
    MEM_ALLOC("CPolylineVisuals")
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the polyline
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CPolylineVisuals::draw(CCamera* const _pCamera,
                            const IObject* const _pObject) const
{
    METHOD_ENTRY("CPolylineVisuals::draw")

    CPolyLine* pPolyline = static_cast<CPolyLine*>(m_pPolyline->getShapeCur());
    
    m_Graphics.polyline(pPolyline->getVertices(), pPolyline->getLineType(),
                        -_pCamera->getCenter() + 
                        (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a polyline to polyline visuals
///
/// \param _pPolyline Polyline to attach
///
////////////////////////////////////////////////////////////////////////////////
void CPolylineVisuals::attach(CDoubleBufferedShape* const _pPolyline)
{
    METHOD_ENTRY("CPolylineVisuals::attach")
    if (_pPolyline->getShapeCur()->getShapeType() == SHAPE_POLYLINE)
    {
        m_pPolyline = _pPolyline;
    }
    else
    {
        ERROR_MSG("Polyline Visuals", "Wrong shape attached to visuals.")
    }
}
