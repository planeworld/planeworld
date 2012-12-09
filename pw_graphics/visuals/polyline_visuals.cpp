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
/// \brief Draw the polyline
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CPolylineVisuals::draw(const CCamera* const _pCamera,
                            const IObject* const _pObject) const
{
    METHOD_ENTRY("CPolylineVisuals::draw")

    m_Graphics.polyline(m_pPolyline->getVertices(), m_pPolyline->getLineType(),
                        -_pCamera->getCenter() + 
                        (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2);
}
