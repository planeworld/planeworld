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
/// \file       engine_manager.cpp
/// \brief      Implementation of class "CEngineManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "engine_manager.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
CEngineManager::CEngineManager() : m_bDone(false),
                                   m_bGotPhysics(false),
                                   m_bGraphicsReady(false),
                                   m_GraphicsThread(&CEngineManager::runGraphics, this),
                                   m_PhysicsThread(&CEngineManager::runPhysics, this)
{
    METHOD_ENTRY("CEngineManager::CEngineManager")
    CTOR_CALL("CEngineManager::CEngineManager")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
////////////////////////////////////////////////////////////////////////////////
CEngineManager::~CEngineManager()
{
    METHOD_ENTRY("CEngineManager::~CEngineManager")
    DTOR_CALL("CEngineManager::~CEngineManager")

    // Free memory if pointer is still existent
    if (m_pPhysicsManager != 0)
    {
        delete m_pPhysicsManager;
        m_pPhysicsManager = 0;
        MEM_FREED("m_pPhysicsManager");
    }
    if (m_pVisualsManager != 0)
    {
        delete m_pVisualsManager;
        m_pVisualsManager = 0;
        MEM_FREED("m_pVisualsManager");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the graphics engine thread
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphicsThread()
{
    METHOD_ENTRY("CEngineManager::runGraphicsThread")
    
    while (!m_bGotPhysics) usleep(100);
//     m_pVisualsManager->getGraphics().getWindow()->setActive(false);
    m_GraphicsThread.launch();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine thread
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runPhysicsThread()
{
    METHOD_ENTRY("CEngineManager::runPhysicsThread")
    m_PhysicsThread.launch();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the graphics engine
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphics()
{
    METHOD_ENTRY("CEngineManager::runGraphics")

    INFO_MSG("Engine Manager", "Graphics thread started.")
    
    m_pVisualsManager->getGraphics().getWindow()->setActive(true);
    m_pVisualsManager->initGraphics();
    m_bGraphicsReady = true;
    m_VisualsTimer.start();
    while (!m_bDone)
    {
        m_pVisualsManager->drawGrid();
        m_pVisualsManager->drawWorld();
        m_pVisualsManager->drawBoundingBoxes();
        
        m_VisualsTimer.sleepRemaining(m_pVisualsManager->getFrequency());
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runPhysics()
{
    METHOD_ENTRY("CEngineManager::runPhysics")
    
    INFO_MSG("Engine Manager", "Physics thread started.")
    m_PhysicsTimer.start();
    
    int nCC=0;

    while (!m_bDone)
    {
        m_pPhysicsManager->addGlobalForces();
        m_pPhysicsManager->moveMasses(nCC);
        m_pPhysicsManager->collisionDetection();
        m_bGotPhysics=true;
        
        m_PhysicsTimer.sleepRemaining(m_pPhysicsManager->getFrequency());
        ++nCC;
    }
}