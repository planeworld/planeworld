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

#include "debris.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::CVisualsManager() : m_pUniverse(nullptr),
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
                                     m_ConsoleWindowID(0)
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
/// \param _pObject Object, the polygon belongs to
/// \param _pCircle The circle to be drawn
/// \param _pCamera The camera to draw the polygon with
///
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawCircle(CObject* _pObject, CCircle* _pCircle, CCamera* _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawCircle")
    double   fRad      = _pCircle->getRadius();
    Vector2d vecCenter = _pCircle->getCenter() - _pCamera->getCenter() +
                         IGridUser::cellToDouble(_pObject->getCell() - _pCamera->getCell());

    if ((vecCenter.norm() <= fRad+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  fRad-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        PolygonType    LineT;
        
        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (std::isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = 2.0*M_PI;
            LineT = PolygonType::LINE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = PolygonType::LINE_STRIP;
        }

        double fInc = CIRCLE_DEFAULT_RESOLUTION * m_Graphics.getResMPX() / fRad; 
        
        if (fInc > 2.0*M_PI / CIRCLE_MINIMUM_SEGMENTS) fInc = 2.0*M_PI / CIRCLE_MINIMUM_SEGMENTS;
        fAngEnd += fInc;
        
        if (fAngEnd < fAng)
        {
            double fTmp = fAng;
            fAng = fAngEnd;
            fAngEnd = fTmp;
            // std::swap<double>(fAng, fAngEnd); // This doesn't work with VC++
        }
        
        m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);

            while ( fAng < fAngEnd)
            {
                m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*fRad,
                                              vecCenter[1]+std::sin(fAng)*fRad));
                fAng += fInc;
            }
        m_Graphics.endLine();
    }
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
            fAngEnd = 2.0*M_PI;
            LineT = PolygonType::LINE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
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
        m_Graphics.setWidth(3.0);
        m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);

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
        m_Graphics.setWidth(1.0);
        
        // Determine PolygonType
        if ((LineT == PolygonType::LINE_LOOP) && (WaterlineList.size() != 0))
          LineT = PolygonType::LINE_STRIP;
        
        // Push back potential last line segment
        if (WaterlineTmp.size() != 0)
        {
          WaterlineList.push_back(WaterlineTmp);
        }
        
        // Draw sea level
        m_Graphics.setWidth(2.0);
        m_Graphics.setColor(0.0,0.0,0.7);
        for (auto i=0u; i<WaterlineList.size(); ++i)
        {
          m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);
          for (auto j=0u; j<WaterlineList[i].size(); ++j)
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
/// \brief Draw all debris
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawDebris(CCamera* const _pCamera) const
{
    METHOD_ENTRY("CVisualsManager::drawDebris")
    
    for (auto Debris : *m_pDataStorage->getDebrisByValueFront())
    {
        switch (Debris.second->getDebrisType())
        {
            case DEBRIS_TYPE_DOT:
            {
                m_Graphics.dots((*Debris.second->getPositions()),-_pCamera->getCenter()+
                      IGridUser::cellToDouble(Debris.second->getCell() - _pCamera->getCell()));
                break;
            }
            case DEBRIS_TYPE_THRUST:
            {
                double fSizeR = 1.0 / Debris.second->getPositions()->size();
        
                if (m_Graphics.getResPMX() > 0.02)
                {
                    m_Graphics.cacheSinCos(12);
                    for (auto i=0u; i<Debris.second->getPositions()->size(); ++i)
                    {
                        if (_pCamera->getBoundingBox().isInside(Debris.second->getPositions()->at(i)))
                        {
                            m_Graphics.setColor(std::sqrt(fSizeR * i), fSizeR * i, fSizeR * i * 0.2, 0.05);
                            m_Graphics.filledCircle(Debris.second->getPositions()->at(i) - _pCamera->getCenter()+
                                            IGridUser::cellToDouble(Debris.second->getCell() - _pCamera->getCell()),
                                            (double(Debris.second->getPositions()->size()-i) * 0.01 + 3.0),
                                            12, GRAPHICS_CIRCLE_USE_CACHE
                            );
                        }
                    }
                    m_Graphics.setColor(1.0,1.0,1.0);
                }
                break;
            }
            default:
            {
                WARNING_MSG("Visuals Manager", "Wrong debris type, visualisation not implemented.")
                break;
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
/// \brief Draw center of mass (COM) of all objects
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawCOM() const
{
    METHOD_ENTRY("CVisualsManager::drawCOM")

    if (m_nVisualisations & VISUALS_OBJECT_COM)
    {
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
        m_Graphics.setColor(0.0, 1.0, 0.0, 0.8);
        m_Graphics.rect(m_pCamera->getBoundingBox().getLowerLeft()-
                        m_pCamera->getCenter(),
                        m_pCamera->getBoundingBox().getUpperRight()-
                        m_pCamera->getCenter());
        m_Graphics.setColor(0.0, 0.5, 0.0, 0.1);
        m_Graphics.filledRect(m_pCamera->getBoundingBox().getLowerLeft()-
                              m_pCamera->getCenter(),
                              m_pCamera->getBoundingBox().getUpperRight()-
                              m_pCamera->getCenter());
        m_Graphics.setColor(0.0, 1.0, 0.0, 0.8);
        m_Graphics.circle(m_pCamera->getCenter()-m_pCamera->getCenter(),
                          m_pCamera->getBoundingCircleRadius(),100.0);
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        m_Graphics.setDepth(GRAPHICS_DEPTH_DEFAULT);
        
        for (const auto pObj : *m_pDataStorage->getObjectsByValueFront())
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
        for (const auto Debris : *m_pDataStorage->getDebrisByValueFront())
        {
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
            m_Graphics.rect(Debris.second->getBoundingBox().getLowerLeft() - m_pCamera->getCenter(),
                            Debris.second->getBoundingBox().getUpperRight()- m_pCamera->getCenter());
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
            m_Graphics.filledRect(Debris.second->getBoundingBox().getLowerLeft() - m_pCamera->getCenter(),
                                  Debris.second->getBoundingBox().getUpperRight()- m_pCamera->getCenter());
        }
    }

    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
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
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawGrid() const
{
    METHOD_ENTRY("CVisualsManager::drawGrid")
    
    if (m_nVisualisations & VISUALS_UNIVERSE_GRID)
    {
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
            m_Graphics.beginLine(PolygonType::LINE_SINGLE,-15.3);
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
            m_Graphics.beginLine(PolygonType::LINE_SINGLE,-15.3);
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
        
        m_Graphics.setWidth(2.0);
        
        // Vertical grid lines
        while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
        {
            m_Graphics.beginLine(PolygonType::LINE_SINGLE,-15.2);
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
            m_Graphics.beginLine(PolygonType::LINE_SINGLE,-15.2);
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
        m_Graphics.setWidth(1.0);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws the universe grid HUD displaying the zoom level
///
/// Zoom level is displayed as lines where the number of lines represents the
/// power of the zoom level. Thus, three lines is 1.0e3 m. There are also
/// negative powers if lines are increasing to the left instead of right.
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawGridHUD() const
{
    METHOD_ENTRY("CVisualsManager::drawGridHU")

    if (m_nVisualisations & VISUALS_UNIVERSE_GRID)
    {
//         m_Graphics.setColor(1.0,1.0,1.0,0.6);
// 
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
        
        m_Graphics.getWindow()->pushGLStates();
        sf::Text Text;
        Text.setString(oss.str());
        Text.setFont(m_Font);
        Text.setCharacterSize(12);
        Text.setPosition(m_Graphics.getWidthScr()/2, 0.0f);
        m_Graphics.getWindow()->draw(Text);
        m_Graphics.getWindow()->popGLStates();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws information from a kinematics state
///
/// This method draws information from a kinematics state e.g. the local
/// coordinate system and references.
///
/// \param _KinematicsState kinematics State to be drawn
/// \param _fSize Size of coordinate system that is drawn
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawKinematicsState(const CKinematicsState& _KinematicsState,
                                          const double& _fSize) const
{
    METHOD_ENTRY("CVisualsManager::drawKinematicsState")
    
    if (_fSize * m_pCamera->getZoom() > 10.0)
    {
        double fTransparency = 0.5;
      
        m_Graphics.setColor(1.0, 1.0, 1.0, fTransparency);
        m_Graphics.showVec(
            _KinematicsState.getPosition(Vector2d(_fSize, 0.0))-
            _KinematicsState.getOrigin(),
            _KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin()
        );
        m_Graphics.showVec(
            _KinematicsState.getPosition(Vector2d(0.0, _fSize))-
            _KinematicsState.getOrigin(),
            _KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin()
        );
        
        // Now draw the text
        std::stringstream oss;
        
        sf::Text text;
        sf::Color color(255.0, 255.0, 255.0, 255.0 * fTransparency);
        oss << "Local angle:    " <<  _KinematicsState.getLocalAngle() << "\n";
        oss << "Local origin:   " <<  _KinematicsState.getLocalOrigin()[0] << ", "
                                  <<  _KinematicsState.getLocalOrigin()[1] << "\n";
        oss << "Angle:          " <<  _KinematicsState.getAngle() << "\n";
        oss << "Origin:         " <<  _KinematicsState.getOrigin()[0] << ", "
                                  <<  _KinematicsState.getOrigin()[1] << "\n";
        text.setString(oss.str());
        text.setFont(m_Font);
        text.setCharacterSize(12.0);
        text.setFillColor(color);
        text.setPosition(m_Graphics.world2Screen(_KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin())[0],
                        m_Graphics.world2Screen(_KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin())[1]);

        m_Graphics.getWindow()->pushGLStates();
        m_Graphics.getWindow()->draw(text);
        m_Graphics.getWindow()->popGLStates();
        
        if (_KinematicsState.gotReference())
        {
            Vector2d vecRef = _KinematicsState.getRef()->getOrigin() - _KinematicsState.getOrigin();
            m_Graphics.showVec(
                vecRef,
                _KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin()
            );
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
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawKinematicsStates() const
{
    METHOD_ENTRY("CVisualsManager::drawKinematicsStates")
    
    if (m_nVisualisations & VISUALS_KINEMATICS_STATES)
    {
        for (const auto pObj : *m_pDataStorage->getObjectsByValueFront())
        {
            this->drawKinematicsState(
                pObj.second->getKinematicsState(),
                pObj.second->getGeometry()->getBoundingBox().getHeight() +
                pObj.second->getGeometry()->getBoundingBox().getWidth() * 0.5 * 0.33
            );
        }
        this->drawKinematicsState(m_pCamera->getKinematicsState(),m_pCamera->getBoundingCircleRadius() * 0.1);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws global and local simulation timers
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawTimers() const
{
    METHOD_ENTRY("CVisualsManager::drawTimers")
    
    if (m_nVisualisations & VISUALS_TIMERS)
    {
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
        
        /// \todo Check: Somehow showing text increases framerate when activating the first time
        
        m_Graphics.getWindow()->pushGLStates();
        sf::Text Text;
        if (m_pDataStorage->getTimeScale() > 1.0)
        {
            Text.setFillColor(sf::Color(255.0, 0.0, 0.0, 255.0));
            oss << "\nWarning: Decreasing accuracy." << std::endl;
        }
        Text.setString(oss.str());
        Text.setFont(m_Font);
        Text.setCharacterSize(12);
        Text.setPosition(0.0f, 0.0f);
        m_Graphics.getWindow()->draw(Text);
        m_Graphics.getWindow()->popGLStates();
    }
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
            m_Graphics.beginLine(PolygonType::LINE_STRIP, -15.0);
            
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
/// \brief Draws all visuals from list
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawWorld() const
{
    METHOD_ENTRY("CVisualsManager::drawWorld")
    
    //const double fRmax = 3.6e20;
    //const double fBGDensityFactor = 1.0e18; 
    
    //const double fStarfieldSizeX = 3.0e19; // * 2
    //const double fStarfieldSizeY = 3.0e19; // * 2
    
    //const double fStarfieldBackgroundSizeX = 0.5 * fStarfieldSizeX;
    //const double fStarfieldBackgroundSizeY = 0.5 * fStarfieldSizeY;
    
    //const double fScreenWidthAtZoomZeroInM  = m_Graphics.getWidthScr()/GRAPHICS_PX_PER_METER;
    //const double fScreenHeightAtZoomZeroInM = m_Graphics.getHeightScr()/GRAPHICS_PX_PER_METER;
    
//     const double fDefaultUnitSize = GRAPHICS_RIGHT_DEFAULT - GRAPHICS_LEFT_DEFAULT;
//     const double fMaxZoom = fDefaultUnitSize / (2.0*fStarfieldSizeX);
//     m_pCamera->zoomTo(fMaxZoom);
    
//     if (1.0e13 * m_Graphics.getResPMX() < 1.0)
    {
        for (auto i=0u; i<m_pUniverse->getStarSystems().size(); ++i)
        {
            Vector2d vecPos = CKinematicsState::clipToWorldLimit(
                                m_pUniverse->getStarSystems()[i]->Star().getOrigin() +
                                IGridUser::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell())
                              );
            Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(
                                    m_pUniverse->getStarSystems()[i]->Star().getOrigin() - m_pCamera->getCenter() +
                                     IGridUser::cellToDouble
                                     (m_pUniverse->getStarSystems()[i]->getCell() -
                                      m_pCamera->getCell())
                                );
            

            // Draw stars in original scale
            if (m_pCamera->getBoundingBox().isInside(vecPos))
            {
                
                double fColor = 0.1*m_pUniverse->getStarSystems()[i]->Star().getStarType()+0.3;
                m_Graphics.setColor(0.8,fColor,0.3);
                
                double fDrawSize = (m_pUniverse->getStarSystems()[i]->Star().getStarType()*0.3+1) * m_Graphics.getResMPX();
                double fRadius   = m_pUniverse->getStarSystems()[i]->Star().getRadius();
                if (fDrawSize > fRadius)
                    m_Graphics.filledCircle(vecPosRel, fDrawSize, 7.0);
                else
                    m_Graphics.filledCircle(vecPosRel, (m_pUniverse->getStarSystems()[i]->Star().getRadius()), 100.0);
            }
//             // Draw stars in reduced scale for background
//             if (m_pCamera->getBoundingBox().isInside(1.0/fBGDensityFactor*(vecPosRel-Vector2d(fStarfieldSizeX*0.5, fStarfieldSizeY*0.5)) + m_pCamera->getCenter()+IGridUser::cellToDouble(m_pCamera->getCell())))
//             {
//                 
//                 double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
//                 m_Graphics.setColor(0.3,0.3,fColor);
//                 m_Graphics.setPointSize(m_pUniverse->getStarSystems()[i]->getStarType());
//                 
//                 m_Graphics.dot(1.0/m_pCamera->getZoom()/fBGDensityFactor*(vecPosRel-Vector2d(fStarfieldSizeX*0.5, fStarfieldSizeY*0.5)));
// 
//             }
//             
        }
    }
//     else
//     {
//         for (int i=0; i<m_pUniverse->getStarSystems().size(); ++i)
//         {
//             if (m_nStarIndex != i)
//             {
//                 if ((m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell()).cast<double>().norm() < 200.1)
//                 {
//                     double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
//                     m_pUniverse->m_pStar->setName(m_pUniverse->getStarSystems()[i]->getName());
//                     m_pUniverse->m_pStar->setCell(m_pUniverse->getStarSystems()[i]->getCell());
//                     m_pUniverse->m_pStar->setOrigin(m_pUniverse->getStarSystems()[i]->getCenter());
//                     static_cast<CCircle*>(m_pUniverse->m_pStar->getGeometry()->getShapes()->front()->getShapeCur())->setRadius(
//                         double(m_pUniverse->getStarSystems()[i]->getStarType()+1)*1.0e9
//                     );
//                     m_pUniverse->m_pStar->getGeometry()->getShapes()->front()->updateBuffer();
//                     m_pUniverse->m_pStar->init();
//                     m_pUniverse->m_pStar->setVelocity(Vector2d(3.0e9,0.0));
//                     m_nStarIndex = i;
//                     
//                     std::mt19937 LocalGenerator;
//                     std::normal_distribution<double>  OrbitDistribution(0, 1.0e12);
// 
//                     LocalGenerator.seed(m_pUniverse->getStarSystems()[i]->getSeed());
//                     
//                     std::cout << m_pUniverse->m_pStar->getName() << std::endl;
//                     for (int j=0; j<m_pUniverse->getStarSystems()[i]->getNumberOfPlanets(); ++j)
//                     {
//                         m_Graphics.circle(m_pUniverse->getStarSystems()[i]->getCenter()-m_pCamera->getCenter()+
//                                           IGridUser::cellToDouble(
//                                               m_pUniverse->getStarSystems()[i]->getCell()-
//                                               m_pCamera->getCell()),
//                                           std::fabs(OrbitDistribution(LocalGenerator))
//                                          );
// //                          std::cout << "Orbit with radius " << std::fabs(OrbitDistribution(LocalGenerator)) << std::endl;
//                     }
//                 }
//             }
//             else
//             {
//                 m_pUniverse->getStarSystems()[i]->setCell(m_pUniverse->m_pStar->getCell());
//                 m_pUniverse->getStarSystems()[i]->setCenter(m_pUniverse->m_pStar->getCOM());
//             }
//         }
//     }
    m_Graphics.setPointSize(1.0);
    
    //--- Draw names (this is quite hacked and just a proof of concept) ------//
    
    if (m_nVisualisations & VISUALS_NAMES)
    {
        int nTextSize = 16;
                
        m_Graphics.getWindow()->pushGLStates();    
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
                sf::Text text;
                double fColor = (m_pCamera->getZoom() * pObj.second->getGeometry()->getBoundingBox().getWidth() - 1.0) * 255.0;
                if (fColor > 255.0) fColor = 255.0;
                sf::Color color(255.0,255.0,255.0, fColor);
                
                text.setString(pObj.second->getName());
                text.setFont(m_Font);
                text.setCharacterSize(nTextSize);
                text.setFillColor(color);
                text.setPosition(m_Graphics.world2Screen(vecPosRel)[0], m_Graphics.world2Screen(vecPosRel)[1]);

                m_Graphics.getWindow()->draw(text);
            }
        }
        for (const auto Debris : *m_pDataStorage->getDebrisByValueFront())
        {
            if (m_pCamera->getZoom() * Debris.second->getBoundingBox().getWidth() > 1.0)
            {
                Vector2d vecPosRel = CKinematicsState::clipToWorldLimit( 
                                    Debris.second->getBoundingBox().getUpperRight()-
                                    m_pCamera->getCenter()+
                                    IGridUser::cellToDouble
                                    (Debris.second->getCell()-
                                    m_pCamera->getCell()));
                
                // Now draw the text
                sf::Text text;
                double fColor = (m_pCamera->getZoom() * Debris.second->getBoundingBox().getWidth() - 1.0) * 255.0;
                if (fColor > 255.0) fColor = 255.0;
                sf::Color color(255.0,255.0,255.0, fColor);
                
                text.setString(Debris.second->getName());
                text.setFont(m_Font);
                text.setCharacterSize(nTextSize);
                text.setFillColor(color);
                text.setPosition(m_Graphics.world2Screen(vecPosRel)[0], m_Graphics.world2Screen(vecPosRel)[1]);

                m_Graphics.getWindow()->draw(text);
            }
        }
        if (1.0e9 * m_Graphics.getResPMX() < 1.0)
        {
            for (auto i=0u; i<m_pUniverse->getStarSystems().size(); ++i)
            {
                Vector2d vecPos = CKinematicsState::clipToWorldLimit(m_pUniverse->getStarSystems()[i]->Star().getOrigin() +
                                  IGridUser::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell()));
                if (m_pCamera->getBoundingBox().isInside(vecPos))
                {
                    Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(m_pUniverse->getStarSystems()[i]->Star().getOrigin()-
                                        m_pCamera->getCenter()+
                                        IGridUser::cellToDouble
                                        (m_pUniverse->getStarSystems()[i]->getCell()-
                                          m_pCamera->getCell()));
                    
                    // Now draw the text
                    sf::Text text;
                    double fColor=0.1*m_pUniverse->getStarSystems()[i]->Star().getStarType()+0.3;
                    sf::Color color(0.8*255, fColor*255, 0.3*255);
                    
                    text.setString(m_pUniverse->getStarSystems()[i]->Star().getName() + "\n" +
                                   std::to_string(m_pUniverse->getStarSystems()[i]->Star().getRadius()));
                    text.setFont(m_Font);
                    text.setCharacterSize(nTextSize);
                    text.setFillColor(color);
                    text.setPosition(m_Graphics.world2Screen(vecPosRel)[0],m_Graphics.world2Screen(vecPosRel)[1]);

                    m_Graphics.getWindow()->draw(text);
                }
            }
        }
        
        m_Graphics.getWindow()->popGLStates();
    }
    
    this->drawObjects(m_pCamera);
    this->drawDebris(m_pCamera);

    for (auto i=0u; i<m_pUniverse->getStarSystems().size(); ++i)
    {
        if (m_nStarIndex == i)
        {
            std::mt19937 LocalGenerator;
            std::normal_distribution<double>  OrbitDistribution(0, 1.0e12);
            
            LocalGenerator.seed(m_pUniverse->getStarSystems()[i]->getSeed());
            
            for (int j=0; j<m_pUniverse->getStarSystems()[i]->getNumberOfPlanets(); ++j)
            {
                m_Graphics.setColor(0.2,0.2,0.5);
                m_Graphics.circle(m_pUniverse->getStarSystems()[i]->Star().getOrigin()-m_pCamera->getCenter()+
                                    IGridUser::cellToDouble(
                                        m_pUniverse->getStarSystems()[i]->getCell()-
                                        m_pCamera->getCell()),
                                    std::fabs(OrbitDistribution(LocalGenerator))
                                    );
                         std::cout << "Orbit with radius " << std::fabs(OrbitDistribution(LocalGenerator)) << std::endl;
            }
        }
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
    
    // When calling init, all relevant variables have to be set up. Thus, sub 
    // components like the visuals data storage kann be set up, eventually.
    m_pVisualsDataStorage->setFont(m_Font);
    m_pVisualsDataStorage->setComInterface(m_pComInterface);
    
    m_ConsoleWidgetID = m_pVisualsDataStorage->createWidget(WidgetTypeType::CONSOLE);
    CWidgetConsole* pConsoleWidget = static_cast<CWidgetConsole*>(m_pVisualsDataStorage->getWidgetByValue(m_ConsoleWidgetID));
    pConsoleWidget->setFont(&m_Font);
    pConsoleWidget->setFontSize(16);
    pConsoleWidget->setFontColor({0.0, 1.0, 0.0, 1.0}, WIN_INHERIT);
    pConsoleWidget->setComConsole(m_pVisualsDataStorage->getComConsole());
    
    m_ConsoleWindowID = m_pVisualsDataStorage->createWindow();
    CWindow* pConsoleWindow  = m_pVisualsDataStorage->getWindowByValue(m_ConsoleWindowID);
    pConsoleWindow->setTitle("Command console");
    pConsoleWindow->setFont(&m_Font);
    pConsoleWindow->setFontSize(20);
    pConsoleWindow->setFontColor({1.0, 1.0, 1.0, 1.0}, WIN_NO_INHERIT);
    pConsoleWindow->setWidget(pConsoleWidget);
    pConsoleWindow->setColorBG({0.1, 0.1, 0.1, 0.75}, WIN_INHERIT);
    pConsoleWindow->setColorFG({0.3, 0.3, 0.3, 0.75}, WIN_INHERIT);
    pConsoleWindow->setPosition(10, 10);
    pConsoleWindow->resize(800, 150);
    pConsoleWindow->setVisibilty(false);
    pConsoleWindow->setClosability(false);
    
    return (m_Graphics.init());
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
    DEBUG(Log.setLoglevel(LOG_LEVEL_NOTICE);)
    m_pDataStorage->swapFront();
    DEBUG(Log.setLoglevel(LOG_LEVEL_DEBUG);)

    // Attach camera to current front buffer (at the moment, this is needed for the kinematics state)
    if (m_pCamera->gotRef())
        m_pCamera->attachTo(m_pDataStorage->getObjectsByValueFront()->at(m_pCamera->getUIDRef()));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Processes one visual frame
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::processFrame()
{
    METHOD_ENTRY("CVisualsManager::processFrame")

    m_Graphics.switchToWorldSpace();
    
    m_pVisualsDataStorage->addWidgetsFromQueue();
    m_pVisualsDataStorage->addWindowsFromQueue();
    m_pComInterface->callWriters("visuals");
    
    m_pCamera = m_pVisualsDataStorage->getCamerasByIndex().operator[](m_unCameraIndex);
    m_pCamera->update();
    
    this->drawGrid();
    this->drawTrajectories();
    this->drawWorld();
    this->drawKinematicsStates();
    this->drawCOM();
    this->drawBoundingBoxes();
    
    m_Graphics.switchToScreenSpace();
    
    this->drawGridHUD();
    this->drawTimers();
    this->drawWindows();
    this->updateUI();
    
    this->finishFrame();
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
/// \brief Updates user interface based on UI mode 
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::updateUI()
{
    METHOD_ENTRY("CVisualsManager::updateUI")

    if (m_bMBLeft)
    {
        auto it = m_pVisualsDataStorage->getWindowUIDsInOrder()->rbegin();
        while (it != m_pVisualsDataStorage->getWindowUIDsInOrder()->rend())
        {
            CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(*it);
                
            // Test, if cursor is in the area that closes the window
            if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::CLOSE))
            {
                m_pVisualsDataStorage->closeWindow(*it);
                m_bMBLeft = false; // Avoid focussing the underlying window
                break;
            }
            // Test, if cursor is in the area that resizes the window
            else if (pWin->isInside(m_nCursorX0, m_nCursorY0, WinAreaType::RESIZE))
            {
                pWin->resize(pWin->getWidth() +m_nCursorX-m_nCursorX0,
                             pWin->getHeight()+m_nCursorY-m_nCursorY0);
                break;
            }
            // Test, if mouse cursor is inside of window. Cursor positon of
            // the previous frame must be used, since windows are not updated
            // yet.
            else if (pWin->isInside(m_nCursorX0, m_nCursorY0))
            {
                // Test for offset (position of cursor within windows).
                // Offset = 0 indicates, that no offset was calculated yet.
                if (m_nCursorOffsetX == 0 && m_nCursorOffsetY == 0)
                {
                    m_nCursorOffsetX = m_nCursorX0 - pWin->getPositionX();
                    m_nCursorOffsetY = m_nCursorY0 - pWin->getPositionY();
                }
                
                pWin->setPosition(m_nCursorX-m_nCursorOffsetX, m_nCursorY-m_nCursorOffsetY);
                
                if (it != m_pVisualsDataStorage->getWindowUIDsInOrder()->rbegin())
                {
                    m_pVisualsDataStorage->getWindowUIDsInOrder()->push_back(*it);
                    m_pVisualsDataStorage->getWindowUIDsInOrder()->erase(std::prev(it.base()));
                }
                break;
            }
            ++it;
        }
    }
    
    m_nCursorX0 = m_nCursorX;
    m_nCursorY0 = m_nCursorY;
    
    if (m_bCursor)
    {
        m_Graphics.setColor(1.0, 1.0, 1.0, 0.8);
        m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
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
    }
    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
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
    
    std::ostringstream ossWidgetType("");
    for (auto WidgetType : STRING_TO_WIDGET_TYPE_MAP) ossWidgetType << " " << WidgetType.first;
    
    //----------------------------------------------------------------------
    // System package
    //----------------------------------------------------------------------
    m_pComInterface->registerFunction("cam_attach_to",
                                      CCommand<void, int>([&](const int _nUID)
                                      {
                                          auto it = m_pDataStorage->getObjectsByValueFront()->find(_nUID);
                                          if (it != m_pDataStorage->getObjectsByValueFront()->end())
                                          {
                                                m_pCamera->attachTo(it->second);
                                          }
                                          else
                                          {
                                              WARNING_MSG("Visuals Manager", "Unknown object with UID <" << _nUID << ">")
                                          }
                                      }),
                                      "Hook camera on given object.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Object UID"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_cycle",
                                      CCommand<void>([&](){this->cycleCamera();}),
                                      "Cycle through registered cameras",
                                      {{ParameterType::NONE,"No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("cam_get_current",
                                      CCommand<CCamera*>([&](){return this->getCurrentCamera();}),
                                      "Returns pointer to active camera",
                                      {{ParameterType::UNDEFINED, "CCamera*, Currently active camera"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_get_position",
                                      CCommand<Vector2d>([&]() -> Vector2d{return m_pCamera->getCenter();}),
                                      "Get the global position of currently active camera",
                                      {{ParameterType::VEC2DDOUBLE, "Position (x, y)"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_get_resolution",
                                      CCommand<double>([&](){return m_Graphics.getResMPX();}),
                                      "Get the resolution of currently active camera",
                                      {{ParameterType::DOUBLE, "Resolution in m/px"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_get_zoom",
                                      CCommand<double>([&](){return m_pCamera->getZoom();}),
                                      "Returns zoom level of active camera",
                                      {{ParameterType::DOUBLE, "Zoom level of active camera"}},
                                      "system"
    );
    m_pComInterface->registerFunction("cam_rotate_by",
                                      CCommand<void, double>([&](const double& _fAngle){m_pCamera->rotateBy(_fAngle);}),
                                      "Rotate camera by given angle.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::DOUBLE, "Angle to rotate the camera by"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_position",
                                      CCommand<void, double, double>([&](const double& _fX,
                                                                         const double& _fY)
                                      {m_pCamera->setPosition(_fX,_fY);}),
                                      "Set position of currently active camera to given coordinates.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::DOUBLE, "Position X"},
                                       {ParameterType::DOUBLE, "Position Y"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_resolution_mpx",
                                      CCommand<void, double>([&](const double& _fR)
                                      {m_pCamera->zoomTo(m_Graphics.getResMPX()/_fR * m_pCamera->getZoom());}),
                                      "Set resolution of currently active camera (m/px).",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::DOUBLE, "Resolution in m/px"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_set_resolution_pxm",
                                      CCommand<void, double>([&](const double& _fR)
                                      {m_pCamera->zoomTo(_fR / m_Graphics.getResPMX() * m_pCamera->getZoom());}),
                                      "Set resolution of currently active camera (px/m).",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::DOUBLE, "Resolution in px/m"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_translate_by",
                                      CCommand<void, double, double>([&](const double& _fX,
                                                                         const double& _fY)
                                      {m_pCamera->translateBy(Vector2d(_fX,_fY));}),
                                      "Translate camera by given vector.",
                                      {{ParameterType::NONE, "No return value"},
                                       {ParameterType::DOUBLE, "X component to translate the camera by"},
                                       {ParameterType::DOUBLE, "Y component to translate the camera by"}},
                                      "system", "visuals"  
    );
    m_pComInterface->registerFunction("cam_zoom_by",
                                      CCommand<void,double>([&](const double& _fZoom){m_pCamera->zoomBy(_fZoom);}),
                                      "Zooms active camera by given level.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::DOUBLE, "Level to zoom active camera by"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("cam_zoom_to",
                                      CCommand<void,double>([&](const double& _fZoom){m_pCamera->zoomTo(_fZoom);}),
                                      "Zooms active camera to given level.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::DOUBLE, "Level to zoom active camera to"}},
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
    m_pComInterface->registerFunction("com_console_get_current",
                                      CCommand<std::string>([&]() -> std::string {return m_pVisualsDataStorage->getComConsole()->getCurrentCommand();}),
                                      "Returns current command in com console.",
                                      {{ParameterType::STRING, "Current command in console"}},
                                      "system"
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
                                      CCommand<void,int>([&](const int _nSize){static_cast<CWidgetConsole*>(m_pVisualsDataStorage->getWidgetByValue(m_ConsoleWidgetID))->setFontSize(_nSize);}),
                                      "Sets font size for command console.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Font size"}},
                                      "system","visuals"
    );
    m_pComInterface->registerFunction("create_widget",
                                      CCommand<int, std::string>([&](const std::string& _strS) -> int
                                      {
                                          return m_pVisualsDataStorage->createWidget(mapStringToWidgetType(_strS),
                                                                                     CreationModeType::QUEUED);
                                      }),
                                      "Creates a widget of given type.",
                                      {{ParameterType::INT, "Window UID"},
                                       {ParameterType::STRING, "Widget Type (" + ossWidgetType.str() + ")"}},
                                      "system"
    );
    m_pComInterface->registerFunction("create_window",
                                      CCommand<int>([&]() -> int
                                      {
                                          return m_pVisualsDataStorage->createWindow(CreationModeType::QUEUED);
                                      }),
                                      "Creates generic window.",
                                      {{ParameterType::INT, "Window UID"}},
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
    m_pComInterface->registerFunction("widget_set_font_color",
                                      CCommand<void,int,double,double,double,double>(
                                          [&](const int _nUID,
                                              const double _fR,
                                              const double _fG,
                                              const double _fB,
                                              const double _fA)
                                            {
                                                IWidget* pWidget = m_pVisualsDataStorage->getWidgetByValue(_nUID);
                                                if (pWidget != nullptr)
                                                {
                                                    pWidget->setFontColor({_fR, _fG, _fB, _fA}, WIN_INHERIT);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                      "Sets color of widget text.",
                                      {{ParameterType::NONE, "No return value"},
                                      {ParameterType::INT, "Widget UID"},
                                      {ParameterType::DOUBLE, "Color red (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color green (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Color blue (0.0-1.0)"},
                                      {ParameterType::DOUBLE, "Alpha (0.0-1.0)"}},
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
                                                        static_cast<CWidgetText*>(pWidget)->setText(_strText);
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
    m_pComInterface->registerFunction("win_set_title",
                                      CCommand<void, int, std::string>(
                                          [&](const int _nUID, const std::string _strTitle)
                                            {
                                                CWindow* pWin = m_pVisualsDataStorage->getWindowByValue(_nUID);
                                                if (pWin != nullptr)
                                                {
                                                    pWin->setTitle(_strTitle);
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
                                                    pWin->center();
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
    m_pComInterface->registerFunction("win_close",
                                      CCommand<void, int>(
                                          [&](const int _nUID)
                                            {
                                                if (_nUID != m_ConsoleWindowID)
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
    m_pComInterface->registerFunction("win_main_resize",
                                      CCommand<void, double, double>([=](const double& _fX,
                                                                         const double& _fY)
                                      {
                                          m_Graphics.resizeWindow(int(_fX), int(_fY));
                                          if (m_pCamera != nullptr)
                                          {
                                                m_pCamera->setViewport(m_Graphics.getViewPort().rightplane - m_Graphics.getViewPort().leftplane - 20.0,
                                                                       m_Graphics.getViewPort().topplane   - m_Graphics.getViewPort().bottomplane - 20.0);
                                          }
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
                                                    pWin->setColorBG({_fR, _fG, _fB, _fA}, WIN_INHERIT);
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
                                                    pWin->setColorFG({_fR, _fG, _fB, _fA}, WIN_INHERIT);
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
                                                    pWin->setFontColor({_fR, _fG, _fB, _fA}, WIN_INHERIT);
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
