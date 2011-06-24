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
/// \file       terrain_visuals.cpp
/// \brief      Implementation of class "CTerrainVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#include "terrain_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pTerrain Terrain to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CTerrainVisuals::CTerrainVisuals(CTerrain* _pTerrain): m_pTerrain(_pTerrain)
{
    METHOD_ENTRY("CTerrainVisuals::CTerrainVisuals")
    CTOR_CALL("CTerrainVisuals::CTerrainVisuals")
    
    METHOD_EXIT("CTerrainVisuals::CTerrainVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CTerrainVisuals::~CTerrainVisuals()
{
    METHOD_ENTRY("CTerrainVisuals::~CTerrainVisuals()");
    DTOR_CALL("CTerrainVisuals::~CTerrainVisuals()");

    METHOD_EXIT("CTerrainVisuals::~CTerrainVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Terrain
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CTerrainVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CTerrainVisuals::draw()");

    double   fWidth    = m_pTerrain->getWidth();
    double   fHeight   = m_pTerrain->getHeight();
    Vector2d vecCenter = m_pTerrain->getCenter();
    
    double      fLeft0  = -fWidth*0.5+vecCenter[0];    
    double      fRight0 =  fWidth*0.5+vecCenter[0];
    double      fLeft   = fLeft0;    
    double      fRight  = fRight0;
    
    if (_pCamera->getBoundingBox().getLowerLeft()[0] > fLeft)
        fLeft = _pCamera->getBoundingBox().getLowerLeft()[0];
    if (_pCamera->getBoundingBox().getUpperRight()[0] < fRight)
        fRight = _pCamera->getBoundingBox().getUpperRight()[0];

    double fInc = m_pTerrain->getGroundResolution();
    
    // Subsample terrain surface when zooming out.
    if (_pCamera->getZoom()*m_pTerrain->getGroundResolution() <= 1.0)
        fInc /= _pCamera->getZoom()*m_pTerrain->getGroundResolution();
    
    if (fLeft  < fLeft0)  fLeft  = fLeft0;
    if (fRight > fRight0) fRight = fRight0;
    
    // It is very important to do camera positioning after clipping. Otherwise,
    // camera movement would also be clipped and thus quantized.
    fLeft  -= _pCamera->getCenter()[0];
    fRight -= _pCamera->getCenter()[0];
    
    m_Graphics.beginLine(GRAPHICS_LINETYPE_STRIP, SHAPE_DEFAULT_DEPTH);
    
    while ( fLeft <= fRight)
    {
        double fHght = m_pTerrain->getSurface(fLeft+_pCamera->getCenter()[0])-_pCamera->getCenter()[1];
        
        m_Graphics.addVertex(Vector2d(fLeft,fHght));

        fLeft += fInc;
    }

    m_Graphics.endLine();

    METHOD_EXIT("CTerrainVisuals::draw()");
}
