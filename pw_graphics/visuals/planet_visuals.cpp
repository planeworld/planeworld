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
/// \file       planet_visuals.cpp
/// \brief      Implementation of class "CPlanetVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-17
///
////////////////////////////////////////////////////////////////////////////////

#include "planet_visuals.h"

#include "planet.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPlanetVisuals::~CPlanetVisuals()
{
    METHOD_ENTRY("CPlanetVisuals::~CPlanetVisuals")
    DTOR_CALL("CPlanetVisuals::~CPlanetVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone a planet shape visual
///
/// \param _pShape Shape the visuals are attached to
///
/// \return Clone of planet shape visuals
///
///////////////////////////////////////////////////////////////////////////////
IVisuals* CPlanetVisuals::clone(CDoubleBufferedShape* const _pShape) const
{
    METHOD_ENTRY("CCircleVisuals::clone");
    
    CPlanetVisuals* pClone = new CPlanetVisuals(_pShape);
    MEM_ALLOC("CPlanetVisuals")
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Planet
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CPlanetVisuals::draw(CCamera* const _pCamera,
                          const IObject* const _pObject) const
{
    METHOD_ENTRY("CPlanetVisuals::draw")

    CPlanet* pPlanet = static_cast<CPlanet*>(m_pPlanet->getShapeCur());
    
    double   fRad      = pPlanet->getRadius();
    double   fHeight   = pPlanet->getHeight();
    double   fPAng     = pPlanet->getAngle();
    double   fSeaLevel = pPlanet->getSeaLevel();
    int      nSeed     = pPlanet->getSeed();
    double   fSmooth   = pPlanet->getSmoothness();
    Vector2d vecCenter = pPlanet->getCenter()-_pCamera->getCenter() +
                         (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2;
    
    if ((vecCenter.norm() <= fRad+fHeight+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  fRad-fHeight-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        LineType    LineT;
        
       std::vector<std::vector<Vector2d> > WaterlineList;
       std::vector<Vector2d> WaterlineTmp;
       bool bInWater = false;

        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = 2.0*M_PI;
            LineT = GRAPHICS_LINETYPE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = GRAPHICS_LINETYPE_STRIP;
        }

        double fInc  = pPlanet->getGroundResolution() / fRad;
        
        // Subsample planet surface when zooming out.
        if (_pCamera->getZoom()*pPlanet->getGroundResolution() <= PLANET_VISUALS_DEFAULT_RESOLUTION)
        {
            // 1. Normalise according to planet's ground resolution
            // 2. Adjust to given visual quality (length of lines)
            // 3. Adjust to given visual quality (maximum planet amplitude, planet is nearly circular)
            //   3a. General adjustment by zoom
            //   3b. Ensure enough segments for circular object, but also incorporate resolution
            // 4. Adjust octaves
            fInc /= pPlanet->getGroundResolution(); 
            fInc *= PLANET_VISUALS_DEFAULT_RESOLUTION * m_Graphics.getResMPX(); 
            if (fHeight*_pCamera->getZoom() < PLANET_VISUALS_DEFAULT_RESOLUTION)
            {
                fInc *= PLANET_VISUALS_DEFAULT_RESOLUTION / (fHeight*_pCamera->getZoom());
                if (fInc > PLANET_VISUALS_DEFAULT_MINIMUM_ANGLE)
                {
                    fInc = PLANET_VISUALS_DEFAULT_MINIMUM_ANGLE;
                    if (fInc * fRad * m_Graphics.getResPMX() < PLANET_VISUALS_DEFAULT_RESOLUTION) 
                        fInc = PLANET_VISUALS_DEFAULT_RESOLUTION * m_Graphics.getResMPX() / fRad;
                }
            }
            pPlanet->setSampling(m_Graphics.getResPMX()); 
        }
        
        fAngEnd += fInc;
        
        // Snap angle to ground resolution grid to avoid flickering.
        // If angle is started at arbitrary position, aliasing causes flickering when zooming
        // or moving, since height is always sampled at different positions.
        fAng -= ((fAng/fInc)-floor(fAng/fInc))*fInc;
        fAngEnd += ((fAng/fInc)-floor(fAng/fInc))*fInc;
        
        if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
        
//         double fZoom = _pCamera->getZoom() * fHeight;
//         if (fZoom > 1.0) fZoom = 1.0;
        
        double fAngBak = fAng;
        
        double fHght;
        double fTerrainType;
        double fWeightMountains;
        double fWeightFlat;
        
        const double TERRAIN_CROSSOVER = 1.0e-10;
        const double TERRAIN_CROSSOVER_INV = 0.5/(TERRAIN_CROSSOVER);
        m_Graphics.setWidth(3.0);
        m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);

            while (fAng < fAngEnd)
            {
                fHght = pPlanet->getSurface()->GetValue(std::cos(fAng-fPAng)*fRad,
                                                        std::sin(fAng-fPAng)*fRad);
                if (fHght < fSeaLevel)
                {
                  m_Graphics.setColor(0.0,0.0,0.7*(fHght+1.0)/(fSeaLevel+1.0));
                }
                else
                {
                  fTerrainType = 0.1;// pPlanet->getTerrainType()->GetValue(std::cos(fAng-fPAng)*fRad,
                                     //                               std::sin(fAng-fPAng)*fRad) * 0.5 + 0.5;
                  if (fTerrainType < 0.5)
                  {
                      fWeightMountains = 0.0 + (fTerrainType - (0.5-TERRAIN_CROSSOVER))*TERRAIN_CROSSOVER_INV;
                      if (fWeightMountains < 0.0) fWeightMountains = 0.0;
                      fWeightFlat      = 1.0 - (fTerrainType - (0.5-TERRAIN_CROSSOVER))*TERRAIN_CROSSOVER_INV;
                      if (fWeightFlat  > 1.0) fWeightFlat = 1.0;
                  }
                  else
                  {
                      fWeightFlat      = 0.0 + ((0.5+TERRAIN_CROSSOVER) - fTerrainType)*TERRAIN_CROSSOVER_INV;
                      if (fWeightFlat  < 0.0) fWeightFlat = 0.0;
                      fWeightMountains = 1.0 - ((0.5+TERRAIN_CROSSOVER) - fTerrainType)*TERRAIN_CROSSOVER_INV;
                      if (fWeightMountains > 1.0) fWeightMountains = 1.0;
                  }
                  m_Graphics.setColor((fHght+2.0)*0.3*fWeightMountains*0.4 + 0.1*(fHght+2.0)*0.8*fWeightFlat,
                                      (fHght+2.0)*0.3*fWeightMountains*0.4 + 0.2*(fHght+2.0)*0.8*fWeightFlat,
                                      (fHght+2.0)*0.3*fWeightMountains*0.4 + 0.1*(fHght+2.0)*0.8*fWeightFlat
                                    );
                }
//                 m_Graphics.setColor(0.2,(0.5+fTerrainType), 0.2);
                
                m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
                                              vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));
//                 m_Graphics.setColor(0.5+0.2*fHght,0.3+0.1*fHght,0.1);
//                 m_Graphics.dot(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                         vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));

                if (fHght < fSeaLevel)
                {
                  WaterlineTmp.push_back(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fSeaLevel*fHeight),vecCenter[1]+std::sin(fAng)*(fRad+fSeaLevel*fHeight)));
                  bInWater = true;
                }
                else
                {
                  if (bInWater)
                  {
                    WaterlineList.push_back(WaterlineTmp);
                    WaterlineTmp.clear();
                    bInWater = false;
                  }
                }
                
                fAng += fInc;
            }
        m_Graphics.endLine();
        m_Graphics.setWidth(1.0);
        
        // Determine LineType
        if ((LineT == GRAPHICS_LINETYPE_LOOP) && (WaterlineList.size() != 0))
          LineT = GRAPHICS_LINETYPE_STRIP;
        
        // Push back potential last line segment
        if (WaterlineTmp.size() != 0)
        {
          WaterlineList.push_back(WaterlineTmp);
        }
        
        // Draw sea level
        m_Graphics.setWidth(2.0);
        m_Graphics.setColor(0.0,0.0,0.7);
        for (int i=0; i<WaterlineList.size(); ++i)
        {
          m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);
          for (int j=0; j<WaterlineList[i].size(); ++j)
            m_Graphics.addVertex(WaterlineList[i][j]);
          m_Graphics.endLine();
        }
        m_Graphics.setWidth(1.0);
        
