////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       terrain_visuals.cpp
/// \brief      Implementation of class "CTerrainVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#include "terrain_visuals.h"

#include "terrain.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pTerrain Terrain to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CTerrainVisuals::CTerrainVisuals(CDoubleBufferedShape* const _pTerrain)
{
    METHOD_ENTRY("CTerrainVisuals::CTerrainVisuals")
    CTOR_CALL("CTerrainVisuals::CTerrainVisuals")
    
    this->attachTo(_pTerrain);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CTerrainVisuals::~CTerrainVisuals()
{
    METHOD_ENTRY("CTerrainVisuals::~CTerrainVisuals")
    DTOR_CALL("CTerrainVisuals::~CTerrainVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone a terrain shape visual
///
/// \param _pShape Shape the visuals are attached to
///
/// \return Clone of terrain shape visual
///
///////////////////////////////////////////////////////////////////////////////
IVisuals* CTerrainVisuals::clone(CDoubleBufferedShape* const _pShape) const
{
    METHOD_ENTRY("CTerrainVisuals::clone");
    
    CTerrainVisuals* pClone = new CTerrainVisuals(_pShape);
    MEM_ALLOC("CTerrainVisuals")
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Terrain
///
/// \note It is very important to do camera positioning after clipping.
///       Otherwise, camera movement would also be clipped and thus quantized.
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CTerrainVisuals::draw(CCamera* const _pCamera,
                           IObject* const _pObject) const
{
    METHOD_ENTRY("CTerrainVisuals::draw")
    
    CTerrain* pTerrain = static_cast<CTerrain*>(m_pRef->getShapeCur());

    double   fWidth    = pTerrain->getWidth();
    Vector2d vecCenter = pTerrain->getCenter();
    
    double      fLeft0  = -fWidth*0.5+vecCenter[0];    
    double      fRight0 =  fWidth*0.5+vecCenter[0];
    double      fLeft   = fLeft0;    
    double      fRight  = fRight0;
    
    if (_pCamera->getBoundingBox().getLowerLeft()[0] > fLeft)
        fLeft = _pCamera->getBoundingBox().getLowerLeft()[0];
    if (_pCamera->getBoundingBox().getUpperRight()[0] < fRight)
        fRight = _pCamera->getBoundingBox().getUpperRight()[0];

    double fInc = pTerrain->getGroundResolution();
    
    // Subsample terrain surface when zooming out.
    if (_pCamera->getZoom()*pTerrain->getGroundResolution() <= 1.0)
        fInc /= _pCamera->getZoom()*pTerrain->getGroundResolution();
    
    if (fLeft  < fLeft0)  fLeft  = fLeft0;
    if (fRight > fRight0) fRight = fRight0;
    
    fLeft = pTerrain->snapToTerrainGrid(fLeft);
    fRight = pTerrain->snapToTerrainGrid(fRight);
    
    m_Graphics.beginLine(PolygonType::LINE_STRIP, SHAPE_DEFAULT_DEPTH);
    
    while ( fLeft <= fRight)
    {
        double fHght = pTerrain->getSurface(fLeft);
        
        m_Graphics.addVertex(Vector2d(fLeft-_pCamera->getCenter()[0],
                                      fHght-_pCamera->getCenter()[1]));

        fLeft += fInc;
    }

    m_Graphics.endLine();
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
std::istream& CTerrainVisuals::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CTerrainVisuals::myStreamIn")
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CTerrainVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CTerrainVisuals::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CTerrainVisuals::myStreamOut")
    return _os;
}

