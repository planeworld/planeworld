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
/// \file       physics_manager.cpp
/// \brief      Implementation of class "CPhysicsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-07-07
///
////////////////////////////////////////////////////////////////////////////////

#include "physics_manager.h"

#include "debris_emitter.h"
#include "physics_manager.h"

CPhysicsManager* CPhysicsManager::m_pLuaThis;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::CPhysicsManager() : m_pUniverse(0),
                                     m_fFrequency(PHYSICS_DEFAULT_FREQUENCY),
                                     m_fFrequencyDebris(PHYSICS_DEBRIS_DEFAULT_FREQUENCY),
                                     m_fFrequencyLua(PHYSICS_LUA_DEFAULT_FREQUENCY),
                                     m_fTimeAccel(1.0),
                                     m_fCellUpdateResidual(0.0),
                                     m_bCellUpdateFirst(true),
                                     m_strLuaPhysicsInterface(PHYSICS_DEFAULT_LUA_INTERFACE)
                                   
{
    METHOD_ENTRY("CPhysicsManager::CPhysicsManager")
    CTOR_CALL("CPhysicsManager::CPhysicsManager")
    
    m_vecConstantGravitation.setZero();
    m_pLuaThis = this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::~CPhysicsManager()
{
    METHOD_ENTRY("CPhysicsManager::~CPhysicsManager")
    DTOR_CALL("CPhysicsManager::~CPhysicsManager")

    lua_close(m_pLuaState);
    
    for (EmittersType::iterator it = m_Emitters.begin();
        it != m_Emitters.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IEmitter")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IEmitter", "Memory already freed."))
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register a thruster component which registers the emitter.
///
/// \param _pThruster Thruster component which should be registered.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::registerComponent(const CThruster* const _pThruster)
{
    METHOD_ENTRY("CPhysicsManager::registerComponent")
    if (_pThruster->getEmitter() != nullptr)
    {
        this->addEmitter(_pThruster->getEmitter()); ///< \todo not needed any more
    }
    else
    {
        NOTICE_MSG("Physics Manager", "Couldn't register thruster component. "
                   "Probably CThruster::init wasn't called.")
    }
}


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add global forces to all objects
///
/// This method adds global forces to all objects, for example gravitation.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addGlobalForces()
{
    METHOD_ENTRY("CPhysicsManager::addGlobalForces")

    ObjectsType::const_iterator cj;
    double fCCSqr;
    Vector2d vecCC;
    Vector2d vecG;

    for (std::list< IJoint* >::const_iterator ci = m_pDataStorage->getJoints().begin();
        ci != m_pDataStorage->getJoints().end(); ++ci)
    {
        (*ci)->react();
    }

    for (ObjectsType::const_iterator ci = m_pDataStorage->getDynamicObjects().begin();
        ci != m_pDataStorage->getDynamicObjects().end(); ++ci)
    {
        cj = ci;
        ++cj;
        
        if (ci->second->getGravitationState() == true)
        {
            while (cj != m_pDataStorage->getDynamicObjects().end())
            {
                vecCC = ci->second->getCOM() - cj->second->getCOM() +
                        IUniverseScaled::cellToDouble(ci->second->getCell()-cj->second->getCell());
                
                fCCSqr = vecCC.squaredNorm();
                
                if (fCCSqr > 400.0)
                {
                    vecG = vecCC.normalized() * (ci->second->getMass() * cj->second->getMass()) / fCCSqr
//                             * 6.6742e+0;
                        * 6.67384e-11;
                    if (cj->second->getGravitationState() == true)
                        ci->second->addForce(-vecG, ci->second->getCOM());
                    cj->second->addForce(vecG, cj->second->getCOM());
                }
                ++cj;
            }
        }

        ci->second->addAcceleration(m_vecConstantGravitation);
    };
    
    for (std::list< CDebris* >::const_iterator ci = m_pDataStorage->getDebris().begin();
        ci != m_pDataStorage->getDebris().end(); ++ci)
    {
        (*ci)->setForce(Vector2d(0.0, -9.81));
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add an emitter to internal list of emitters
///
/// \param _Emitter Emitter that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addEmitter(IEmitter* _Emitter)
{
    METHOD_ENTRY("CPhysicsManager::addEmitter")
    m_Emitters.push_back(_Emitter);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a list of emitters to internal list of emitters
///
/// \param _Emitters Emitters that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addEmitters(const EmittersType& _Emitters)
{
    METHOD_ENTRY("CPhysicsManager::addEmitters")

    EmittersType::const_iterator ci = _Emitters.begin();
  
    while (ci != _Emitters.end())
    {
        this->addEmitter((*ci));
        ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests all objects for collision
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::collisionDetection()
{
    METHOD_ENTRY("CPhysicsManager::collisionDetection")

//     m_ContactList.clear();
    m_CollisionManager.setDebris(m_pDataStorage->getDebris());
    m_CollisionManager.setDynamicObjects(m_pDataStorage->getDynamicObjects());
    m_CollisionManager.setStaticObjects(m_pDataStorage->getStaticObjects());
    m_CollisionManager.detectCollisions();
    
//  for (ContactList::const_iterator ci = m_ContactList.begin();
//      ci != m_ContactList.end(); ++ci)
//  {
//      (*ci).getObjectA()->disableDynamics();
//      (*ci).getObjectB()->disableDynamics();
//  }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Moves and rotates the masses
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::moveMasses(int nTest)
{
    METHOD_ENTRY("CPhysicsManager::moveMasses")
    
    if (nTest % static_cast<int>(m_fFrequency/m_fFrequencyLua) == 0)
    {
        CTimer FrameTimeLua;
        FrameTimeLua.start();
        
        lua_getglobal(m_pLuaState, "physics_interface");
        if (lua_pcall(m_pLuaState, 0, 0, 0) != 0)
        {
            WARNING_MSG("Physics Manager", "Couldn't call Lua function.")
            WARNING_MSG("Physics Manager", "Lua Error: " << lua_tostring(m_pLuaState, -1))
        }
        
        FrameTimeLua.stop();
        if (FrameTimeLua.getTime() > 1.0/m_fFrequencyLua)
        {
          NOTICE_MSG("Physics Manager", "Execution time of Lua code is too large: " << FrameTimeLua.getTime() << 
                                        "s of " << 1.0/m_fFrequencyLua << "s max.")
        }
    }

    for (EmittersType::const_iterator ci = m_Emitters.cbegin();
        ci != m_Emitters.cend(); ++ci)
    {
        (*ci)->emit(1.0/m_fFrequency*m_fTimeAccel);
    }
    for (ObjectsType::const_iterator ci = m_pDataStorage->getDynamicObjects().begin();
        ci != m_pDataStorage->getDynamicObjects().end(); ++ci)
    {
        ci->second->dynamics(1.0/m_fFrequency*m_fTimeAccel);
        ci->second->transform();
        ci->second->clearForces();
    }
    if (nTest % static_cast<int>(m_fFrequency/m_fFrequencyDebris) == 0)
    {
        CTimer FrameTimeDebris;
        FrameTimeDebris.start();
        
        for (std::list< CDebris* >::const_iterator ci = m_pDataStorage->getDebris().begin();
            ci != m_pDataStorage->getDebris().end(); ++ci)
        {
            (*ci)->dynamics(1.0/m_fFrequencyDebris*m_fTimeAccel);
        }
        
        FrameTimeDebris.stop();
        if (FrameTimeDebris.getTime() > 1.0/m_fFrequencyDebris)
        {
          NOTICE_MSG("Physics Manager", "Execution time of debris code is too large: " << FrameTimeDebris.getTime() << 
                                        "s of " << 1.0/m_fFrequencyDebris << "s max.")
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise all emitters
///
/// Emitters create objects or debris. Some emitters do just emit objects once,
/// e.g. if they spatially distribute objects. Others emit on a timely basis.
/// All emitted objects must be added to the list of objects that is handeled
/// by the physics manager, so especially emitters that emit only once should
/// be called before object initialisiation to be in a valid state at the
/// beginning of the simulation.
/// After emitation, emitters are destroyed if they are one time emitters. All
/// others will be called frequently by the physics manager.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::initEmitters()
{
    METHOD_ENTRY("CPhysicsManager::initEmitters")

    INFO_MSG("Physics Manager", "Initialising emitters.")

    EmittersType::iterator it = m_Emitters.begin();
    while (it != m_Emitters.end())
    {
        (*it)->setWorldDataStorage(m_pDataStorage);
        (*it)->init();
        if ((*it)->getMode() == EMITTER_MODE_EMIT_ONCE)
        {
            (*it)->emit();
            
            delete (*it);
            (*it) = 0;
            MEM_FREED("IEmitter")
            
            it = m_Emitters.erase(it);
        }
        else
        {
            ++it;
        }
        
    };
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise Lua scripting engine
///
/// \return Initialisation succesful?
///
///////////////////////////////////////////////////////////////////////////////
bool CPhysicsManager::initLua()
{
    METHOD_ENTRY("CPhysicsManager::initLua")

    //--- Test Lua -----------------------------------------------------------//
    m_pLuaState = luaL_newstate();
 
    luaL_openlibs(m_pLuaState);
    lua_register(m_pLuaState, "apply_force", luaApplyForce);
    lua_register(m_pLuaState, "get_frequency", luaGetFrequency);
    lua_register(m_pLuaState, "get_position", luaGetPosition);
    lua_register(m_pLuaState, "get_velocity", luaGetVelocity);
    if (luaL_dofile(m_pLuaState, m_strLuaPhysicsInterface.c_str()) != 0)
    {
        ERROR_MSG("Physics Manager", "File " << m_strLuaPhysicsInterface <<
                                     " could not be loaded.")
        WARNING_MSG("Physics Manager", "Lua Error: " << lua_tostring(m_pLuaState, -1))
        return false;
    }
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise all objects
///
/// Initialisiation of all objects resets objects position, speed etc. to
/// their state at the beginning of the simulation. It has to be called at
/// least once at start to ensure a proper state of fixed objects.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::initObjects()
{
    METHOD_ENTRY("CPhysicsManager::initObjects")

    //--- Init objects -------------------------------------------------------//
    m_Timer.stop();
    m_Timer.start();

    INFO_MSG("Physics Manager", "Initialising objects.")

    for (ObjectsType::const_iterator ci = m_pDataStorage->getDynamicObjects().begin();
        ci != m_pDataStorage->getDynamicObjects().end(); ++ci)
    {
        ci->second->init();
    };
    for (ObjectsType::const_iterator ci = m_pDataStorage->getStaticObjects().begin();
        ci != m_pDataStorage->getStaticObjects().end(); ++ci)
    {
        ci->second->init();
    };
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Method that updates the cells of all objects.
///
/// An object has a maximum speed of 3e9 m/s, which means it moves 
/// cell update has to be checked every DEFAULT_GRID_SIZE_2 / 3e9 seconds. 
/// Given n objects to be checked, the frequency should be
/// n / (DEFAULT_GRID_SIZE_2 / 3e9) Hz
/// Since the actual frequency might be higher than the frequency the method is
/// called at, the number of objects updated is chosen accordingly.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::runCellUpdate()
{
    METHOD_ENTRY("CPhysicsManager::runCellUpdate")
    
    // Use double frequency just to avoid any surprises
    double fFreq = 6.0e9*m_fTimeAccel*m_pDataStorage->getDynamicObjects().size()/DEFAULT_CELL_SIZE_2;

    double      fNrOfObj = fFreq/m_fFrequency + m_fCellUpdateResidual;
    uint32_t    nNrOfObj = static_cast<int>(fNrOfObj);

    if (nNrOfObj > m_pDataStorage->getDynamicObjects().size()) nNrOfObj = m_pDataStorage->getDynamicObjects().size();
    
    m_fCellUpdateResidual = fNrOfObj - nNrOfObj;
    
    for (uint32_t i=0; i<nNrOfObj; ++i)
    {
        // Initialise the iterator for dynamic cell update.
        /// \todo Somehow, this doesn't work if initialised outside of loop. Evaluate.
        if (m_bCellUpdateFirst)
        {
            m_pDataStorage->memorizeDynamicObject("CellUpdater", m_pDataStorage->getDynamicObjects().begin());
            m_bCellUpdateFirst = false;
        }
        
        ObjectsType::const_iterator ci = m_pDataStorage->recallDynamicObject("CellUpdater");
        ci->second->updateCell();
        if (++ci == m_pDataStorage->getDynamicObjects().end())
            ci = m_pDataStorage->getDynamicObjects().begin();
        m_pDataStorage->memorizeDynamicObject("CellUpdater", ci);
    }
    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to apply force on object.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaApplyForce(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaApplyForce")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 5)
    {
        // Read force vector and point of attack (POA).
        Vector2d vecForce(lua_tonumber(_pLuaState, 2), lua_tonumber(_pLuaState, 3));
        Vector2d vecPOA(lua_tonumber(_pLuaState, 4), lua_tonumber(_pLuaState, 5));
        
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getDynamicObjects().find(strObject) != 
            m_pLuaThis->m_pDataStorage->getDynamicObjects().end())
        {
            m_pLuaThis->m_pDataStorage->getDynamicObjects().at(strObject)->addForce(
                vecForce * m_pLuaThis->m_fFrequency/m_pLuaThis->m_fFrequencyLua, vecPOA);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function apply_force (" << nParam << "/5).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get frequency.
///
/// \param _pLuaState Lua access to frequency
///
/// \return Frequency in Hz and time acceleration.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetFrequency(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetFrequency")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 0)
    {
        lua_pushnumber(_pLuaState, m_pLuaThis->m_fFrequency);
        lua_pushnumber(_pLuaState, m_pLuaThis->m_fTimeAccel);
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_position (" << nParam << "/0).")
    }
    
    return 2;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get object position.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetPosition(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetPosition")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getDynamicObjects().find(strObject) != 
            m_pLuaThis->m_pDataStorage->getDynamicObjects().end())
        {
            Vector2d vecPos(m_pLuaThis->m_pDataStorage->getDynamicObjects().at(strObject)->getCOM());
            lua_pushnumber(_pLuaState, vecPos[0]);
            lua_pushnumber(_pLuaState, vecPos[1]);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_position (" << nParam << "/1).")
    }
    
    return 2;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get object velocity.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetVelocity(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetVelocity")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getDynamicObjects().find(strObject) != 
            m_pLuaThis->m_pDataStorage->getDynamicObjects().end())
        {
            Vector2d vecVel(m_pLuaThis->m_pDataStorage->getDynamicObjects().at(strObject)->getVelocity());
            lua_pushnumber(_pLuaState, vecVel[0]);
            lua_pushnumber(_pLuaState, vecVel[1]);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_velocity (" << nParam << "/1).")
    }
    
    return 2;
}

