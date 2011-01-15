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
/// \file       spring_visuals.cpp
/// \brief      Implementation of class "CSpringVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "spring_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pSpring Spring to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CSpringVisuals::CSpringVisuals(CSpring* _pSpring): m_pSpring(_pSpring)
{
    METHOD_ENTRY("CSpringVisuals::CSpringVisuals")
    CTOR_CALL("CSpringVisuals::CSpringVisuals")
    
    METHOD_EXIT("CSpringVisuals::CSpringVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CSpringVisuals::~CSpringVisuals()
{
    METHOD_ENTRY("CSpringVisuals::~CSpringVisuals()");
    DTOR_CALL("CSpringVisuals::~CSpringVisuals()");

    METHOD_EXIT("CSpringVisuals::~CSpringVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Spring
///
///////////////////////////////////////////////////////////////////////////////
void CSpringVisuals::draw() const
{
    METHOD_ENTRY("CSpringVisuals::draw()");

    Vector2d vec1 = m_pSpring->getObjectA()->getAnchor(m_pSpring->getAnchorIDA());
    Vector2d vec2 = m_pSpring->getObjectB()->getAnchor(m_pSpring->getAnchorIDB());
    Vector2d vec21 = vec2 - vec1;
    Vector2d vecOrth = Vector2d(-vec21[1], vec21[0]).normalized()*2.0;

    m_Graphics.beginLine(GRAPHICS_LINETYPE_STRIP, -15.0);
    m_Graphics.addVertex(vec1);

    m_Graphics.addVertex(vec1+vec21*0.1);
    m_Graphics.addVertex(vec1+vec21*0.2 - vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.3 + vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.4 - vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.5 + vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.6 - vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.7 + vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.8 - vecOrth);
    m_Graphics.addVertex(vec1+vec21*0.9);

    m_Graphics.addVertex(vec2);
    m_Graphics.endLine();
    
    METHOD_EXIT("CSpringVisuals::draw()");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of spring
///
/// \return Bounding box
///
/// \todo Return valid bounding box
///
////////////////////////////////////////////////////////////////////////////////
const CBoundingBox& CSpringVisuals::getBoundingBox()
{
    METHOD_ENTRY("CSpringVisuals::getBoundingBox")
    
    Vector2d vec1 = m_pSpring->getObjectA()->getAnchor(m_pSpring->getAnchorIDA());
    Vector2d vec2 = m_pSpring->getObjectB()->getAnchor(m_pSpring->getAnchorIDB());
    Vector2d vec21 = vec2 - vec1;
    Vector2d vecOrth = Vector2d(-vec21[1], vec21[0]).normalized()*2.0;
    
    m_BoundingBox.setLowerLeft(vec1-vecOrth);
    m_BoundingBox.setUpperRight(vec1-vecOrth);
    m_BoundingBox.update(vec1+vecOrth);
    m_BoundingBox.update(vec2-vecOrth);
    m_BoundingBox.update(vec2+vecOrth);

    METHOD_EXIT("CSpringVisuals::getBoundingBox")
    return m_BoundingBox;
}