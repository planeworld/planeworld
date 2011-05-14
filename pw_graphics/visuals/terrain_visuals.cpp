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
    double   fPAng     = m_pTerrain->getAngle();
    int      nSeed     = m_pTerrain->getSeed();
    double   fSmooth   = m_pTerrain->getSmoothness();
    Vector2d vecCenter = m_pTerrain->getCenter()-_pCamera->getCenter();
    
    
        Vector2d    vecEx(1.0, 0.0);
        double      fLeft  = -fWidth*0.5+vecCenter[0];    
        double      fRight =  fWidth*0.5+vecCenter[0];
        
        if (_pCamera->getBoundingBox().getLowerLeft()[0]-_pCamera->getCenter()[0] > fLeft)
            fLeft = _pCamera->getBoundingBox().getLowerLeft()[0]-_pCamera->getCenter()[0];
        if (_pCamera->getBoundingBox().getUpperRight()[0]-_pCamera->getCenter()[0] < fRight)
            fRight = _pCamera->getBoundingBox().getUpperRight()[0]-_pCamera->getCenter()[0];
//         double fAlpha = fabs(std::asin(_pCamera->getBoundingBox() / vecDist.norm()));
//         if (isnan(fAlpha))
//         {
//             fAng = 0.0;
//             fAngEnd = 2.0*M_PI;
//             LineT = GRAPHICS_LINETYPE_LOOP;
//         }
//         else
//         {
//             double fAng0 = std::acos((- vecDist.dot(vecEx)) / vecDist.norm());
//             
//             if (vecDist[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
//             
//             fAng = fAng0-fAlpha;
//             fAngEnd = fAng0+fAlpha;
//             LineT = GRAPHICS_LINETYPE_STRIP;
//         }

        double fInc = m_pTerrain->getGroundResolution();
        
        // Subsample terrain surface when zooming out.
        if (_pCamera->getZoom()*m_pTerrain->getGroundResolution() <= 1.0)
            fInc /= _pCamera->getZoom()*m_pTerrain->getGroundResolution();
        
        // Snap start and end to ground resolution grid to avoid flickering.
        // If angle is started at arbitrary position, aliasing causes flickering when zooming
        // or moving, since height is always sampled at different positions.
        fLeft  -= ((fLeft /fInc)-floor(fLeft /fInc))*fInc;
        fRight += ((fRight/fInc)-floor(fRight/fInc))*fInc;
        
//         if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
               
        m_Graphics.beginLine(GRAPHICS_LINETYPE_STRIP, SHAPE_DEFAULT_DEPTH);
        
        double fX = fLeft;

        while ( fX <= fRight)
        {
            double fHght = m_pTerrain->getSurface().GetValue(fX-vecCenter[0],0.0,0.0);
            
            m_Graphics.addVertex(Vector2d(fX,vecCenter[1]+fHght*fHeight));

            fX += fInc;
        }

        m_Graphics.endLine();

    METHOD_EXIT("CTerrainVisuals::draw()");
}
