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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::CVisualsManager() : m_pUniverse(nullptr),
                                     m_pPhysicsManager(nullptr),
                                     m_fFrequency(VISUALS_DEFAULT_FREQUENCY),
                                     m_nVisualisations(0),
                                     m_nStarIndex(-1),
                                     m_unCameraIndex(0u),
                                     m_pCamera(nullptr),
                                     m_pComConsole(nullptr),
                                     m_bConsoleMode(false)
{
    METHOD_ENTRY("CVisualsManager::CVisualsManager")
    CTOR_CALL("CVisualsManager::CVisualsManager")
    
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
    Vector2d vecCenter = _pCircle->getCenter() - _pCamera->getCenter()/* +
                         IUniverseScaled::cellToDouble(_pObject->getCell() - _pCamera->getCell())*/;

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
                         (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2;
    
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
                (_pObject->getCell() - _pCamera->getCell()).cast<double>() * DEFAULT_CELL_SIZE_2);
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
                m_Graphics.dot(Obj.second->getCOM()-_pCamera->getCenter());
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
            
    //         m_Graphics.circle(m_pRef->getCOM() - _pCamera->getCenter(), 0.6);
    //         m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
    //         m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(-0.6, 0.0)) -_pCamera->getCenter());
    //         m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(+0.6, 0.0)) -_pCamera->getCenter());
    //         m_Graphics.endLine();
    //         m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
    //         m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(0.0, -0.6)) -_pCamera->getCenter());
    //         m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(0.0, +0.6)) -_pCamera->getCenter());
    //         m_Graphics.endLine();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws console if console mode is active
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawConsole() const
{
    METHOD_ENTRY("CVisualsManager::drawConsole")
    
    int nTextSize = 12;
    int nComHistory = 5;
    int nWindowBorderLeft = 10;
    int nWindowBorderTop = 10;
    int nWindowHeight = (m_Font.getLineSpacing(nTextSize))*(nComHistory+1) + nWindowBorderLeft;
    int nWindowWidth = 300;
    
    if (m_bConsoleMode)
    {
        m_Graphics.setColor(1.0, 0.0, 0.0, 0.8);
        m_Graphics.rectSS(Vector2d(nWindowBorderLeft, nWindowHeight),
                          Vector2d(nWindowBorderLeft+nWindowWidth, nWindowBorderTop));
        m_Graphics.setColor(0.25, 0.0, 0.0, 0.8);
        m_Graphics.filledRectSS(Vector2d(nWindowBorderLeft, nWindowHeight),
                                Vector2d(nWindowBorderLeft+nWindowWidth, nWindowBorderTop));
        m_Graphics.setDepth(GRAPHICS_DEPTH_DEFAULT);
        

        std::stringstream oss;
        int i = m_pComConsole->getCommands().size() - nComHistory;
        if (i<0) i=0;
        while (i < m_pComConsole->getCommands().size())
        {
            oss << "> " << m_pComConsole->getCommands().at(i);
            if (m_pComConsole->getReturnValues().at(i) != "")
            {
                oss << " => " << m_pComConsole->getReturnValues().at(i);
            }
            oss << "\n";
            
            ++i;
        }
        oss << "> " << m_pComConsole->getCurrentCommand() << "_";
        
        m_Graphics.getWindow()->pushGLStates();
        sf::Text Text;

        Text.setString(oss.str());
        Text.setFont(m_Font);
        Text.setCharacterSize(nTextSize);
        Text.setPosition(nWindowBorderLeft, nWindowBorderTop);
        m_Graphics.getWindow()->draw(Text);
        m_Graphics.getWindow()->popGLStates();        
            
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        
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
                            IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                            pObj.second->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                            IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
            m_Graphics.filledRect(pObj.second->getGeometry()->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                  IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                  pObj.second->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                  IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
            // Singleframe
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
            m_Graphics.rect(pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getLowerLeft() - m_pCamera->getCenter() + 
                            IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                            pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getUpperRight()- m_pCamera->getCenter() +
                            IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
            m_Graphics.filledRect(pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getLowerLeft() - m_pCamera->getCenter() + 
                                  IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()),
                                  pObj.second->getGeometry()->getBoundingBox(AABBType::SINGLEFRAME).getUpperRight()- m_pCamera->getCenter() +
                                  IUniverseScaled::cellToDouble(pObj.second->getCell()-m_pCamera->getCell()));
             // Shape bounding boxes
            for (const auto pShp : pObj.second->getGeometry()->getShapes())
            {
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.8);
                m_Graphics.rect(pShp->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                IUniverseScaled::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()),
                                pShp->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                IUniverseScaled::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()));
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.2);
                m_Graphics.filledRect(pShp->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() +
                                    IUniverseScaled::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()),
                                    pShp->getBoundingBox().getUpperRight() - m_pCamera->getCenter() +
                                    IUniverseScaled::cellToDouble(pShp->getBoundingBox().getCell()-m_pCamera->getCell()));
            }
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
    
        m_pCamera->update();

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
            fGridLeftCell = IUniverseScaled::cellToDouble(m_pCamera->getCell())[0];
            fGridTopCell  = IUniverseScaled::cellToDouble(m_pCamera->getCell())[1];
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
            << m_pPhysicsManager->getSimTimerGlobal().getYears() << "y "
            << m_pPhysicsManager->getSimTimerGlobal().getDaysPart() << "d "
            << m_pPhysicsManager->getSimTimerGlobal().getHoursPart() << "h "
            << m_pPhysicsManager->getSimTimerGlobal().getMinutesPart() << "m "
            << m_pPhysicsManager->getSimTimerGlobal().getSecondsPart() << "s" << std::endl;
        for (auto i=0u; i<3; ++i)
        {
            oss << "Local timer " << i+1 << ":   "
                << m_pPhysicsManager->getSimTimerLocal()[i].getYears() << "y "
                << m_pPhysicsManager->getSimTimerLocal()[i].getDaysPart() << "d "
                << m_pPhysicsManager->getSimTimerLocal()[i].getHoursPart() << "h "
                << m_pPhysicsManager->getSimTimerLocal()[i].getMinutesPart() << "m "
                << m_pPhysicsManager->getSimTimerLocal()[i].getSecondsPart() << "s" << std::endl;
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
                    IUniverseScaled::cellToDouble(pObj.second->getTrajectory().getCells().at(i)-m_pCamera->getCell())
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
                                IUniverseScaled::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell())
                              );
            Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(
                                    m_pUniverse->getStarSystems()[i]->Star().getOrigin() - m_pCamera->getCenter() +
                                     IUniverseScaled::cellToDouble
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
//             if (m_pCamera->getBoundingBox().isInside(1.0/fBGDensityFactor*(vecPosRel-Vector2d(fStarfieldSizeX*0.5, fStarfieldSizeY*0.5)) + m_pCamera->getCenter()+IUniverseScaled::cellToDouble(m_pCamera->getCell())))
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
//                                           IUniverseScaled::cellToDouble(
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
                                    IUniverseScaled::cellToDouble
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
//         if (1.0e9 * m_Graphics.getResPMX() < 1.0)
        {
            for (auto i=0u; i<m_pUniverse->getStarSystems().size(); ++i)
            {
                Vector2d vecPos = CKinematicsState::clipToWorldLimit(m_pUniverse->getStarSystems()[i]->Star().getOrigin() +
                                  IUniverseScaled::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell()));
                if (m_pCamera->getBoundingBox().isInside(vecPos))
                {
                    Vector2d vecPosRel = CKinematicsState::clipToWorldLimit(m_pUniverse->getStarSystems()[i]->Star().getOrigin()-
                                        m_pCamera->getCenter()+
                                        IUniverseScaled::cellToDouble
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
    for (auto ci = m_pVisualsDataStorage->getDebrisVisuals().begin();
         ci != m_pVisualsDataStorage->getDebrisVisuals().end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    for (std::list<CDebrisVisualsThruster*>::const_iterator ci = m_pVisualsDataStorage->getDebrisVisualsThruster().begin();
         ci != m_pVisualsDataStorage->getDebrisVisualsThruster().end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
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
                                    IUniverseScaled::cellToDouble(
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
/// \brief Drawing finished, now swap buffers
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::finishFrame()
{
    METHOD_ENTRY("CVisualsManager::finishFrame")
    m_Graphics.swapBuffers();
    m_pDataStorage->swapFront();

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

    m_pComInterface->callWriters("visuals");
    
    m_pCamera = m_pVisualsDataStorage->getCamerasByIndex().operator[](m_unCameraIndex);
    
    this->drawGrid();
    this->drawTrajectories();
    this->drawWorld();
    this->drawKinematicsStates();
    this->drawBoundingBoxes();
    this->drawGridHUD();
    this->drawTimers();
    this->drawConsole();
    
    this->finishFrame();
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
    
    m_pComInterface->registerFunction("cycle_camera",
                                      CCommand<void>([&](){this->cycleCamera();}),
                                      "Cycle through registered cameras",
                                      {{ParameterType::NONE,"No return value"}},
                                      "system", "visuals"
    );
    m_pComInterface->registerFunction("get_current_camera",
                                      CCommand<CCamera*>([&](){return this->getCurrentCamera();}),
                                      "Returns pointer to active camera",
                                      {{ParameterType::UNDEFINED, "CCamera*, Currently active camera"}},
                                      "system"
    );
    m_pComInterface->registerFunction("toggle_bboxes",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_OBJECT_BBOXES);}),
                                      "Toggle bounding boxes on and off.",
                                      {{ParameterType::NONE, "No return value"}},
                                      "visuals", "visuals"
    );
    m_pComInterface->registerFunction("toggle_grid",
                                      CCommand<void>([&](){this->toggleVisualisations(VISUALS_UNIVERSE_GRID);}),
                                      "Toggle universe grid on and off.",
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
