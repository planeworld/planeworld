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
// #include <list>

//--- Program header ---------------------------------------------------------//
#include "physics_manager.h"
#include "visuals_manager.h"

const double ENGINE_DEFAULT_FREQUENCY = 30.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the engine(s)
///
/// This class manages the the different engine modules, like physics, graphics.
/// 
////////////////////////////////////////////////////////////////////////////////
class CEngineManager
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CEngineManager();
        ~CEngineManager();
        
        //--- Constant Methods -----------------------------------------------//
        void runPhysics() const;
        void runGraphics() const;
        
        //--- Methods --------------------------------------------------------//
        void setPhysicsManager(CPhysicsManager*);
        void setVisualsManager(CVisualsManager*);
        
    private:
        
        CPhysicsManager*    m_pPhysicsManager;      ///< Instance for handling physics
        CVisualsManager*    m_pVisualsManager;      ///< Instance for handling graphics
};

//--- Implementation is done here for inline optimisation --------------------//

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

    METHOD_EXIT("CEngineManager::setPhysicsManager")
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

    METHOD_EXIT("CEngineManager::setVisualsManager")
}

#endif
