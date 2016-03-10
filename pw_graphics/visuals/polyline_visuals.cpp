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
    
    this->attachTo(_pPolyline);
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

    CPolyLine* pPolyline = static_cast<CPolyLine*>(m_pRef->getShapeCur());
    
    m_Graphics.polyline(pPolyline->getVertices(), pPolyline->getLineType(),
                        -_pCamera->getCenter() + 
                        (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2);
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
std::istream& CPolylineVisuals::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CPolylineVisuals::myStreamIn")
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CPolylineVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CPolylineVisuals::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CPolylineVisuals::myStreamOut")
    return _os;
}

