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
/// \file       engine_manager.h
/// \brief      Prototype of class "CEngineManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_MANAGER_H
#define ENGINE_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "physics_manager.h"
#include "visuals_manager.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the engine(s)
///
/// This class manages the the different engine modules, like physics, graphics.
///
/// \todo Mutex to synchronize graphics with physics might be set tighter to
///       allow longer async time.
/// 
////////////////////////////////////////////////////////////////////////////////
class CEngineManager
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CEngineManager();
        ~CEngineManager();
        
        //--- Constant Methods -----------------------------------------------//
        const bool& isGraphicsReady() const;
                
        //--- Methods --------------------------------------------------------//
        void runGraphicsThread();
        void runPhysicsThread();
        void setPhysicsManager(CPhysicsManager*);
        void setVisualsManager(CVisualsManager*);
        void stop();
        
    private:
        
        void runPhysics();
        void runGraphics();
        
        sf::Thread          m_GraphicsThread;      ///< Thread, running graphics
        sf::Thread          m_PhysicsThread;       ///< Thread, running physics
        
        bool                m_bDone;                ///< Stop threads if program ends
        bool                m_bGotPhysics;          ///< Indicates first run of physics thread
        bool                m_bGraphicsReady;       ///< Graphics have been initialised
        
        CTimer              m_PhysicsTimer;         ///< Timer for physics updates
        CTimer              m_VisualsTimer;         ///< Timer for physics updates
        
        CPhysicsManager*    m_pPhysicsManager;      ///< Instance for handling physics
        CVisualsManager*    m_pVisualsManager;      ///< Instance for handling graphics
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns if graphics are initialised
///
/// \return Graphics initialised?
///
////////////////////////////////////////////////////////////////////////////////
inline const bool& CEngineManager::isGraphicsReady() const
{
    METHOD_ENTRY("CEngineManager::isGraphicsReady")
    return m_bGraphicsReady;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets up instance for physics
///
/// \param _pPhys Pointer to physic manager instance
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::setPhysicsManager(CPhysicsManager* _pPhys)
{
    METHOD_ENTRY("CEngineManager::setPhysicsManager")
    m_pPhysicsManager = _pPhys;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets up instance for visualisation
///
/// \param _pVis Pointer to visualisation instance
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::setVisualsManager(CVisualsManager* _pVis)
{
    METHOD_ENTRY("CEngineManager::setVisualsManager")
    m_pVisualsManager = _pVis;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stop the whole engine, i.e. stop all event loops
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::stop()
{
    METHOD_ENTRY("CEngineManager::stop")
    m_bDone = true;
}

#endif
