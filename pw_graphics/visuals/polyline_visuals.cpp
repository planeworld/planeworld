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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPolyline Polyline to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CPolylineVisuals::CPolylineVisuals(CPolyLine* _pPolyline): m_pPolyline(_pPolyline)
{
    METHOD_ENTRY("CPolylineVisuals::CPolylineVisuals")
    CTOR_CALL("CPolylineVisuals::CPolylineVisuals")
    
    METHOD_EXIT("CPolylineVisuals::CPolylineVisuals")
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

    METHOD_EXIT("CPolylineVisuals::~CPolylineVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the polyline
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CPolylineVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CPolylineVisuals::draw")

    m_Graphics.beginLine(m_pPolyline->getLineType(), SHAPE_DEFAULT_DEPTH);

    std::list<Vector2d>::const_iterator ci = m_pPolyline->getVertices().begin();
    while (ci != m_pPolyline->getVertices().end())
    {
        m_Graphics.addVertex((*ci)-_pCamera->getPosition());
        ++ci;
    }

    m_Graphics.endLine();

    METHOD_EXIT("CPolylineVisuals::draw")
}