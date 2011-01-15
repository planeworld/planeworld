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
        for (CKeyMap<IVisuals*>::const_iterator ci = m_VisualsMap.begin();
            ci != m_VisualsMap.end(); ++ci)
        {
    
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.8);
            m_Graphics.rect((*ci)->getBoundingBox().getLowerLeft(),
                            (*ci)->getBoundingBox().getUpperRight());
            m_Graphics.setColor(0.0, 0.0, 1.0, 0.2);
            m_Graphics.filledRect((*ci)->getBoundingBox().getLowerLeft(),
                                  (*ci)->getBoundingBox().getUpperRight());
            m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        }
    }

    m_Graphics.swapBuffers();

    METHOD_EXIT("CVisualsManager::drawBoundingBox")
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
    
    m_Graphics.transCamTo(Vector3d(m_pCamera->getPosition()[0],
                                   m_pCamera->getPosition()[1],
                                   0.0));
    m_Graphics.rotCamTo(m_pCamera->getAngle());
    m_Graphics.zoomCamTo(m_pCamera->getZoom());
    m_Graphics.applyCamMovement();
    
    for (CKeyMap<IVisuals*>::const_iterator ci = m_VisualsMap.begin();
         ci != m_VisualsMap.end(); ++ci)
    {
        (*ci)->draw();
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
