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
CVisualsManager::CVisualsManager() : m_pUniverse(0),
                                     m_fFrequency(VISUALS_DEFAULT_FREQUENCY),
                                     m_nVisualisations(0),
                                     m_nStarIndex(-1),
                                     m_pCamera(0)
{
    METHOD_ENTRY("CVisualsManager::CVisualsManager")
    CTOR_CALL("CVisualsManager::CVisualsManager")
    
    m_strFont = "consola.ttf";
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

    // Free memory if pointer is still existent
    if (m_pCamera != 0)
    {
        delete m_pCamera;
        m_pCamera = 0;
        MEM_FREED("CCamera");
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
                          m_pCamera->getBoundingCircleRadius());
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        m_Graphics.setDepth(GRAPHICS_DEPTH_DEFAULT);
        
        for (std::vector<IObjectVisuals*>::const_iterator ci = m_pDataStorage->getObjectVisuals().begin();
            ci != m_pDataStorage->getObjectVisuals().end(); ++ci)
        {
            // Object bounding boxes
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.4);
            m_Graphics.rect((*ci)->getObject()->getGeometry()->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                            IUniverseScaled::cellToDouble((*ci)->getObject()->getCell()-m_pCamera->getCell()),
                            (*ci)->getObject()->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                            IUniverseScaled::cellToDouble((*ci)->getObject()->getCell()-m_pCamera->getCell()));
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.1);
            m_Graphics.filledRect((*ci)->getObject()->getGeometry()->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                  IUniverseScaled::cellToDouble((*ci)->getObject()->getCell()-m_pCamera->getCell()),
                                  (*ci)->getObject()->getGeometry()->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                  IUniverseScaled::cellToDouble((*ci)->getObject()->getCell()-m_pCamera->getCell()));
            
            // Shape bounding boxes
            for (std::vector<IVisuals*>::const_iterator cj = (*ci)->getShapeVisuals().begin();
                 cj != (*ci)->getShapeVisuals().end(); ++cj)
            {
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.8);
                m_Graphics.rect((*cj)->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() + 
                                IUniverseScaled::cellToDouble((*cj)->getBoundingBox().getCell()-m_pCamera->getCell()),
                                (*cj)->getBoundingBox().getUpperRight()- m_pCamera->getCenter() +
                                IUniverseScaled::cellToDouble((*cj)->getBoundingBox().getCell()-m_pCamera->getCell()));
                m_Graphics.setColor(0.0, 0.0, 1.0, 0.2);
                m_Graphics.filledRect((*cj)->getBoundingBox().getLowerLeft() - m_pCamera->getCenter() +
                                    IUniverseScaled::cellToDouble((*cj)->getBoundingBox().getCell()-m_pCamera->getCell()),
                                    (*cj)->getBoundingBox().getUpperRight() - m_pCamera->getCenter() +
                                    IUniverseScaled::cellToDouble((*cj)->getBoundingBox().getCell()-m_pCamera->getCell()));
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
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 100.0)
            fGrid*=10.0;
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) < 10.0)
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
        fGridLeft=(floor((m_pCamera->getBoundingBox().getLowerLeft()[0] +
                          fGridLeftCell)/fGrid)+1.0)*fGrid-fGridLeftCell;
        fGridTop =(floor((m_pCamera->getBoundingBox().getLowerLeft()[1] + 
                          fGridTopCell )/fGrid)+1.0)*fGrid-fGridTopCell;
        
        // Change colour if on cell grid
        if (fGrid == DEFAULT_CELL_SIZE)
            m_Graphics.setColor(0.3, 0.0, 0.0);
        else
            m_Graphics.setColor(0.1, 0.1, 0.1);
     
        // Vertical sub grid lines
        while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
        {
            m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_SINGLE,-15.3);
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
            m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_SINGLE,-15.3);
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
            m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_SINGLE,-15.2);
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
            m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_SINGLE,-15.2);
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
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 100.0)
            fGrid*=10.0;
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) < 10.0)
            fGrid*=0.1;
