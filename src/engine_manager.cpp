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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::CEngineManager() : m_bDone(false)
{
    METHOD_ENTRY("CEngineManager::CEngineManager")
    CTOR_CALL("CEngineManager::CEngineManager")

    METHOD_EXIT("CEngineManager::CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::~CEngineManager()
{
    METHOD_ENTRY("CEngineManager::~CEngineManager")
    DTOR_CALL("CEngineManager::~CEngineManager")

    METHOD_EXIT("CEngineManager::~CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::runPhysics()
{
    METHOD_ENTRY("CEngineManager::runPhysics")

    m_PhysicsTimer.start();
    
    while (!m_bDone)
    {
        m_pPhysicsManager->addGlobalForces();
        m_pPhysicsManager->moveMasses();
        m_pPhysicsManager->collisionDetection();
        
        m_PhysicsTimer.stop();
        double fFrametime = 1.0/m_pPhysicsManager->getFrequency()-m_PhysicsTimer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        m_PhysicsTimer.start();
    }
    
    METHOD_EXIT("CEngineManager::runPhysics")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the graphics engine
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphics()
{
    METHOD_ENTRY("CEngineManager::runGraphics")

    m_pVisualsManager->initGraphics();
    m_VisualsTimer.start();
    
    while (!m_bDone)
    {
        m_pVisualsManager->drawWorld();
        
        m_VisualsTimer.stop();
        double fFrametime = 1.0/m_pVisualsManager->getFrequency()-m_VisualsTimer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        m_VisualsTimer.start();
    }

    METHOD_EXIT("CEngineManager::runGraphics")
}