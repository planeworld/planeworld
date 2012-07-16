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
#include "X11/Xlib.h" // For XInitThreads (see below)

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
CEngineManager::CEngineManager() : m_bDone(false),
                                   m_bGotPhysics(false),
                                   m_bGraphicsReady(false)
{
    METHOD_ENTRY("CEngineManager::CEngineManager")
    CTOR_CALL("CEngineManager::CEngineManager")
    
    // Important to allow more than one gui thread. This is not the case here,
    // but somehow there is an error if not calling XInitThreads.
    XInitThreads(); 
    
    m_pMutex = SDL_CreateMutex();
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

    // Clean up threading
    SDL_WaitThread(m_pPhysicsThread, NULL);
    SDL_WaitThread(m_pGraphicsThread, NULL);
    SDL_DestroyMutex(m_pMutex);

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
/// This method calls a static caller function. This is a work around to enable
/// usage of SDL threading of member methods.
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphicsThread()
{
    METHOD_ENTRY("CEngineManager::runGraphicsThreadThread")
    m_pGraphicsThread = SDL_CreateThread(CEngineManager::callRunGraphics, this);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine thread
///
/// This method calls a static caller function. This is a work around to enable
/// SDL threading of member methods.
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runPhysicsThread()
{
    METHOD_ENTRY("CEngineManager::runPhysicsThread")
    m_pPhysicsThread = SDL_CreateThread(CEngineManager::callRunPhysics, this);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the graphics engine method
///
/// This static caller function is a workaround to enable SDL threading of member
/// methods. It calls the actual graphics processing method.
///
/// \param _pCallerType Data passed to thread. Here, it's the caller type (this)
///
////////////////////////////////////////////////////////////////////////////////
int CEngineManager::callRunGraphics(void* _pCallerType)
{
    METHOD_ENTRY("CEngineManager::callRunGraphicsThread")
    static_cast<CEngineManager*>(_pCallerType)->runGraphics();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine thread
///
/// This static caller function is a workaround to enable SDL threading of member
/// methods. It calls the actual physics processing method.
///
/// \param _pCallerType Data passed to thread. Here, it's the caller type (this)
///
////////////////////////////////////////////////////////////////////////////////
int CEngineManager::callRunPhysics(void* _pCallerType)
{
    METHOD_ENTRY("CEngineManager::callRunPhysicsPhysicsThread")
    static_cast<CEngineManager*>(_pCallerType)->runPhysics();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the graphics engine
///
////////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphics()
{
    METHOD_ENTRY("CEngineManager::runGraphics")

    INFO_MSG("Engine Manager", "Graphics thread startet.")
    while (!m_bGotPhysics) usleep(1000);
    m_pVisualsManager->initGraphics();
    m_bGraphicsReady = true;
    m_VisualsTimer.start();
    while (!m_bDone)
    {
        SDL_LockMutex(m_pMutex);
        m_pVisualsManager->drawWorld();
        m_pVisualsManager->drawGrid();
        m_pVisualsManager->drawBoundingBoxes();
        SDL_UnlockMutex(m_pMutex);
        
        m_VisualsTimer.stop();
        double fFrametime = 1.0/m_pVisualsManager->getFrequency()-m_VisualsTimer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        m_VisualsTimer.start();
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
    
    INFO_MSG("Engine Manager", "Physics thread startet.")
    m_PhysicsTimer.start();
    
    int nCC=0;
    
    while (!m_bDone)
    {
        m_pPhysicsManager->addGlobalForces();
        SDL_LockMutex(m_pMutex);
        m_pPhysicsManager->moveMasses(nCC);
        m_pPhysicsManager->collisionDetection();
        SDL_UnlockMutex(m_pMutex);
        m_bGotPhysics=true;
        m_PhysicsTimer.stop();
        double fFrametime = 1.0/m_pPhysicsManager->getFrequency()-m_PhysicsTimer.getTime();
        if (fFrametime > 0.0)
        {
            unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
            usleep(unFrametime);
        }
        m_PhysicsTimer.start();
        ++nCC;
    }
}