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
CVisualsManager::CVisualsManager() : m_fFrequency(VISUALS_DEFAULT_FREQUENCY),
                                     m_nVisualisations(0),
                                     m_pCamera(0)
{
    METHOD_ENTRY("CVisualsManager::CVisualsManager")
    CTOR_CALL("CVisualsManager::CVisualsManager")

    METHOD_EXIT("CVisualsManager::CVisualsManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsManager::~CVisualsManager()
{
    METHOD_ENTRY("CVisualsManager::~CVisualsManager")
    
    for (CKeyMap<IVisuals*>::iterator it = m_VisualsMap.begin();
        it != m_VisualsMap.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IVisuals*");
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
    
    METHOD_EXIT("CVisualsManager::~CVisualsManager")
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
        
        for (CKeyMap<IVisuals*>::const_iterator ci = m_VisualsMap.begin();
            ci != m_VisualsMap.end(); ++ci)
        {
    
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.8);
            m_Graphics.rect((*ci)->getBoundingBox().getLowerLeft() -
                            m_pCamera->getCenter(),
                            (*ci)->getBoundingBox().getUpperRight()-
                            m_pCamera->getCenter());
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.2);
            m_Graphics.filledRect((*ci)->getBoundingBox().getLowerLeft() -
                                  m_pCamera->getCenter(),
                                  (*ci)->getBoundingBox().getUpperRight()-
                                  m_pCamera->getCenter());
        }
    }

    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
    m_Graphics.swapBuffers();

    METHOD_EXIT("CVisualsManager::drawBoundingBox")
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
                m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                                        m_pCamera->getCenter()[1]);
                m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getUpperRight()[1]-
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
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                                            m_pCamera->getCenter()[1]);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getUpperRight()[1]-
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
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getLowerLeft()[1]-
                                                                            m_pCamera->getCenter()[1]);
                    m_Graphics.addVertex(fGridLeft-m_pCamera->getCenter()[0], m_pCamera->getBoundingBox().getUpperRight()[1]-
                                                                            m_pCamera->getCenter()[1]);
                m_Graphics.endLine();
                fGridLeft += fGrid;
            }
            // Horizontal grid lines
            while (fGridTop  < m_pCamera->getBoundingBox().getUpperRight()[1])
            {
                m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE,-15.1);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getLowerLeft()[0]-
                                        m_pCamera->getCenter()[0],
                                        fGridTop-m_pCamera->getCenter()[1]);
                    m_Graphics.addVertex(m_pCamera->getBoundingBox().getUpperRight()[0]-
                                        m_pCamera->getCenter()[0],
                                        fGridTop-m_pCamera->getCenter()[1]);
                m_Graphics.endLine();
                fGridTop += fGrid;
            }
        }
        
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        
    }

    METHOD_EXIT("CVisualsManager::drawBoundingGrid")
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
    
    for (CKeyMap<IVisuals*>::const_iterator ci = m_VisualsMap.begin();
         ci != m_VisualsMap.end(); ++ci)
    {
//         if ((((*ci)->getBoundingBox().getWidth() * m_pCamera->getZoom()) > 0.01) && 
//             (((*ci)->getBoundingBox().getHeight() * m_pCamera->getZoom()) > 0.01))
            if ((*ci)->getBoundingBox().overlaps(m_pCamera->getBoundingBox()))
                if ((((*ci)->getBoundingBox().getWidth() * m_pCamera->getZoom()) <  0.3) && 
                    (((*ci)->getBoundingBox().getHeight() * m_pCamera->getZoom()) < 0.3))
                {
                    m_Graphics.dot((*ci)->getBoundingBox().getLowerLeft() -
                                   m_pCamera->getCenter()-m_pCamera->getCell().cast<double>()*DEFAULT_CELL_SIZE_2);
                }
                else
                {
                    (*ci)->draw(m_pCamera);
                }
    }
    for (std::list<CDebrisVisuals*>::const_iterator ci = m_DebrisVisuals.begin();
         ci != m_DebrisVisuals.end(); ++ci)
    {
        (*ci)->draw(m_pCamera);
    }
    
    METHOD_EXIT("CVisualsManager::drawWorld")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds visuals of an object to list
///
/// \param _pVisuals Visuals that should be added to list
///
/// \return Unique ID
///
////////////////////////////////////////////////////////////////////////////////
KeyType CVisualsManager::addVisuals(IVisuals* _pVisuals)
{
    METHOD_ENTRY("CVisualsManager::addVisuals")
    
    METHOD_EXIT("CVisualsManager::addVisuals")
    return m_VisualsMap.insert(_pVisuals);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a list of visuals of an object to list
///
/// \param _VisualsList Visuals that should be added to list
///
/// \return List of unique IDs
///
////////////////////////////////////////////////////////////////////////////////
VisualsIDListType CVisualsManager::addVisualsList(const VisualsListType& _VisualsList)
{
    METHOD_ENTRY("CVisualsManager::addVisualsList")
    
    std::list<VisualsIDType> VisualsIDList;
    for (VisualsListType::const_iterator it  = _VisualsList.begin();
                                         it != _VisualsList.end(); ++it)
    {
        VisualsIDList.push_back(m_VisualsMap.insert((*it)));
    }

    METHOD_EXIT("CVisualsManager::addVisualsList")
    return VisualsIDList;
}
