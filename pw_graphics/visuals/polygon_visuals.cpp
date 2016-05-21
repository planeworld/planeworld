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
/// \file       circle_visuals.cpp
/// \brief      Implementation of class "CPolygonVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "polygon_visuals.h"

#include "polygon.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPolygon Polygon to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CPolygonVisuals::CPolygonVisuals(CDoubleBufferedShape* const _pPolygon)
{
    METHOD_ENTRY("CPolygonVisuals::CPolygonVisuals")
    CTOR_CALL("CPolygonVisuals::CPolygonVisuals")
    
    this->attachTo(_pPolygon);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPolygonVisuals::~CPolygonVisuals()
{
    METHOD_ENTRY("CPolygonVisuals::~CPolygonVisuals")
    DTOR_CALL("CPolygonVisuals::~CPolygonVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone a polygon shape visual
///
/// \param _pShape Shape the visuals are attached to
///
/// \return Clone of polygon shape visuals
///
///////////////////////////////////////////////////////////////////////////////
IVisuals* CPolygonVisuals::clone(CDoubleBufferedShape* const _pShape) const
{
    METHOD_ENTRY("CPolygonVisuals::clone");
    
    CPolygonVisuals* pClone = new CPolygonVisuals(_pShape);
    MEM_ALLOC("CPolygonVisuals")
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the polygon
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CPolygonVisuals::draw(CCamera* const _pCamera,
                            IObject* const _pObject) const
{
    METHOD_ENTRY("CPolygonVisuals::draw")

    CPolygon* pPolygon = static_cast<CPolygon*>(m_pRef->getShapeCur());
    
    
    m_Graphics.polygon(pPolygon->getVertices(), pPolygon->getPolygonType(),
                        -_pCamera->getCenter() + 
                        (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2);
//     m_Graphics.circle(_pObject->getKinematicsState().getLocalPosition(pPolygon->getCentroid()) -_pCamera->getCenter(), 0.2);
//     m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
//     m_Graphics.addVertex(_pObject->getKinematicsState().getLocalPosition(pPolygon->getCentroid() - Vector2d(-0.2, 0.0)) -_pCamera->getCenter());
//     m_Graphics.addVertex(_pObject->getKinematicsState().getLocalPosition(pPolygon->getCentroid() - Vector2d(+0.2, 0.0)) -_pCamera->getCenter());
//     m_Graphics.endLine();
//     m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
//     m_Graphics.addVertex(_pObject->getKinematicsState().getLocalPosition(pPolygon->getCentroid() - Vector2d(0.0, -0.2)) -_pCamera->getCenter());
//     m_Graphics.addVertex(_pObject->getKinematicsState().getLocalPosition(pPolygon->getCentroid() - Vector2d(0.0, +0.2)) -_pCamera->getCenter());
//     m_Graphics.endLine();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& CPolygonVisuals::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CPolygonVisuals::myStreamIn")
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CPolygonVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CPolygonVisuals::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CPolygonVisuals::myStreamOut")
    return _os;
}

