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
/// \file       physics_manager.h
/// \brief      Prototype of class "CPhysicsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-07
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "collision_manager.h"
#include "emitter.h"
#include "universe.h"
#include "world_data_storage_user.h"

//--- Misc header ------------------------------------------------------------//
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

const double      PHYSICS_DEFAULT_FREQUENCY     = 200.0; ///< Default physics frequency
const std::string PHYSICS_DEFAULT_LUA_INTERFACE = "physics_interface.lua"; ///< Default lua interface file

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the physics
///
/// This class computes everything that is physics related.
///
/// \todo Create own list for gravity enabled object. This makes computation
///       faster, especially for debris.
/// \todo Implement methods to move dynamic/static objects between lists to
///       allow online state change.
/// 
////////////////////////////////////////////////////////////////////////////////
class CPhysicsManager : public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPhysicsManager();
        ~CPhysicsManager();
        
        //--- Constant Methods -----------------------------------------------//
        const double     getFrequency() const;
        CUniverse* const getUniverse() const;

        //--- Methods --------------------------------------------------------//
        void setConstantGravity(const Vector2d&);
        void setPhysicsInterface(const std::string&);
        void setUniverse(CUniverse* const);
        
        void addGlobalForces();
        void addEmitter(IEmitter*);
        void addEmitters(const EmittersType&);
        void collisionDetection();
        void initEmitters();
        void initObjects();
        void moveMasses(int);
        void accelerateTime();
        void decelerateTime();
        void resetTime();
        void runCellUpdate();

    private:
        
        CUniverse*          m_pUniverse;            ///< The procedurally generated universe
        CCollisionManager   m_CollisionManager;     ///< Instance for collision handling

        CTimer              m_Timer;                ///< Timer for physics
        double              m_fFrequency;           ///< Frequency of physics calculation
        double              m_fTimeAccel;           ///< Factor for global acceleration

        Vector2d m_vecConstantGravitation;          ///< Vector for constant gravitation

        EmittersType    m_Emitters;                 ///< List of emitters
        
        double          m_fCellUpdateResidual;      ///< Residual for calculation of cell update
        bool            m_bCellUpdateFirst;         ///< Indicates the first cell update (to initialise access)
        
        ///--- Lua access ----------------------------------------------------//
        lua_State*                 m_pLuaState;                 ///< Lua state for external access
        std::string                m_strLuaPhysicsInterface;    ///< Lua physics interface file
        static CPhysicsManager*    m_pLuaThis;                  ///< Store this-pointer for Lua access
        static int                 luaApplyAcceleration(lua_State*);
        static int                 luaGetPosition(lua_State*);
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of physics calculations
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double CPhysicsManager::getFrequency() const
{
    METHOD_ENTRY("CPhysicsManager::getFrequency()")
    return (m_fFrequency);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the procedurally generated universe
///
/// \return Procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline CUniverse* const CPhysicsManager::getUniverse() const
{
    METHOD_ENTRY("CPhysicsManager::getUniverse()")
    return m_pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set vector for a constant gravity
///
/// \param _vecG Gravity vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setConstantGravity(const Vector2d& _vecG)
{
    METHOD_ENTRY("CPhysicsManager::setConstantGravity")
    m_vecConstantGravitation = _vecG;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the lua physics interface
///
/// \param _strPhysicsInterface Lua physics interface
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setPhysicsInterface(const std::string& _strPhysicsInterface)
{
    METHOD_ENTRY("CPhysicsManager::setPhysicsInterface")
    m_strLuaPhysicsInterface = _strPhysicsInterface;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the procedurally generated universe
///
/// \param _pUniverse The procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setUniverse(CUniverse* const _pUniverse)
{
    METHOD_ENTRY("CPhysicsManager::setUniverse")
    m_pUniverse = _pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Accelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::accelerateTime()
{
    METHOD_ENTRY("CPhysicsManager::accelerateTime")
    if (m_fTimeAccel < 10000000.0)
        m_fTimeAccel *= 2.0;
        
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration factor: " << m_fTimeAccel))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Decelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::decelerateTime()
{
    METHOD_ENTRY("CPhysicsManager::decelerateTime")
    m_fTimeAccel *= 0.5;
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration factor: " << m_fTimeAccel))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets time acceleration factor
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::resetTime()
{
    METHOD_ENTRY("CPhysicsManager::resetTime")
    m_fTimeAccel = 1.0;
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration factor: " << m_fTimeAccel))
}

#endif
