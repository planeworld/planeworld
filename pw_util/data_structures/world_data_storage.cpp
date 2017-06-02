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
#include "debris.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CWorldDataStorage::CWorldDataStorage() : m_bFrontNew(false),
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
    
    for (auto i=0u; i<m_DebrisByValue.getBufferSize(); ++i)
    {
        for (auto pDebris : *m_DebrisByValue.getBuffer(i))
        {
            // Free memory if pointer is still existent
            if (pDebris.second != nullptr)
            {
                delete pDebris.second;
                pDebris.second = nullptr;
                MEM_FREED("CDebris")
            }
            else
            {
                DOM_MEMF(DEBUG_MSG("CDebris", "Memory already freed."))
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a debris to list
///
/// This method adds the given debris object to the list of debris.
///
/// \param _pDebris Debris that should be added to list
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addDebris(CDebris* _pDebris)
{
    METHOD_ENTRY("CWorldDataStorage::addDebris")
    
    std::array<CDebris*, BUFFER_QUADRUPLE> aDebris =
    {_pDebris->clone(),_pDebris->clone(),_pDebris->clone(),_pDebris};
    
    std::array<IUniqueIDUser*, BUFFER_QUADRUPLE> aUIDUsers =
    {aDebris[0],aDebris[1],aDebris[2],aDebris[3]};
    
    m_DebrisByName.add(_pDebris->getName(), aDebris);
    m_DebrisByValue.add(_pDebris->getUID(), aDebris);
    
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
    {_pObject->clone(),_pObject->clone(),_pObject->clone(),_pObject};
    std::array<IUniqueIDUser*, BUFFER_QUADRUPLE> aUIDUsers =
    {aObjects[0], aObjects[1], aObjects[2], aObjects[3]};
    
    // Initialise new objects
    for (auto pObj : aObjects) pObj->init();
    
    m_ObjectsByName.add(_pObject->getName(),aObjects);
    m_ObjectsByValue.add(_pObject->getUID(), aObjects);
    
    for (auto Shape : _pObject->getGeometry()->getShapes())
    {
        m_ShapesByValue.insert(std::pair<UIDType, IShape*>(Shape->getUID(), Shape));
    }
    
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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a UID user to buffer
///
/// \param _pUIDUser UID user to add to buffer
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CWorldDataStorage::addUIDUser(IUniqueIDUser* _pUIDUser)
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

    m_DebrisByName.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    m_DebrisByValue.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
    
    m_DebrisByValue.copyDeep(BUFFER_QUADRUPLE_BACK, BUFFER_QUADRUPLE_MIDDLE_BACK);
//     m_EmittersByValue.copyDeep(BUFFER_TRIPLE_BACK, BUFFER_TRIPLE_MIDDLE);
    m_ObjectsByName.swap(BUFFER_QUADRUPLE_MIDDLE_BACK, BUFFER_QUADRUPLE_MIDDLE_FRONT);
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
        m_DebrisByName.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
        m_DebrisByValue.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
//         m_EmittersByValue.swap(BUFFER_TRIPLE_MIDDLE, BUFFER_TRIPLE_FRONT);
        m_ObjectsByName.swap(BUFFER_QUADRUPLE_MIDDLE_FRONT, BUFFER_QUADRUPLE_FRONT);
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
//     for (auto it : _WDS.m_Debris)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CDebris")
//         }
//     }
//     _WDS.m_Debris.clear();
//     for (auto it : _WDS.m_DebrisVisuals)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CDebrisVisuals")
//         }
//     }
//     _WDS.m_DebrisVisuals.clear();
    
    _WDS.m_UIDUserRef.clear();
    
    
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
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pObjVis->attachTo(static_cast<CObject*>(pUIDUser));
//             _WDS.addObjectVisuals(pObjVis);
//             
// //             for (const auto ci : pObjVis->getShapeVisuals())
// //             {
// //                 ci->attachTo(UIDShapeRef[ci->getUIDRef()]);
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
//         if (_WDS.m_pCamera->getKinematicsState().gotReference())
//         {
//             UIDType UID = _WDS.m_pCamera->getKinematicsState().getUIDRef();
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             _WDS.m_pCamera->hook(static_cast<IKinematicsStateUser*>(static_cast<CObject*>(pUIDUser)));
//         }
//     }
    
//     //-------------------------------------------------------------------------
//     // Stream in all debris
//     //-------------------------------------------------------------------------
//     {
//         DebrisType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of debris to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CDebris* pDebris = new CDebris;
//             MEM_ALLOC("CDebris")
//             _is >> pDebris;
//             
//             _WDS.addDebris(pDebris);
//             
//             Log.progressBar("Loading debris", i, nSize);
//         }
//     }
//     
//     //-------------------------------------------------------------------------
//     // Stream in debris visuals
//     //-------------------------------------------------------------------------
//     {
//         DebrisVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of debris visuals to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CDebrisVisuals* pDebVis = new CDebrisVisuals;
//             MEM_ALLOC("CDebrisVisuals")
//             _is >> pDebVis;
//             
//             UIDType UID = pDebVis->getUIDRef();
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pDebVis->attachTo(static_cast<CDebris*>(pUIDUser));
//             _WDS.addDebrisVisuals(pDebVis);
//             
//             Log.progressBar("Loading debris visuals", i, nSize);
//         }
//     }

    /// \todo Implement DebrisEmitter serialisation to make debris work
    
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
    
//     _os << _WDS.m_Debris.size() << std::endl;
//     for (const auto ci : _WDS.m_Debris)
//     {
//         _os << ci << std::endl;
//     }
//     _os << _WDS.m_DebrisVisuals.size() << std::endl;
//     for (const auto ci : _WDS.m_DebrisVisuals)
//     {
//         _os << ci << std::endl;
//     }
    /// \todo Implement DebrisEmitter serialisation to make debris work
    
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
bool CWorldDataStorage::addUIDUser(const std::array<IUniqueIDUser*, BUFFER_QUADRUPLE>& _aUIDUser)
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
