////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2017 Torsten Büschenfeld
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
/// \file       visuals_manager.h
/// \brief      Implementation of class "CVisualsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-06
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals_manager.h"

#include <random>

#include "object_planet.h"
#include "particle.h"
#include "widget_cam.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::CVisualsManager() : m_unLightmapSubsampling(32),
                                     m_bFullscreen(false),
                                     m_nMainWinHeight(600),
                                     m_nMainWinWidth(800),
                                     m_nVisualisations(0),
                                     m_nStarIndex(-1),
                                     m_unCameraIndex(0u),
                                     m_pCamera(nullptr),
                                     m_nCursorX(0),
                                     m_nCursorY(0),
                                     m_nCursorX0(0),
                                     m_nCursorY0(0),
                                     m_nCursorOffsetX(0),
                                     m_nCursorOffsetY(0),
                                     m_bCursor(false),
                                     m_bMBLeft(false),
                                     m_ConsoleWidgetID(0),
                                     m_ConsoleWindowID(0),
                                     m_strDataPath(DEFAULTDATADIR),
                                     m_UIDVisuals(&m_FontManager),
                                     m_TextParticle(&m_FontManager),
                                     m_TextDebugInfo(&m_FontManager),
                                     m_TextDebugRender(&m_FontManager),
                                     m_TextObjects(&m_FontManager),
                                     m_TextScale(&m_FontManager),
                                     m_TextTimers(&m_FontManager),
                                     m_TextStarSystems(&m_FontManager),
                                     m_TextVersion(&m_FontManager)
{
    METHOD_ENTRY("CVisualsManager::CVisualsManager")
    CTOR_CALL("CVisualsManager::CVisualsManager")
    
    #ifdef PW_MULTITHREADING
        m_strModuleName = "Visuals Manager";
    #endif
    m_strFont = "";
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::~CVisualsManager()
{
    METHOD_ENTRY("CVisualsManager::~CVisualsManager")
    DTOR_CALL("CVisualsManager::~CVisualsManager")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a circle using graphics base
///
/// \param _vecCell Cell of circle to be drawn
/// \param _vecCenter Center of circle to be drawn
/// \param _fRad Radius to be drawn
/// \param _pCamera The camera to draw the polygon with
///
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawCircle(const Vector2i& _vecCell,
                                 const Vector2d& _vecCenter,
                                 const double& _fRad,
                                 CCamera* _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawCircle")
    Vector2d vecCenter = _vecCenter - _pCamera->getCenter() +
                         IGridUser::cellToDouble(_vecCell - _pCamera->getCell());

    if ((vecCenter.norm() <= _fRad+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  _fRad-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        
        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (std::isnan(fAlpha))
        {
            m_Graphics.drawCircleDyn(vecCenter, _fRad);
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = MATH_2PI - fAng0;
            
            m_Graphics.drawArcDyn(vecCenter, _fRad, fAng0-fAlpha, fAng0+fAlpha);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a circle using shape \ref CCircle
///
/// \param _pObject Object, the polygon belongs to
/// \param _pCircle The circle to be drawn
/// \param _pCamera The camera to draw the polygon with
///
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawCircle(CObject* _pObject, CCircle* _pCircle, CCamera* _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawCircle")
    this->drawCircle(_pObject->getCell(), _pCircle->getCenter(), _pCircle->getRadius(), _pCamera);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Planet
///
/// \param _pObject Object, the planet belongs to
/// \param _pPlanet The planet to be drawn
/// \param _pCamera The camera to draw the planet with
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawPlanet(CObject* _pObject, CPlanet* _pPlanet, CCamera* _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::draw")

    double   fRad      = _pPlanet->getRadius();
    double   fHeight   = _pPlanet->getHeight();
    double   fPAng     = _pPlanet->getAngle();
    double   fSeaLevel = _pPlanet->getSeaLevel();
    //int      nSeed     = _pPlanet->getSeed();
    //double   fSmooth   = _pPlanet->getSmoothness();
    Vector2d vecCenter = _pPlanet->getCenter()-_pCamera->getCenter() +
                         IGridUser::cellToDouble(_pObject->getCell() - _pCamera->getCell());
    
    if ((vecCenter.norm() <= fRad+fHeight+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  fRad-fHeight-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        PolygonType    LineT;
        
        std::vector<std::vector<Vector2d> > WaterlineList;
        std::vector<Vector2d> WaterlineTmp;
        bool bInWater = false;

        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (std::isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = MATH_2PI;
            LineT = PolygonType::LINE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = MATH_2PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = PolygonType::LINE_STRIP;
        }
        double fInc  = _pPlanet->getGroundResolution() / fRad;
        
        // Subsample planet surface when zooming out.
        if (_pCamera->getZoom()*_pPlanet->getGroundResolution() <= PLANET_VISUALS_DEFAULT_RESOLUTION)
        {
            // 1. Normalise according to planet's ground resolution
            // 2. Adjust to given visual quality (length of lines)
            // 3. Adjust to given visual quality (maximum planet amplitude, planet is nearly circular)
            //   3a. General adjustment by zoom
            //   3b. Ensure enough segments for circular object, but also incorporate resolution
            // 4. Adjust octaves
            fInc /= _pPlanet->getGroundResolution(); 
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
            _pPlanet->setSampling(m_Graphics.getResPMX()); 
        }
        
        fAngEnd += fInc;
        
        // Snap angle to ground resolution grid to avoid flickering.
        // If angle is started at arbitrary position, aliasing causes flickering when zooming
        // or moving, since height is always sampled at different positions.
        double fAngleSampling = fAng-fPAng;
        double fAngleSnap = fAngleSampling-floor(fAngleSampling/fInc)*fInc;
        fAng    -= fAngleSnap;
        fAngEnd += fAngleSnap;
        
        if (fAngEnd < fAng)
        {
            double fTmp = fAng;
            fAng = fAngEnd;
            fAngEnd = fTmp;
            // std::swap<double>(fAng, fAngEnd); // This doesn't work with VC++
        }
        
//         double fAngBak = fAng;
        
//         double fZoom = _pCamera->getZoom() * fHeight;
//         if (fZoom > 1.0) fZoom = 1.0;
        
        double fHght;
        double fTerrainType;
        double fWeightMountains;
        double fWeightFlat;
        
        const double TERRAIN_CROSSOVER = 1.0e-10;
        const double TERRAIN_CROSSOVER_INV = 0.5/(TERRAIN_CROSSOVER);

        m_Graphics.beginLine(LineT);

            while (fAng < fAngEnd)
            {
                fHght = _pPlanet->getSurface()->GetValue(std::cos(fAng-fPAng)*fRad,
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
        
        // Determine PolygonType
        if ((LineT == PolygonType::LINE_LOOP) && (WaterlineList.size() != 0))
          LineT = PolygonType::LINE_STRIP;
        
        // Push back potential last line segment
        if (WaterlineTmp.size() != 0)
        {
          WaterlineList.push_back(WaterlineTmp);
        }
        
        // Draw sea level
        m_Graphics.setColor(0.0,0.0,0.7);
        for (auto i=0u; i<WaterlineList.size(); ++i)
        {
          m_Graphics.beginLine(LineT);
          for (auto j=0u; j<WaterlineList[i].size(); ++j)
            m_Graphics.addVertex(WaterlineList[i][j]);
          m_Graphics.endLine();
        }
        
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
// //                 if (fTerrainType < 0.5)
//                 {
//                     
//                     double fAngGrass01 = fAng-0.5*noise::ValueNoise2D(fAng*100000,fAng,1);
//                     double fAngGrass02 = fAng-noise::ValueNoise2D(fAng*100000,fAng,1);
//                     double fHghtGrass01 = fabs(0.5-fTerrainType);
//                     double fHghtGrass02 = 0.5*fabs(0.5-fTerrainType);
//                     m_Graphics.beginLine(PolygonType::LINE_STRIP,SHAPE_DEFAULT_DEPTH);
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
        
        _pPlanet->resetSampling();
    }
//     m_Graphics.circle(_pObject->getKinematicsState().getLocalPosition(
//                        pPlanet->getCentroid()) -_pCamera->getCenter(), 0.2);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a polygon using graphics base
///
/// \param _pObject Object, the polygon belongs to
/// \param _pPolygon The polygon to be drawn
/// \param _pCamera The camera to draw the polygon with
///
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawPolygon(CObject* _pObject, CPolygon* _pPolygon, CCamera* _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawPolygon")
    m_Graphics.polygon(_pPolygon->getVertices(), _pPolygon->getPolygonType(),
                -_pCamera->getCenter() + 
                IGridUser::cellToDouble(_pObject->getCell() - _pCamera->getCell()));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw all particles
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawParticles(CCamera* const _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawParticles")
    
    for (auto Particle : *m_pDataStorage->getParticlesByValueFront())
    {
        if (Particle.second->getBoundingBox().overlaps(_pCamera->getBoundingBox()))
        {
            switch (Particle.second->getParticleType())
            {
                case ParticleTypeType::DOT:
                {
                    m_Graphics.dots((*Particle.second->getPositions()),-_pCamera->getCenter()+
                        IGridUser::cellToDouble(Particle.second->getCell() - _pCamera->getCell()));
                    break;
                }
                case ParticleTypeType::SMOKE:
                case ParticleTypeType::THRUST:
                {
                    auto   nSize   = Particle.second->getPositions()->size();
                    double fSizeR  = 1.0;
                    double fGrowth = (Particle.second->getSizeDeath() - Particle.second->getSizeBirth()) *
                                      nSize/Particle.second->getPositions()->capacity();
                    ColorTypeRGBA paColBirth = *Particle.second->getColorBirth();
                    ColorTypeRGBA paColDeath = *Particle.second->getColorDeath();
                                      
                    if (Particle.second->getBoundingBox().getWidth()  * m_Graphics.getResPMX() > 2.0 &&
                        Particle.second->getBoundingBox().getHeight() * m_Graphics.getResPMY() > 2.0)
                    {
                        m_Graphics.cacheSinCos(12);
                        for (auto i=0u; i<nSize; ++i)
                        {
                            if (Particle.second->getStates()->at(i) == PARTICLE_STATE_ACTIVE &&
                                _pCamera->getBoundingBox().isInside(Particle.second->getPositions()->at(i)))
                            {
                                double fAge = double(Particle.second->getAge()->at(i)) / Particle.second->getMaxAge();
                                
                                double fR = paColBirth[0] * (1.0 - fAge) + paColDeath[0] * fAge;
                                double fG = paColBirth[1] * (1.0 - fAge) + paColDeath[1] * fAge;
                                double fB = paColBirth[2] * (1.0 - fAge) + paColDeath[2] * fAge;
                                double fA = paColBirth[3] * (1.0 - fAge) + paColDeath[3] * fAge;
                                m_Graphics.setColor(fR, fG, fB, fA);
                                
                                fSizeR = Particle.second->getSizeBirth() + fGrowth * fAge;
                                
                                    m_Graphics.filledCircle(Particle.second->getPositions()->at(i) - _pCamera->getCenter()+
                                                IGridUser::cellToDouble(Particle.second->getCell() - _pCamera->getCell()),
                                                fSizeR, 12, GRAPHICS_CIRCLE_USE_CACHE);
                            }
                        }
                        m_Graphics.setColor(1.0,1.0,1.0);
                    }
                    break;
                }
                default:
                {
                    WARNING_MSG("Visuals Manager", "Wrong particle type, visualisation not implemented.")
                    break;
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw all shape visuals of this object
///
/// \todo At the moment, an object is only drawn if in the same cell as the
///       camera, otherwise, when zoomed out, a dot is drawn. Bounding Boxes are
///       not fully adjusted to cells yet.
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawObjects(CCamera* const _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawObjects")

    for (auto Obj : *m_pDataStorage->getObjectsByValueFront())
    {
        if (Obj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).overlaps(_pCamera->getBoundingBox()))
        {
            if (Obj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getWidth() * m_Graphics.getResPMX() < 2.0)
            {
                m_Graphics.dot(Obj.second->getCOM()-_pCamera->getCenter() +
                               IGridUser::cellToDouble(Obj.second->getCell() - _pCamera->getCell()));
            }
            else
            {
                for (const auto pShape : Obj.second->getGeometry()->getShapes())
                {
                    switch (pShape->getShapeType())
                    {
                        case ShapeType::CIRCLE:
                            this->drawCircle(Obj.second, static_cast<CCircle*>(pShape), _pCamera);
                            break;
                        case ShapeType::PLANET:
                            this->drawPlanet(Obj.second, static_cast<CPlanet*>(pShape), _pCamera);
                            break;
                        case ShapeType::POLYGON:
                            this->drawPolygon(Obj.second, static_cast<CPolygon*>(pShape), _pCamera);
                            break;
                        case ShapeType::TERRAIN:
                            break;
                        case ShapeType::NONE:
                            WARNING_MSG("Object Visuals", "Shapetype unknown, not drawing. This shouldn't happen!")
                            break;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw atmospheres of planetary objects
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawObjectsPlanetsAtmospheres(CCamera* const _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawObjectsPlanetsAtmospheres")
    
    for (auto pObjPl : *m_pDataStorage->getObjectsPlanetsByValueFront())
    {
        if (_pCamera->getZoom() < 1.0)
        {
            constexpr double fZoomFacBegin = 10.0;
            constexpr double fZoomFacEnd = 1.0e-5;
            
            double fZoomMax = fZoomFacBegin/(1.0-fZoomFacEnd) * (_pCamera->getZoom() - fZoomFacEnd);
            if (fZoomMax < 0.0) fZoomMax = 0.0;
            else if (fZoomMax > 1.0) fZoomMax = 1.0;
            
            double fI = pObjPl.second->getRadius();
            double fAlpha = 1.0;
            
            while (fAlpha > 0.1)
            {
                fAlpha = (1.0 - fZoomMax) * 
                         pObjPl.second->getPressureAtRadius(fI)/pObjPl.second->getPressureAtGround();
                
                m_Graphics.setColor(0.0, 0.0, 1.0, fAlpha);
                this->drawCircle(pObjPl.second->getCell(), pObjPl.second->getCOM(), fI, _pCamera);
                if (1000.0 * _pCamera->getZoom() < 2.0)
                    fI += 2.0 / _pCamera->getZoom();
                else
                    fI += 1000.0;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw center of mass (COM) of all objects
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawCOM() const
{
    METHOD_ENTRY("CVisualsManager::drawCOM")

    if (m_nVisualisations & VISUALS_OBJECT_COM)
    {
        m_Graphics.beginRenderBatch("world");
            for (auto Obj : *m_pDataStorage->getObjectsByValueFront())
            {
                
                if (Obj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).overlaps(m_pCamera->getBoundingBox()))
                {
                    if (m_Graphics.getResPMX() > 0.5)
                    {
                        m_Graphics.setColor(1.0, 1.0, 0.0, 0.8);
                        m_Graphics.filledCircle(Obj.second->getCOM() - m_pCamera->getCenter(), 0.5, 36.0);
                        m_Graphics.setColor(0.2, 0.2, 0.0, 1.0);
                        m_Graphics.filledCircle(Obj.second->getCOM() - m_pCamera->getCenter(), 0.3, 36.0);
                        
                        m_Graphics.setColor(1.0, 1.0, 0.0, 0.8);
                        // Draw shapes centroids
                        for (auto Shp : Obj.second->getGeometry()->getShapes())
                        {
                            m_Graphics.filledCircle(
                                Obj.second->getKinematicsState().getPosition(
                                    Shp->getCentroid()
                                ) - m_pCamera->getCenter(), 0.3, 36.0);
                        }
                        m_Graphics.setColor(0.2, 0.2, 0.0, 1.0);
                        // Draw shapes centroids
                        for (auto Shp : Obj.second->getGeometry()->getShapes())
                        {
                            m_Graphics.filledCircle(
                                Obj.second->getKinematicsState().getPosition(
                                    Shp->getCentroid()
                                ) - m_pCamera->getCenter(), 0.2, 36.0);
                        }
                        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
                    }
                }
            }
        m_Graphics.endRenderBatch();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws bounding box of objects
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawBoundingBoxes() const
{
    METHOD_ENTRY("CVisualsManager::drawBoundingBox")

    if (m_nVisualisations & VISUALS_OBJECT_BBOXES)
    {
        
        m_Graphics.beginRenderBatch("world");
            m_Graphics.setColor(0.0, 1.0, 0.0, 0.8);
            m_Graphics.rect(m_pCamera->getBoundingBox().getLowerLeft()-
                            m_pCamera->getCenter(),
                            m_pCamera->getBoundingBox().getUpperRight()-
                            m_pCamera->getCenter());
            m_Graphics.setColor(0.0, 0.5, 0.0, 0.1);
    //         m_Graphics.filledRect(m_pCamera->getBoundingBox().getLowerLeft()-
    //                               m_pCamera->getCenter(),
    //                               m_pCamera->getBoundingBox().getUpperRight()-
    //                               m_pCamera->getCenter());
            m_Graphics.setColor(0.0, 1.0, 0.0, 0.8);
            m_Graphics.circle(m_pCamera->getCenter()-m_pCamera->getCenter(),
                            m_pCamera->getBoundingCircleRadius(),100.0);
            m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
            m_Graphics.setDepth(GRAPHICS_DEPTH_DEFAULT);
            
            for (const auto pObj : *m_pDataStorage->getObjectsByValueFront())
            {
                if (m_pCamera->getBoundingBox().isInside(pObj.second->getCOM()))
                {
                    // Object bounding boxes
                    // Multiframe
                    m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
                    m_Graphics.rect(pObj.second->getGeometry()->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                    IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                    pObj.second->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                    IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
                    m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
                    m_Graphics.filledRect(pObj.second->getGeometry()->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                        IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                        pObj.second->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                        IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
                    // Singleframe
                    m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
                    m_Graphics.rect(pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getLowerLeft() - m_pCamera->getCenter() + 
                                    IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                    pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getUpperRight()- m_pCamera->getCenter() +
                                    IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
                    m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
                    m_Graphics.filledRect(pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getLowerLeft() - m_pCamera->getCenter() + 
                                        IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                        pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getUpperRight()- m_pCamera->getCenter() +
                                        IGridUser::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
                    // Shape bounding boxes
                    for (const auto pShp : pObj.second->getGeometry()->getShapes())
                    {
                        m_Graphics.setColor(0.0, 0.0, 1.0, 0.8);
                        m_Graphics.rect(pShp->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                        IGridUser::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()),
                                        pShp->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                        IGridUser::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()));
                        m_Graphics.setColor(0.0, 0.0, 1.0, 0.2);
                        m_Graphics.filledRect(pShp->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() +
                                            IGridUser::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()),
                                            pShp->getBoundingBox().getUpperRight() - m_pCamera->getCenter() +
                                            IGridUser::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()));
                    }
                }
            }
            for (const auto Particle : *m_pDataStorage->getParticlesByValueFront())
            {
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
                m_Graphics.rect(Particle.second->getBoundingBox().getLowerLeft() - m_pCamera->getCenter(),
                                Particle.second->getBoundingBox().getUpperRight()- m_pCamera->getCenter());
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
                m_Graphics.filledRect(Particle.second->getBoundingBox().getLowerLeft() - m_pCamera->getCenter(),
                                    Particle.second->getBoundingBox().getUpperRight()- m_pCamera->getCenter());
            }
        m_Graphics.endRenderBatch();
    }

    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns if given visualisation is set
///
/// \param _nVis Visualisation (flag) that is tested for state
///
/// \return True or false
///
////////////////////////////////////////////////////////////////////////////////
bool CVisualsManager::getVisualisation(const int& _nVis) const
{
    METHOD_ENTRY("CVisualsManager::getVisualisation")

    if ((m_nVisualisations & _nVis) == _nVis)
    {
        METHOD_EXIT("CVisualsManager::getVisualisation")
        return true;
    }
    else
    {
        METHOD_EXIT("CVisualsManager::getVisualisation")
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws trajectories of objects
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawTrajectories() const
{
    METHOD_ENTRY("CVisualsManager::drawTrajectories")

    if (m_nVisualisations & VISUALS_OBJECT_TRAJECTORIES)
    {
        
        for (auto pObj : *m_pDataStorage->getObjectsByValueFront())
        {
            // Draw objects trajectories
            double fColourFade = 0.1;
            m_Graphics.beginLine(PolygonType::LINE_STRIP);
            
            for (auto i=0u; i<pObj.second->getTrajectory().getPositions().size(); ++i)
            {
                m_Graphics.setColor(0.5, 0.0, 0.8, fColourFade);
                m_Graphics.addVertex(pObj.second->getTrajectory().getPositions().at(i) - m_pCamera->getCenter() +
                    IGridUser::cellToDouble(pObj.second->getTrajectory().getCells().at(i)-m_pCamera->getCell())
                );
                fColourFade += 0.9/TRAJECTORY_CAPACITY;
            }
            m_Graphics.endLine();
            
            pObj.second->getTrajectory().unlock();
        }
        
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Cycle through cameras, using index
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::cycleCamera()
{
    METHOD_ENTRY("CVisualsManager::cycleCamera")
    ++m_unCameraIndex;
    if (m_unCameraIndex == m_pVisualsDataStorage->getCamerasByIndex().size())
        m_unCameraIndex = 0u;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the Graphics
///
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
bool CVisualsManager::init()
{
    METHOD_ENTRY("CVisualsManager::init")
    
    // Execute calls here, since processFrame hasn't been called yet. This 
    // is needed for calls, such as the setting up the data path.
    m_pComInterface->callWriters("visuals_pre_init");
    
    //--------------------------------------------------------------------------
    // Setup rendering
    //--------------------------------------------------------------------------
    
    CShader VertexShaderComposition;
    CShader VertexShaderFont;
    CShader VertexShaderMainScreen;
    CShader VertexShaderStars;
    CShader VertexShaderWorld;
    CShader FragmentShaderCamWidget;
    CShader FragmentShaderComposition;
    CShader FragmentShaderFont;
    CShader FragmentShaderMainScreen;
    CShader FragmentShaderStars;
    CShader FragmentShaderWorld;
    
    m_RenderTargetLights.init(m_Graphics.getWidthScr(), m_Graphics.getHeightScr(), m_unLightmapSubsampling);
    m_RenderTargetScene.init(m_Graphics.getWidthScr(), m_Graphics.getHeightScr());
    m_RenderTargetScreen.init(m_Graphics.getWidthScr(), m_Graphics.getHeightScr());

    VertexShaderComposition.load(m_strDataPath+"/shader/composition.vert", GL_VERTEX_SHADER);
    VertexShaderFont.load(m_strDataPath+"/shader/font.vert", GL_VERTEX_SHADER);    
    VertexShaderMainScreen.load(m_strDataPath+"/shader/main_screen.vert", GL_VERTEX_SHADER);
    VertexShaderStars.load(m_strDataPath+"/shader/stars.vert", GL_VERTEX_SHADER);
    VertexShaderWorld.load(m_strDataPath+"/shader/shader.vert", GL_VERTEX_SHADER);

    FragmentShaderCamWidget.load(m_strDataPath+"/shader/cam_widget_default.frag", GL_FRAGMENT_SHADER);
    FragmentShaderComposition.load(m_strDataPath+"/shader/composition.frag", GL_FRAGMENT_SHADER);
    FragmentShaderFont.load(m_strDataPath+"/shader/font.frag", GL_FRAGMENT_SHADER);
    FragmentShaderMainScreen.load(m_strDataPath+"/shader/main_screen.frag", GL_FRAGMENT_SHADER);
    FragmentShaderStars.load(m_strDataPath+"/shader/stars.frag", GL_FRAGMENT_SHADER);
    FragmentShaderWorld.load(m_strDataPath+"/shader/shader.frag", GL_FRAGMENT_SHADER);
    
    m_ShaderProgramCamWidget.create(VertexShaderMainScreen, FragmentShaderCamWidget);
    
    m_ShaderProgramFont.create(VertexShaderFont, FragmentShaderFont);
    m_RenderModeFont.setShaderProgram(&m_ShaderProgramFont);
    m_RenderModeFont.setRenderModeType(RenderModeType::VERT3COL4TEX2);
    
    m_ShaderProgramMainScreen.create(VertexShaderMainScreen, FragmentShaderMainScreen);
    m_RenderModeMainScreen.setShaderProgram(&m_ShaderProgramMainScreen);
    m_RenderModeMainScreen.setRenderModeType(RenderModeType::VERT3COL4TEX2);
    m_RenderModeMainScreen.setTexture0("ScreenTexture", m_RenderTargetScreen.getIDTex());
    
    m_ShaderProgramStars.create(VertexShaderStars, FragmentShaderStars);
    m_RenderModeStars.setShaderProgram(&m_ShaderProgramStars);
    m_RenderModeStars.setRenderModeType(RenderModeType::VERT3COL4);
    
    m_ShaderProgramWorld.create(VertexShaderWorld, FragmentShaderWorld);
    m_RenderModeWorld.setShaderProgram(&m_ShaderProgramWorld);
    m_RenderModeWorld.setRenderModeType(RenderModeType::VERT3COL4);
    
    m_RenderModeLights.setShaderProgram(&m_ShaderProgramWorld);
    m_RenderModeLights.setRenderModeType(RenderModeType::VERT3COL4);

    m_ShaderProgramComposition.create(VertexShaderComposition, FragmentShaderComposition);    
    m_RenderModeComposition.setShaderProgram(&m_ShaderProgramComposition);
    m_RenderModeComposition.setRenderModeType(RenderModeType::VERT3COL4TEX2X2);
    m_RenderModeComposition.setTexture0("SceneTexture", m_RenderTargetScene.getIDTex());
    m_RenderModeComposition.setTexture1("LightsTexture", m_RenderTargetLights.getIDTex());
    
    m_Graphics.registerRenderMode("composition", &m_RenderModeComposition);
    m_Graphics.registerRenderMode("font", &m_RenderModeFont);
    m_Graphics.registerRenderMode("lights", &m_RenderModeLights);
    m_Graphics.registerRenderMode("main_screen", &m_RenderModeMainScreen);
    m_Graphics.registerRenderMode("stars", &m_RenderModeStars);
    m_Graphics.registerRenderMode("world", &m_RenderModeWorld);
    
    m_RenderModeWorld.use();

    //--------------------------------------------------------------------------
    // Setup fonts
    //--------------------------------------------------------------------------
    
    m_FontManager.addFont("anka_c87_r", m_strDataPath + "/fonts/AnkaCoder-C87-r.ttf");
    m_FontManager.addFont("anka_c87_i", m_strDataPath + "/fonts/AnkaCoder-C87-i.ttf");
    m_FontManager.addFont("anka_c87_b", m_strDataPath + "/fonts/AnkaCoder-C87-b.ttf");
    m_FontManager.addFont("anka_c87_bi", m_strDataPath + "/fonts/AnkaCoder-C87-bi.ttf");
    m_FontManager.setRenderModeName("font");
    
    m_strFont = FONT_MGR_FONT_DEFAULT;
    
    //--------------------------------------------------------------------------
    // Setup visuals and windows
    //--------------------------------------------------------------------------
    
    // Setup engine global visuals first
    m_UIDVisuals.UIDText.setFont(m_strFont);
    m_UIDVisuals.UIDText.setColor({{1.0, 0.0, 1.0, 1.0}});
    m_UIDVisuals.UIDText.setSize(12);
    m_UIDVisuals.setBGColor({{0.1, 0.0, 0.1, 1.0}});
    
    // When calling init, all relevant variables have to be set up. Thus, sub 
    // components like the visuals data storage can be set up, eventually.
    m_pVisualsDataStorage->setFontManager(&m_FontManager);
    m_pVisualsDataStorage->setComInterface(m_pComInterface);
    m_pVisualsDataStorage->setUIDVisuals(&m_UIDVisuals);
    
    m_ConsoleWidgetID = this->createWidget(WidgetTypeType::CONSOLE);
    CWidgetConsole* pConsoleWidget = static_cast<CWidgetConsole*>(m_pVisualsDataStorage->getWidgetByValue(m_ConsoleWidgetID));
    
    pConsoleWidget->ConsoleText.setFont(m_strFont);
    pConsoleWidget->ConsoleText.setSize(16);
    pConsoleWidget->ConsoleText.setColor({{0.0, 1.0, 0.0, 1.0}});
    pConsoleWidget->setComConsole(m_pVisualsDataStorage->getComConsole());
    
    m_ConsoleWindowID = this->createWindow();
    CWindow* pConsoleWindow  = m_pVisualsDataStorage->getWindowByValue(m_ConsoleWindowID);
    
    pConsoleWindow->Title.setText("Command console");
    pConsoleWindow->Title.setFont(m_strFont);
    pConsoleWindow->Title.setSize(20);
    pConsoleWindow->Title.setColor({{1.0, 1.0, 1.0, 1.0}});
    pConsoleWindow->setWidget(pConsoleWidget);
    pConsoleWindow->setColorBG({{0.1, 0.1, 0.1, 0.75}}, WIN_INHERIT);
    pConsoleWindow->setColorFG({{0.3, 0.3, 0.3, 0.75}}, WIN_INHERIT);
    pConsoleWindow->setPosition(10, 10);
    pConsoleWindow->resize(800, 150);
    pConsoleWindow->setVisibilty(false);
    pConsoleWindow->setClosability(false);
    
    // Initialise UI text objects
    m_TextParticle.setFont(m_strFont);
    m_TextParticle.setSize(12);
    m_TextDebugInfo.setColor({{1.0, 0.0, 1.0, 0.8}});
    m_TextDebugInfo.setFont(m_strFont);
    m_TextDebugInfo.setPosition(10, 10);
    m_TextDebugInfo.setSize(16);
    m_TextDebugRender.setColor({{1.0, 0.0, 1.0, 0.8}});
    m_TextDebugRender.setFont(m_strFont);
    m_TextDebugRender.setSize(32);
    m_TextObjects.setFont(m_strFont);
    m_TextObjects.setSize(12);
    m_TextScale.setFont(m_strFont);
    m_TextScale.setPosition(m_Graphics.getWidthScr()/2, 32.0f, TEXT_POSITION_CENTERED_X);
    m_TextScale.setSize(16);
    m_TextStarSystems.setFont(m_strFont);
    m_TextStarSystems.setSize(12);
    m_TextTimers.setFont(m_strFont);
    m_TextTimers.setPosition(10.0f, 10.0f);
    m_TextTimers.setSize(12);
    m_TextVersion.setFont(m_strFont);
    m_TextVersion.setSize(10);
    m_TextVersion.setColor({{1.0, 1.0, 1.0, 0.75}});
    m_TextVersion.setText("Version "+PW_VERSION_FULL);
    m_TextVersion.setPosition(10, m_Graphics.getHeightScr()-m_TextVersion.getFontSize()-10);
    
    return m_Graphics.init();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a generic camera
///
/// This method creates a camera and adds it to storage. In
/// \ref CreationModeType::QUEUED, it will be moved to a waiter queue and later
/// be added to storage. The reason for this lies in multithreading and return
/// value retrieval. When a camera is created, the UID has to be returned.
/// Hence, it is defined as a reader command (see \ref CComInterface). The
/// newly created camera is stored temporary in a concurrent queue and later
/// added to storage by the appropriate thread.
///
/// \param _pMode Mode of creation (direct to storage or threading queue)
///
/// \return UID of newly created camera
///
///////////////////////////////////////////////////////////////////////////////
UIDType CVisualsManager::createCamera(const CreationModeType _pMode)
{
    METHOD_ENTRY("CVisualsManager::createCamera")

    m_CreatorLock.acquireLock();
    m_pVisualsDataStorage->AccessCameras.setLock();
    
    CCamera* pCam = new CCamera();
    MEM_ALLOC("CCamera")
    
    if (_pMode == CreationModeType::DIRECT)
    {
        m_pVisualsDataStorage->addCamera(pCam);
    }
    else
    {
        m_CamerasQueue.enqueue(pCam);
    }
    m_CreatorLock.releaseLock();
    
    return pCam->getUID();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a widget of the given type
///
/// This method creates a widget and adds it to storage. In
/// \ref CreationModeType::QUEUED, it will be moved to a waiter queue and later
/// be added to storage. The reason for this lies in multithreading and return
/// value retrieval. When a widget is created, the UID has to be returned.
/// Hence, it is defined as a reader command (see \ref CComInterface). The
/// newly created widget is stored temporary in a concurrent queue and later
/// added to storage by the appropriate thread.
///
/// \param _WidgetType Type of widget to be created
/// \param _pMode Mode of creation (direct to storage or threading queue)
///
/// \return UID of newly created window
///
///////////////////////////////////////////////////////////////////////////////
UIDType CVisualsManager::createWidget(const WidgetTypeType _WidgetType,
                                      const CreationModeType _pMode)
{
    METHOD_ENTRY("CVisualsManager::createWidget")
    
    IWidget* pWidget = nullptr;
    
    switch (_WidgetType)
    {
        case WidgetTypeType::CAMERA:
        {
            CWidgetCam* pCameraWidget = new CWidgetCam(&m_FontManager);
            pCameraWidget->setUIDVisuals(&m_UIDVisuals);
            pWidget = pCameraWidget;
            break;
        }
        case WidgetTypeType::CONSOLE:
        {
            CWidgetConsole* pConsoleWidget = new CWidgetConsole(&m_FontManager);
            pConsoleWidget->setUIDVisuals(&m_UIDVisuals);
            pConsoleWidget->setComConsole(m_pVisualsDataStorage->getComConsole());
            pConsoleWidget->ConsoleText.setFont(m_strFont);
            m_pVisualsDataStorage->getComConsole()->setComInterface(m_pComInterface);
            pWidget = pConsoleWidget;
            break;
        }
        case WidgetTypeType::TEXT:
        {
            CWidgetText* pTextWidget = new CWidgetText(&m_FontManager);
            pTextWidget->setUIDVisuals(&m_UIDVisuals);
            pTextWidget->Text.setFont(m_strFont);
            pWidget = pTextWidget;
            break;
        }
        default:
        {
            WARNING_MSG("Visuals Data Storage", "Unknown widget type.")
            break;
        }
    }
    MEM_ALLOC("IWidget")

    if (pWidget != nullptr)
    {
        if (_pMode == CreationModeType::DIRECT)
        {
            m_pVisualsDataStorage->addWidget(pWidget);
        }
        else
        {
            m_WidgetsQueue.try_enqueue(pWidget);
        }
    
        return pWidget->getUID();
    }
    else
    {
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a generic window
///
/// This method creates a window and adds it to storage. In
/// \ref CreationModeType::QUEUED, it will be moved to a waiter queue and later
/// be added to storage. The reason for this lies in multithreading and return
/// value retrieval. When a window is created, the UID has to be returned.
/// Hence, it is defined as a reader command (see \ref CComInterface). The
/// newly created window is stored temporary in a concurrent queue and later
/// added to storage by the appropriate thread.
///
/// \param _pMode Mode of creation (direct to storage or threading queue)
///
/// \return UID of newly created window
///
///////////////////////////////////////////////////////////////////////////////
UIDType CVisualsManager::createWindow(const CreationModeType _pMode)
{
    METHOD_ENTRY("CVisualsManager::createWindow")
    
    CWindow* pWin = new CWindow(&m_FontManager);
    MEM_ALLOC("CWindow")
    
    pWin->setUIDVisuals(&m_UIDVisuals);
    pWin->Title.setFont(m_strFont);
    if (_pMode == CreationModeType::DIRECT)
    {
        m_pVisualsDataStorage->addWindow(pWin);
    }
    else
    {
        m_WindowsQueue.try_enqueue(pWin);
    }
    
    return pWin->getUID();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Drawing finished, now swap buffers
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::finishFrame()
{
    METHOD_ENTRY("CVisualsManager::finishFrame")
    
    m_Graphics.swapBuffers();
    DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_NOTICE);)
    m_pDataStorage->swapFront();
    DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_DEBUG);)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Processes one visual frame
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CVisualsManager::processFrame()
{
    METHOD_ENTRY("CVisualsManager::processFrame")
    
    // Setup main camera before calling any commands, to ensure correct
    // setup when fullscreen is toggled
    this->addCamerasFromQueue();
    bool bGotCam = m_pVisualsDataStorage->getCamerasByIndex().size() > 0;
    if (bGotCam)
    {
        m_pCamera = m_pVisualsDataStorage->getCamerasByIndex().operator[](m_unCameraIndex);
        m_pCamera->update();
    }
    
    this->addWidgetsFromQueue();
    this->addWindowsFromQueue();
    m_pComInterface->callWriters("visuals");

    if (bGotCam)
    {
        for (auto CamWidget : m_pVisualsDataStorage->getCameraWidgets())
        {
            if (CamWidget.second->getCamera()->isValid())
            {
                CamWidget.second->getRenderTarget()->bind(RENDER_TARGET_CLEAR);
                //
                    m_Graphics.setupWorldSpace();
                    
                    m_pCamera = CamWidget.second->getCamera()->get();
                    m_pCamera->update();
                    this->drawStars();
                    this->drawWorld();
                //
                CamWidget.second->getRenderTarget()->unbind();
            }
        }
    }
    m_RenderTargetScene.bind(RENDER_TARGET_CLEAR);
        m_Graphics.setupWorldSpace();
        if (bGotCam)
        {
            m_pCamera = m_pVisualsDataStorage->getCamerasByIndex().operator[](m_unCameraIndex);
            m_pCamera->update();
            
            this->drawGrid(DrawModeType::VISUALS);
            // this->drawTrajectories();

            m_Graphics.setColor({{1.0, 1.0, 1.0, 1.0}});
            
            this->drawStars();
            m_Graphics.setLineWidth(5.0);
            this->drawWorld();
            m_Graphics.setLineWidth(1.0);
            this->drawCOM();
            this->drawBoundingBoxes();
            this->drawKinematicsStates(DrawModeType::VISUALS);
        }
    m_RenderTargetScene.unbind();
    
    ////////////////////////////////////////////////////////////////////////////
    // Hacked, just for testing purposes
    ////////////////////////////////////////////////////////////////////////////
    m_RenderTargetLights.bind(RENDER_TARGET_CLEAR);
        m_Graphics.beginRenderBatch("lights");
        for (auto Particle : *m_pDataStorage->getParticlesByValueFront())
        {
            if (Particle.second->getBoundingBox().overlaps(m_pCamera->getBoundingBox()))
            {
                if (Particle.second->getParticleType() == ParticleTypeType::THRUST)
                {
                    auto   nSize   = Particle.second->getPositions()->size();
                    double fSizeR  = 1.0;
                    double fGrowth = (Particle.second->getSizeDeath() - Particle.second->getSizeBirth()) * 30.0 *
                                        nSize/Particle.second->getPositions()->capacity();
                    ColorTypeRGBA paColBirth = *Particle.second->getColorBirth();
                    ColorTypeRGBA paColDeath = *Particle.second->getColorDeath();
                                        
                    if (Particle.second->getBoundingBox().getWidth()  * m_Graphics.getResPMX() > 2.0 &&
                        Particle.second->getBoundingBox().getHeight() * m_Graphics.getResPMY() > 2.0)
                    {
                        m_Graphics.cacheSinCos(100);
                        for (auto i=0u; i<nSize; ++i)
                        {
                            if (Particle.second->getStates()->at(i) == PARTICLE_STATE_ACTIVE &&
                                m_pCamera->getBoundingBox().isInside(Particle.second->getPositions()->at(i)))
                            {
                                double fAge = double(Particle.second->getAge()->at(i)) / Particle.second->getMaxAge();
                                
                                double fR = paColBirth[0] * (1.0 - fAge) + paColDeath[0] * fAge;
                                double fG = paColBirth[1] * (1.0 - fAge) + paColDeath[1] * fAge;
                                double fB = paColBirth[2] * (1.0 - fAge) + paColDeath[2] * fAge;
                                double fA = paColBirth[3] * (1.0 - fAge) + paColDeath[3] * fAge;
                                fA *= 0.1;
                                m_Graphics.setColor(fR, fG, fB, fA);
                                
                                fSizeR = Particle.second->getSizeBirth() * 30.0 + fGrowth * fAge;
                                
                                    m_Graphics.filledCircle(Particle.second->getPositions()->at(i) - m_pCamera->getCenter()+
                                                IGridUser::cellToDouble(Particle.second->getCell() - m_pCamera->getCell()),
                                                fSizeR, 100, GRAPHICS_CIRCLE_USE_CACHE);
                            }
                        }
                        m_Graphics.setColor(1.0,1.0,1.0);
                    }
                }
            }
        }
        m_Graphics.endRenderBatch();
    m_RenderTargetLights.unbind();
    ////////////////////////////////////////////////////////////////////////////
    // End of Hack
    ////////////////////////////////////////////////////////////////////////////
    
    m_Graphics.setupScreenSpace();
    
    m_Graphics.setColor({{1.0, 1.0, 1.0, 1.0}});
    glViewport(0, 0, m_Graphics.getWidthScr(), m_Graphics.getHeightScr());

    // Compose scene and light information from accordant textures
    m_RenderTargetScreen.bind();    
        m_Graphics.beginRenderBatch("composition");
            m_Graphics.texturedRect(Vector2d(0.0, m_Graphics.getHeightScr()), Vector2d(m_Graphics.getWidthScr(), 0.0),
                                    &m_RenderTargetScene.getTexUV(), &m_RenderTargetLights.getTexUV());
        m_Graphics.endRenderBatch();
    m_RenderTargetScreen.unbind();
    
    // Render texture to screen
    m_RenderModeMainScreen.setTexture0("ScreenTexture", m_RenderTargetScreen.getIDTex());
    m_Graphics.beginRenderBatch("main_screen");
        m_Graphics.texturedRect(Vector2d(0.0, m_Graphics.getHeightScr()), Vector2d(m_Graphics.getWidthScr(), 0.0), &m_RenderTargetScreen.getTexUV());
    m_Graphics.endRenderBatch();
    
    this->drawKinematicsStates(DrawModeType::TEXT);
    if (bGotCam) this->drawGrid(DrawModeType::TEXT);
    
    m_Graphics.beginRenderBatch("font");
        m_TextVersion.display();
    m_Graphics.endRenderBatch();
    
    this->drawTimers();
    this->drawWindows();
    this->updateUI();
    this->drawDebugInfo();
    
    this->finishFrame();
    
    // Attach cameras to current front buffer
    if (bGotCam)
    {
        for (auto pCam : m_pVisualsDataStorage->getCamerasByIndex())
        {
            if (pCam->isAttached())
                pCam->getKinematicsState().setRef(static_cast<CKinematicsState*>(m_pDataStorage->getUIDUsersByValueFront()->at(pCam->getKinematicsState().getRef()->getUID())));
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add cameras from queue to storage
///
/// This method will add the cameras from queue (see
/// \ref CVisualsDataStorage::addCamera) to storage.
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsManager::addCamerasFromQueue()
{
    METHOD_ENTRY("CVisualsManager::addCamerasFromQueue")
    
    m_CreatorLock.acquireLock();
    
    CCamera* pCam = nullptr;
    while (m_CamerasQueue.try_dequeue(pCam))
    {
        m_pVisualsDataStorage->addCamera(pCam);
    }
    m_pVisualsDataStorage->AccessCameras.releaseLock();
    
    m_CreatorLock.releaseLock();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add widgets from queue to storage
///
/// This method will add the widgets from queue (see
/// \ref CVisualsDataStorage::addWidget) to storage.
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsManager::addWidgetsFromQueue()
{
    METHOD_ENTRY("CVisualsManager::addWidgetsFromQueue")
    
    IWidget* pWidget = nullptr;
    while (m_WidgetsQueue.try_dequeue(pWidget))
    {
        m_pVisualsDataStorage->addWidget(pWidget);
        
        if (pWidget->getType() == WidgetTypeType::CAMERA)
        {
            static_cast<CWidgetCam*>(pWidget)->setShaderProgram(&m_ShaderProgramCamWidget);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add windows from queue to storage
///
/// This method will add the windows from queue (see
/// \ref CVisualsDataStorage::addWindow) to storage.
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsManager::addWindowsFromQueue()
{
    METHOD_ENTRY("CVisualsManager::addWindowsFromQueue")
    
    CWindow* pWindow = nullptr;
    while (m_WindowsQueue.try_dequeue(pWindow))
    {
        m_pVisualsDataStorage->addWindow(pWindow);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws graphics debug information, such as the number of draw calls.
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawDebugInfo()
{
    METHOD_ENTRY("CVisualsManager::drawDebugInfo")
    
    if (m_nVisualisations & VISUALS_DEBUG_RENDER)
    {
        auto fBorderX = m_Graphics.getWidthScr() * 0.01;
        auto fBorderY = m_Graphics.getHeightScr() * 0.01;
        
        m_RenderModeMainScreen.setTexture0("LightsTexture", m_RenderTargetLights.getIDTex());
        m_Graphics.beginRenderBatch("main_screen");
            m_Graphics.texturedRect(Vector2d(fBorderX, m_Graphics.getHeightScr()>>1), Vector2d(m_Graphics.getWidthScr()>>1, fBorderY),
                                    &m_RenderTargetLights.getTexUV());
        m_Graphics.endRenderBatch();
        
        m_RenderModeMainScreen.setTexture0("SceneTexture", m_RenderTargetScene.getIDTex());
        m_Graphics.beginRenderBatch("main_screen");
            m_Graphics.texturedRect(Vector2d(m_Graphics.getWidthScr()>>1, m_Graphics.getHeightScr()>>1),
                                    Vector2d(m_Graphics.getWidthScr()-fBorderX, fBorderY),
                                    &m_RenderTargetScene.getTexUV());
        m_Graphics.endRenderBatch();
        m_RenderModeMainScreen.setTexture0("ScreenTexture", m_RenderTargetScreen.getIDTex());
        m_Graphics.beginRenderBatch("main_screen");
            m_Graphics.texturedRect(Vector2d(fBorderX, m_Graphics.getHeightScr()-fBorderY),
                                    Vector2d(m_Graphics.getWidthScr()>>1, m_Graphics.getHeightScr()>>1),
                                    &m_RenderTargetScreen.getTexUV());
        m_Graphics.endRenderBatch();
        
        m_Graphics.setColor({{1.0, 0.0, 1.0, 0.8}});
        m_Graphics.beginRenderBatch("world");
            m_Graphics.rect(Vector2d(fBorderX, m_Graphics.getHeightScr()>>1), Vector2d(m_Graphics.getWidthScr()>>1, fBorderY));
            m_Graphics.rect(Vector2d(m_Graphics.getWidthScr()>>1, m_Graphics.getHeightScr()>>1),
                                    Vector2d(m_Graphics.getWidthScr()-fBorderX, fBorderY));
            m_Graphics.rect(Vector2d(fBorderX, m_Graphics.getHeightScr()-fBorderY),
                                    Vector2d(m_Graphics.getWidthScr()>>1, m_Graphics.getHeightScr()>>1));
        m_Graphics.endRenderBatch();

        m_TextDebugRender.setPosition(fBorderX*2.0, fBorderY*2.0);
        m_TextDebugRender.setText("Lights");
        m_Graphics.beginRenderBatch("font");
            m_TextDebugRender.display();
        m_Graphics.endRenderBatch();
        
        m_TextDebugRender.setPosition((m_Graphics.getWidthScr()>>1)+fBorderX*2.0, fBorderY*2.0);
        m_TextDebugRender.setText("Scene");
        m_Graphics.beginRenderBatch("font");
            m_TextDebugRender.display();
        m_Graphics.endRenderBatch();
        
        m_TextDebugRender.setPosition(fBorderX*2.0, (m_Graphics.getHeightScr()>>1)+fBorderY*2.0);
        m_TextDebugRender.setText("Composition");
        m_Graphics.beginRenderBatch("font");
            m_TextDebugRender.display();
        m_Graphics.endRenderBatch();
    }
    if (m_nVisualisations & VISUALS_DEBUG_INFO)
    {
        std::ostringstream oss;
        oss << "GRAPHICS\n\n  Drawcalls: " << m_Graphics.getDrawCalls()+1 << "\n";
        oss << "  Lines: " << m_Graphics.getLinesPerFrame() << "\n";
        oss << "  Points: " << m_Graphics.getPointsPerFrame() << "\n";
        oss << "  Triangles: " << m_Graphics.getTrianglesPerFrame() << "\n";
        oss << "  Vertices: " << m_Graphics.getVerticesPerFrame() << "\n";
        
        oss << "\n";
        
        oss << "FONT RENDERER\n\n  Available fonts:\n";
        for (auto strFont : m_FontManager.getFontsAvailable())
        {
            oss << "   - " << strFont.first << "\n";
        }
        
        oss << "\n";

        static double fBufferSizePhysics = m_pComInterface->call<double>("get_frequency_physics") / 4;
        static double fBufferSizeLua = m_pComInterface->call<double>("get_frequency_lua") / 4;
        static CCircularBuffer<double> TimeBufferLua(fBufferSizeLua);
        static CCircularBuffer<double> TimeBufferPhysics(fBufferSizePhysics);
        static CCircularBuffer<double> TimeBufferPhysicsBufferCopy(fBufferSizePhysics);
        static CCircularBuffer<double> TimeBufferPhysicsObjects(fBufferSizePhysics);
        static CCircularBuffer<double> TimeBufferPhysicsParticles(fBufferSizePhysics);
        static CCircularBuffer<double> TimeBufferVisuals(m_fFrequency / 4);
        
        double fTimeProcessedLua = this->smoothFrameTime(
                                        &TimeBufferLua,
                                        m_pComInterface->call<double>("get_time_processed_lua"),
                                        fBufferSizeLua);
        double fTimeProcessedPhysics = this->smoothFrameTime(
                                        &TimeBufferPhysics,
                                        m_pComInterface->call<double>("get_time_processed_physics"),
                                        fBufferSizePhysics);
        double fTimeProcessedPhysicsBufferCopy = this->smoothFrameTime(
                                        &TimeBufferPhysicsBufferCopy,
                                        m_pComInterface->call<double>("get_time_processed_physics_buffer_copy"),
                                        fBufferSizePhysics);
        double fTimeProcessedPhysicsObjects = this->smoothFrameTime(
                                        &TimeBufferPhysicsObjects,
                                        m_pComInterface->call<double>("get_time_processed_physics_objects"),
                                        fBufferSizePhysics);
        double fTimeProcessedPhysicsParticles = this->smoothFrameTime(
                                        &TimeBufferPhysicsParticles,
                                        m_pComInterface->call<double>("get_time_processed_physics_particles"),
                                        fBufferSizePhysics);
        
        double fTimeProcessedVisuals = this->smoothFrameTime(
                                        &TimeBufferVisuals,
                                        this->getTimeProcessed(),
                                        m_fFrequency / 4);
                
        
        oss << "PERFORMANCE\n\n";
        oss << std::fixed << std::setprecision(1);
        oss << "  Visuals:       " << fTimeProcessedVisuals*1000.0 << " of " <<
                                      this->getTimePerFrame()*1000.0 << " ms\n";
        oss << "  Physics:       " << fTimeProcessedPhysics*1000.0 << " of " <<
                                      m_pComInterface->call<double>("get_time_per_frame_physics")*1000.0 << " ms\n";
        oss << "  - Objects:     " << fTimeProcessedPhysicsObjects*1000.0 << " ms\n";
        oss << "  - Particles:   " << fTimeProcessedPhysicsParticles*1000.0 << " ms\n";
        oss << "  - Buffer Copy: " << fTimeProcessedPhysicsBufferCopy*1000.0 << " ms\n";
        oss << "  Lua:           " << fTimeProcessedLua*1000.0 << " of " <<
                                      m_pComInterface->call<double>("get_time_per_frame_lua")*1000.0 << " ms\n";
                
        m_TextDebugInfo.setText(oss.str());
        m_Graphics.setColor({{0.1, 0.0, 0.1, 0.8}});
        
        m_Graphics.beginRenderBatch("world");
            double fSizeX = m_TextDebugInfo.getLength()+5.0;
            int nLines = 20;
            m_Graphics.filledRect(Vector2d(10, 10),
                                  Vector2d(10 + fSizeX, 20+
                                  (nLines+m_FontManager.getFontsAvailable().size())*m_TextDebugInfo.getFontSize()));
        m_Graphics.endRenderBatch();
        
        m_Graphics.beginRenderBatch("font");
            m_TextDebugInfo.display();
        m_Graphics.endRenderBatch();
        
        m_Graphics.setColor({{1.0, 1.0, 1.0, 1.0}});
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws the universe grid
///
/// To overcome problems with numeric limits of double values a grid is incor-
/// porated. The double valued position lives within a cell of an integer grid.
/// Hence, objects may not be larger than the size of one grid element to allow
/// drawing, collision detection and so on.
///
/// \param _DrawMode Draw visuals or text
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawGrid(const DrawModeType _DrawMode)
{
    METHOD_ENTRY("CVisualsManager::drawGrid")
    
    if (m_nVisualisations & VISUALS_UNIVERSE_GRID)
    {
        if (_DrawMode == DrawModeType::VISUALS)
        {
            m_Graphics.beginRenderBatch("world");
            
                // Default sub grid size every 1m
                double fGrid = 1.0;
                double fGridLeft;
                double fGridTop;
                double fGridLeftCell;
                double fGridTopCell;
                
                // Automatically scale grid depending on zoom level
                while ((m_pCamera->getBoundingCircleRadius() / fGrid) > 100.0)
                    fGrid*=10.0;
                while ((m_pCamera->getBoundingCircleRadius() / fGrid) < 10.0)
                    fGrid*=0.1;
                
                // If zoomed out to larger grids then universe cell, the bounding box
                // cell of the camera has to be considered.
                if (fGrid < 1.0e12)
                {
                    fGridLeftCell = 0.0;
                    fGridTopCell = 0.0;
                }
                else
                {
                    fGridLeftCell = IGridUser::cellToDouble(m_pCamera->getCell())[0];
                    fGridTopCell  = IGridUser::cellToDouble(m_pCamera->getCell())[1];
                }

                // Snap sub grid to sub grid size
                fGridLeft=(std::floor((m_pCamera->getBoundingBox().getLowerLeft()[0] +
                                fGridLeftCell)/fGrid)+1.0)*fGrid-fGridLeftCell;
                fGridTop =(std::floor((m_pCamera->getBoundingBox().getLowerLeft()[1] + 
                                fGridTopCell )/fGrid)+1.0)*fGrid-fGridTopCell;
                
                // Change colour if on cell grid
                if (fGrid == DEFAULT_CELL_SIZE)
                    m_Graphics.setColor(0.3, 0.0, 0.0);
                else
                    m_Graphics.setColor(0.1, 0.1, 0.1);
            
                // Vertical sub grid lines
                while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
                {
                    m_Graphics.beginLine(PolygonType::LINE_SINGLE);
                        m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0],
                                                    m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                    m_pCamera->getCenter()[1]);
                        m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0],
                                                    m_pCamera->getBoundingBox().getUpperRight()[1]-
                                                    m_pCamera->getCenter()[1]);
                    m_Graphics.endLine();
                    fGridLeft += fGrid;
                }
                // Horizontal sub grid lines
                while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
                {
                    m_Graphics.beginLine(PolygonType::LINE_SINGLE);
                        m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                            m_pCamera->getCenter()[0],
                                            fGridTop-m_pCamera->getCenter()[1]);
                        m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                            m_pCamera->getCenter()[0],
                                            fGridTop-m_pCamera->getCenter()[1]);
                    m_Graphics.endLine();
                    fGridTop += fGrid;
                }
                
                // Second grid is more coarse
                fGrid *= 10.0;
                
                // Snap grid to grid size
                fGridLeft=(floor((m_pCamera->getBoundingBox().getLowerLeft()[0] +
                                fGridLeftCell)/fGrid)+1.0)*fGrid - fGridLeftCell;
                fGridTop =(floor((m_pCamera->getBoundingBox().getLowerLeft()[1] + 
                                fGridTopCell)/fGrid)+1.0)*fGrid - fGridTopCell;
                
                // Change colour if on cell grid
                if (fGrid == DEFAULT_CELL_SIZE)
                    m_Graphics.setColor(0.3, 0.0, 0.0);
                else
                    m_Graphics.setColor(0.2, 0.2, 0.2);
                
                // Vertical grid lines
                while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
                {
                    m_Graphics.beginLine(PolygonType::LINE_SINGLE);
                        m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0],
                                                        m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                        m_pCamera->getCenter()[1]);
                        m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0],
                                                        m_pCamera->getBoundingBox().getUpperRight()[1]-
                                                        m_pCamera->getCenter()[1]);
                    m_Graphics.endLine();
                    fGridLeft += fGrid;
                }
                // Horizontal grid lines
                while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
                {
                    m_Graphics.beginLine(PolygonType::LINE_SINGLE);
                        m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                            m_pCamera->getCenter()[0],
                                            fGridTop-m_pCamera->getCenter()[1]);
                        m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                            m_pCamera->getCenter()[0],
                                            fGridTop-m_pCamera->getCenter()[1]);
                    m_Graphics.endLine();
                    fGridTop += fGrid;
                }
                
                m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
            m_Graphics.endRenderBatch();
        }
        else
        {
            m_Graphics.beginRenderBatch("font");
            
            double fGrid = 1.0;
            
            // Automatically scale grid depending on zoom level
            while ((m_pCamera->getBoundingCircleRadius() / fGrid) > 100.0)
                fGrid*=10.0;
            while ((m_pCamera->getBoundingCircleRadius() / fGrid) < 10.0)
                fGrid*=0.1;
            
            // Now draw the text
            std::stringstream oss;

            if (fGrid < 1000.0)
                oss << "Grid Resolution: " << fGrid << "m";
            else
                oss << "Grid Resolution: " << fGrid * 0.001 << "km";
            
            m_TextScale.setText(oss.str());
            m_TextScale.display();
            
            m_Graphics.endRenderBatch();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws information from a kinematics state
///
/// This method draws information from a kinematics state e.g. the local
/// coordinate system and references.
///
/// \param _pKinematicsState Kinematics State to be drawn
/// \param _fSize Size of coordinate system that is drawn
/// \param _DrawMode Draw visuals or text
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawKinematicsState(const CKinematicsState* const _pKinematicsState,
                                          const double& _fSize,
                                          const DrawModeType _DrawMode)
{
    METHOD_ENTRY("CVisualsManager::drawKinematicsState")
    
    if ((_fSize * m_pCamera->getZoom() > 10.0) &&
            (m_pCamera->getBoundingBox().isInside(_pKinematicsState->getOrigin()))) 
    {
        double fTransparency = 0.5;
        
        if (_DrawMode == DrawModeType::VISUALS)
        {
            m_Graphics.beginRenderBatch("world");
            
                m_Graphics.setColor(1.0, 1.0, 1.0, fTransparency);
                m_Graphics.showVec(
                    _pKinematicsState->getPosition(Vector2d(_fSize, 0.0))-
                    _pKinematicsState->getOrigin(),
                    _pKinematicsState->getOrigin()-m_pCamera->getKinematicsState().getOrigin()
                );
                m_Graphics.showVec(
                    _pKinematicsState->getPosition(Vector2d(0.0, _fSize))-
                    _pKinematicsState->getOrigin(),
                    _pKinematicsState->getOrigin()-m_pCamera->getKinematicsState().getOrigin()
                );
                
                if (_pKinematicsState->getRef()->isValid())
                {
                    Vector2d vecRef = _pKinematicsState->getRef()->get()->getOrigin() - _pKinematicsState->getOrigin();
                    m_Graphics.showVec(
                        vecRef,
                        _pKinematicsState->getOrigin()-m_pCamera->getKinematicsState().getOrigin()
                    );
                }

            m_Graphics.endRenderBatch();
        }
        else
        {
            // Now draw the text
            m_Graphics.beginRenderBatch("font");
            
                std::stringstream oss;
                
                CText TextKState(&m_FontManager);
                oss << "Local angle:    " <<  _pKinematicsState->getLocalAngle() << "\n";
                oss << "Local origin:   " <<  _pKinematicsState->getLocalOrigin()[0] << ", "
                                        <<  _pKinematicsState->getLocalOrigin()[1] << "\n";
                oss << "Angle:          " <<  _pKinematicsState->getAngle() << "\n";
                oss << "Origin:         " <<  _pKinematicsState->getOrigin()[0] << ", "
                                        <<  _pKinematicsState->getOrigin()[1] << "\n";
                TextKState.setText(oss.str());
                TextKState.setFont(m_strFont);
                TextKState.setSize(12);
                TextKState.setColor({{1.0, 1.0, 1.0, fTransparency}});
                TextKState.setPosition(m_Graphics.world2Screen(_pKinematicsState->getOrigin()-m_pCamera->getKinematicsState().getOrigin())[0],
                                       m_Graphics.world2Screen(_pKinematicsState->getOrigin()-m_pCamera->getKinematicsState().getOrigin())[1]);

            
                TextKState.display();
                
            m_Graphics.endRenderBatch();
        }
        
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws information from the kinematics states
///
/// This method draws information from the kinematics states e.g. the local
/// coordinate system and references.
///
/// \param _DrawMode Draw visuals or text
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawKinematicsStates(const DrawModeType _DrawMode)
{
    METHOD_ENTRY("CVisualsManager::drawKinematicsStates")
    
    if (m_nVisualisations & VISUALS_KINEMATICS_STATES)
    {
        for (const auto pObj : *m_pDataStorage->getObjectsByValueFront())
        {
            this->drawKinematicsState(
                &pObj.second->getKinematicsState(),
                pObj.second->getGeometry()->getBoundingBox().getHeight() +
                pObj.second->getGeometry()->getBoundingBox().getWidth() * 0.5 * 0.33,
                _DrawMode
            );
        }
        this->drawKinematicsState(&m_pCamera->getKinematicsState(),
                                  m_pCamera->getBoundingCircleRadius() * 0.1,
                                  _DrawMode);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws stars
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawStars()
{
    METHOD_ENTRY("CVisualsManager::drawStars")
    
    m_Graphics.beginRenderBatch("stars");
    if (m_pDataStorage->getUniverse() != nullptr)
    {
        m_pDataStorage->getUniverse()->Access.acquireLock();

        for (auto i=0u; i<m_pDataStorage->getUniverse()->getStarSystems()->size(); ++i)
        {
            CStarSystem* pStarSystem = m_pDataStorage->getUniverse()->getStarSystems()->at(i);
            CStar&       Star(pStarSystem->Star());
            Vector2d vecPos = CKinematicsState::clipToWorldLimit(
                                Star.getOrigin() +
                                IGridUser::cellToDouble(pStarSystem->getCell()-m_pCamera->getCell())
                            );
            Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(
                                    Star.getOrigin() - m_pCamera->getCenter() +
                                    IGridUser::cellToDouble
                                    (pStarSystem->getCell() -
                                    m_pCamera->getCell())
                                );
            

            // Draw stars in original scale
            if (m_pCamera->getBoundingBox().isInside(vecPos))
            {
                
                double fColor = 0.1*Star.getStarType()+0.3;
                m_Graphics.setColor(0.8,fColor,0.3);
                
                double fDrawSize = (Star.getStarType()*0.3+1) * m_Graphics.getResMPX();
                double fRadius   =  Star.getRadius();
                if (fDrawSize > fRadius)
                    m_Graphics.filledCircle(vecPosRel, fDrawSize, 7.0);
                else
                    m_Graphics.filledCircle(vecPosRel, (Star.getRadius()), 100.0);
            }
        }
        m_pDataStorage->getUniverse()->Access.releaseLock();
    }
    m_Graphics.endRenderBatch();
}    
    
////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws global and local simulation timers
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawTimers()
{
    METHOD_ENTRY("CVisualsManager::drawTimers")
    
    if (m_nVisualisations & VISUALS_TIMERS)
    {
        m_Graphics.beginRenderBatch("font");
        
            // Now draw the text
            std::stringstream oss;
            
            oss << "Simulation time: "
                << m_pComInterface->call<int,int>("get_time_years", 0) << "y "
                << m_pComInterface->call<int,int>("get_time_days_part", 0) << "d "
                << m_pComInterface->call<int,int>("get_time_hours_part", 0) << "h "
                << m_pComInterface->call<int,int>("get_time_minutes_part", 0) << "m "
                << m_pComInterface->call<int,int>("get_time_seconds_part", 0) << "s" << std::endl;
            for (auto i=1; i<m_pComInterface->call<int>("get_nrof_timers"); ++i)
            {
                oss << "Local timer " << i << ":   "
                    << m_pComInterface->call<int,int>("get_time_years", i) << "y "
                    << m_pComInterface->call<int,int>("get_time_days_part", i) << "d "
                    << m_pComInterface->call<int,int>("get_time_hours_part", i) << "h "
                    << m_pComInterface->call<int,int>("get_time_minutes_part", i) << "m "
                    << m_pComInterface->call<int,int>("get_time_seconds_part", i) << "s" << std::endl;
            }
            
            if (m_pDataStorage->getTimeScale() > 1.0)
            {
                m_TextTimers.setColor({{1.0, 0.0, 0.0, 1.0}});
                oss << "\nWarning: Decreasing accuracy." << std::endl;
            }
            else
            { 
                m_TextTimers.setColor({{1.0, 1.0, 1.0, 1.0}});
            }
            m_TextTimers.setText(oss.str());
            m_TextTimers.display();
        
        m_Graphics.endRenderBatch();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws windows if visible
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawWindows()
{
    METHOD_ENTRY("CVisualsManager::drawWindows")
    
    for (const auto WinUID : *m_pVisualsDataStorage->getWindowUIDsInOrder())
    {
        m_pVisualsDataStorage->getWindowsByValue()->at(WinUID)->draw();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws all visuals from list
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawWorld()
{
    METHOD_ENTRY("CVisualsManager::drawWorld")

    
    m_Graphics.beginRenderBatch("world");
    
    this->drawObjectsPlanetsAtmospheres(m_pCamera);
    this->drawObjects(m_pCamera);
    this->drawParticles(m_pCamera);
    
    m_Graphics.endRenderBatch();
    
    //--- Draw names (this is quite hacked and just a proof of concept) ------//
    
    if (m_nVisualisations & VISUALS_NAMES)
    {
        m_Graphics.setupScreenSpace();
        m_Graphics.beginRenderBatch("font");

        for (const auto pObj : *m_pDataStorage->getObjectsByValueFront())
        {
            if (m_pCamera->getZoom() * pObj.second->getGeometry()->getBoundingBox().getWidth() > 1.0)
            {
                Vector2d vecPosRel = CKinematicsState::clipToWorldLimit( 
                                    pObj.second->getCOM()-
                                    m_pCamera->getCenter()+
                                    IGridUser::cellToDouble
                                    (pObj.second->getCell()-
                                    m_pCamera->getCell()));
                
                // Now draw the text
                double fColor = m_pCamera->getZoom() * pObj.second->getGeometry()->getBoundingBox().getWidth() - 1.0;
                if (fColor > 1.0) fColor = 1.0;
                
                m_TextObjects.setColor({{1.0, 1.0, 1.0, fColor}});
                m_TextObjects.setText(pObj.second->getName());
                m_TextObjects.setPosition(m_Graphics.world2Screen(vecPosRel)[0], m_Graphics.world2Screen(vecPosRel)[1]);
                m_TextObjects.display();
            }
        }
        for (const auto Particle : *m_pDataStorage->getParticlesByValueFront())
        {
            if (m_pCamera->getZoom() * Particle.second->getBoundingBox().getWidth() > 1.0)
            {
                Vector2d vecPosRel = CKinematicsState::clipToWorldLimit( 
                                    Particle.second->getBoundingBox().getUpperRight()-
                                    m_pCamera->getCenter()+
                                    IGridUser::cellToDouble
                                    (Particle.second->getCell()-
                                    m_pCamera->getCell()));
                
                // Now draw the text
                double fColor = m_pCamera->getZoom() * Particle.second->getBoundingBox().getWidth() - 1.0;
                if (fColor > 1.0) fColor = 1.0;
                
                m_TextParticle.setColor({{1.0, 1.0, 1.0, fColor}});
                m_TextParticle.setText(Particle.second->getName());
                m_TextParticle.setPosition(m_Graphics.world2Screen(vecPosRel)[0], m_Graphics.world2Screen(vecPosRel)[1]);
                m_TextParticle.display();
            }
        }
        if ((1.0e9 * m_Graphics.getResPMX() < 1.0) &&
            (m_pDataStorage->getUniverse() != nullptr))
        {
            m_pDataStorage->getUniverse()->Access.acquireLock();
            
            for (auto i=0u; i<m_pDataStorage->getUniverse()->getStarSystems()->size(); ++i)
            {
                CStarSystem* pStarSystem = m_pDataStorage->getUniverse()->getStarSystems()->at(i);
                CStar&       Star(pStarSystem->Star());
                
                Vector2d vecPos = CKinematicsState::clipToWorldLimit(Star.getOrigin() +
                                  IGridUser::cellToDouble(pStarSystem->getCell()-m_pCamera->getCell()));
                if (m_pCamera->getBoundingBox().isInside(vecPos))
                {
                    Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(Star.getOrigin()-
                                        m_pCamera->getCenter()+
                                        IGridUser::cellToDouble
                                        (pStarSystem->getCell()-
                                          m_pCamera->getCell()));
                    
                    // Now draw the text
                    double fColor=0.1*Star.getStarType()+0.3;
                    m_TextStarSystems.setColor({{0.8, fColor, 0.3, 0.5}});
                    m_TextStarSystems.setText(Star.getName() + /*"\n" +*/
                                              std::to_string(Star.getRadius()));
                    m_TextStarSystems.setPosition(m_Graphics.world2Screen(vecPosRel)[0],m_Graphics.world2Screen(vecPosRel)[1]);
                    m_TextStarSystems.display();
                }
            }
            m_pDataStorage->getUniverse()->Access.releaseLock();
        }
        
        m_Graphics.endRenderBatch();
        m_Graphics.setupWorldSpace();
    }

//     for (auto i=0u; i<m_pDataStorage->getUniverse()->getStarSystems().size(); ++i)
//     {
//         if (m_nStarIndex == i)
//         {
//             std::mt19937 LocalGenerator;
//             std::normal_distribution<double>  OrbitDistribution(0, 1.0e12);
//             
//             LocalGenerator.seed(m_pDataStorage->getUniverse()->getStarSystems()[i]->getSeed());
//             
//             for (int j=0; j<m_pDataStorage->getUniverse()->getStarSystems()[i]->getNumberOfPlanets(); ++j)
//             {
//                 m_Graphics.setColor(0.2,0.2,0.5);
//                 m_Graphics.circle(m_pDataStorage->getUniverse()->getStarSystems()[i]->Star().getOrigin()-m_pCamera->getCenter()+
//                                     IGridUser::cellToDouble(
//                                         m_pDataStorage->getUniverse()->getStarSystems()[i]->getCell()-
//                                         m_pCamera->getCell()),
//                                     std::fabs(OrbitDistribution(LocalGenerator))
//                                     );
//                          std::cout << "Orbit with radius " << std::fabs(OrbitDistribution(LocalGenerator)) << std::endl;
//             }
//         }
//     }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates user interface based on UI mode 
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::updateUI()
{
    METHOD_ENTRY("CVisualsManager::updateUI")
    
    if (m_bMBLeft)
    {
        auto it = m_pVisualsDataStorage->getWindowUIDsInOrder()->rbegin();
        bool bDone = false;
        while (it != m_pVisualsDataStorage->getWindowUIDsInOrder()->rend() && !bDone)
        {
            CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(*it);
            
            // First, get focus of window
            if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::WIN))
            {
                if (it != m_pVisualsDataStorage->getWindowUIDsInOrder()->rbegin())
                {
                    m_pVisualsDataStorage->getWindowUIDsInOrder()->push_back(*it);
                    m_pVisualsDataStorage->getWindowUIDsInOrder()->erase(std::prev(it.base()));
                    it = m_pVisualsDataStorage->getWindowUIDsInOrder()->rbegin();
                    pWin = m_pVisualsDataStorage->getWindowByValue(*it);
                }
                bDone = true;
            }
                
            // Test, if cursor is in the area that closes the window
            if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::CLOSE))
            {
                m_pVisualsDataStorage->closeWindow(*it);
                m_bMBLeft = false; // Avoid focussing the underlying window
                bDone = true;
            }
            // Test, if cursor is in the area that resizes the window
            else if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::RESIZE))
            {
                if (pWin->getHeight()+m_nCursorY-m_nCursorY0 > 50 &&
                    pWin->getWidth()+m_nCursorX-m_nCursorX0 > 50)
                {
                    pWin->resize(pWin->getWidth() +m_nCursorX-m_nCursorX0,
                                pWin->getHeight()+m_nCursorY-m_nCursorY0);
                }
                bDone = true;
            }
            // Test, if mouse cursor is inside title of window. Cursor positon of
            // the previous frame must be used, since windows are not updated
            // yet.
            else if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::TITLE))
            {
                // Test for offset (position of cursor within windows).
                // Offset = 0 indicates, that no offset was calculated yet.
                if (m_nCursorOffsetX == 0 && m_nCursorOffsetY == 0)
                {
                    m_nCursorOffsetX = m_nCursorX0 - pWin->getPositionX();
                    m_nCursorOffsetY = m_nCursorY0 - pWin->getPositionY();
                }
                pWin->setPosition(m_nCursorX-m_nCursorOffsetX, m_nCursorY-m_nCursorOffsetY);
                bDone = true;
            }
            ++it;
        }
    }
    
    m_nCursorX0 = m_nCursorX;
    m_nCursorY0 = m_nCursorY;
    
    if (m_bCursor)
    {
        m_Graphics.beginRenderBatch("world");
        // 
            m_Graphics.setColor(1.0, 1.0, 1.0, 0.8);
            m_Graphics.beginLine(PolygonType::LINE_SINGLE);
                m_Graphics.addVertex(m_nCursorX-10, m_nCursorY);
                m_Graphics.addVertex(m_nCursorX-3, m_nCursorY);
                m_Graphics.addVertex(m_nCursorX, m_nCursorY-10);
                m_Graphics.addVertex(m_nCursorX, m_nCursorY-3);
                m_Graphics.addVertex(m_nCursorX+10, m_nCursorY);
                m_Graphics.addVertex(m_nCursorX+3, m_nCursorY);
                m_Graphics.addVertex(m_nCursorX, m_nCursorY+10);
                m_Graphics.addVertex(m_nCursorX, m_nCursorY+3);
            m_Graphics.endLine();
            if (m_bMBLeft)
                m_Graphics.circle(Vector2d(m_nCursorX, m_nCursorY), 5);
        
            m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        //
        m_Graphics.endRenderBatch();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Smoothes frame time, updating with given value
///
/// \param _pBuffer Buffer to use for frame time smoothing
/// \param _fUpdate New value to update Frametime
/// \param _nBufferSize Current size of buffer 
///
/// \return Smoothened value
///
////////////////////////////////////////////////////////////////////////////////
double CVisualsManager::smoothFrameTime(
                                CCircularBuffer<double>* const _pBuffer,
                                const double& _fUpdate,
                                const int _nBufferSize)
{
    METHOD_ENTRY("CVisualsManager::smoothFrameTime")
    
    if (_nBufferSize != int(_pBuffer->size()))
        _pBuffer->reserve(_nBufferSize);
    
    _pBuffer->push_back(_fUpdate);
    double fAvg = 0.0;
    for (auto i=0u; i<_pBuffer->size(); ++i)
    {
        fAvg += (*_pBuffer)[i];
    }
    return fAvg / _pBuffer->size();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise the command interface
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsManager::myInitComInterface()
{
    METHOD_ENTRY("CVisualsManager::myInitComInterface")

    INFO_MSG("Visuals Manager", "Initialising com interace.")
    
    // Register additional domain for initialisation
    m_pComInterface->registerWriterDomain("visuals_pre_init");
    
    std::ostringstream ossFrameStyle("");
    for (auto FrameStyle : STRING_TO_FRAME_STYLE_TYPE_MAP) ossFrameStyle << " " << FrameStyle.first;
    std::ostringstream ossWidgetType("");
    for (auto WidgetType : STRING_TO_WIDGET_TYPE_MAP) ossWidgetType << " " << WidgetType.first;
    
    m_strFont = "anka_c87_r";
    
    // Callback to log entry
    std::function<void(std::string, std::string, std::string, std::string)> Func =
        [&](const std::string& _strSrc, const std::string& _strMsg, const std::string& _strLev, const std::string& _strDom)
        {
            // Prevent recursive calls
            if (_strSrc != "Text" && _strSrc != "Font Manager")
            {
                // Create log window for warnings and errors
                if (_strLev == "LOG_LEVEL_ERROR" || _strLev == "LOG_LEVEL_WARNING") 
                {
                    CWidgetText* pWidget = new CWidgetText(&m_FontManager);
                    MEM_ALLOC("IWidget")
                    pWidget->setUIDVisuals(&m_UIDVisuals);
                    pWidget->Text.setText(_strSrc + ": " + _strMsg);
                    pWidget->Text.setFont(m_strFont);
                    m_pVisualsDataStorage->addWidget(pWidget);
                    
                    CWindow* pWin = new CWindow(&m_FontManager);
                    MEM_ALLOC("CWindow")

                    pWin->setUIDVisuals(&m_UIDVisuals);
                    m_pVisualsDataStorage->addWindow(pWin);
                    pWin->Title.setFont(m_strFont);
                    pWin->Title.setText(_strLev + " [" + _strDom + "]");
                    pWin->setWidget(pWidget);
                    pWin->setColorBG({{0.25, 0.0, 0.0, 0.8}}, WIN_INHERIT);
                    pWin->setColorFG({{0.5, 0.0, 0.0, 0.8}}, WIN_INHERIT);
                    pWin->center();
                    
                    m_pVisualsDataStorage->centerWindow(pWin, WIN_KEEP_CENTERED);
                }
            }
        };
    m_pComInterface->registerCallback("e_log_entry", Func, "visuals");
    
    // Callback to window resize
    std::function<void(double, double)> FuncResize =
        [&](const double& _fX, const double& _fY)
        {
            m_RenderTargetLights.init(std::uint16_t(_fX), std::uint16_t(_fY), m_unLightmapSubsampling);
            m_RenderTargetScene.init(std::uint16_t(_fX), std::uint16_t(_fY));
            m_RenderTargetScreen.init(std::uint16_t(_fX), std::uint16_t(_fY));
            if (m_pCamera != nullptr)
            {
                m_pCamera->setViewport(m_Graphics.getViewPort().rightplane - m_Graphics.getViewPort().leftplane,
                                       m_Graphics.getViewPort().topplane   - m_Graphics.getViewPort().bottomplane);
            }
            // Reposition all centered elements
            m_TextScale.setPosition(m_Graphics.getWidthScr()/2, 32.0f, TEXT_POSITION_CENTERED_X);
            m_TextVersion.setPosition(10, m_Graphics.getHeightScr()-m_TextVersion.getFontSize()-10);
            for (auto pWin : *m_pVisualsDataStorage->getWindowsCenteredByValue())
            {
                pWin.second->center();
            }
        };
    m_pComInterface->registerCallback("e_resize", FuncResize, "visuals");
    
    //----------------------------------------------------------------------
    // System package
    //----------------------------------------------------------------------
    m_pComInterface->registerFunction("toggle_fullscreen",
                                    CCommand<void>(
                                    [&]()
                                    {
                                        if (m_Graphics.getWindow() != nullptr)
                                        {
                                            if (m_bFullscreen)
                                            {
                                                m_Graphics.getWindow()->create(sf::VideoMode(m_nMainWinWidth, m_nMainWinHeight),"Planeworld", sf::Style::Default,
                                                sf::ContextSettings(24,8,4,4,2,sf::ContextSettings::Core));
                                            }
                                            else
                                            {
                                                m_nMainWinHeight = m_Graphics.getWindow()->getSize().y;
                                                m_nMainWinWidth = m_Graphics.getWindow()->getSize().x;
                                                m_Graphics.getWindow()->create(sf::VideoMode::getDesktopMode(),"Planeworld", sf::Style::Fullscreen,
                                                sf::ContextSettings(24,8,4,4,2,sf::ContextSettings::Core));
                                            }
                                            m_bFullscreen ^= 1;
                                            
                                            // New mode resets GL context, hence, graphics and render targets have to be updated
                                            m_Graphics.init();
                                            for (auto CamWidget : m_pVisualsDataStorage->getCameraWidgets())
                                            {
                                                CamWidget.second->getRenderTarget()->init(CamWidget.second->getWidth(), CamWidget.second->getHeight());
                                            }
                                            // New video mode triggers "e_resize" event which adjusts main render target and viewport
                                        }
                                        else
                                        {
                                            WARNING_MSG("Visuals Manager", "Cannot switch to fullscreen, no window created yet.")
                                        }
                                    }),
                                    "Toggle fullscreen.",
                                    {{ParameterType::NONE, "No return value"}},
                                    "system", "visuals"
    );
    
    m_pComInterface->registerFunction("cam_attach_to",
                                      CCommand<void, int, int>([&](const int _nUIDCam, const int _nUIDObj)
                                      {
                                          CCamera* pCam = m_pVisualsDataStorage->getCameraByValue(_nUIDCam);
                                          if (pCam != nullptr)
                                          {
                                            CObject* pObject = m_pDataStorage->getObjectByValueFront(_nUIDObj);
                                            if (pObject != nullptr)    
                                            {
                                                pCam->attachTo(pObject);
                                            }
                                          }
                                      }),
                                      "Hook camera on given object.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Camera UID"},
                                      {ParameterType::INT, "Object UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_cycle",
                                      CCommand<void>([&](){this->cycleCamera();}),
                                      "Cycle through registered cameras",
                                      {{ParameterType::NONE,"No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("cam_fit_object",
                                      CCommand<void, int>([&](const int _nUID)
                                      {
                                          UIDType nUIDCam = 0;
                                          if (m_pVisualsDataStorage->getCamerasByIndex().size() != 0)
                                          {
                                               nUIDCam = (m_pVisualsDataStorage->getCamerasByIndex()[m_unCameraIndex])->getUID();
                                          }
                                          if (nUIDCam != 0)
                                          {
                                                CCamera* pCam = m_pVisualsDataStorage->getCameraByValue(nUIDCam);
                                                if (pCam != nullptr)
                                                {
                                                    CObject* pObject = m_pDataStorage->getObjectByValueFront(_nUID);
                                                    if (pObject != nullptr)    
                                                    {
                                                        pCam->setCell(pObject->getCell());
                                                        if (pCam->getKinematicsState().getRef()->isValid())
                                                        {
                                                            pCam->setPosition(pObject->getKinematicsState().getOriginReferredTo(
                                                                                *(pCam->getKinematicsState().getRef()->get())) +
                                                                                pObject->getGeometry()->getCOM());
                                                        }
                                                        else
                                                        {
                                                            pCam->setPosition(pObject->getCOM());
                                                        }
                                                        double fCam = 1.0;
                                                        double fObj = 1.0;
                                                        if (pCam->getViewportWidth() > pCam->getViewportHeight())
                                                        {
                                                            fCam = pCam->getViewportHeight();
                                                        }
                                                        else
                                                        {
                                                            fCam = pCam->getViewportWidth();
                                                        }
                                                        fObj = std::sqrt(
                                                                    pObject->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getHeight()*
                                                                    pObject->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getHeight() + 
                                                                    pObject->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getWidth()*
                                                                    pObject->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getWidth());
                                                        pCam->zoomTo(fCam/fObj);
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                WARNING_MSG("Visuals manager", "No camera defined yet.")
                                            }
                                      }),
                                      "Adjust main camera for given object to fit viewport.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Object UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_get_position",
                                      CCommand<Vector2d>([&]() -> Vector2d{return m_pCamera->getCenter();}),
                                      "Get the global position of currently active camera",
                                      {{ParameterType::VEC2DDOUBLE, "Position (x, y)"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_get_resolution",
                                      CCommand<double, int>([&](const int _nUID) -> double
                                      {
                                          return m_Graphics.getResMPX();
                                          
                                      }),
                                      "Get the resolution of camera with given UID",
                                      {{ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Resolution in m/px"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_get_zoom",
                                      CCommand<double, int>([&](const int _nUID) -> double
                                      {
//                                           while (m_bCamCreation) {}
                                          if (_nUID != 0)
                                          {
                                            CCamera* pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                            if (pCam != nullptr)
                                            {
                                                return pCam->getZoom();
                                            }
                                          }
                                          return 1.0;
                                      }),
                                      "Returns zoom level of camera with given UID",
                                      {{ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Zoom level of camera"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_rotate_by",
                                      CCommand<void, int, double>([&](const int _nUID, const double& _fAngle)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->rotateBy(_fAngle);
                                          }
                                      }),
                                      "Rotate camera by given angle.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                      {ParameterType::DOUBLE, "Angle to rotate the camera by"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_position",
                                      CCommand<void, int, double, double>([&](const int _nUID,
                                                                              const double& _fX,
                                                                              const double& _fY)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->setPosition(_fX,_fY);
                                          }
                                      }),
                                      "Set position of currently active camera to given coordinates.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Position X"},
                                       {ParameterType::DOUBLE, "Position Y"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_resolution_mpx",
                                      CCommand<void, int, double>([&](const int _nUID, const double& _fR)
                                      {
                                          
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->zoomTo(m_Graphics.getResMPX()/_fR * pCam->getZoom());
                                          }
                                      }),
                                      "Set resolution of camera (m/px) with given UID.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Resolution in m/px"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_resolution_pxm",
                                      CCommand<void, int, double>([&](const int _nUID, const double& _fR)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->zoomTo(_fR / m_Graphics.getResPMX());
                                          }
                                      }),
                                      "Set resolution of camera (px/m) with given UID.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Resolution in px/m"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_translate_by",
                                      CCommand<void, int, double, double>([&](const int _nUID,
                                                                              const double& _fX,
                                                                              const double& _fY)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->translateBy(Vector2d(_fX,_fY));
                                          }
                                      }),
                                      "Translate camera by given vector.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "X component to translate the camera by"},
                                       {ParameterType::DOUBLE, "Y component to translate the camera by"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_zoom_by",
                                      CCommand<void, int, double>([&](const int _nUID, const double& _fZoom)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->zoomBy(_fZoom);
                                          }
                                      }),
                                      "Zooms camera with given UID by given level.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Level to zoom camera by"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("cam_zoom_to",
                                      CCommand<void, int, double>([&](const int _nUID, const double& _fZoom)
                                      {
                                          auto pCam = m_pVisualsDataStorage->getCameraByValue(_nUID);
                                          if (pCam != nullptr)
                                          {
                                            pCam->zoomTo(_fZoom);
                                          }
                                      }),
                                      "Zooms camera with given UID to given value.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Camera UID"},
                                       {ParameterType::DOUBLE, "Value to zoom camera to"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_backspace",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getComConsole()->backspace();}),
                                      "Removes last character from currently entered command.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_complement",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getComConsole()->complementCommand();}),
                                      "Complements the currently entered command (this is relevant for tab completion).",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_execute",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getComConsole()->execute();}),
                                      "Executes current command in com console.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_expand",
                                      CCommand<void, std::string>([&](const std::string& _strS){m_pVisualsDataStorage->getComConsole()->expandCommand(_strS);}),
                                      "Expands command string by given sequence.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::STRING, "String (char) for command expansion"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_next",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getComConsole()->nextCommand();}),
                                      "Get the next command in com console (this is mostly relevant for tab completion).",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_console_on",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getWindowByValue(m_ConsoleWindowID)->setVisibilty(true);}),
                                      "Switch command console on.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("com_console_off",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getWindowByValue(m_ConsoleWindowID)->setVisibilty(false);}),
                                      "Switch command console off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("com_console_prev",
                                      CCommand<void>([&](){m_pVisualsDataStorage->getComConsole()->prevCommand();}),
                                      "Get the previous command in com console (this is mostly relevant for tab completion).",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_set_mode",
                                      CCommand<void,std::string>([&](const std::string& _strS){m_pVisualsDataStorage->getComConsole()->setMode(mapStringToConsoleMode(_strS));}),
                                      "Sets mode for console command interpretation.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::STRING, "Mode (lua, raw)"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("com_set_font_size",
                                      CCommand<void,int>([&](const int _nSize){static_cast<CWidgetConsole*>(m_pVisualsDataStorage->getWidgetByValue(m_ConsoleWidgetID))->ConsoleText.setSize(_nSize);}),
                                      "Sets font size for command console.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Font size"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("create_camera",
                                      CCommand<int>([&]() -> int
                                      {
                                          return this->createCamera(CreationModeType::QUEUED);
                                      }),
                                      "Creates a default camera.",
                                      {{ParameterType::INT, "Camera UID"}},
                                      "system"
    );
    m_pComInterface->registerFunction("create_widget",
                                      CCommand<int, std::string>([&](const std::string& _strS) -> int
                                      {
                                          return this->createWidget(mapStringToWidgetType(_strS),
                                                                    CreationModeType::QUEUED);
                                      }),
                                      "Creates a widget of given type.",
                                      {{ParameterType::INT, "Widget UID"},
                                       {ParameterType::STRING, "Widget Type (" + ossWidgetType.str() + ")"}},
                                      "system"
    );
    m_pComInterface->registerFunction("create_window",
                                      CCommand<int>([&]() -> int
                                      {
                                          return this->createWindow(CreationModeType::QUEUED);
                                      }),
                                      "Creates generic window.",
                                      {{ParameterType::INT, "Window UID"}},
                                      "system"
    );
    m_pComInterface->registerFunction("get_main_camera",
                                      CCommand<int>([&]() -> int
                                      {
                                          if (m_pVisualsDataStorage->getCamerasByIndex().size() != 0)
                                          {
                                              return (m_pVisualsDataStorage->getCamerasByIndex()[m_unCameraIndex])->getUID();
                                          }
                                          else
                                          {
                                              return 0;
                                          }
                                      }),
                                      "Return UID of currently active camera on main screen.",
                                      {{ParameterType::INT, "Camera UID"}},
                                      "system"
    );
    m_pComInterface->registerFunction("get_screen_resolution",
                                      CCommand<Vector2i>([&]() -> Vector2i
                                      {
                                          return m_Graphics.getScreenRes();
                                      }),
                                      "Return screen resolution.",
                                      {{ParameterType::VEC2DINT, "Screen resolution (width, height)"}},
                                      "system"
    );
    m_pComInterface->registerFunction("mouse_cursor_on",
                                      CCommand<void>([&]() {m_bCursor = true;}),
                                      "Enables mouse cursor.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("mouse_cursor_off",
                                      CCommand<void>([&](){m_bCursor = false;}),
                                      "Disable mouse cursor.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("mouse_set_cursor",
                                      CCommand<void, int, int>([&](const int _nX,
                                                                   const int& _nY)
                                      {
                                          m_nCursorX = _nX;
                                          m_nCursorY = _nY;
                                      }),
                                      "Set position of the mouse cursor in screen space.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::INT, "Position X"},
                                       {ParameterType::INT, "Position Y"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("mouse_mbl_pressed",
                                      CCommand<void>([&](){m_bMBLeft = true;}),
                                      "Indicates that left mouse button was pressed.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("mouse_mbl_released",
                                      CCommand<void>([&]()
                                      {
                                          m_bMBLeft = false;
                                          m_nCursorOffsetX = 0;
                                          m_nCursorOffsetY = 0;
                                      }),
                                      "Indicates that left mouse button was released.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("set_data_path_visuals",
                                          CCommand<void, std::string>([&](const std::string& _strDataPath)
                                          {
                                              this->setDataPath(_strDataPath);
                                          }),
                                          "Sets the path where visuals data is located.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Location of visuals data (path)"}},
                                           "system", "visuals_pre_init");
    m_pComInterface->registerFunction("set_lightmap_subsampling",
                                          CCommand<void, int>([&](const int _nSS)
                                          {
                                              this->setLightMapSubsampling(_nSS);
                                          }),
                                          "Sets subsampling for lightmap texture.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Subsampling value for lightmap texture"}},
                                           "system", "visuals");
    m_pComInterface->registerFunction("set_frequency_visuals",
                                          CCommand<void, double>([&](const double& _fFrequency)
                                          {
                                              this->setFrequency(_fFrequency);
                                          }),
                                          "Sets the frequency of the visuals thread.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Frequency"}},
                                           "system", "visuals");
    m_pComInterface->registerFunction("uid_vis_hide",
                                      CCommand<void>([&](){m_UIDVisuals.hide();}),
                                      "Hide UIDs.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("uid_vis_show",
                                      CCommand<void>([&](){m_UIDVisuals.show();}),
                                      "Show UIDs.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("uid_vis_toggle",
                                      CCommand<void>([&](){m_UIDVisuals.toggle();}),
                                      "Toggle UIDs on/off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system", "visuals"
    );
//     m_pComInterface->registerFunction("widget_set_font_color",
//                                       CCommand<void,int,double,double,double,double>(
//                                           [&](const int _nUID,
//                                               const double _fR,
//                                               const double _fG,
//                                               const double _fB,
//                                               const double _fA)
//                                             {
//                                                 IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
//                                                 if (pWidget != nullptr)
//                                                 {
//                                                     pWidget->setFontColor({_fR, _fG, _fB, _fA}, WIN_INHERIT);
//                                                 }
//                                                 else
//                                                 {
//                                                     throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
//                                                 }
//                                             }),
//                                       "Sets color of widget text.",
//                                       {{ParameterType::NONE, "No return value"},
//                                       {ParameterType::INT, "Widget UID"},
//                                       {ParameterType::DOUBLE, "Color red (0.0-1.0)"},
//                                       {ParameterType::DOUBLE, "Color green (0.0-1.0)"},
//                                       {ParameterType::DOUBLE, "Color blue (0.0-1.0)"},
//                                       {ParameterType::DOUBLE, "Alpha (0.0-1.0)"}},
//                                       "system","visuals"
//     );
    m_pComInterface->registerFunction("widget_set_cam",
                                      CCommand<void, int, int>(
                                          [&](const int _nUID, const int _nUIDCam)
                                            {
                                                IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
                                                if (pWidget != nullptr)
                                                {
                                                    if (pWidget->getType() == WidgetTypeType::CAMERA)
                                                    {
                                                        CCamera* pCam = m_pVisualsDataStorage->getCameraByValue(_nUIDCam);
                                                        if (pCam != nullptr)
                                                        {
                                                            static_cast<CWidgetCam*>(pWidget)->setCamera(pCam);
                                                        }
                                                        else
                                                        {
                                                            throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("Visuals Manager", "Wrong widget type, unknown method <setRef>.")
                                                    }
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Attach camera to camera widget.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Widget UID"},
                                      {ParameterType::INT, "Camera UID"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("widget_set_frame_style",
                                      CCommand<void, int, std::string>(
                                          [&](const int _nUID, const std::string _strFrameStyle)
                                            {
                                                IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
                                                if (pWidget != nullptr)
                                                {
                                                    pWidget->setFrameStyle(mapStringToFrameStyleType(_strFrameStyle));
                                                }
                                            }),
                                      "Sets frame style of widget.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Widget UID"},
                                      {ParameterType::STRING, "Frame style  (" + ossFrameStyle.str() + ")"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("widget_set_text",
                                      CCommand<void, int, std::string>(
                                          [&](const int _nUID, const std::string _strText)
                                            {
                                                IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
                                                if (pWidget != nullptr)
                                                {
                                                    if (pWidget->getType() == WidgetTypeType::TEXT)
                                                    {
                                                        static_cast<CWidgetText*>(pWidget)->Text.setText(_strText);
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("Visuals Manager", "Wrong widget type, unknown method <setText>.")
                                                    }
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Set text of text widget.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Widget UID"},
                                      {ParameterType::STRING, "Widget text"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("widget_set_display_alpha",
                                      CCommand<void, int, double>(
                                          [&](const int _nUID, const double _fT)
                                            {
                                                IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
                                                if (pWidget != nullptr)
                                                {
                                                    if (pWidget->getType() == WidgetTypeType::CAMERA)
                                                    {
                                                        static_cast<CWidgetCam*>(pWidget)->setTransparency(_fT);
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("Visuals Manager", "Wrong widget type, unknown method <setTransparency>.")
                                                    }
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Set transparency of camera display.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Widget UID"},
                                      {ParameterType::DOUBLE, "Camera display transparency"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_frame_style",
                                      CCommand<void, int, std::string>(
                                          [&](const int _nUID, const std::string _strFrameStyle)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->setFrameStyle(mapStringToFrameStyleType(_strFrameStyle));
                                                }
                                            }),
                                      "Sets frame style of window.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::STRING, "Frame style  (" + ossFrameStyle.str() + ")"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_title",
                                      CCommand<void, int, std::string>(
                                          [&](const int _nUID, const std::string _strTitle)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->Title.setText(_strTitle);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Set title of window.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::STRING, "Window title"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_widget",
                                      CCommand<void, int, int>(
                                          [&](const int _nUIDWin, const int _nUIDWidget)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUIDWin);
                                                if (pWin != nullptr)
                                                {
                                                    IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUIDWidget);
                                                    if (pWidget != nullptr)
                                                    {
                                                        pWin->setWidget(pWidget);
                                                    }
                                                    else
                                                    {
                                                        throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                    }
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Sets widget for the given window.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::INT, "Widget UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("win_center",
                                      CCommand<void, int>(
                                          [&](const int _nUID)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    m_pVisualsDataStorage->centerWindow(pWin);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Center window referring to the main application.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("win_center_keep",
                                      CCommand<void, int>(
                                          [&](const int _nUID)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    m_pVisualsDataStorage->centerWindow(pWin, true);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Center window referring to the main application and keep centered if resized.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("win_center_release",
                                      CCommand<void, int>(
                                          [&](const int _nUID)
                                            {
                                                m_pVisualsDataStorage->releaseCenteredWindow(_nUID);
                                            }),
                                      "Release window from automatic centering.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("win_close",
                                      CCommand<void, int>(
                                          [&](const int _nUID)
                                            {
                                                if (_nUID != int(m_ConsoleWindowID))
                                                {
                                                    if (!m_pVisualsDataStorage->closeWindow(_nUID))
                                                    {
                                                        throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                    }
                                                }
                                                else
                                                {
                                                    NOTICE_MSG("Visuals Manager", "Deleting native console window is not allowed. Maybe hide the window.")
                                                }
                                            }),
                                      "Center window referring to the main application.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("win_hide_all",
                                      CCommand<void>([&]()
                                        {
                                            for (auto Win : *m_pVisualsDataStorage->getWindowsByValue())
                                            {
                                                if (Win.second->getUID() != m_ConsoleWindowID)
                                                    Win.second->setVisibilty(false);
                                            }
                                        }),
                                      "Hide all windows.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_main_resize_viewport",
                                      CCommand<void, double, double>([&](const double& _fX,
                                                                         const double& _fY)
                                      {
                                          m_Graphics.resizeViewport(int(_fX), int(_fY));
                                      }),
                                      "Resize graphics viewport based on given pixel resolution. This does not actually resize the window, "
                                      "but only the internal state. Use 'win_main_resize' to resize the window instead.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::DOUBLE, "Window width (x)"},
                                      {ParameterType::DOUBLE, "Window height (y)"}
                                      },
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("win_main_resize",
                                      CCommand<void, double, double>([&](const double& _fX,
                                                                         const double& _fY)
                                      {
                                          m_Graphics.resizeWindow(int(_fX), int(_fY));
                                      }),
                                      "Resize window to given size.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::DOUBLE, "Window width (x)"},
                                      {ParameterType::DOUBLE, "Window height (y)"}
                                      },
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("win_resize",
                                      CCommand<void,int,int,int>(
                                          [&](const int _nUID, const int _nWidth, const int _nHeight)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->resize(_nWidth, _nHeight);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Resize given window.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::INT, "Window width"},
                                      {ParameterType::INT, "Window height"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_bg_color",
                                      CCommand<void,int,double,double,double,double>(
                                          [&](const int _nUID,
                                              const double _fR,
                                              const double _fG,
                                              const double _fB,
                                              const double _fA)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->setColorBG({{_fR, _fG, _fB, _fA}}, WIN_INHERIT);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Sets color of console window background.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::DOUBLE, "Color red (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color green (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color blue (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Alpha (0.0-1.0)"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_fg_color",
                                      CCommand<void,int,double,double,double,double>(
                                          [&](const int _nUID,
                                              const double _fR,
                                              const double _fG,
                                              const double _fB,
                                              const double _fA)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->setColorFG({{_fR, _fG, _fB, _fA}}, WIN_INHERIT);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Sets color of console window foreground.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::DOUBLE, "Color red (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color green (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color blue (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Alpha (0.0-1.0)"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_font_color",
                                      CCommand<void,int,double,double,double,double>(
                                          [&](const int _nUID,
                                              const double _fR,
                                              const double _fG,
                                              const double _fB,
                                              const double _fA)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->Title.setColor({{_fR, _fG, _fB, _fA}});
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Sets color of window text.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::DOUBLE, "Color red (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color green (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color blue (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Alpha (0.0-1.0)"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_set_position",
                                      CCommand<void,int,int,int>(
                                          [&](const int _nUID, const int _nX, const int _nY)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->setPosition(_nX, _nY);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Set window position (top left corner coordinates).",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"},
                                      {ParameterType::INT, "Window position X (left)"},
                                      {ParameterType::INT, "Window position Y (top)"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_show_all",
                                      CCommand<void>([&]()
                                        {
                                            for (auto Win : *m_pVisualsDataStorage->getWindowsByValue())
                                            {
                                                if (Win.second->getUID() != m_ConsoleWindowID)
                                                    Win.second->setVisibilty(true);
                                            }
                                        }),
                                      "Shows all windows.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("win_toggle_focus",
                                      CCommand<void>(
                                          [&]()
                                            {
                                                m_pVisualsDataStorage->getWindowUIDsInOrder()->push_back(m_pVisualsDataStorage->getWindowUIDsInOrder()->front());
                                                m_pVisualsDataStorage->getWindowUIDsInOrder()->pop_front();
                                            }),
                                      "Center window referring to the main application.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Window UID"}},
                                      "system", "visuals"  
    );
    //----------------------------------------------------------------------
    // Visuals package
    //----------------------------------------------------------------------
    m_pComInterface->registerFunction("toggle_bboxes",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_OBJECT_BBOXES);}),
                                      "Toggle bounding boxes on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_com",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_OBJECT_COM);}),
                                      "Toggle center of mass (COM) on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_debug_info",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_DEBUG_INFO);}),
                                      "Toggle debug information (e.g. drawcalls) on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_debug_render",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_DEBUG_RENDER);}),
                                      "Toggle render debug information (scene, lightmap) on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_grid",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_UNIVERSE_GRID);}),
                                      "Toggle universe grid on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_kin_states",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_KINEMATICS_STATES);}),
                                      "Toggle kinematics states on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_names",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_NAMES);}),
                                      "Toggle objects names on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_timers",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_TIMERS);}),
                                      "Toggle timers on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
}
