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
/// \file       rectangle_visuals.cpp
/// \brief      Implementation of class "CRectangleVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "rectangle_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pRect Rectangle to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CRectangleVisuals::CRectangleVisuals(CRectangle* _pRect): m_pRectangle(_pRect)
{
    METHOD_ENTRY("CRectangleVisuals::CRectangleVisuals")
    CTOR_CALL("CRectangleVisuals::CRectangleVisuals")
    
    METHOD_EXIT("CRectangleVisuals::CRectangleVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CRectangleVisuals::~CRectangleVisuals()
{
    METHOD_ENTRY("CRectangleVisuals::~CRectangleVisuals()");
    DTOR_CALL("CRectangleVisuals::~CRectangleVisuals()");

    METHOD_EXIT("CRectangleVisuals::~CRectangleVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the rectangle
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CRectangleVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CRectangleVisuals::draw()");

    m_Graphics.beginLine(GRAPHICS_LINETYPE_LOOP, SHAPE_DEFAULT_DEPTH);
        m_Graphics.addVertex(m_pRectangle->getVertices()[0]-_pCamera->getCenter());
        m_Graphics.addVertex(m_pRectangle->getVertices()[1]-_pCamera->getCenter());
        m_Graphics.addVertex(m_pRectangle->getVertices()[2]-_pCamera->getCenter());
        m_Graphics.addVertex(m_pRectangle->getVertices()[3]-_pCamera->getCenter());
    m_Graphics.endLine();

    METHOD_EXIT("CRectangleVisuals::draw()");
}