//         
//         // Draw zoom scale
//         m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
//             m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2- 26,10));
//             m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+101,10));
//         m_Graphics.endLine();
//         for (int i=-1; i<=4; ++i)
//         {
//             m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+25*i,11));
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+25*i,15));
//             m_Graphics.endLine();
//         }
//         // Draw zoom level
//         for (int i=static_cast<int>(std::log10(fGrid)); i<0; ++i)
//         {
//             m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,20));
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,40));
//             m_Graphics.endLine();
//         }
//         for (int i=1; i<=static_cast<int>(std::log10(fGrid)); ++i)
//         {
//             m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,20));
//                 m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,40));
//             m_Graphics.endLine();
//         }
//         
//         m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        
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
        for (auto ci =  m_pDataStorage->getObjectVisuals().cbegin();
                  ci != m_pDataStorage->getObjectVisuals().cend(); ++ci)
        {
            this->drawKinematicsState((*ci)->getObject()->getKinematicsState(),
                                      ((*ci)->getObject()->getGeometry()->getBoundingBox().getHeight() + 
                                       (*ci)->getObject()->getGeometry()->getBoundingBox().getWidth()) * 0.5 * 0.33
                                     );
//             m_Graphics.showVec((*ci)->getObject()->getForce(), (*ci)->getObject()->getKinematicsState().getOrigin() - m_pCamera->getKinematicsState().getOrigin());
        }
        
        this->drawKinematicsState(m_pCamera->getKinematicsState(),m_pCamera->getBoundingCircleRadius() * 0.1);

        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
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
const bool CVisualsManager::getVisualisation(const int& _nVis) const
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
        
        for (ObjectsType::const_iterator ci = m_pDataStorage->getDynamicObjects().begin();
            ci != m_pDataStorage->getDynamicObjects().end(); ++ci)
        {
            ci->second->getTrajectory().lock();
            
            // Draw objects trajectories
            double fColourFade = 0.1;
            m_Graphics.beginLine(LineType::GRAPHICS_LINETYPE_STRIP, -15.0);
            
            TrajectoryType::const_iterator cj = ci->second->getTrajectory().getPositions().begin();
            TrajectoryCellType::const_iterator ck = ci->second->getTrajectory().getCells().begin();
            while (cj != ci->second->getTrajectory().getPositions().end())
            {
                m_Graphics.setColor(0.5, 0.0, 0.8, fColourFade);
                m_Graphics.addVertex((*cj) - m_pCamera->getCenter() +
                    IUniverseScaled::cellToDouble((*ck)-m_pCamera->getCell())
                );
                fColourFade += 0.9/TRAJECTORY_CAPACITY;
                
                ++cj; ++ck;
            }
            m_Graphics.endLine();
            
            ci->second->getTrajectory().unlock();
        }
        
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
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
            Vector2d vecPos = m_pUniverse->getStarSystems()[i]->getCenter() +
                              IUniverseScaled::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell());
            Vector2d vecPosRel = m_pUniverse->getStarSystems()[i]->getCenter() - m_pCamera->getCenter() +
                                     IUniverseScaled::cellToDouble
                                     (m_pUniverse->getStarSystems()[i]->getCell() -
                                      m_pCamera->getCell());
            

            // Draw stars in original scale
            if (m_pCamera->getBoundingBox().isInside(vecPos))
            {
                
                double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
                m_Graphics.setColor(0.8,fColor,0.3);
                m_Graphics.setPointSize(m_pUniverse->getStarSystems()[i]->getStarType());
                m_Graphics.dot(vecPosRel);
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
        for (std::vector<IObjectVisuals*>::const_iterator ci = m_pDataStorage->getObjectVisuals().begin();
            ci != m_pDataStorage->getObjectVisuals().end(); ++ci)
        {
            if (m_pCamera->getZoom() * (*ci)->getObject()->getGeometry()->getBoundingBox().getWidth() > 1.0)
            {
                Vector2d vecPosRel = (*ci)->getObject()->getCOM()-
                                    m_pCamera->getCenter()+
                                    IUniverseScaled::cellToDouble
                                    ((*ci)->getObject()->getCell()-
                                    m_pCamera->getCell());
                
                // Now draw the text
                sf::Text text;
                double fColor = (m_pCamera->getZoom() * (*ci)->getObject()->getGeometry()->getBoundingBox().getWidth() - 1.0) * 255.0;
                if (fColor > 255.0) fColor = 255.0;
                sf::Color color(255.0,255.0,255.0, fColor);
                
                text.setString((*ci)->getObject()->getName());
                text.setFont(m_Font);
                text.setCharacterSize(nTextSize);
                text.setColor(color);
                text.setPosition(m_Graphics.world2Screen(vecPosRel)[0], m_Graphics.world2Screen(vecPosRel)[1]);

                m_Graphics.getWindow()->draw(text);
            }
        }
        if (1.0e9 * m_Graphics.getResPMX() < 1.0)
        {
            for (auto i=0u; i<m_pUniverse->getStarSystems().size(); ++i)
            {
                Vector2d vecPos = m_pUniverse->getStarSystems()[i]->getCenter() +
                                  IUniverseScaled::cellToDouble(m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell());
                if (m_pCamera->getBoundingBox().isInside(vecPos))
                {
                    Vector2d vecPosRel = m_pUniverse->getStarSystems()[i]->getCenter()-
                                        m_pCamera->getCenter()+
                                        IUniverseScaled::cellToDouble
                                        (m_pUniverse->getStarSystems()[i]->getCell()-
                                          m_pCamera->getCell());
                    
                    // Now draw the text
                    sf::Text text;
                    double fColor=0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
                    sf::Color color(0.8*255, fColor*255, 0.3*255);
                    
                    text.setString(m_pUniverse->getStarSystems()[i]->getName());
                    text.setFont(m_Font);
                    text.setCharacterSize(nTextSize);
                    text.setColor(color);
                    text.setPosition(m_Graphics.world2Screen(vecPosRel)[0],m_Graphics.world2Screen(vecPosRel)[1]);

                    m_Graphics.getWindow()->draw(text);
                }
            }
        }
        
        m_Graphics.getWindow()->popGLStates();
    }
    
    for (std::vector<IObjectVisuals*>::const_iterator ci = m_pDataStorage->getObjectVisuals().begin();
         ci != m_pDataStorage->getObjectVisuals().end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    for (std::list<CDebrisVisuals*>::const_iterator ci = m_pDataStorage->getDebrisVisuals().begin();
         ci != m_pDataStorage->getDebrisVisuals().end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    for (std::list<CDebrisVisualsThruster*>::const_iterator ci = m_pDataStorage->getDebrisVisualsThruster().begin();
         ci != m_pDataStorage->getDebrisVisualsThruster().end(); ++ci)
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
                m_Graphics.circle(m_pUniverse->getStarSystems()[i]->getCenter()-m_pCamera->getCenter()+
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
        oss << "Local angle:    " <<  _KinematicsState.getLocalAngle() / M_PI * 180.0 << "\n";
        oss << "Local origin:   " <<  _KinematicsState.getLocalOrigin()[0] << ", "
                                  <<  _KinematicsState.getLocalOrigin()[1] << "\n";
        oss << "Angle:          " <<  _KinematicsState.getAngle() / M_PI * 180.0 << "\n";
        oss << "Origin:         " <<  _KinematicsState.getOrigin()[0] << ", "
                                  <<  _KinematicsState.getOrigin()[1] << "\n";
        text.setString(oss.str());
        text.setFont(m_Font);
        text.setCharacterSize(12.0);
        text.setColor(color);
        text.setPosition(m_Graphics.world2Screen(_KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin())[0],
                        m_Graphics.world2Screen(_KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin())[1]);

        m_Graphics.getWindow()->pushGLStates();
        m_Graphics.getWindow()->draw(text);
        m_Graphics.getWindow()->popGLStates();
        
        if (_KinematicsState.gotReference())
        {
            Vector2d vecRef = _KinematicsState.getReference()->getOrigin() - _KinematicsState.getOrigin();
            m_Graphics.showVec(
                vecRef,
                _KinematicsState.getOrigin()-m_pCamera->getKinematicsState().getOrigin()
            );
        }
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
    }
}
