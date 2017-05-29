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
#include "shape.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::CPhysicsManager() : m_pUniverse(0),
                                     m_fG(6.67408e-11),
                                     m_fFrequencyDebris(PHYSICS_DEBRIS_DEFAULT_FREQUENCY),
                                     m_strCellUpdateLast(""),
                                     m_fCellUpdateResidual(0.0),
                                     m_bCellUpdateFirst(true),
                                     m_bPaused(false),
                                     m_bProcessOneFrame(false)
                                   
{
    METHOD_ENTRY("CPhysicsManager::CPhysicsManager")
    CTOR_CALL("CPhysicsManager::CPhysicsManager")
    
    #ifdef PW_MULTITHREADING
        m_strModuleName = "Physics Manager";
    #endif
    m_vecConstantGravitation.setZero();
    
    // Start global timer (index 0)
    m_SimTimer[0].start();
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

    // Stop global timer (index 0)
    m_SimTimer[0].stop();
    
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
/// \brief Create an object and insert to world data storage
///
/// \return UID value of object
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createObject()
{
    METHOD_ENTRY("CPhysicsManager::createObject")
    
    CObject* pObject = new CObject();
    MEM_ALLOC("CObject")
    
    m_ObjectsToBeAddedToWorld.enqueue(pObject);
    
    return pObject->getUID();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a shape of given type
///
/// \param _ShapeType Type of shape to be created
///
/// \return UID value of object
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createShape(const ShapeType _ShapeType)
{
    METHOD_ENTRY("CPhysicsManager::createShape")
    
    UIDType nUID=0u;
    
    switch (_ShapeType)
    {
        case ShapeType::CIRCLE:
        {
            CCircle* pCircle = new CCircle();
            MEM_ALLOC("IShape")
            nUID = pCircle->getUID();
            m_ShapesToBeAddedToWorld.enqueue(pCircle);
            break;
        }
        case ShapeType::PLANET:
        {
            CPlanet* pPlanet = new CPlanet();
            MEM_ALLOC("IShape")
            nUID = pPlanet->getUID();
            m_ShapesToBeAddedToWorld.enqueue(pPlanet);
            break;
        }
        case ShapeType::POLYGON:
        {
            CPolygon* pPolygon = new CPolygon();
            MEM_ALLOC("IShape")
            nUID = pPolygon->getUID();
            m_ShapesToBeAddedToWorld.enqueue(pPolygon);
            break;
        }
        default:
        {
            WARNING_MSG("Physics Manager", "Unknown shape type. Cannot create shape")
        }
    }
    
    return nUID;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a shape of given type
///
/// \param _strShapeType Type of shape to be created as string
///
/// \return UID value of object
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createShape(const std::string& _strShapeType)
{
    METHOD_ENTRY("CPhysicsManager::createShape")
    return this->createShape(mapStringToShapeType(_strShapeType));
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
    m_pDataStorage->addUIDUser(_pComponent);
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
    m_pDataStorage->addUIDUser(_pEmitter);
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
    INFO_BLK(
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
/// \brief Processes one single frame
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::processFrame()
{
    METHOD_ENTRY("CPhysicsManager::processFrame")

    static auto nFrame = 0u;
    
    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
        Obj.second->clearForces();

    // Add new objects and new shapes to world
    CObject* pObj = nullptr;
    while (m_ObjectsToBeAddedToWorld.try_dequeue(pObj))
    {
        m_pDataStorage->addObject(pObj);
    }
    IShape* pShp = nullptr;
    while (m_ShapesToBeAddedToWorld.try_dequeue(pShp))
    {
        m_pDataStorage->addShape(pShp);
    }
    m_pComInterface->callWriters("physics");
    
    if ((!m_bPaused) || (m_bPaused && m_bProcessOneFrame))
    {
        for (auto i=0u; i < m_SimTimer.size(); ++i)        
        {
            m_SimTimer[i].inc(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        }
        this->addGlobalForces();
    }
    
    if ((!m_bPaused) || (m_bPaused && m_bProcessOneFrame))
    {    
        this->moveMasses(nFrame);
        this->collisionDetection();
        this->updateCells();
        
        DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_NOTICE);)
        m_pDataStorage->swapBack();
        DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_DEBUG);)
        m_bProcessOneFrame = false;
        
        
    }
    if (++nFrame == 10000) nFrame = 0;    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Moves and rotates the masses
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::moveMasses(int nTest) const
{
    METHOD_ENTRY("CPhysicsManager::moveMasses")
    
    for (auto ci = m_Components.cbegin();
        ci != m_Components.cend(); ++ci)
    {
        (*ci).second->execute();
    }
    for (auto ci = m_Emitters.cbegin();
        ci != m_Emitters.cend(); ++ci)
    {
        (*ci).second->emit(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
    }
    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
    {
        Obj.second->dynamics(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        Obj.second->transform();
    }
//     if (nTest % static_cast<int>(m_fFrequency/m_fFrequencyDebris) == 0)
    {
        CTimer FrameTimeDebris;
        FrameTimeDebris.start();
        
        for (const auto Debris : *m_pDataStorage->getDebrisByValueBack())
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
                        IGridUser::cellToDouble((*ci).second->getCell()-(*cj).second->getCell());

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
        // Helpers
        std::ostringstream ossDebrisType("");
        for (auto DebrisType : STRING_TO_DEBRIS_TYPE_MAP) ossDebrisType << " " << DebrisType.first;
        std::ostringstream ossShapeType("");
        for (auto ShpType : STRING_TO_SHAPE_TYPE_MAP) ossShapeType << " " << ShpType.first;

        //----------------------------------------------------------------------
        // System package
        //----------------------------------------------------------------------
        m_pComInterface->registerFunction("accelerate_time",
                                          CCommand<void,bool>([&](bool _bAllowTimeScaling){this->accelerateTime(_bAllowTimeScaling);}),
                                          "Accelerates time using more cpu power unless scaling is allowed, which will increase the time step.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::BOOL, "Flag if time scaling by increasing time step is allowed (reduces accuracy)"}},
                                          "system", "physics"
                                         );
        m_pComInterface->registerFunction("create_obj",
                                          CCommand<int>([&]() -> int {return this->createObject();}),
                                          "Creates a default object.",
                                          {{ParameterType::INT, "UID of object"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_shp",
                                          CCommand<int, std::string>([&](const std::string& _strShapeType) -> int {return this->createShape(_strShapeType);}),
                                          "Creates a shape.",
                                          {{ParameterType::INT, "UID of shape"},
                                           {ParameterType::STRING, "Shape type ("+ossShapeType.str()+" )"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("debris_set_type",
                                          CCommand<void, int, std::string>(
                                            [&](const int _nUID, const std::string& _strType)
                                            {
                                                const auto ci = m_pDataStorage->getDebrisByValueBack()->find(_nUID);
                                                if (ci != m_pDataStorage->getDebrisByValueBack()->end())
                                                {
                                                    ci->second->setDebrisType(mapStringToDebrisType(_strType));
                                                }
                                                else
                                                {
                                                    WARNING_MSG("World Data Storage", "Unknown debris with UID <" << _nUID << ">")
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets type of given debris.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Debris UID"},
                                           {ParameterType::STRING, "Debris type (" + ossDebrisType.str() + " )"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("decelerate_time",
                                          CCommand<void>([&](){this->decelerateTime();}),
                                          "Decelerates time.",
                                          {{ParameterType::NONE, "No return value"}},
                                          "system", "physics"
                                         );
        m_pComInterface->registerFunction("get_time_accel",
                                          CCommand<double>([&](){return m_fTimeAccel;}),
                                          "Returns time acceleration factor, keeping step size.",
                                          {{ParameterType::DOUBLE, "Time acceleration factor"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("get_uid",
                                          CCommand<int, std::string>(
                                            [&](const std::string& _strName) -> int
                                            {
                                                int nUID(0);
                                                const auto ci = m_pDataStorage->getUIDsByName()->find(_strName);
                                                if (ci != m_pDataStorage->getUIDsByName()->end())
                                                {
                                                    nUID = ci->second;
                                                }
                                                else
                                                {
                                                    WARNING_MSG("World Data Storage", "Unknown object <" << _strName << ">")
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return nUID;
                                            }),
                                          "Returns UID for entity with given name.",
                                          {{ParameterType::INT, "UID of entity with given name"},
                                           {ParameterType::STRING, "Name of entity"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("pause",
                                          CCommand<void>([&](){this->m_bPaused = true;}),
                                          "Pauses physics simulation.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("obj_add_shp",
                                          CCommand<void, int, int>(
                                            [&](const int _nUIDObj, const int _nUIDShp)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUIDObj);
                                                IShape*  pShp = m_pDataStorage->getShapeByValue(_nUIDShp);
                                                if (pObj != nullptr && pShp != nullptr)
                                                {
                                                    pObj->getGeometry()->addShape(pShp);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Add shape with given UID to object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"},
                                           {ParameterType::INT, "UID of shape to be added"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("process_one_frame",
                                          CCommand<void>([&](){this->processOneFrame();}),
                                          "Processes one single frame of the physics simulation.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("reset_time",
                                          CCommand<void>([&](){this->resetTime();}),
                                          "Resets time to realtime.",
                                          {{ParameterType::NONE, "No return value"}},
                                          "system", "physics"
                                         );
        m_pComInterface->registerFunction("resume",
                                          CCommand<void>([&](){this->m_bPaused = false;}),
                                          "Resumes physics simulation if paused.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("toggle_pause",
                                          CCommand<void>([&](){this->togglePause();}),
                                          "Pauses or unpauses physics simulation.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("toggle_timer",
                                          CCommand<void, int>([&](const int& _nNr)
                                          {
                                              if (_nNr > 0 && _nNr < int(m_SimTimer.size()))
                                              {
                                                  this->m_SimTimer[_nNr].toggle();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nNr << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                              }
                                          }),
                                          "Toggles the given local timer of or on.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "ID of timer"}},
                                           "system", "physics"
                                         );
        //----------------------------------------------------------------------
        // Physics package
        //----------------------------------------------------------------------
        m_pComInterface->registerFunction("get_nrof_timers",
                                          CCommand<int>([&]() -> int {return this->m_SimTimer.size();}),
                                          "Provides number of simulation timers.",
                                          {{ParameterType::INT, "Number of simulation timers"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_mass",
                                          CCommand<double, int>(
                                              [&](const int _nUID) -> double
                                              {
                                                double fMass(1.0);
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    fMass = pObj->getMass();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return fMass;
                                              }),
                                          "Returns mass of a given object.",
                                          {{ParameterType::DOUBLE, "Mass"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_apply_force",
                                          CCommand<void, int, double, double, double, double>(
                                              [&](const int _nUID,
                                                  const double _fForceX,
                                                  const double _fForceY,
                                                  const double _fPOAX,
                                                  const double _fPOAY)
                                              {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->addForceLC(Vector2d(_fForceX, _fForceY), Vector2d(_fPOAX, _fPOAY));
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                              }),
                                          "Applies a force on given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Force X"},
                                           {ParameterType::DOUBLE, "Force Y"},
                                           {ParameterType::DOUBLE, "Point of attack X"},
                                           {ParameterType::DOUBLE, "Point of attack Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_angle",
                                          CCommand<double, int>(
                                              [&](const int _nUID) -> double
                                              {
                                                double fAngle(0.0);
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    fAngle = pObj->getAngle();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return fAngle;
                                              }),
                                          "Returns angle of a given object.",
                                          {{ParameterType::DOUBLE, "Angle"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_angle_vel",
                                          CCommand<double, int>(
                                              [&](const int _nUID) -> double
                                              {
                                                double fAngVel(0.0);
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    fAngVel = pObj->getAngleVelocity();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return fAngVel;
                                              }),
                                          "Returns angle velocity of a given object.",
                                          {{ParameterType::DOUBLE, "Angle velocity"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_cell",
                                          CCommand<Vector2i, int>(
                                              [&](const int _nUID) -> const Vector2i
                                              {
                                                Vector2i vecCell; vecCell.setZero();
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    vecCell = pObj->getCell();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return vecCell;
                                              }),
                                          "Returns cell of object with given uid.",
                                          {{ParameterType::VEC2DINT, "Cell (x, y)"},
                                           {ParameterType::INT, "Object's UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_inertia",
                                          CCommand<double, int>(
                                              [&](const int _nUID) -> double
                                              {
                                                double fInertia(1.0);
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    fInertia = pObj->getInertia();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return fInertia;
                                              }),
                                          "Returns inertia of a given object.",
                                          {{ParameterType::DOUBLE, "Inertia"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_position",
                                          CCommand<Vector2d, int>(
                                              [&](const int _nUID) -> const Vector2d
                                              {
                                                Vector2d vecPosition; vecPosition.setZero();
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    vecPosition = pObj->getOrigin();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return vecPosition;
                                              }),
                                          "Returns position of a given object.",
                                          {{ParameterType::VEC2DDOUBLE, "Position (x, y)"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_position_ref",
                                          CCommand<Vector2d, int, int>(
                                              [&](const int _nUID, const int _nUIDRef) -> const Vector2d
                                              {
                                                Vector2d vecPosition; vecPosition.setZero();
                                                CObject* pObj    = m_pDataStorage->getObjectByValueBack(_nUID);
                                                CObject* pObjRef = m_pDataStorage->getObjectByValueBack(_nUIDRef);
                                                if (pObj != nullptr && pObjRef != nullptr)
                                                {
                                                    vecPosition = pObj->getKinematicsState().getOriginReferredTo(pObjRef->getKinematicsState());
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return vecPosition;
                                              }),
                                          "Returns position of a given object Obj_1 referred to another object Obj_2.",
                                          {{ParameterType::VEC2DDOUBLE, "Position (x, y)"},
                                           {ParameterType::INT, "Object UID (Obj_1)"},
                                           {ParameterType::INT, "Reference object's UID (Obj_2)"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_velocity",
                                          CCommand<Vector2d, int>(
                                              [&](const int _nUID) -> const Vector2d
                                              {
                                                Vector2d vecVelocity; vecVelocity.setZero();
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    vecVelocity = pObj->getVelocity();
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return vecVelocity;
                                              }),
                                          "Returns velocity of a given object.",
                                          {{ParameterType::VEC2DDOUBLE, "Velocity (x, y)"},
                                           {ParameterType::INT, "Object UID"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_get_velocity_ref",
                                          CCommand<Vector2d, int, int>(
                                              [&](const int _nUID, const int _nUIDRef) -> const Vector2d
                                              {
                                                Vector2d vecVelocity; vecVelocity.setZero();
                                                CObject* pObj    = m_pDataStorage->getObjectByValueBack(_nUID);
                                                CObject* pObjRef = m_pDataStorage->getObjectByValueBack(_nUIDRef);
                                                if (pObj != nullptr && pObjRef != nullptr)
                                                {
                                                    vecVelocity = pObj->getKinematicsState().getVelocityReferredTo(pObjRef->getKinematicsState());
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                                return vecVelocity;
                                              }),
                                          "Returns velocity of a given object Obj_1 referred to another object Obj_2.",
                                          {{ParameterType::VEC2DDOUBLE, "Velocity (x, y)"},
                                           {ParameterType::INT, "Object UID (Obj_1)"},
                                           {ParameterType::INT, "Reference object's UID (Obj_2)"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_angle",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fAngle)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setAngle(_fAngle);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets rotation angle of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Angle"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_angle_vel",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fAngleVel)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setAngleVelocity(_fAngleVel);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets angle velocity of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Angle velocity"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_cell",
                                          CCommand<void, int, int, int>(
                                            [&](const int _nUID, const int _nX, const int _nY)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setCell(_nX, _nY);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets residing grid cell of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::INT, "Cell X"},
                                           {ParameterType::INT, "Cell Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_position",
                                          CCommand<void, int, double, double>(
                                            [&](const int _nUID, const double& _fX, const double& _fY)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setOrigin(_fX, _fY);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets position of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Position X"},
                                           {ParameterType::DOUBLE, "Position Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_velocity",
                                          CCommand<void, int, double, double>(
                                            [&](const int _nUID, const double& _fX, const double& _fY)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setVelocity(Vector2d(_fX, _fY));
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Sets velocity of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Velocity X"},
                                           {ParameterType::DOUBLE, "Velocity Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("shp_set_mass",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fMass)
                                            {
                                                IShape* pShp = m_pDataStorage->getShapeByValue(_nUID);
                                                if (pShp != nullptr)
                                                {
                                                    pShp->setMass(_fMass);
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Set mass of shape with given UID.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Shapes UID"},
                                           {ParameterType::DOUBLE, "Mass to be set"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("shp_set_radius",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fRad)
                                            {
                                                IShape* pShp = m_pDataStorage->getShapesByValue()->at(_nUID);
                                                if (pShp != nullptr)
                                                {
                                                    if (pShp->getShapeType() == ShapeType::CIRCLE)
                                                    {
                                                        static_cast<CCircle*>(pShp)->setRadius(_fRad);
                                                    }
                                                    else
                                                    {
                                                        WARNING_MSG("World Data Storage", "Wrong shape type of shape with UID <" << _nUID << ">")
                                                        throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                    }
                                                }
                                                else
                                                {
                                                    throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                }
                                            }),
                                          "Set radius of shape (if applicable) with given UID.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Shapes UID"},
                                           {ParameterType::DOUBLE, "Radius to be set"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("shp_set_vertices",
                                          CCommand<void, int, std::vector<double>>(
                                                [&](const int _nUID, const std::vector<double>& _vecVerts)
                                                {
                                                    IShape* pShp = m_pDataStorage->getShapesByValue()->at(_nUID);
                                                    if (pShp != nullptr)
                                                    {
                                                        if (pShp->getShapeType() == ShapeType::POLYGON)
                                                        {
                                                            CPolygon* pPoly = static_cast<CPolygon*>(pShp);
                                                            if (_vecVerts.size() % 2 != 0)
                                                            {
                                                                WARNING_MSG("Polygon", "Invalid number of parameters. Must be an even number (x,y).")
                                                            }
                                                            else
                                                            {
                                                                for (auto i=0u; i<_vecVerts.size(); i+=2)
                                                                {
                                                                    pPoly->addVertex(_vecVerts[i], _vecVerts[i+1]);
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            WARNING_MSG("World Data Storage", "Wrong shape type of shape with UID <" << _nUID << ">")
                                                            throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        throw CComInterfaceException(ComIntExceptionType::PARAM_ERROR);
                                                    }
                                                }),
                                          "Set vertices of shape (if applicable) with given UID.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Shapes UID"},
                                           {ParameterType::DYN_ARRAY, "Vertices (x0, y0, x1, y1, ... xN, yN)"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("get_time",
                                          CCommand<double>([&]() -> double {return this->m_SimTimer[0].getSecondsRaw();}),
                                          "Provides simulation time (raw seconds, years excluded).",
                                          {{ParameterType::DOUBLE, "Seconds of simulation time"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("get_time_seconds_part",
                                          CCommand<int,int>([&](const int _nT) -> int 
                                          {
                                              if (_nT >= 0 && _nT < int(this->m_SimTimer.size()))
                                              {
                                                  return this->m_SimTimer[_nT].getSecondsPart();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nT << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                  return 0;
                                              }
                                          }),
                                          "Provides simulation time (only seconds part).",
                                          {{ParameterType::INT, "Seconds part of simulation time"},
                                           {ParameterType::INT, "Index of timer"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("get_time_minutes_part",
                                          CCommand<int,int>([&](const int _nT) -> int 
                                          {
                                              if (_nT >= 0 && _nT < int(this->m_SimTimer.size()))
                                              {
                                                  return this->m_SimTimer[_nT].getMinutesPart();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nT << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                  return 0;
                                              }
                                          }),
                                          "Provides simulation time (only minutes part).",
                                          {{ParameterType::INT, "Minutes part of simulation time"},
                                           {ParameterType::INT, "Index of timer"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("get_time_hours_part",
                                          CCommand<int,int>([&](const int _nT) -> int 
                                          {
                                              if (_nT >= 0 && _nT < int(this->m_SimTimer.size()))
                                              {
                                                  return this->m_SimTimer[_nT].getHoursPart();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nT << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                  return 0;
                                              }
                                          }),
                                          "Provides simulation time (only hours part).",
                                          {{ParameterType::INT, "Hours part of simulation time"},
                                           {ParameterType::INT, "Index of timer"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("get_time_days_part",
                                          CCommand<int,int>([&](const int _nT) -> int 
                                          {
                                              if (_nT >= 0 && _nT < int(this->m_SimTimer.size()))
                                              {
                                                  return this->m_SimTimer[_nT].getDaysPart();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nT << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                  return 0;
                                              }
                                          }),
                                          "Provides simulation time (only days part).",
                                          {{ParameterType::INT, "Days part of simulation time"},
                                           {ParameterType::INT, "Index of timer"}},
                                           "physics"
                                         );
        m_pComInterface->registerFunction("get_time_years",
                                          CCommand<int,int>([&](const int _nT) -> int 
                                          {
                                              if (_nT >= 0 && _nT < int(this->m_SimTimer.size()))
                                              {
                                                  return this->m_SimTimer[_nT].getYears();
                                              }
                                              else
                                              {
                                                  WARNING_MSG("Sim Timer", "Unknown ID <" << _nT << ">")
                                                  throw CComInterfaceException(ComIntExceptionType::INVALID_VALUE);
                                                  return 0;
                                              }
                                          }),
                                          "Provides simulation time (only years part).",
                                          {{ParameterType::INT, "Years part of simulation time"},
                                           {ParameterType::INT, "Index of timer"}},
                                           "physics"
                                         );
        
        //----------------------------------------------------------------------
        // Sim package
        //----------------------------------------------------------------------
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
    
    // Use double frequency of v_max (v_max = speed of light = 3.0e9m/s)
    // just to avoid any surprises
    double      fFreq = 6.0e9*m_pDataStorage->getTimeScale()*
                        m_fTimeAccel *
                        m_pDataStorage->getObjectsByValueBack()->size()/DEFAULT_CELL_SIZE_2;
                        
    double      fNrOfObj = fFreq/m_fFrequency + m_fCellUpdateResidual;
    uint32_t    nNrOfObj = static_cast<int>(fNrOfObj);

    if (nNrOfObj > m_pDataStorage->getObjectsByValueBack()->size())
        nNrOfObj = m_pDataStorage->getObjectsByValueBack()->size();
    
    m_fCellUpdateResidual = fNrOfObj - nNrOfObj;
    
    // From now on use ObjectsByName since they are ordered by name in a std::map.
    // Hence, a correct entry point to continue cell update can be found
    if (m_bCellUpdateFirst)
    {
        // Make sure not to try to access ->begin() within empty vector
        if (nNrOfObj > 0)
        {
            m_strCellUpdateLast = m_pDataStorage->getObjectsByNameBack()->begin()->second->getName();
            m_bCellUpdateFirst = false;
        }
    }
    else
    {
        auto it = m_pDataStorage->getObjectsByNameBack()->find(m_strCellUpdateLast);
        for (auto i=0u; i<nNrOfObj; ++i)
        {
            it->second->updateCell();
            if (++it == m_pDataStorage->getObjectsByNameBack()->end())
                it =  m_pDataStorage->getObjectsByNameBack()->begin();
        }
        m_strCellUpdateLast = it->second->getName();
    }
}
