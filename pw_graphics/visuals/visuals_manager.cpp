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
/// \file       visuals.cpp
/// \brief      Implementation of class "CVisualsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-06
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals_manager.h"
#include <boost-1_49/boost/concept_check.hpp>

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

    for (std::vector<IObjectVisuals*>::iterator it = m_ObjectVisuals.begin();
        it != m_ObjectVisuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IObjectVisuals*");
        }
    }
    for (std::list<CDebrisVisuals*>::iterator it = m_DebrisVisuals.begin();
        it != m_DebrisVisuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("CDebrisVisuals*");
        }
    }
    
    // Free memory if pointer is still existent
    if (m_pCamera != 0)
    {
        delete m_pCamera;
        m_pCamera = 0;
        MEM_FREED("m_pCamera");
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
        
        for (std::vector<IObjectVisuals*>::const_iterator ci = m_ObjectVisuals.begin();
            ci != m_ObjectVisuals.end(); ++ci)
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
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.3);
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
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.3);
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
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
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
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
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
        m_Graphics.setColor(1.0,1.0,1.0,0.6);

        double fGrid = 1.0;
        
        // Automatically scale grid depending on zoom level
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 100.0)
            fGrid*=10.0;
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) < 10.0)
            fGrid*=0.1;
        
        // Draw zoom scale
        m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
            m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2- 26,10));
            m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+101,10));
        m_Graphics.endLine();
        for (int i=-1; i<=4; ++i)
        {
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+25*i,11));
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+25*i,15));
            m_Graphics.endLine();
        }
        // Draw zoom level
        for (int i=static_cast<int>(std::log10(fGrid)); i<0; ++i)
        {
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,20));
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,40));
            m_Graphics.endLine();
        }
        for (int i=1; i<=static_cast<int>(std::log10(fGrid)); ++i)
        {
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,20));
                m_Graphics.addVertex(m_Graphics.screen2World(m_Graphics.getWidthScr()/2+5*i,40));
            m_Graphics.endLine();
        }
        
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
/// \brief Draws all visuals from list
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::drawWorld()
{
    METHOD_ENTRY("CVisualsManager::drawWorld")
    
    for (std::vector<IObjectVisuals*>::const_iterator ci = m_ObjectVisuals.begin();
         ci != m_ObjectVisuals.end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    for (std::list<CDebrisVisuals*>::const_iterator ci = m_DebrisVisuals.begin();
         ci != m_DebrisVisuals.end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    if (1.0e9 * m_Graphics.getResPMX() < 1.0)
    {
        for (int i=0; i<m_pUniverse->getStarSystems().size(); ++i)
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
                double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
                m_Graphics.setColor(0.8,fColor,0.3);
                m_Graphics.setPointSize(m_pUniverse->getStarSystems()[i]->getNumberOfPlanets());
                m_Graphics.dot(vecPosRel);
            }
        }
    }
    else
    {
        for (int i=0; i<m_pUniverse->getStarSystems().size(); ++i)
        {
//             if (m_nStarIndex != i)
            {
                if ((m_pUniverse->getStarSystems()[i]->getCell()-m_pCamera->getCell()).cast<double>().norm() < 2.1)
                {
                    double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
                    // RACE_CONDITION if object dynamic
                    m_pUniverse->m_pStar->setCell(m_pUniverse->getStarSystems()[i]->getCell());
                    m_pUniverse->m_pStar->setOrigin(m_pUniverse->getStarSystems()[i]->getCenter());
                    
                    // The whole double buffering for physical objects, which is
                    // needed for collision detection ist not working. This is
                    // because pointers to shapes, e.g. visualisation, do not
                    // know their buffered clone. Hence, the following method
                    // does only set the radius for one shape, not for its
                    // clone. Maybe copying is needed or some tricky method.
                    m_pUniverse->m_pStarShape->setRadius(double(m_pUniverse->getStarSystems()[i]->getStarType()+1)*1.0e9);
                    m_pUniverse->m_pStar->init();
                    m_nStarIndex = i;
                }
            }
        }
    }
    m_Graphics.setPointSize(1.0);

}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds visuals of an object to list
///
/// \param _pObjectVisuals Visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::addVisuals(IObjectVisuals* _pObjectVisuals)
{
    METHOD_ENTRY("CVisualsManager::addVisuals")
    m_ObjectVisuals.push_back(_pObjectVisuals);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a list of visuals of an object to list
///
/// \param _VisualsList Visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
void CVisualsManager::addVisualsList(const std::vector<IObjectVisuals*>& _VisualsList)
{
    METHOD_ENTRY("CVisualsManager::addVisualsList")
    
    for (std::vector<IObjectVisuals*>::const_iterator ci  = _VisualsList.begin();
                                         ci != _VisualsList.end(); ++ci)
    {
        m_ObjectVisuals.push_back(*ci);
    }
}
