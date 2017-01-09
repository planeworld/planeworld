////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2017 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

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
#include "joint.h"
#include "physics_manager.h"

CPhysicsManager* CPhysicsManager::m_pLuaThis;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::CPhysicsManager() : m_pUniverse(0),
                                     m_fG(6.67408e-11),
                                     m_fFrequency(PHYSICS_DEFAULT_FREQUENCY),
                                     m_fFrequencyDebris(PHYSICS_DEBRIS_DEFAULT_FREQUENCY),
                                     m_fFrequencyLua(PHYSICS_LUA_DEFAULT_FREQUENCY),
                                     m_fTimeAccel(1.0),
                                     m_fTimeSlept(1.0),
                                     m_fCellUpdateResidual(0.0),
                                     m_bCellUpdateFirst(true),
                                     m_bPaused(false),
                                     m_bProcessOneFrame(false),
                                     m_strLuaPhysicsInterface("")
                                   
{
    METHOD_ENTRY("CPhysicsManager::CPhysicsManager")
    CTOR_CALL("CPhysicsManager::CPhysicsManager")
    
    m_vecConstantGravitation.setZero();
    m_pLuaThis = this;
    m_SimTimerGlobal.start();
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

    if (m_strLuaPhysicsInterface != "") lua_close(m_pLuaState);
    m_SimTimerGlobal.stop();
    
    for (auto it = m_Emitters.begin();
        it != m_Emitters.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it).second != nullptr)
        {
            delete (*it).second;
            (*it).second = nullptr;
            MEM_FREED("IEmitter")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IEmitter", "Memory already freed."))
        }
    }
    for (auto it = m_Components.begin();
        it != m_Components.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it).second != nullptr)
        {
            delete (*it).second;
            (*it).second = nullptr;
            MEM_FREED("CThruster")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CThruster", "Memory already freed."))
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a component to internal list of components
///
/// \param _pComponent Component that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addComponent(CThruster* const _pComponent)
{
    METHOD_ENTRY("CPhysicsManager::addComponent")
    m_Components.insert(std::pair<std::string,CThruster*>(_pComponent->getName(), _pComponent));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a list of components to internal list of components
///
/// \param _Components Components that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addComponents(const ComponentsType& _Components)
{
    METHOD_ENTRY("CPhysicsManager::addComponents")

    auto ci = _Components.cbegin();
  
    while (ci != _Components.cend())
    {
        this->addComponent((*ci).second);
        ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add an emitter to internal list of emitters
///
/// \param _pEmitter Emitter that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addEmitter(IEmitter* const _pEmitter)
{
    METHOD_ENTRY("CPhysicsManager::addEmitter")
    m_Emitters.insert(std::pair<std::string,IEmitter*>(_pEmitter->getName(), _pEmitter));
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

    auto ci = _Emitters.cbegin();
  
    while (ci != _Emitters.cend())
    {
        this->addEmitter((*ci).second);
        ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise all components
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::initComponents()
{
    METHOD_ENTRY("CPhysicsManager::initComponents")

    INFO_MSG("Physics Manager", "Initialising components.")

//     auto ci = m_Components.cbegin();
//     while (ci != m_Components.cend())
//     {
//         (*ci)->init();
//         ++ci;
//     }
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

    auto it = m_Emitters.begin();
    while (it != m_Emitters.end())
    {
        (*it).second->setWorldDataStorage(m_pDataStorage);
        (*it).second->init();
        if ((*it).second->getMode() == EMITTER_MODE_EMIT_ONCE)
        {
            (*it).second->emit();
            
            delete (*it).second;
            (*it).second = nullptr;
            MEM_FREED("IEmitter")
            
            it = m_Emitters.erase(it);
        }
        else
        {
            ++it;
        }
        
    }
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

    //--- Init Lua -----------------------------------------------------------//
    if (m_strLuaPhysicsInterface != "")
    {
        const struct luaL_Reg VarAccessMetaTable[] =
        {
          { "__index", luaGetVar},
          { "__newindex", luaSetVar},
          { NULL, NULL }
        };
      
        m_pLuaState = luaL_newstate();
        
        luaL_openlibs(m_pLuaState);
        
        lua_newtable(m_pLuaState);
            lua_newtable(m_pLuaState);
                lua_pushcfunction(m_pLuaState, luaAccelerateTime);
                lua_setfield(m_pLuaState, -2, "accelerate_time");
                lua_pushcfunction(m_pLuaState, luaDecelerateTime);
                lua_setfield(m_pLuaState, -2, "decelerate_time");
                lua_pushcfunction(m_pLuaState, luaGetFrequency);
                lua_setfield(m_pLuaState, -2, "get_frequency");
                lua_pushcfunction(m_pLuaState, [](lua_State* _pLuaState) -> int {m_pLuaThis->m_bPaused = true; return 0;});
                lua_setfield(m_pLuaState, -2, "pause");
                lua_pushcfunction(m_pLuaState, luaResume);
                lua_setfield(m_pLuaState, -2, "resume");
                lua_pushcfunction(m_pLuaState, luaSetFrequency);
                lua_setfield(m_pLuaState, -2, "set_frequency");
            lua_setfield(m_pLuaState, -2, "system");
            lua_newtable(m_pLuaState);
                lua_pushcfunction(m_pLuaState, luaActivateThruster);
                lua_setfield(m_pLuaState, -2, "activate_thruster");
                lua_pushcfunction(m_pLuaState, luaDeactivateThruster);
                lua_setfield(m_pLuaState, -2, "deactivate_thruster");
            lua_setfield(m_pLuaState, -2, "sim");
            lua_newtable(m_pLuaState);
                lua_pushcfunction(m_pLuaState, luaApplyForce);
                lua_setfield(m_pLuaState, -2, "apply_force");
                lua_pushcfunction(m_pLuaState, luaGetAngle);
                lua_setfield(m_pLuaState, -2, "get_angle");
                lua_pushcfunction(m_pLuaState, luaGetAngleRef);
                lua_setfield(m_pLuaState, -2, "get_angle_ref");
                lua_pushcfunction(m_pLuaState, luaGetAngleVelocity);
                lua_setfield(m_pLuaState, -2, "get_angle_vel");
                lua_pushcfunction(m_pLuaState, luaGetAngleVelocityRef);
                lua_setfield(m_pLuaState, -2, "get_angle_vel_ref");
                lua_pushcfunction(m_pLuaState, luaGetInertia);
                lua_setfield(m_pLuaState, -2, "get_inertia");
                lua_pushcfunction(m_pLuaState, luaGetMass);
                lua_setfield(m_pLuaState, -2, "get_mass");
                lua_pushcfunction(m_pLuaState, luaGetPosition);
                lua_setfield(m_pLuaState, -2, "get_position");
                lua_pushcfunction(m_pLuaState, luaGetPositionRef);
                lua_setfield(m_pLuaState, -2, "get_position_ref");
                lua_pushcfunction(m_pLuaState, luaGetTime);
                lua_setfield(m_pLuaState, -2, "get_time");
                lua_pushcfunction(m_pLuaState, luaGetTimeYears);
                lua_setfield(m_pLuaState, -2, "get_time_years");
                lua_pushcfunction(m_pLuaState, luaGetVelocity);
                lua_setfield(m_pLuaState, -2, "get_velocity");
                lua_pushcfunction(m_pLuaState, luaGetVelocityRef);
                lua_setfield(m_pLuaState, -2, "get_velocity_ref");
                
                luaL_newmetatable(m_pLuaState, "VarAccessMetaTable");
                luaL_setfuncs(m_pLuaState, VarAccessMetaTable, 0);
                lua_setmetatable(m_pLuaState, -2);
              
            lua_setfield(m_pLuaState, -2, "universe");
        lua_setglobal(m_pLuaState, "pw");

        if (luaL_dofile(m_pLuaState, m_strLuaPhysicsInterface.c_str()) != 0)
        {
            ERROR_MSG("Physics Manager", "File " << m_strLuaPhysicsInterface <<
                                        " could not be loaded.")
            WARNING_MSG("Physics Manager", "Lua Error: " << lua_tostring(m_pLuaState, -1))
            return false;
        }
    }
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
    INFO_MSG("Physics Manager", "Initialising objects.")

//     for (auto i=0u; i<m_pDataStorage->getObjectsBuffer().getBufferSize(); ++i)
//     {
//         for (auto Obj : *m_pDataStorage->getObjectsBuffer().getBuffer(i))
//         {
//             Obj.second->init();
//         }
//     }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Toggles physics processing from pause to running
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::togglePause()
{
    METHOD_ENTRY("CPhysicsManager::togglePause")
    m_bPaused ^= 1;
    INFO(
        if (m_bPaused)
        {
            INFO_MSG("Physics Manager", "Physics processing paused.")
        }
        else
        {
            INFO_MSG("Physics Manager", "Physics processing resumed.")
        }
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Accelerates time by factor 2
///
/// \param _bAllowTimeScaling Indicates if time scaling is allowed
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::accelerateTime(const bool _bAllowTimeScaling)
{
    METHOD_ENTRY("CPhysicsManager::accelerateTime")
    if (m_fTimeSlept <= 0.0)
    {
        if (m_pDataStorage->getTimeScale() < 10000000.0)
        {
            if (_bAllowTimeScaling) m_pDataStorage->setTimeScale(m_pDataStorage->getTimeScale() * 2.0);
        }
    }
    else
    {
        if (m_pDataStorage->getTimeScale() < 1.0)
            m_pDataStorage->setTimeScale(m_pDataStorage->getTimeScale() * 2.0);
        else
            m_fTimeAccel *= 2.0;
    }
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration: " << m_fTimeAccel << ", " << m_pDataStorage->getTimeScale()))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Decelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::decelerateTime()
{
    METHOD_ENTRY("CPhysicsManager::decelerateTime")
    
    if (m_pDataStorage->getTimeScale() > 1.0)
        m_pDataStorage->setTimeScale(m_pDataStorage->getTimeScale() * 0.5);
    else
    {
        if (m_fTimeAccel > 1.0)
            m_fTimeAccel *= 0.5;
        else
            m_pDataStorage->setTimeScale(m_pDataStorage->getTimeScale() * 0.5);
    }
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration: " << m_fTimeAccel << ", " << m_pDataStorage->getTimeScale()))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets time acceleration factor
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::resetTime()
{
    METHOD_ENTRY("CPhysicsManager::resetTime")
    m_pDataStorage->setTimeScale(1.0);
    m_fTimeAccel = 1.0;
    DOM_VAR(INFO_MSG("Physics Manager", "Time acceleration: " << m_fTimeAccel << ", " << m_pDataStorage->getTimeScale()))
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise all objects
///
/// Initialisiation of all objects resets objects position, speed etc. to
/// their state at the beginning of the simulation. It has to be called at
/// least once at start to ensure a proper state of fixed objects.
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::processFrame()
{
    METHOD_ENTRY("CPhysicsManager::processFrame")
 
    if ((!m_bPaused) || (m_bPaused && m_bProcessOneFrame))
    {
        m_SimTimerGlobal.inc(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        m_SimTimerLocal[0].inc(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        m_SimTimerLocal[1].inc(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        m_SimTimerLocal[2].inc(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        
        static auto nFrame = 0u;
        if (++nFrame == 10000) nFrame = 0;    
        
        
        
        m_pComInterface->callWriters("physics");
        this->addGlobalForces();
        this->moveMasses(nFrame);
        this->collisionDetection();
    //     this->updateCells();
        
        DEBUG(Log.setLoglevel(LOG_LEVEL_NOTICE);)
        m_pDataStorage->swapBack();
        DEBUG(Log.setLoglevel(LOG_LEVEL_DEBUG);)
        m_bProcessOneFrame = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Moves and rotates the masses
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::moveMasses(int nTest) const
{
    METHOD_ENTRY("CPhysicsManager::moveMasses")
    
    for (const auto pComp : m_Components)
    {
        pComp.second->IObjectReferrer::attachTo(
            static_cast<CObject*>(
                m_pDataStorage->getUIDUsersByValueBack()->operator[](
                    pComp.second->IObjectReferrer::getUIDRef()
                )
            )
        );
    }
    for (const auto pEmit : m_Emitters)
    {
        pEmit.second->getKinematicsState().attachTo(
            static_cast<CKinematicsState*>(
                m_pDataStorage->getUIDUsersByValueBack()->operator[](
                    pEmit.second->getKinematicsState().getUIDRef()
                )
            )
        );
    }
    
    if ((nTest % static_cast<int>(m_fFrequency/m_fFrequencyLua) == 0) &&
        (m_strLuaPhysicsInterface != ""))
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

    for (auto ci = m_Emitters.cbegin();
        ci != m_Emitters.cend(); ++ci)
    {
        (*ci).second->emit(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
    }
    for (auto ci = m_Components.cbegin();
        ci != m_Components.cend(); ++ci)
    {
        (*ci).second->execute();
    }
    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
    {
        Obj.second->dynamics(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        Obj.second->transform();
    }
    if (nTest % static_cast<int>(m_fFrequency/m_fFrequencyDebris) == 0)
    {
        CTimer FrameTimeDebris;
        FrameTimeDebris.start();
        
        for (const auto Debris : *m_pDataStorage->getDebrisByValueBack())
//         for (const auto ci  = m_pDataStorage->getDebrisByNameBack()->cbegin();
//                         ci != m_pDataStorage->getDebrisByNameBack()->cend(); ++ci)
        {
            Debris.second->dynamics(1.0/m_fFrequencyDebris*m_pDataStorage->getTimeScale());
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
/// \brief Add global forces to all objects
///
/// This method adds global forces to all objects, for example gravitation.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addGlobalForces()
{
    METHOD_ENTRY("CPhysicsManager::addGlobalForces")
    
    ObjectsByValueType::const_iterator cj;
    double fCCSqr;
    Vector2d vecCC;
    Vector2d vecG;

    for (std::list< IJoint* >::const_iterator ci = m_pDataStorage->getJoints().begin();
        ci != m_pDataStorage->getJoints().end(); ++ci)
    {
        (*ci)->react();
    }

    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
        Obj.second->clearForces();

    for (auto ci  = m_pDataStorage->getObjectsByValueBack()->cbegin();
              ci != m_pDataStorage->getObjectsByValueBack()->cend(); ++ci)
    {
        cj = ci;
        ++cj;
        
        if ((*ci).second->getGravitationState() == true)
        {
            while (cj != m_pDataStorage->getObjectsByValueBack()->cend())
            {
                vecCC = (*ci).second->getCOM() - (*cj).second->getCOM() +
                        IUniverseScaled::cellToDouble((*ci).second->getCell()-(*cj).second->getCell());

                fCCSqr = vecCC.squaredNorm();
                
                if (fCCSqr > 400.0)
                {
                    vecG = vecCC.normalized() * ((*ci).second->getMass() * (*cj).second->getMass()) / fCCSqr
//                             * 6.6742e+0;
                        * m_fG;
                    if ((*cj).second->getGravitationState() == true)
                        (*ci).second->addForce(-vecG, (*ci).second->getCOM());
                    (*cj).second->addForce(vecG, (*cj).second->getCOM());
                }
                ++cj;
            }
        }

        (*ci).second->addAcceleration(m_vecConstantGravitation);
    };
    
//     for (auto ci = m_pDataStorage->getDebris().cbegin();
//         ci != m_pDataStorage->getDebris().cend(); ++ci)
//     {
// //         (*ci)->setForce(Vector2d(0.0, -1.81));
//     }
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
//     m_CollisionManager.setDebris(m_pDataStorage->getDebris());
//     m_CollisionManager.setDynamicObjects(m_pDataStorage->getObjectsByValueBack());
//     m_CollisionManager.setStaticObjects(m_pDataStorage->getObjectsByValueBack());
//     m_CollisionManager.detectCollisions();
    
//  for (ContactList::const_iterator ci = m_ContactList.begin();
//      ci != m_ContactList.end(); ++ci)
//  {
//      (*ci).getObjectA()->disableDynamics();
//      (*ci).getObjectB()->disableDynamics();
//  }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise the command interface
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::myInitComInterface()
{
    METHOD_ENTRY("CPhysicsManager::myInitComInterface")

    INFO_MSG("Physics Manager", "Initialising com interace.")
    if (m_pComInterface != nullptr)
    {
        m_pComInterface->registerFunction("accelerate_time",
                                          CCommand<void,bool>([&](bool _bAllowTimeScaling){this->accelerateTime(_bAllowTimeScaling);}),
                                          "Accelerates time using more cpu power unless scaling is allowed, which will increase the time step.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::BOOL, "Flag if time scaling by increasing time step is allowed (reduces accuracy)"}},
                                          "sim", "physics"
                                         );
        m_pComInterface->registerFunction("decelerate_time",
                                          CCommand<void>([&](){this->decelerateTime();}),
                                          "Decelerates time.",
                                          {{ParameterType::NONE, "No return value"}},
                                          "sim", "physics"
                                         );
        m_pComInterface->registerFunction("reset_time",
                                          CCommand<void>([&](){this->resetTime();}),
                                          "Resets time to realtime.",
                                          {{ParameterType::NONE, "No return value"}},
                                          "sim", "physics"
                                         );
        m_pComInterface->registerFunction("pause",
                                          CCommand<void>([&](){this->m_bPaused = true;}),
                                          "Pauses physics simulation.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("resume",
                                          CCommand<void>([&](){this->m_bPaused = false;}),
                                          "Resumes physics simulation if paused.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("set_angle",
                                          CCommand<void, std::string, double>(
                                                [&](const std::string& _strName, const double& _fAngle)
                                                {
                                                    try
                                                    {
                                                        m_pDataStorage->getObjectsByNameBack()->at(_strName)->setAngle(_fAngle);
                                                    }
                                                    catch (const std::out_of_range& oor)
                                                    {
                                                        WARNING_MSG("World Data Storage", "Unknown object <" << _strName << ">")
                                                        throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                    }
                                                }),
                                          "Sets rotation angle of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::STRING, "Object name"},
                                           {ParameterType::DOUBLE, "Angle"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("toggle_pause",
                                          CCommand<void>([&](){this->togglePause();}),
                                          "Pauses or unpauses physics simulation.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("toggle_timer",
                                          CCommand<void, int>([&](const int& _nNr)
                                          {
                                              if (_nNr < 0 || _nNr < m_SimTimerLocal.size())
                                              {
                                                  this->m_SimTimerLocal[_nNr].toggle();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Physics manager", "Invalid sim timer ID")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                              }
                                          }),
                                          "Toggles the given local timer of or on.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "ID of timer"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("get_time",
                                          CCommand<double>([&]() -> double {return this->m_SimTimerGlobal.getSecondsRaw();}),
                                          "Provides simulation time (raw seconds, years excluded).",
                                          {{ParameterType::DOUBLE, "Seconds of simulation time"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("get_time_years",
                                          CCommand<int>([&]() -> int {return this->m_SimTimerGlobal.getYears();}),
                                          "Provides full years of simulation time.",
                                          {{ParameterType::INT, "Full years of simulation time"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("apply_force",
                                          CCommand<void, std::string, double, double, double, double>(
                                              [&](const std::string& _strName,
                                                  const double _fForceX,
                                                  const double _fForceY,
                                                  const double _fPOAX,
                                                  const double _fPOAY)
                                              {
                                                try
                                                {
                                                    m_pDataStorage->getObjectsByNameBack()->at(_strName)->addForceLC(
                                                    Vector2d(_fForceX, _fForceY) /** m_pLuaThis->m_fFrequency/m_pLuaThis->m_fFrequencyLua*/, Vector2d(_fPOAX, _fPOAY));
                                                }
                                                catch (const std::out_of_range& oor)
                                                {
                                                    WARNING_MSG("World Data Storage", "Unknown object <" << _strName << ">")
                                                    throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                }
                                              }),
                                          "Applies a force on given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::STRING, "Object name"},
                                           {ParameterType::DOUBLE, "Force X"},
                                           {ParameterType::DOUBLE, "Force Y"},
                                           {ParameterType::DOUBLE, "Point of attack X"},
                                           {ParameterType::DOUBLE, "Point of attack Y"}},
                                           "universe", "physics"
                                         );
        m_pComInterface->registerFunction("get_angle",
                                          CCommand<double, std::string>(
                                              [&](const std::string& _strName) -> const double
                                              {
                                                double fAngle = 0.0;
                                                try
                                                {
                                                    fAngle = m_pDataStorage->getObjectsByNameBack()->at(_strName)->getAngle();
                                                }
                                                catch (const std::out_of_range& oor)
                                                {
                                                    WARNING_MSG("World Data Storage", "Unknown object <" << _strName << ">")
                                                    throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                }
                                                return fAngle;
                                              }),
                                          "Returns angle of a given object.",
                                          {{ParameterType::DOUBLE, "Angle"},
                                           {ParameterType::STRING, "Object name"}},
                                           "universe"
                                         );
        m_pComInterface->registerFunction("get_velocity",
                                          CCommand<Vector2d, std::string>(
                                              [&](const std::string& _strName) -> const Vector2d
                                              {
                                                Vector2d vecVelocity; vecVelocity.setZero();
                                                try
                                                {
                                                    vecVelocity = m_pDataStorage->getObjectsByNameBack()->at(_strName)->getKinematicsState().getVelocity();
                                                }
                                                catch (const std::out_of_range& oor)
                                                {
                                                    WARNING_MSG("World Data Storage", "Unknown object <" << _strName << ">")
                                                    throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                }
                                                return vecVelocity;
                                              }),
                                          "Returns velocity of a given object.",
                                          {{ParameterType::VEC2DDOUBLE, "Velocity (x, y)"},
                                           {ParameterType::STRING, "Object name"}},
                                           "universe"
                                         );
        m_pComInterface->registerFunction("activate_thruster",
                                          CCommand<double,std::string,double>(
                                              [&](const std::string& _strName, const double& _fThrust) -> double 
                                              {
                                                    auto itThruster  = m_Components.find(_strName);
                                                    if ( itThruster != m_Components.end())
                                                    {
                                                        return (*itThruster).second->activate(_fThrust);
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("Physics Manager", "Unknown thruster " << _strName)
                                                        throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                    }
                                              }),
                                          "Activates thruster with given thrust.",
                                          {{ParameterType::DOUBLE, "Actually applied thrust"},
                                           {ParameterType::STRING, "Thruster name"},
                                           {ParameterType::STRING, "Thrust to be applied when activated"}},
                                          "sim", "physics"
                                         );
        m_pComInterface->registerFunction("deactivate_thruster",
                                          CCommand<void, std::string>(
                                              [&](const std::string& _strName)
                                              {
                                                    auto itThruster  = m_Components.find(_strName);
                                                    if ( itThruster != m_Components.end())
                                                    {
                                                        return (*itThruster).second->deactivate();
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("Physics Manager", "Unknown thruster " << _strName)
                                                        throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                    }
                                              }),
                                          "Deactivates thruster.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::STRING, "Thrust to be applied when activated"}},
                                          "sim", "physics"
                                         );
    }
    else
    {
        WARNING_MSG("Physics Manager", "Com interface not set, cannot register functions.")
    }
    
//     auto ci = m_Components.cbegin();
//     while (ci != m_Components.cend())
//     {
//         (*ci)->init();
//         ++ci;
//     }
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
void CPhysicsManager::updateCells()
{
    METHOD_ENTRY("CPhysicsManager::updateCells")
    
//     // Use double frequency just to avoid any surprises
//     double fFreq = 6.0e9*m_pDataStorage->getTimeScale()*m_pDataStorage->getObjectsByValueBack()->size()/DEFAULT_CELL_SIZE_2;
// 
//     double      fNrOfObj = fFreq/m_fFrequency + m_fCellUpdateResidual;
//     uint32_t    nNrOfObj = static_cast<int>(fNrOfObj);
// 
//     if (nNrOfObj > m_pDataStorage->getObjectsByValueBack()->size()) nNrOfObj = m_pDataStorage->getObjectsByValueBack()->size();
//     
//     m_fCellUpdateResidual = fNrOfObj - nNrOfObj;
//     
//     for (uint32_t i=0; i<nNrOfObj; ++i)
//     {
//         // Initialise the iterator for dynamic cell update.
//         /// \todo Somehow, this doesn't work if initialised outside of loop. Evaluate.
//         if (m_bCellUpdateFirst)
//         {
//             m_pDataStorage->memorizeDynamicObject("CellUpdater", m_pDataStorage->getObjectsByValueBack()->begin());
//             m_bCellUpdateFirst = false;
//         }
//         
//         ObjectsType::const_iterator ci = m_pDataStorage->recallDynamicObject("CellUpdater");
//         ci->second->updateCell();
//         if (++ci == m_pDataStorage->getObjectsByValueBack()->end())
//             ci = m_pDataStorage->getObjectsByValueBack()->begin();
//         m_pDataStorage->memorizeDynamicObject("CellUpdater", ci);
//     }
    
}

#ifdef PW_MULTITHREADING
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Runs the physics engine, called as a thread.
  ///
  ///////////////////////////////////////////////////////////////////////////////
  void CPhysicsManager::run()
  {
      METHOD_ENTRY("CPhysicsManager::run")
      
      INFO_MSG("Physics Manager", "Physics thread started.")
      m_bRunning = true;
      
      CTimer PhysicsTimer;
      
      PhysicsTimer.start();
      while (m_bRunning)
      {
          this->processFrame();
          m_fTimeSlept = PhysicsTimer.sleepRemaining(m_fFrequency * m_fTimeAccel);
          
          if (m_fTimeSlept < 0.0 && m_fTimeAccel == 1.0)
          {
              NOTICE_MSG("Physics Manager", "Execution time of physics code is too large: " << 1.0/m_fFrequency - m_fTimeSlept << 
                                          "s of " << 1.0/m_fFrequency << "s max.")
          }
      }
      INFO_MSG("Physics Manager", "Physics thread stopped.")
  }
#endif

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Accelerates time by decreasing remaining sleep in between frames
///        or increasing time step if forced to do so 
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaAccelerateTime(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaAccelerateTime")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 0)
    {
        m_pLuaThis->accelerateTime();
    }
    else if (nParam == 1)
    {
        bool bAllowTimeScaling = lua_toboolean(_pLuaState,1);
        m_pLuaThis->accelerateTime(bAllowTimeScaling);
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function luaAccelerateTime (" << nParam << "/1[max]).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Decelerates time by decreasing remaining sleep in between frames or
///        reducing time step.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaDecelerateTime(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaDecelerateTime")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 0)
    {
        m_pLuaThis->decelerateTime();
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function luaDecelerateTime (" << nParam << "/0).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Activates the given thruster
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaActivateThruster(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaActivateThruster")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 2)
    {
        size_t l;
        std::string     strThruster = lua_tolstring(_pLuaState,1,&l);
        double          fThrust     = lua_tonumber (_pLuaState,2);
        
        auto itThruster  = m_pLuaThis->m_Components.find(strThruster);
        if ( itThruster != m_pLuaThis->m_Components.end())
        {
            lua_pushnumber(_pLuaState, (*itThruster).second->activate(fThrust));
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown thruster " << strThruster)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function activate_thruster (" << nParam << "/2).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Deactivates the given thruster
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaDeactivateThruster(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaDeactivateThruster")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 1)
    {
        size_t l;
        std::string strThruster = lua_tolstring(_pLuaState,1,&l);
        
        auto itThruster  = m_pLuaThis->m_Components.find(strThruster);
        if ( itThruster != m_pLuaThis->m_Components.end())
        {
            (*itThruster).second->deactivate();
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown thruster " << strThruster)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function deactivate_thruster (" << nParam << "/1).")
    }
    
    return 0;
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
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->addForceLC(
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
/// \brief Lua access to get objects angle.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetAngle(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetAngle")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            double fAng = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState().getAngle();
            lua_pushnumber(_pLuaState, fAng);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_angle (" << nParam << "/1).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects angle with reference to another object.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetAngleRef(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetAngleRef")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 2)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        std::string strReference = lua_tolstring(_pLuaState,2,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strReference) != 
                m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
            {
                CKinematicsState KinObj = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState();
                CKinematicsState KinRef = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strReference)->getKinematicsState();
                double fAng = KinObj.getAngleReferredTo(KinRef);
                lua_pushnumber(_pLuaState, fAng);
            }
            else
            {
                WARNING_MSG("Physics Manager", "Unknown reference " << strReference)
            }
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_angle_ref (" << nParam << "/2).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects angle velocity
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetAngleVelocity(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetAngleVelocity")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            double fAngVel = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState().getAngleVelocity();
            lua_pushnumber(_pLuaState, fAngVel);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_angle_vel (" << nParam << "/1).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects angle velocity with reference to another object.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetAngleVelocityRef(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetAngleVelocityRef")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 2)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        std::string strReference = lua_tolstring(_pLuaState,2,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strReference) != 
                m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
            {
                CKinematicsState KinObj = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState();
                CKinematicsState KinRef = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strReference)->getKinematicsState();
                double fAngVel = KinObj.getAngleVelocityReferredTo(KinRef);
                lua_pushnumber(_pLuaState, fAngVel);
            }
            else
            {
                WARNING_MSG("Physics Manager", "Unknown reference " << strReference)
            }
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_angle_vel_ref (" << nParam << "/2).")
    }
    
    return 1;
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
        lua_pushnumber(_pLuaState, m_pLuaThis->m_fFrequency * m_pLuaThis->m_pDataStorage->getTimeScale());
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_frequency (" << nParam << "/0).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects mass.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetMass(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetMass")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            double fMass = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getMass();
            lua_pushnumber(_pLuaState, fMass);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_mass (" << nParam << "/1).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects inertia.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetInertia(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetInertia")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 1)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            double fInertia = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getInertia();
            lua_pushnumber(_pLuaState, fInertia);
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_mass (" << nParam << "/1).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects position.
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
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            Vector2d vecPos(m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getOrigin());
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
/// \brief Lua access to get objects position with reference to another object.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetPositionRef(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetPositionRef")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 2)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        std::string strReference = lua_tolstring(_pLuaState,2,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strReference) != 
                m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
            {
                CKinematicsState KinObj = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState();
                CKinematicsState KinRef = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strReference)->getKinematicsState();
                Vector2d vecPos(KinObj.getOriginReferredTo(KinRef));
                lua_pushnumber(_pLuaState, vecPos[0]);
                lua_pushnumber(_pLuaState, vecPos[1]);
            }
            else
            {
                WARNING_MSG("Physics Manager", "Unknown reference " << strReference)
            }
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_position (" << nParam << "/2).")
    }
    
    return 2;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get simulation time.
///
/// \param _pLuaState Lua access to simulation time
///
/// \return Simulation time in seconds.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetTime(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetTime")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 0)
    {
        lua_pushnumber(_pLuaState, m_pLuaThis->m_SimTimerGlobal.getSecondsRaw());
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_time (" << nParam << "/0).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get years of simulation time.
///
/// \param _pLuaState Lua access to years of simulation time
///
/// \return Simulation time in years.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetTimeYears(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetTimeYears")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 0)
    {
        lua_pushnumber(_pLuaState, m_pLuaThis->m_SimTimerGlobal.getYears());
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_time_years (" << nParam << "/0).")
    }
    
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects velocity.
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
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            Vector2d vecVel(m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getVelocity());
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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Lua access to get objects velocity with reference to another object.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetVelocityRef(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetVelocityRef")  
    
    int nParam = lua_gettop(_pLuaState);

    if (nParam == 2)
    {
        size_t l;
        std::string strObject = lua_tolstring(_pLuaState,1,&l);
        std::string strReference = lua_tolstring(_pLuaState,2,&l);
        if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strObject) != 
            m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
        {
            if (m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->find(strReference) != 
                m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->end())
            {
//                 CKinematicsState KinObj = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strObject)->getKinematicsState();
//                 CKinematicsState KinRef = m_pLuaThis->m_pDataStorage->getObjectsByNameBack()->at(strReference)->getKinematicsState();
                Vector2d vecVel;//(KinObj.getVelocityReferredTo(KinRef));
                lua_pushnumber(_pLuaState, vecVel[0]);
                lua_pushnumber(_pLuaState, vecVel[1]);
            }
            else
            {
                WARNING_MSG("Physics Manager", "Unknown reference " << strReference)
            }
        }
        else
        {
            WARNING_MSG("Physics Manager", "Unknown object " << strObject)
        }
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function get_velocity (" << nParam << "/2).")
    }
    
    return 2;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Pauses the simulation
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaPause(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaPause")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 0)
    {
        m_pLuaThis->m_bPaused = true;
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function pause (" << nParam << "/0).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Resumes the simulation
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaResume(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaResume")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 0)
    {
        m_pLuaThis->m_bPaused = false;
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function resume (" << nParam << "/0).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the frequency for lua calls.
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaSetFrequency(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaSetFrequency")

    int nParam = lua_gettop(_pLuaState);
    
    if (nParam == 1)
    {
        m_pLuaThis->m_fFrequencyLua = lua_tonumber(_pLuaState, 1);
    }
    else
    {
        WARNING_MSG("Physics Manager", "Invalid number of parameters for Lua function set_frequency (" << nParam << "/1).")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Gets an internal variable from Lua script
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaGetVar(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaGetVar")

    const char* pKey = luaL_checkstring(_pLuaState, 2);

    if (strcmp(pKey, "G") == 0)
    {
        lua_pushnumber(_pLuaState, m_pLuaThis->m_fG);
        return 1;
    }
    else
    {
        WARNING_MSG("Physics Manager", "Unknown variable " << pKey << ".")
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an internal variable from Lua script
///
/// \param _pLuaState Lua access to physics
///
/// \return Number of parameters returned to Lua script.
///
///////////////////////////////////////////////////////////////////////////////
int CPhysicsManager::luaSetVar(lua_State* _pLuaState)
{
    METHOD_ENTRY("luaSetVar")

    const char* pKey = luaL_checkstring(_pLuaState, 2);

    if (strcmp(pKey, "G") == 0)
        m_pLuaThis->m_fG = luaL_checknumber(_pLuaState, 3);
    else
    {
        // Fall back to default lua creation of value within table
        lua_rawset(m_pLuaThis->m_pLuaState, -3);
    }
    
    return 0;
}
