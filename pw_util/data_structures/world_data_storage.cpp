////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       world_data_storage.cpp
/// \brief      Implementation of class "CWorldDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-07-07
///
////////////////////////////////////////////////////////////////////////////////

#include "world_data_storage.h"

#include "camera.h"
#include "emitter.h"
#include "particle.h"
#include "object.h"
#include "thruster.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CWorldDataStorage::CWorldDataStorage() : m_pUniverse(nullptr),
                                         m_bFrontNew(false),
                                         m_fTimeScale(1.0)                                 
{
    METHOD_ENTRY("CWorldDataStorage::CWorldDataStorage")
    CTOR_CALL("CWorldDataStorage::CWorldDataStorage")
    for (auto i=0u; i<m_UIDUsersByValue.getBufferSize(); ++i)
        m_UIDUsersByValue.getBuffer(i)->resize(WDS_DEFAULT_UID_BUFFER_SIZE);
    
    // Setup a clean environment
    for (auto i=0u; i<m_UIDUsersByValue.getBufferSize(); ++i)
        std::fill(m_UIDUsersByValue.getBuffer(i)->begin(), m_UIDUsersByValue.getBuffer(i)->end(), nullptr);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CWorldDataStorage::~CWorldDataStorage()
{
    METHOD_ENTRY("CWorldDataStorage::~CWorldDataStorage")
    DTOR_CALL("CWorldDataStorage::~CWorldDataStorage")
    
    if (m_pUniverse != nullptr)
    {
        delete m_pUniverse;
        MEM_FREED("CUniverse")
        m_pUniverse = nullptr;
    }
    
    for (auto pEmitter : m_EmittersByValue)
    {
        
        // Free memory if pointer is still existent
        if (pEmitter.second != nullptr)
        {
            delete pEmitter.second;
            pEmitter.second = nullptr;
            MEM_FREED("IEmitter")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IEmitter", "Memory already freed."))
        }
    }
    
    for (auto i=0u; i<m_ObjectsByValue.getBufferSize(); ++i)
    {
        for (auto pObj : *m_ObjectsByValue.getBuffer(i))
        {
            // Free memory if pointer is still existent
            if (pObj.second != nullptr)
            {
                delete pObj.second;
                pObj.second = nullptr;
                MEM_FREED("CObject")
            }
            else
            {
                DOM_MEMF(DEBUG_MSG("CObject", "Memory already freed."))
            }
        }
    }
    
    for (auto i=0u; i<m_ParticlesByValue.getBufferSize(); ++i)
    {
        for (auto pParticle : *m_ParticlesByValue.getBuffer(i))
        {
            // Free memory if pointer is still existent
            if (pParticle.second != nullptr)
            {
                delete pParticle.second;
                pParticle.second = nullptr;
                MEM_FREED("CParticle")
            }
            else
            {
                DOM_MEMF(DEBUG_MSG("CParticle", "Memory already freed."))
            }
        }
    }
    
    for (auto pJoint : m_Joints)
    {
        // Free memory if pointer is still existent
        if (pJoint != nullptr)
        {
            delete pJoint;
            pJoint = nullptr;
            MEM_FREED("IJoint")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IJoint", "Memory already freed."))
        }
    }
    
    for (auto pThruster : m_ThrustersByValue)
    {
        // Free memory if pointer is still existent
        if (pThruster.second != nullptr)
        {
            delete pThruster.second;
            pThruster.second = nullptr;
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
/// \brief Add an emitter to list
///
/// This method adds the given emitter to the list of emitters.
///
/// \param _pEmitter Emitter that should be added to list
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addEmitter(IEmitter* _pEmitter)
{
    METHOD_ENTRY("CWorldDataStorage::addEmitter")
    
    m_EmittersByValue.insert({_pEmitter->getUID(), _pEmitter});
    
    if (!this->addUIDUser(_pEmitter)) return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a particle to list
///
/// This method adds the given particle object to the list of particles.
///
/// \param _pParticle Particle that should be added to list
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addParticle(CParticle* _pParticle)
{
    METHOD_ENTRY("CWorldDataStorage::addParticle")
    
    std::array<CParticle*, BUFFER_QUADRUPLE> aParticle =
    {{_pParticle->clone(),_pParticle->clone(),_pParticle->clone(),_pParticle}};
    
    std::array<IUIDUser*, BUFFER_QUADRUPLE> aUIDUsers =
    {{aParticle[0],aParticle[1],aParticle[2],aParticle[3]}};
    
    m_ParticlesByName.add(_pParticle->getName(), aParticle);
    m_ParticlesByValue.add(_pParticle->getUID(), aParticle);
    
    if (!this->addUIDUser(aUIDUsers)) return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a joint to list
///
/// This method adds the given joint to the list of joints.
///
/// \param _pJoint Joint that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addJoint(IJoint* _pJoint)
{
    METHOD_ENTRY("CWorldDataStorage::addJoint")

    m_Joints.push_back(_pJoint);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a object to list
///
/// This method adds the given object to the list of object.
///
/// \param _pObject Mass that should be added to list
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addObject(CObject* _pObject)
{
    METHOD_ENTRY("CWorldDataStorage::addObject")

    std::array<CObject*, BUFFER_QUADRUPLE> aObjects =
    {{_pObject->clone(),_pObject->clone(),_pObject->clone(),_pObject}};
    std::array<IUIDUser*, BUFFER_QUADRUPLE> aUIDUsers =
    {{aObjects[0], aObjects[1], aObjects[2], aObjects[3]}};
    std::array<IUIDUser*, BUFFER_QUADRUPLE> aUIDUsersKinState =
    {{&aObjects[0]->getKinematicsState(), &aObjects[1]->getKinematicsState(),
      &aObjects[2]->getKinematicsState(), &aObjects[3]->getKinematicsState()}};
    
    // Initialise new objects
    for (auto pObj : aObjects) pObj->init();
    
    m_ObjectsByValue.add(_pObject->getUID(), aObjects);
    
    for (auto Shape : _pObject->getGeometry()->getShapes())
    {
        m_ShapesByValue.insert(std::pair<UIDType, IShape*>(Shape->getUID(), Shape));
    }
    
    if (!this->addUIDUser(aUIDUsersKinState)) {}
    if (!this->addUIDUser(aUIDUsers)) return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Explicitely add a shape to data storage
///
/// This method adds the given shape to data storage, which is normally done
/// by its object (\ref addObject). It is needed, if shapes are created and
/// externally while not added to an object or if the shape is added for an
/// object that is already registered to data storage.
///
/// \param _pShape Shape that should be added to map
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addShape(IShape* _pShape)
{
    METHOD_ENTRY("CWorldDataStorage::addShape")
    m_ShapesByValue.insert(std::pair<UIDType, IShape*>(_pShape->getUID(), _pShape));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a thruster to list
///
/// This method adds the given thruster to the list of thrusters.
///
/// \param _pThruster Thruster that should be added to list
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addThruster(CThruster* _pThruster)
{
    METHOD_ENTRY("CWorldDataStorage::addThruster")

    m_ThrustersByValue.insert({_pThruster->getUID(), _pThruster});
    
    if (!this->addUIDUser(_pThruster)) return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates an object whichs structure was modified externally
///
/// This method updates an object if structural modifications where done. At
/// the moment, this applies for adding a new shape, which than has to be
/// added to the map of shapes.
///
/// \param _nUID UID of object that should be updated
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::updateObject(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::updateObject")

    for (auto pShape : m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->at(_nUID)->getGeometry()->getShapes())
    {
        if (m_ShapesByValue.find(pShape->getUID()) == m_ShapesByValue.end())
        {
            m_ShapesByValue.insert(std::pair<UIDType, IShape*>(pShape->getUID(), pShape));
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return emitter, accessed by given UID value
///
/// \param _nUID UID of emitter to return
///
/// \return Emitter with given UID value
///
////////////////////////////////////////////////////////////////////////////////
IEmitter* CWorldDataStorage::getEmitterByValue(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getEmitterByValue")

    const auto ci = m_EmittersByValue.find(_nUID);
    if (ci != m_EmittersByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown emitter with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return object, accessed by given UID value
///
/// \param _nUID UID of object to return
///
/// \return Object with given UID value
///
////////////////////////////////////////////////////////////////////////////////
CObject* CWorldDataStorage::getObjectByValueBack(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getObjectByValueBack")

    const auto ci = m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->find(_nUID);
    if (ci != m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown object with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return object, accessed by given UID value
///
/// \param _nUID UID of object to return
///
/// \return Object with given UID value
///
////////////////////////////////////////////////////////////////////////////////
CObject* CWorldDataStorage::getObjectByValueFront(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getObjectByValueFront")

    const auto ci = m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_FRONT)->find(_nUID);
    if (ci != m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_FRONT)->end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown object with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return particle, accessed by given UID value
///
/// \param _nUID UID of particle to return
///
/// \return Particle with given UID value
///
////////////////////////////////////////////////////////////////////////////////
CParticle* CWorldDataStorage::getParticleByValueBack(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getParticleByValueBack")

    const auto ci = m_ParticlesByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->find(_nUID);
    if (ci != m_ParticlesByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown particle with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return shape, accessed by given UID value
///
/// \param _nUID UID of shape to return
///
/// \return Shape with given UID value
///
////////////////////////////////////////////////////////////////////////////////
IShape* CWorldDataStorage::getShapeByValue(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getShapeByValue")

    const auto ci = m_ShapesByValue.find(_nUID);
    if (ci != m_ShapesByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown shape with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return thruster, accessed by given UID value
///
/// \param _nUID UID of thruster to return
///
/// \return Thruster with given UID value
///
////////////////////////////////////////////////////////////////////////////////
CThruster* CWorldDataStorage::getThrusterByValue(const UIDType _nUID)
{
    METHOD_ENTRY("CWorldDataStorage::getThrusterByValue")

    const auto ci = m_ThrustersByValue.find(_nUID);
    if (ci != m_ThrustersByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("World Data Storage", "Unknown thruster with UID <" << _nUID << ">")
        return nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a UID user to buffer
///
/// \param _pUIDUser UID user to add to buffer
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addUIDUser(IUIDUser* _pUIDUser)
{
    METHOD_ENTRY("CWorldDataStorage::addUIDUser")

    if (_pUIDUser->getUID() >= m_UIDUsersByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->size())
    {
        WARNING_MSG("World Data Storage", "Buffer too small, to many UID users, " <<
                                            "cannot add object."
        )
        return false;
    }
    else
    {
        m_UIDUsersByValue.setAt(_pUIDUser->getUID(), _pUIDUser);
        m_UIDsByName.insert(std::pair<std::string,UIDType>(_pUIDUser->getName(), _pUIDUser->getUID()));
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swaps back buffer for all internal buffers
///
////////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::swapBack()
{
    METHOD_ENTRY("CWorldDataStorage::swapBack")

    std::lock_guard<std::mutex> lock(m_MutexFrontNew);
    
    m_ParticlesByName.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    m_ParticlesByValue.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    m_ParticlesByValue.copyDeep(BUFFER_QUADRUPLE_BACK, BUFFER_QUADRUPLE_MIDDLE_BACK);

    m_ObjectsByValue.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    m_UIDUsersByValue.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    
    m_ObjectsByValue.copyDeep(BUFFER_QUADRUPLE_BACK, BUFFER_QUADRUPLE_MIDDLE_BACK);
    
    m_bFrontNew = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swaps front buffer for all internal buffers
///
////////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::swapFront()
{
    METHOD_ENTRY("CWorldDataStorage::swapFront")
   
    std::lock_guard<std::mutex> lock(m_MutexFrontNew);
    
    if (m_bFrontNew)
    {
        m_ParticlesByName.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
        m_ParticlesByValue.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
        m_ObjectsByValue.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
        m_UIDUsersByValue.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
        m_bFrontNew = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _WDS CWorldDataStorage instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CWorldDataStorage& _WDS)
{
    METHOD_ENTRY("CWorldDataStorage::operator>>")
    
    // Temporary reference to shape by UID to connect visuals with their shapes,
    // after sequentually loading shapes and then visuals.
    std::unordered_map<UIDType, IShape*> UIDShapeRef;
    
//     // Delete all old data first
//     for (auto pObj : *_WDS.m_ObjectsByValue.getBack())
//     {
//         if (pObj != nullptr)
//         {
//             delete pObj;
//             pObj = nullptr;
//             MEM_FREED("CObject")
//         }
//     }
//     _WDS.m_ObjectsByValue.getBack()->clear();
//     
//     for (auto it : _WDS.m_ObjectVisuals)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("IObjectVisuals")
//         }
//     }
//     _WDS.m_ObjectVisuals.clear();
//     for (auto it : _WDS.m_Particle)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CParticle")
//         }
//     }
//     _WDS.m_Particle.clear();
//     for (auto it : _WDS.m_ParticleVisuals)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CParticleVisuals")
//         }
//     }
//     _WDS.m_ParticleVisuals.clear();
    
//     _WDS.m_UIDUserRef.clear();
    
    
//     //-------------------------------------------------------------------------
//     // Stream in object visuals
//     //-------------------------------------------------------------------------
//     {
//         ObjectVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of visuals to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             IObjectVisuals* pObjVis = new IObjectVisuals;
//             MEM_ALLOC("CObjectVisuals")
//             _is >> pObjVis;
//             
//             UIDType UID = pObjVis->getUIDRef();
//             IUIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pObjVis->setRef(static_cast<CObject*>(pUIDUser));
//             _WDS.addObjectVisuals(pObjVis);
//             
// //             for (const auto ci : pObjVis->getShapeVisuals())
// //             {
// //                 ci->setRef(UIDShapeRef[ci->getUIDRef()]);
// //             }
//             
//             Log.progressBar("Loading object visuals", i, nSize);
//         }
//     }
    
//     //-------------------------------------------------------------------------
//     // Stream in camera
//     //-------------------------------------------------------------------------
//     {
//         if (_WDS.m_pCamera != nullptr)
//         {
//             delete _WDS.m_pCamera;
//             MEM_FREED("CCamera")
//             _WDS.m_pCamera = nullptr;
//         }
//         _WDS.m_pCamera = new CCamera;
//         MEM_ALLOC("CCamera")
//         _is >> _WDS.m_pCamera;
// 
//         /// \todo Clean up camera hooks via kinematics states and uids.
//         if (_WDS.m_pCamera->getKinematicsState().hasReference())
//         {
//             UIDType UID = _WDS.m_pCamera->getKinematicsState().getUIDRef();
//             IUIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             _WDS.m_pCamera->hook(static_cast<IKinematicsStateUser*>(static_cast<CObject*>(pUIDUser)));
//         }
//     }
    
//     //-------------------------------------------------------------------------
//     // Stream in all particle
//     //-------------------------------------------------------------------------
//     {
//         ParticleType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of particle to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CParticle* pParticle = new CParticle;
//             MEM_ALLOC("CParticle")
//             _is >> pParticle;
//             
//             _WDS.addParticle(pParticle);
//             
//             Log.progressBar("Loading particle", i, nSize);
//         }
//     }
//     
//     //-------------------------------------------------------------------------
//     // Stream in particle visuals
//     //-------------------------------------------------------------------------
//     {
//         ParticleVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of particle visuals to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CParticleVisuals* pDebVis = new CParticleVisuals;
//             MEM_ALLOC("CParticleVisuals")
//             _is >> pDebVis;
//             
//             UIDType UID = pDebVis->getUIDRef();
//             IUIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pDebVis->setRef(static_cast<CParticle*>(pUIDUser));
//             _WDS.addParticleVisuals(pDebVis);
//             
//             Log.progressBar("Loading particle visuals", i, nSize);
//         }
//     }

    /// \todo Implement ParticleEmitter serialisation to make particle work
    
    _is >> _WDS.m_fTimeScale;
        
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _WDS CWorldDataStorage instance to stream
///
/// \return Stream with game state information of CWorldDataStorage instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CWorldDataStorage& _WDS)
{
    METHOD_ENTRY("CWorldDataStorage::operator<<")

//     auto nSize=_WDS.m_ObjectsByValue.getBack()->size();
//     _os << nSize << std::endl;
//     
//     std::uint32_t i=0u;
//     for (const auto pObj : *_WDS.m_ObjectsByValue.getBack())
//     {
//         _os << pObj << std::endl;
//         _os << pObj << std::endl;
//         Log.progressBar("Saving dynamic objects", i++, nSize);
//     }
//     _os << _WDS.m_ObjectVisuals.size() << std::endl;
//     for (const auto ci : _WDS.m_ObjectVisuals)
//     {
//         _os << ci << std::endl;
//     }

//     _os << _WDS.m_pCamera << std::endl;
    
//     _os << _WDS.m_Particle.size() << std::endl;
//     for (const auto ci : _WDS.m_Particle)
//     {
//         _os << ci << std::endl;
//     }
//     _os << _WDS.m_ParticleVisuals.size() << std::endl;
//     for (const auto ci : _WDS.m_ParticleVisuals)
//     {
//         _os << ci << std::endl;
//     }
    /// \todo Implement ParticleEmitter serialisation to make particle work
    
    _os << _WDS.m_fTimeScale << std::endl;
    
    return _os;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a UID user to buffer, buffered elements are given
///
/// \param _aUIDUser UID user with buffered elements to add to buffer
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addUIDUser(const std::array<IUIDUser*, BUFFER_QUADRUPLE>& _aUIDUser)
{
    METHOD_ENTRY("CWorldDataStorage::addUIDUser")

    if (_aUIDUser[0]->getUID() >= m_UIDUsersByValue.getBuffer(BUFFER_QUADRUPLE_BACK)->size())
    {
        WARNING_MSG("World Data Storage", "Buffer too small, to many UID users, " <<
                                            "cannot add object."
        )
        return false;
    }
    else
    {
        m_UIDUsersByValue.setAt(_aUIDUser[0]->getUID(), _aUIDUser);
        m_UIDsByName.insert(std::pair<std::string,UIDType>(_aUIDUser[0]->getName(), _aUIDUser[0]->getUID()));
        return true;
    }
}

