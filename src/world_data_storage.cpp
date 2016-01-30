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
/// \file       world_data_storage.cpp
/// \brief      Implementation of class "CWorldDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-07-07
///
////////////////////////////////////////////////////////////////////////////////

#include "world_data_storage.h"

#include "rigidbody.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CWorldDataStorage::CWorldDataStorage()                                  
{
    METHOD_ENTRY("CWorldDataStorage::CWorldDataStorage")
    CTOR_CALL("CWorldDataStorage::CWorldDataStorage")
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

    for (ObjectsType::iterator it = m_DynamicObjects.begin();
        it != m_DynamicObjects.end(); ++it)
    {
        // Free memory if pointer is still existent
        if (it->second != nullptr)
        {
            delete it->second;
            it->second = nullptr;
            MEM_FREED("IObject")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObject", "Memory already freed."))
        }
    }
    
    
    for (ObjectsType::iterator it = m_StaticObjects.begin();
        it != m_StaticObjects.end(); ++it)
    {
        // Free memory if pointer is still existent
        if (it->second != nullptr)
        {
            delete it->second;
            it->second = nullptr;
            MEM_FREED("IObject")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObject", "Memory already freed."))
        }
    }
    
    for (ObjectVisualsType::iterator it = m_ObjectVisuals.begin();
        it != m_ObjectVisuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
            MEM_FREED("IObjectVisuals")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObjectVisuals", "Memory already freed."))
        }
    }
        
    for (DebrisType::iterator it = m_Debris.begin();
        it != m_Debris.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
            MEM_FREED("CDebris")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CDebris", "Memory already freed."))
        }
    }
    
    for (DebrisVisualsType::iterator it = m_DebrisVisuals.begin();
        it != m_DebrisVisuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
            MEM_FREED("CDebrisVisuals")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CDebrisVisuals", "Memory already freed."))
        }
    }
    
    for (JointsType::iterator it = m_Joints.begin();
        it != m_Joints.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
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
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addDebris(CDebris* _pDebris)
{
    METHOD_ENTRY("CWorldDataStorage::addDebris")
    m_Debris.push_back(_pDebris);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a debris visuals to list
///
/// This method adds the given debris visual to the list of debris visuals.
///
/// \param _pDebrisVisuals Debris visual that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addDebrisVisuals(CDebrisVisuals* _pDebrisVisuals)
{
    METHOD_ENTRY("CWorldDataStorage::addDebrisVisuals")
    m_DebrisVisuals.push_back(_pDebrisVisuals);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a thruster debris visuals to list
///
/// This method adds the given thruster debris visual to the list of thruster
/// debris visuals.
///
/// \param _pDebrisVisualsThruster Thruster debris visual that should be added
///                                to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addDebrisVisualsThruster(CDebrisVisualsThruster* _pDebrisVisualsThruster)
{
    METHOD_ENTRY("CWorldDataStorage::addDebrisVisualsThruster")
    m_DebrisVisualsThruster.push_back(_pDebrisVisualsThruster);
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
/// This method adds the given object to the list of object, depending on their
/// dynamics state. 
///
/// \param _pObject Mass that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addObject(IObject* _pObject)
{
    METHOD_ENTRY("CWorldDataStorage::addObject")

    m_ObjectMutex.lock();
    
    // Since objects are stored in a map with their name as the key, we have to
    // take care not to use the same name more than once (it's not a multimap,
    // hence, it has to be unique).
    
    std::string strName = _pObject->getName();
    
    if (_pObject->getDynamicsState())
    {
        while (!m_DynamicObjects.insert({_pObject->getName(), _pObject}).second)
        {
            _pObject->setName(_pObject->getName() + "_" + std::to_string(CRigidBody::getCount()));
        }
    }
    else
        while (!m_StaticObjects.insert({_pObject->getName(), _pObject}).second)
        {
            _pObject->setName(_pObject->getName() + "_" + std::to_string(CRigidBody::getCount()));
        }
    m_ObjectMutex.unlock();
}

// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Add a object to list
// ///
// /// This method adds the given object to the list of object, depending on their
// /// dynamics state. 
// ///
// /// \param _pObject Object that should be added to list
// ///
// ///////////////////////////////////////////////////////////////////////////////
// void CWorldDataStorage::addObject(IObject* _pObject)
// {
//     METHOD_ENTRY("CWorldDataStorage::addObject")
// 
//     m_ObjectMutex.lock();
//     
//     if (_pObject->getDynamicsState())
//         m_DynamicObjects.insert({_pObject->getID(), _pObject});
//     else
//         m_StaticObjects.insert({_pObject->getID(), _pObject});
//     m_ObjectMutex.unlock();
// }


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a list of objects to internal object list
///
/// This method adds the given objects to the list of object, depending on their
/// dynamics state. 
///
/// \param _Objects Objects that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addObjects(ObjectsType _Objects)
{
    METHOD_ENTRY("CWorldDataStorage::addObjects")

    ObjectsType::const_iterator ci = _Objects.begin();
  
    while (ci != _Objects.end())
    {
        this->addObject(ci->second);
        ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a object visuals to internal list
///
/// \param _pObjectVisuals Objects visuals that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::addObjectVisuals(IObjectVisuals* _pObjectVisuals)
{
    METHOD_ENTRY("CWorldDataStorage::addObjectVisuals")
    m_ObjectVisualsMutex.lock();
    m_ObjectVisuals.push_back(_pObjectVisuals);
    m_ObjectVisualsMutex.unlock();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Store index of specific object
///
/// \param _strRef Reference name to access object
/// \param _It Iterator pointing to object to be store
///
///////////////////////////////////////////////////////////////////////////////
void CWorldDataStorage::memorizeDynamicObject(const std::string& _strRef,
                                              const ObjectsType::const_iterator _It)
{
    METHOD_ENTRY("CWorldDataStorage::memorizeDynamicObject")
    m_DynamicObjectsMemory[_strRef] = _It;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Get index of specific memorized object
///
/// \param _strRef Reference name to access object
///
/// \return Iterator pointing to memorized object
///
///////////////////////////////////////////////////////////////////////////////
const ObjectsType::const_iterator CWorldDataStorage::recallDynamicObject(const std::string& _strRef)
{
    METHOD_ENTRY("CWorldDataStorage::recallDynamicObject")
    return m_DynamicObjectsMemory.at(_strRef);
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
    
    ObjectsType::size_type nSize;
    _is >> nSize;
    
    std::cout << "Number of objects: " << nSize << std::endl;
    for (auto i=0u; i<nSize; ++i)
    {
//         std::string::size_type strSize;
//         _is >> strSize;
//         std::cout << strSize << std::endl;
        std::string strName;
        _is >> strName;
        std::cout << strName << std::endl;
        CRigidBody* pObj = new CRigidBody;
        _is >> *pObj;
        std::cout << *pObj << std::endl;
//         _WDS.addObject();
        Log.progressBar("Loading dynamic objects", i, nSize);
        usleep(10000);
    }
        
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

    auto nSize=_WDS.m_DynamicObjects.size();
    _os << nSize << std::endl;
    std::uint32_t i=0u;
    for (auto ci : _WDS.m_DynamicObjects)
    {
        _os << ci.first << std::endl;
        _os << *(ci.second) << std::endl;
        Log.progressBar("Saving dynamic objects", i++, nSize);
        usleep(10000);
    }
    _os << _WDS.m_StaticObjects.size();
    for (auto ci : _WDS.m_StaticObjects)
    {
        _os << *(ci.second);
    }
//     _os << _WDS.m_ObjectVisuals.size();
//     for (auto ci : _WDS.m_ObjectVisuals)
//     {
//         _os << *(ci.second);
//     }    

    return _os;
}
