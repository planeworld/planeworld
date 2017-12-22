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

#include "particle_emitter.h"
#include "joint.h"
#include "objects_emitter.h"
#include "shape.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::CPhysicsManager() : m_fG(6.67408e-11),
                                     m_fFrequencyParticle(PHYSICS_PARTICLE_DEFAULT_FREQUENCY),
                                     m_nCellUpdateLast(0u),
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates an emitter and inserts it to world data storage
///
/// \param _EmitterType Type of emitter to be created
///
/// \return UID value of emitter
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createEmitter(const EmitterType _EmitterType)
{
    METHOD_ENTRY("CPhysicsManager::createEmitter")
    
    UIDType nUID=0u;
    
    switch (_EmitterType)
    {
        case EmitterType::PARTICLE:
        {
            CParticleEmitter* pParticleEmitter = new CParticleEmitter();
            MEM_ALLOC("IEmitter")
            nUID = pParticleEmitter->getUID();
            m_EmittersToBeAddedToWorld.enqueue(pParticleEmitter);
            
            pParticleEmitter->setWorldDataStorage(m_pDataStorage);
            pParticleEmitter->init();
            break;
        }
        case EmitterType::OBJECT:
        {
            CObjectEmitter* pObjectEmitter = new CObjectEmitter();
            MEM_ALLOC("IEmitter")
            nUID = pObjectEmitter->getUID();
            m_EmittersToBeAddedToWorld.enqueue(pObjectEmitter);
            
            pObjectEmitter->setWorldDataStorage(m_pDataStorage);
            pObjectEmitter->init();
            break;
        }
        default:
        {
            WARNING_MSG("Physics Manager", "Unknown emitter type. Cannot create emitter")
        }
    }
    
    return nUID;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Create an emitter of given type
///
/// \param _strEmitterType Type of emitter to be created as string
///
/// \return UID value of emitter
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createEmitter(const std::string& _strEmitterType)
{
    METHOD_ENTRY("CPhysicsManager::createEmitter")
    return this->createEmitter(mapStringToEmitterType(_strEmitterType));
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
    
    pObject->init();
    m_ObjectsToBeAddedToWorld.enqueue(pObject);
    
    return pObject->getUID();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a particle group and inserts it to world data storage
///
/// \param _ParticleType Type of particles to be created
///
/// \return UID value of particle group
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createParticles(const ParticleTypeType _ParticleType)
{
    METHOD_ENTRY("CPhysicsManager::createParticles")
    
    UIDType nUID=0u;
    
    CParticle* pParticle = new CParticle();
    MEM_ALLOC("CParticle")
    
    pParticle->setParticleType(_ParticleType);
    nUID = pParticle->getUID();
    
    m_ParticlesToBeAddedToWorld.enqueue(pParticle);
    
    return nUID;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Create a particle group of given type
///
/// \param _strParticleType Type of particle to be created as string
///
/// \return UID value of particle
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createParticles(const std::string& _strParticleType)
{
    METHOD_ENTRY("CPhysicsManager::createParticles")
    return this->createParticles(mapStringToParticleType(_strParticleType));
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
/// \brief Create a thruster and insert to world data storage
///
/// \return UID value of thruster
///
///////////////////////////////////////////////////////////////////////////////
UIDType CPhysicsManager::createThruster()
{
    METHOD_ENTRY("CPhysicsManager::createThruster")
    
    CThruster* pThruster = new CThruster();
    MEM_ALLOC("CThruster")
    
//     pThruster->init();
    m_ThrustersToBeAddedToWorld.enqueue(pThruster);
    
    return pThruster->getUID();
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
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
bool CPhysicsManager::processFrame()
{
    METHOD_ENTRY("CPhysicsManager::processFrame")

    static auto nFrame = 0u;
    
    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
        Obj.second->clearForces();

    this->processQueues();
    
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
        this->dynamics(nFrame);
        this->collisionDetection();
        this->updateCells();
        
        m_bProcessOneFrame = false;
    }
    DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_NOTICE);)
    m_TimeProcessedBufferCopy.start();
    m_pDataStorage->swapBack();
    m_TimeProcessedBufferCopy.stop();
    DEBUG_BLK(Log.setLoglevel(LOG_LEVEL_DEBUG);)
    if (++nFrame == 10000) nFrame = 0;

    return true;
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
        
        while (cj != m_pDataStorage->getObjectsByValueBack()->cend())
        {
            if ((*ci).second->getGravitationState() == true ||
                (*cj).second->getGravitationState() == true)
            {
                
                vecCC = (*ci).second->getCOM() - (*cj).second->getCOM() +
                        IGridUser::cellToDouble((*ci).second->getCell()-(*cj).second->getCell());

                fCCSqr = vecCC.squaredNorm();
                
                if (fCCSqr > 400.0)
                {
                    vecG = vecCC.normalized() * ((*ci).second->getMass() * (*cj).second->getMass()) / fCCSqr
                           * m_fG;
                    (*ci).second->addForce(-vecG, (*ci).second->getCOM());
                    (*cj).second->addForce(vecG, (*cj).second->getCOM());
                }
            }
            ++cj;
        }

        (*ci).second->addAcceleration(m_vecConstantGravitation);
    }
    
//     for (auto ci = m_pDataStorage->getParticle().cbegin();
//         ci != m_pDataStorage->getParticle().cend(); ++ci)
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
//     m_CollisionManager.setParticle(m_pDataStorage->getParticle());
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Dynamics processing the masses
///
////////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::dynamics(int nTest)
{
    METHOD_ENTRY("CPhysicsManager::dynamics")
    
    for (auto pThruster : *m_pDataStorage->getThrustersByValue())
    {
        pThruster.second->execute();
    }
    
    std::vector<UIDType> vecToBeDeleted;
    for (auto pEmitter : *m_pDataStorage->getEmittersByValue())
    {
        if (pEmitter.second->getMode() == EmitterModeType::ONCE)
        {
            pEmitter.second->emit();
            vecToBeDeleted.push_back(pEmitter.second->getUID());
            
            delete pEmitter.second;
            pEmitter.second = nullptr;
            MEM_FREED("IEmitter")
        }
        else
        {
            pEmitter.second->emit(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        }
    }
    for (auto EmDel : vecToBeDeleted)
    {
        m_pDataStorage->getEmittersByValue()->erase(EmDel);
    }
    
    m_TimeProcessedObjects.start();
    for (const auto Obj : *m_pDataStorage->getObjectsByValueBack())
    {
        Obj.second->dynamics(1.0/m_fFrequency*m_pDataStorage->getTimeScale());
        Obj.second->transform();
    }
    m_TimeProcessedObjects.stop();
//     if (nTest % static_cast<int>(m_fFrequency/m_fFrequencyParticle) == 0)
    {
        
        m_TimeProcessedParticles.start();
        
        for (const auto Particle : *m_pDataStorage->getParticlesByValueBack())
        {
            Particle.second->dynamics(1.0/m_fFrequencyParticle*m_pDataStorage->getTimeScale());
        }
        
        m_TimeProcessedParticles.stop();
    }
    
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
        std::ostringstream ossParticleType("");
        for (auto ParticleType : STRING_TO_PARTICLE_TYPE_MAP) ossParticleType << " " << ParticleType.first;
        std::ostringstream ossEmitterType("");
        for (auto EmitterType : STRING_TO_EMITTER_TYPE_MAP) ossEmitterType << " " << EmitterType.first;
        std::ostringstream ossEmitterDistributionType("");
        for (auto EmitterDistributionType : STRING_TO_EMITTER_DISTRIBUTION_TYPE_MAP) ossEmitterDistributionType << " " << EmitterDistributionType.first;
        std::ostringstream ossEmitterModeType("");
        for (auto EmitterModeType : STRING_TO_EMITTER_MODE_TYPE_MAP) ossEmitterModeType << " " << EmitterModeType.first;
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
        m_pComInterface->registerFunction("create_emitter",
                                          CCommand<int, std::string>([&](const std::string& _strEmitterType) -> int {return this->createEmitter(_strEmitterType);}),
                                          "Creates an emitter.",
                                          {{ParameterType::INT, "UID of emitter"},
                                           {ParameterType::STRING, "Emitter type ("+ossEmitterType.str()+" )"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_obj",
                                          CCommand<int>([&]() -> int {return this->createObject();}),
                                          "Creates a default object.",
                                          {{ParameterType::INT, "UID of object"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_particles",
                                          CCommand<int, std::string>([&](const std::string& _strParticleType) -> int {return this->createParticles(_strParticleType);}),
                                          "Creates a group of particles.",
                                          {{ParameterType::INT, "UID of particles"},
                                           {ParameterType::STRING, "Particle type ("+ossParticleType.str()+" )"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_shp",
                                          CCommand<int, std::string>([&](const std::string& _strShapeType) -> int {return this->createShape(_strShapeType);}),
                                          "Creates a shape.",
                                          {{ParameterType::INT, "UID of shape"},
                                           {ParameterType::STRING, "Shape type ("+ossShapeType.str()+" )"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_thruster",
                                          CCommand<int>([&]() -> int {return this->createThruster();}),
                                          "Creates a default thruster.",
                                          {{ParameterType::INT, "UID of object"}},
                                          "system"
                                         );
        m_pComInterface->registerFunction("create_universe",
                                          CCommand<void, int, int>([&](const int& _nSeed, const int& _nNrOfStars)
                                          {
                                                if (m_pDataStorage->getUniverse() != nullptr)
                                                {
                                                    delete m_pDataStorage->getUniverse();
                                                    MEM_FREED("CUniverse")
                                                }
                                                m_pDataStorage->setUniverse(new CUniverse());
                                                MEM_ALLOC("CUniverse")
                                                m_pDataStorage->getUniverse()->generate(_nSeed, _nNrOfStars);
                                          }),
                                          "Creates a procedurally generated universe.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Seed for procdural generation"},
                                           {ParameterType::INT, "Number of star systems"}},
                                          "system", "physics"
                                         );
        m_pComInterface->registerFunction("emitter_set_angle",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fAngle)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setAngle(_fAngle);
                                            }
                                          }),
                                          "Set angle of entity emitation.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Angle of entity emitation"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_angle_std",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fAngStd)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setAngleStd(_fAngStd);
                                            }
                                          }),
                                          "Set standard deviation of angle of entity emitation.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Standard deviation of angle of entity emitation"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_distribution",
                                          CCommand<void, int, std::string>([&](const int _nUID, const std::string& _strDist)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setDistribution(mapStringToEmitterDistributionType(_strDist));
                                            }
                                          }),
                                          "Set distribution type of emitter.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::STRING, "Distribution ("+ossEmitterDistributionType.str()+" )"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_frequency",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fFrequency)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setFrequency(_fFrequency);
                                            }
                                          }),
                                          "Set frequency of entity emitation.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Frequency of entity emitation"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_mode",
                                          CCommand<void, int, std::string>([&](const int _nUID, const std::string& _strMode)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setMode(mapStringToEmitterModeType(_strMode));
                                            }
                                          }),
                                          "Set mode of emitter.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::STRING, "Mode ("+ossEmitterModeType.str()+" )"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_number",
                                          CCommand<void, int, int>([&](const int _nUID, const int _nNr)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setNumber(_nNr);
                                            }
                                          }),
                                          "Set number of emitted entities.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::INT, "Number of emitted entities"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_particles",
                                          CCommand<void, int, int>([&](const int _nUIDEm, const int _nUIDPa)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUIDEm);
                                            if (pEmitter != nullptr)
                                            {
                                                 if (pEmitter->getEmitterType() == EmitterType::PARTICLE)
                                                {
                                                    CParticle* pParticles = m_pDataStorage->getParticleByValueBack(_nUIDPa);
                                                    if (pParticles != nullptr)
                                                    {
                                                        static_cast<CParticleEmitter*>(pEmitter)->setParticles(pParticles);
                                                    }
                                                }
                                                else
                                                {
                                                    WARNING_MSG("Physics Manager", "Wrong emitter type, unknown method <setRef>.")
                                                }
                                            }
                                          }),
                                          "Attach particle group to emitter.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::INT, "Particle UID"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_position",
                                          CCommand<void, int, double, double>(
                                            [&](const int _nUID, const double& _fX, const double& _fY)
                                            {
                                                IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                                if (pEmitter != nullptr)
                                                {
                                                    pEmitter->setOrigin(Vector2d(_fX, _fY));
                                                }
                                            }),
                                          "Sets position of a given emitter.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Emitter UID"},
                                           {ParameterType::DOUBLE, "Position X"},
                                           {ParameterType::DOUBLE, "Position Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("emitter_set_velocity",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fVelocity)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setVelocity(_fVelocity);
                                            }
                                          }),
                                          "Set velocity of entity emitation.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Velocity of entity emitation"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_velocity_inheritance",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fVelInh)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setVelocityInheritance(_fVelInh);
                                            }
                                          }),
                                          "Set fraction for inheriting velocity of parent object.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Fraction for inheriting velocity of parent object"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("emitter_set_velocity_std",
                                          CCommand<void, int, double>([&](const int _nUID, const double& _fVelStd)
                                          {
                                            IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUID);
                                            if (pEmitter != nullptr)
                                            {
                                                pEmitter->setVelocityStd(_fVelStd);
                                            }
                                          }),
                                          "Set standard deviation of velocity of entity emitation.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Emittter UID"},
                                          {ParameterType::DOUBLE, "Standard deviation of velocity of entity emitation"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("get_frequency_physics",
                                          CCommand<double>([&]() -> double {return m_fFrequency;}),
                                          "Return processing frequency in Hertz.",
                                          {{ParameterType::DOUBLE, "processing frequency in Hertz"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("get_time_per_frame_physics",
                                          CCommand<double>([&]() -> double {return this->getTimePerFrame();}),
                                          "Return time available per frame (i.e. 1.0/Frequency).",
                                          {{ParameterType::DOUBLE, "Time available per frame"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("get_time_processed_physics",
                                          CCommand<double>([&]() -> double {return this->getTimeProcessed();}),
                                          "Return time used for processing.",
                                          {{ParameterType::DOUBLE, "Time used for processing"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("get_time_processed_physics_buffer_copy",
                                          CCommand<double>([&]() -> double {return this->getTimeProcessedBufferCopy();}),
                                          "Return time used for copying object buffers.",
                                          {{ParameterType::DOUBLE, "Time used for object buffer copy"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("get_time_processed_physics_objects",
                                          CCommand<double>([&]() -> double {return this->getTimeProcessedObjects();}),
                                          "Return time used for object processing.",
                                          {{ParameterType::DOUBLE, "Time used for object processing"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("get_time_processed_physics_particles",
                                          CCommand<double>([&]() -> double {return this->getTimeProcessedParticles();}),
                                          "Return time used for particle processing.",
                                          {{ParameterType::DOUBLE, "Time used for particle processing"}},
                                           "system"
                                         );
        m_pComInterface->registerFunction("particles_set_color_birth",
                                          CCommand<void, int, double, double, double, double>(
                                            [&](const int _nUID,
                                                const double& _fR,
                                                const double& _fG,
                                                const double& _fB,
                                                const double& _fA)
                                            {
                                                CParticle* pParticle = m_pDataStorage->getParticleByValueBack(_nUID);
                                                if (pParticle != nullptr)
                                                {
                                                    pParticle->setColorBirth({{_fR, _fG, _fB, _fA}});
                                                }
                                            }),
                                          "Sets color of given newly created particles.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Particle UID"},
                                           {ParameterType::DOUBLE, "Color of newly created particles"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("particles_set_color_death",
                                          CCommand<void, int, double, double, double, double>(
                                            [&](const int _nUID,
                                                const double& _fR,
                                                const double& _fG,
                                                const double& _fB,
                                                const double& _fA)
                                            {
                                                CParticle* pParticle = m_pDataStorage->getParticleByValueBack(_nUID);
                                                if (pParticle != nullptr)
                                                {
                                                    pParticle->setColorDeath({{_fR, _fG, _fB, _fA}});
                                                }
                                            }),
                                          "Sets color of oldest particles.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Particle UID"},
                                           {ParameterType::DOUBLE, "Color of oldest particles"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("particles_set_size_birth",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fSize)
                                            {
                                                CParticle* pParticle = m_pDataStorage->getParticleByValueBack(_nUID);
                                                if (pParticle != nullptr)
                                                {
                                                    pParticle->setSizeBirth(_fSize);
                                                }
                                            }),
                                          "Sets size of given newly created particles.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Particle UID"},
                                           {ParameterType::DOUBLE, "Size of newly created particles"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("particles_set_size_death",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fSize)
                                            {
                                                CParticle* pParticle = m_pDataStorage->getParticleByValueBack(_nUID);
                                                if (pParticle != nullptr)
                                                {
                                                    pParticle->setSizeDeath(_fSize);
                                                }
                                            }),
                                          "Sets size of oldest particles.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Particle UID"},
                                           {ParameterType::DOUBLE, "Size of oldest particles"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("particles_set_type",
                                          CCommand<void, int, std::string>(
                                            [&](const int _nUID, const std::string& _strType)
                                            {
                                                CParticle* pParticle = m_pDataStorage->getParticleByValueBack(_nUID);
                                                if (pParticle != nullptr)
                                                {
                                                    pParticle->setParticleType(mapStringToParticleType(_strType));
                                                }
                                            }),
                                          "Sets type of given particle.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Particle UID"},
                                           {ParameterType::STRING, "Particle type (" + ossParticleType.str() + " )"}},
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
                                                m_pDataStorage->AccessNames.lock();
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
                                                m_pDataStorage->AccessNames.unlock();
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
                                                    pObj->init();
                                                }
                                            }),
                                          "Add shape with given UID to object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"},
                                           {ParameterType::INT, "UID of shape to be added"}},
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("obj_add_shps",
                                          CCommand<void, int, std::vector<int>>(
                                            [&](const int _nUIDObj, const std::vector<int>& _Shapes)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUIDObj);
                                                
                                                if (pObj != nullptr)
                                                {
                                                    for (auto nUIDShp : _Shapes)
                                                    {
                                                        IShape* pShp = m_pDataStorage->getShapeByValue(nUIDShp);
                                                        if (pShp != nullptr)
                                                        {
                                                            pObj->getGeometry()->addShape(pShp);
                                                        }
                                                    }
                                                    pObj->init();
                                                }
                                            }),
                                          "Add shapes with given UIDs to object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"},
                                           {ParameterType::DYN_ARRAY, "UIDs of shapes to be added"}},
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
        m_pComInterface->registerFunction("set_frequency_physics",
                                          CCommand<void, double>([&](const double& _fFrequency)
                                          {
                                              this->setFrequency(_fFrequency);
                                              this->setFrequencyParticle(_fFrequency);
                                          }),
                                          "Sets the frequency of the physics thread.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Frequency"}},
                                           "system", "physics");
        m_pComInterface->registerFunction("thruster_add_emitter",
                                          CCommand<void, int, int>([&](const int _nUIDThr, const int _nUIDEm)
                                          {
                                            CThruster* pThruster = m_pDataStorage->getThrusterByValue(_nUIDThr);
                                            if (pThruster != nullptr)
                                            {
                                                IEmitter* pEmitter = m_pDataStorage->getEmitterByValue(_nUIDEm);
                                                if (pEmitter != nullptr)
                                                {
                                                    pThruster->addEmitter(pEmitter);
                                                }
                                            }
                                          }),
                                          "Add an emitter to thruster.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Thruster UID"},
                                          {ParameterType::INT, "Emitter UID"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("thruster_set_object",
                                          CCommand<void, int, int>([&](const int _nUIDThr, const int _nUIDObj)
                                          {
                                            CThruster* pThruster = m_pDataStorage->getThrusterByValue(_nUIDThr);
                                            if (pThruster != nullptr)
                                            {
                                                CObject* pObject = m_pDataStorage->getObjectByValueBack(_nUIDObj);
                                                if (pObject != nullptr)
                                                {
                                                    pThruster->setObject(pObject);
                                                }
                                            }
                                          }),
                                          "Attach thruster to object.",
                                          {{ParameterType::NONE, "No return value"},
                                          {ParameterType::INT, "Thruster UID"},
                                          {ParameterType::INT, "Object UID"}},
                                          "system", "physics"
                                          );
        m_pComInterface->registerFunction("toggle_pause",
                                          CCommand<void>([&](){this->togglePause();}),
                                          "Pauses or resumes physics simulation.",
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
        m_pComInterface->registerFunction("obj_disable_dynamics",
                                          CCommand<void, int>(
                                            [&](const int _nUID)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->disableDynamics();
                                                }
                                            }),
                                          "Disable dynamics for given object (i.e. object is fixed to its world coordinates).",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_disable_gravitation",
                                          CCommand<void, int>(
                                            [&](const int _nUID)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->disableGravitation();
                                                }
                                            }),
                                          "Disable gravition (object is source of gravitation) for given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_enable_dynamics",
                                          CCommand<void, int>(
                                            [&](const int _nUID)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->enableDynamics();
                                                }
                                            }),
                                          "Enable dynamics for given object (i.e. kinetics, object will react on forces).",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_enable_gravitation",
                                          CCommand<void, int>(
                                            [&](const int _nUID)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->enableGravitation();
                                                }
                                            }),
                                          "Enable gravition (object is source of gravitation) for given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "UID of object"}},
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
        m_pComInterface->registerFunction("obj_refer_state",
                                          CCommand<void, int, int>(
                                            [&](const int _nUID, const int _nUIDRef)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                CObject* pObjRef = m_pDataStorage->getObjectByValueBack(_nUIDRef);
                                                if (pObj != nullptr && pObjRef != nullptr)
                                                {
                                                    pObj->getKinematicsState().referTo(pObjRef->getKinematicsState());
                                                    pObj->init();
                                                }
                                            }),
                                          "Refer kinematics state to that of the other given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::INT, "Reference object UID"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_angle",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fAngle)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    pObj->setAngle(_fAngle);
                                                    pObj->init();
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
                                                    pObj->init();
                                                }
                                            }),
                                          "Sets residing grid cell of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::INT, "Cell X"},
                                           {ParameterType::INT, "Cell Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("obj_set_name",
                                          CCommand<void, int, std::string>(
                                            [&](const int _nUID, const std::string& _strName)
                                            {
                                                CObject* pObj = m_pDataStorage->getObjectByValueBack(_nUID);
                                                if (pObj != nullptr)
                                                {
                                                    std::string strNameOld = pObj->getName();
                                                    pObj->setName(_strName);
                                                    
                                                    m_pDataStorage->AccessNames.lock();
                                                    m_pDataStorage->getUIDsByName()->insert({_strName, pObj->getUID()});
                                                    m_pDataStorage->getUIDsByName()->erase(strNameOld);
                                                    m_pDataStorage->AccessNames.unlock();
                                                }
                                            }),
                                          "Sets name of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::STRING, "Objects name"}},
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
                                                    pObj->init();
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
                                            }),
                                          "Sets velocity of a given object.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Object UID"},
                                           {ParameterType::DOUBLE, "Velocity X"},
                                           {ParameterType::DOUBLE, "Velocity Y"}},
                                           "physics", "physics"
                                         );
        m_pComInterface->registerFunction("set_gravity_vector",
                                          CCommand<void, double, double>(
                                                [&](const double& _fGX, const double& _fGY)
                                                {
                                                    this->setConstantGravity(Vector2d(_fGX, _fGY));
                                                }),
                                          "Set a constant gravity vector that acts on all objects.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Gravity vector X"},
                                           {ParameterType::DOUBLE, "Gravity vector Y"}},
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
                                                    switch (pShp->getShapeType())
                                                    {
                                                        case ShapeType::CIRCLE:
                                                        {
                                                            static_cast<CCircle*>(pShp)->setRadius(_fRad);
                                                            break;
                                                        }
                                                        case ShapeType::PLANET:
                                                        {
                                                            static_cast<CPlanet*>(pShp)->setRadius(_fRad);
                                                            break;
                                                        }
                                                        default:
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
                                           "system", "physics"
                                         );
        m_pComInterface->registerFunction("thruster_set_thrust_max",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fThrustMax)
                                            {
                                                CThruster* pThruster= m_pDataStorage->getThrusterByValue(_nUID);
                                                if (pThruster != nullptr)
                                                {
                                                    pThruster->setThrustMax(_fThrustMax);
                                                }
                                            }),
                                          "Sets maximum thrust level to given value.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Thruster UID"},
                                           {ParameterType::DOUBLE, "Maximum thrust level"}},
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
        m_pComInterface->registerFunction("thruster_activate",
                                          CCommand<void, int, double>(
                                            [&](const int _nUID, const double& _fThrust)
                                            {
                                                CThruster* pThruster= m_pDataStorage->getThrusterByValue(_nUID);
                                                if (pThruster != nullptr)
                                                {
                                                    pThruster->activate(_fThrust);
                                                }
                                            }),
                                          "Activates thruster with given thrust.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Thruster UID"},
                                           {ParameterType::DOUBLE, "Thrust to be applied when activated"}},
                                          "sim", "physics"
                                         );
        m_pComInterface->registerFunction("thruster_deactivate",
                                          CCommand<void, int>(
                                            [&](const int _nUID)
                                            {
                                                CThruster* pThruster= m_pDataStorage->getThrusterByValue(_nUID);
                                                if (pThruster != nullptr)
                                                {
                                                    pThruster->deactivate();
                                                }
                                            }),
                                          "Deactivates given thruster.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::INT, "Thruster UID"}},
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
/// \brief Method that processes all entity and command queues
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::processQueues()
{
    METHOD_ENTRY("CPhysicsManager::processQueues")
    
    // Add new entities to world
    IEmitter* pEmitter = nullptr;
    while (m_EmittersToBeAddedToWorld.try_dequeue(pEmitter))
    {
        m_pDataStorage->addEmitter(pEmitter);
    }
    CObject* pObj = nullptr;
    while (m_ObjectsToBeAddedToWorld.try_dequeue(pObj))
    {
        m_pDataStorage->addObject(pObj);
    }
    CParticle* pParticle = nullptr;
    while (m_ParticlesToBeAddedToWorld.try_dequeue(pParticle))
    {
        m_pDataStorage->addParticle(pParticle);
    }
    IShape* pShp = nullptr;
    while (m_ShapesToBeAddedToWorld.try_dequeue(pShp))
    {
        m_pDataStorage->addShape(pShp);
    }
    CThruster* pThruster = nullptr;
    while (m_ThrustersToBeAddedToWorld.try_dequeue(pThruster))
    {
        m_pDataStorage->addThruster(pThruster);
    }
    m_pComInterface->callWriters("physics");
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
    
    if (m_bCellUpdateFirst)
    {
        // Make sure not to try to access ->begin() within empty vector
        if (nNrOfObj > 0)
        {
            m_nCellUpdateLast = m_pDataStorage->getObjectsByValueBack()->begin()->second->getUID();
            m_bCellUpdateFirst = false;
        }
    }
    else
    {
        auto it = m_pDataStorage->getObjectsByValueBack()->find(m_nCellUpdateLast);
        for (auto i=0u; i<nNrOfObj; ++i)
        {
            it->second->updateCell();
            if (++it == m_pDataStorage->getObjectsByValueBack()->end())
                it =  m_pDataStorage->getObjectsByValueBack()->begin();
        }
        m_nCellUpdateLast = it->second->getUID();
    }
}
