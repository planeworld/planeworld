////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Spring
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CSpringVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CSpringVisuals::draw()");

    Vector2d vec1 = m_pSpring->getObjectA()->getAnchor(m_pSpring->getAnchorIDA()) -
                     _pCamera->getCenter();
    Vector2d vec2 = m_pSpring->getObjectB()->getAnchor(m_pSpring->getAnchorIDB()) -
                     _pCamera->getCenter();
    Vector2d vec21 = vec2 - vec1;
    Vector2d vecOrth = Vector2d(-vec21[1], vec21[0]).normalized()*2.0;

    m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_STRIP, -15.0);
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

    return m_BoundingBox;
}