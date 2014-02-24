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
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IObject*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObject*", "Memory already freed."))
        }
    };
    
    
    for (ObjectsType::iterator it = m_StaticObjects.begin();
        it != m_StaticObjects.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IObject*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObject*", "Memory already freed."))
        }
    };
        
    for (DebrisType::iterator it = m_Debris.begin();
        it != m_Debris.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("CDebris*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CDebris*", "Memory already freed."))
        }
    };
    
    for (JointsType::iterator it = m_Joints.begin();
        it != m_Joints.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IJoint*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IJoint*", "Memory already freed."))
        }
    };
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

    if (_pObject->getDynamicsState())
        m_DynamicObjects.push_back(_pObject);
    else
        m_StaticObjects.push_back(_pObject);
}

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
        if ((*ci)->getDynamicsState())
            m_DynamicObjects.push_back((*ci));
        else
            m_StaticObjects.push_back((*ci));
        ++ci;
    }
}
