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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::CVisualsManager() : m_pUniverse(0),
                                     m_fFrequency(VISUALS_DEFAULT_FREQUENCY),
                                     m_nVisualisations(0),
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
                        m_pCamera->getCenter()-IUniverseScaled::cellToDouble(m_pCamera->getCell()),
                        m_pCamera->getBoundingBox().getUpperRight()-
                        m_pCamera->getCenter()-IUniverseScaled::cellToDouble(m_pCamera->getCell()));
        m_Graphics.setColor(0.0, 0.5, 0.0, 0.1);
        m_Graphics.filledRect(m_pCamera->getBoundingBox().getLowerLeft()-
                              m_pCamera->getCenter()-IUniverseScaled::cellToDouble(m_pCamera->getCell()),
                              m_pCamera->getBoundingBox().getUpperRight()-
                              m_pCamera->getCenter()-IUniverseScaled::cellToDouble(m_pCamera->getCell()));
        m_Graphics.setColor(0.0, 1.0, 0.0, 0.8);
        m_Graphics.circle(m_pCamera->getCenter()-m_pCamera->getCenter(),
                          m_pCamera->getBoundingCircleRadius());
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        m_Graphics.setDepth(GRAPHICS_DEPTH_DEFAULT);
        
        for (std::vector<IObjectVisuals*>::const_iterator ci = m_ObjectVisuals.begin();
            ci != m_ObjectVisuals.end(); ++ci)
        {
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
    m_Graphics.swapBuffers();
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
    METHOD_ENTRY("CVisualsManager::drawBoundingGrid")

    m_pCamera->update();
    
    if (m_nVisualisations & VISUALS_UNIVERSE_GRID)
    {
        // Default sub grid size every 2m
        double fGrid = 1.0;
        double fGridLeft;
        double fGridTop;
        
        // Automatically scale grid depending on zoom level
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 100.0)
            fGrid*=10.0;
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) < 10.0)
            fGrid*=0.1;
        
        // Snap sub grid to sub grid size
        fGridLeft=floor((m_pCamera->getBoundingBox().getLowerLeft()[0])/fGrid+1.0)*fGrid;
        fGridTop =floor((m_pCamera->getBoundingBox().getLowerLeft()[1])/fGrid+1.0)*fGrid;
        
        // Change colour if on default grid
        if (fGrid < DEFAULT_CELL_SIZE)
            m_Graphics.setColor(0.1, 0.1, 0.1);
        else
            m_Graphics.setColor(0.15, 0.0, 0.0);
    
        // Vertical sub grid lines
        while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
        {
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.3);
                m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                               m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                               m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                               m_pCamera->getBoundingBox().getUpperRight()[1]-
                                               m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
            m_Graphics.endLine();
            fGridLeft += fGrid;
        }
        // Horizontal sub grid lines
        while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
        {
            m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.3);
                m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                    m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                    fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                    m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                    fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
            m_Graphics.endLine();
            fGridTop += fGrid;
        }
        
        fGrid = 10.0;
        
        // Automatically scale grid depending on zoom level
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 10.0)
            fGrid*=10.0;
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) < 1.0)
            fGrid*=0.1;

        // Draw grid only if it is lower than the default grid
        if (fGrid < DEFAULT_CELL_SIZE)
        {
            // Snap grid to grid size
            fGridLeft=floor((m_pCamera->getBoundingBox().getLowerLeft()[0])/fGrid+1.0)*fGrid;
            fGridTop =floor((m_pCamera->getBoundingBox().getLowerLeft()[1])/fGrid+1.0)*fGrid;
            m_Graphics.setColor(0.2, 0.2, 0.2);
            
            // Vertical grid lines
            while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
            {
                m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                                   m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                   m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                                   m_pCamera->getBoundingBox().getUpperRight()[1]-
                                                   m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.endLine();
                fGridLeft += fGrid;
            }
            // Horizontal grid lines
            while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
            {
                m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.2);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                        m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                        fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                        m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                        fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.endLine();
                fGridTop += fGrid;
            }
        }
        
        fGrid = DEFAULT_CELL_SIZE_2;
        
        // Automatically scale grid depending on zoom level (only upscaling for this grid)
        while (((m_pCamera->getBoundingBox().getUpperRight()[0]-m_pCamera->getBoundingBox().getLowerLeft()[0]) / fGrid) > 5.0)
            fGrid*=10.0;

        // Draw grid only if it is equal or greater than the default grid
        if (fGrid > DEFAULT_CELL_SIZE)
        {
            // Snap grid to grid size
            fGridLeft=floor((m_pCamera->getBoundingBox().getLowerLeft()[0])/fGrid+1.0)*fGrid - fGrid*0.5;
            fGridTop =floor((m_pCamera->getBoundingBox().getLowerLeft()[1])/fGrid+1.0)*fGrid - fGrid*0.5;
            m_Graphics.setColor(0.3, 0.0, 0.0);
            
            // Vertical grid lines
            while (fGridLeft < m_pCamera->getBoundingBox().getUpperRight()[0])
            {
                m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.1);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                                   m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                   m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                                   m_pCamera->getBoundingBox().getUpperRight()[1]-
                                                   m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.endLine();
                fGridLeft += fGrid;
            }
            // Horizontal grid lines
            while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
            {
                m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.1);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                        m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                        fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                        m_pCamera->getCenter()[0]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[0],
                                        fGridTop-m_pCamera->getCenter()[1]-IUniverseScaled::cellToDouble(m_pCamera->getCell())[1]);
                m_Graphics.endLine();
                fGridTop += fGrid;
            }
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
void CVisualsManager::drawWorld() const
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
                              m_pUniverse->getStarSystems()[i]->getCell().cast<double>() *
                              DEFAULT_CELL_SIZE_2;
            if (m_pCamera->getBoundingBox().isInside(vecPos))
            {
                Vector2d vecPosRel = m_pUniverse->getStarSystems()[i]->getCenter()-
                                     m_pCamera->getCenter()+
                                    (m_pUniverse->getStarSystems()[i]->getCell()-
                                     m_pCamera->getCell()).cast<double>()*DEFAULT_CELL_SIZE_2;
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
            Vector2d vecPos = m_pUniverse->getStarSystems()[i]->getCenter() +
                              m_pUniverse->getStarSystems()[i]->getCell().cast<double>() *
                              DEFAULT_CELL_SIZE_2;
            if (m_pCamera->getBoundingBox().isInside(vecPos))
            {
                double fColor = 0.1*m_pUniverse->getStarSystems()[i]->getStarType()+0.3;
                Vector2d vecPosRel = m_pUniverse->getStarSystems()[i]->getCenter()-
                                     m_pCamera->getCenter()+
                                    (m_pUniverse->getStarSystems()[i]->getCell()-
                                     m_pCamera->getCell()).cast<double>()*DEFAULT_CELL_SIZE_2;
                m_Graphics.setColor(0.8,fColor,0.3);
                m_Graphics.circle(vecPosRel,
                                  double(m_pUniverse->getStarSystems()[i]->getStarType()+1)*1.0e9);
            }
        }
    }
    
    m_Graphics.setPointSize(1.0);
    
    METHOD_EXIT("CVisualsManager::drawWorld")
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