//         // Draw grass
//         m_Graphics.setColor(0.1,0.2,0.1);
//         if (_pCamera->getZoom() >= 1.0)
//         {
//             fAng = fAngBak;
//             while ( fAng <= fAngEnd)
//             {
//                 fHght = pPlanet->getSurface()->GetValue(std::cos(fAng-fPAng)*fRad,
//                                                          std::sin(fAng-fPAng)*fRad);
//                 fTerrainType = pPlanet->getTerrainType()->GetValue(std::cos(fAng-fPAng)*fRad,
//                                                                     std::sin(fAng-fPAng)*fRad) * 0.5 + 0.5;
//                 if (fTerrainType < 0.5)
//                 {
//                     
//                     double fAngGrass01 = fAng-0.5*noise::ValueNoise2D(1.0,1.0,fAng/fInc);
//                     double fAngGrass02 = fAng-noise::ValueNoise2D(1.0,1.0,fAng/fInc);
//                     double fHghtGrass01 = fabs(0.5-fTerrainType);
//                     double fHghtGrass02 = 0.5*fabs(0.5-fTerrainType);
//                     m_Graphics.beginLine(GRAPHICS_LINETYPE_STRIP,SHAPE_DEFAULT_DEPTH);
//                     m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                                   vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));
//                     m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                                   vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight))+
//                                          Vector2d(std::cos(fAngGrass01)*(fHghtGrass01),
//                                                   std::sin(fAngGrass01)*(fHghtGrass01)));
//                     m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                                   vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight))+
//                                          Vector2d(std::cos(fAngGrass01)*(fHghtGrass01),
//                                                   std::sin(fAngGrass01)*(fHghtGrass01))+
//                                          Vector2d(std::cos(fAngGrass02)*(fHghtGrass02),
//                                                   std::sin(fAngGrass02)*(fHghtGrass02)));
//                     m_Graphics.endLine();
//                 }
//                 fAng += fInc;
//             }
//         }
        m_Graphics.setColor(1.0,1.0,1.0,1.0);
        
        pPlanet->resetSampling();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a planet to planet visuals
///
/// \param _pPlanet Planet to attach
///
////////////////////////////////////////////////////////////////////////////////
void CPlanetVisuals::attach(CDoubleBufferedShape* const _pPlanet)
{
    METHOD_ENTRY("CPlanetVisuals::attach")

    if (_pPlanet->getShapeCur()->getShapeType() == SHAPE_PLANET)
    {
        m_pPlanet = _pPlanet;
    }
    else
    {
        ERROR_MSG("Planet Visuals", "Wrong shape attached to visuals.")
    }
}
