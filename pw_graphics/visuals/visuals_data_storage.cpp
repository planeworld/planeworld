////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2016 Torsten Büschenfeld
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
/// \file       visuals__data_storage.cpp
/// \brief      Implementation of class "CVisualsDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-28
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals_data_storage.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsDataStorage::CVisualsDataStorage() 
//                                         : m_pCamera(nullptr)
{
    METHOD_ENTRY("CVisualsDataStorage::CVisualsDataStorage")
    CTOR_CALL("CVisualsDataStorage::CVisualsDataStorage")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CVisualsDataStorage::~CVisualsDataStorage()
{
    METHOD_ENTRY("CVisualsDataStorage::~CVisualsDataStorage")
    DTOR_CALL("CVisualsDataStorage::~CVisualsDataStorage")
    
//     // Free memory if pointer is still existent
//     if (m_pCamera != nullptr)
//     {
//         delete m_pCamera;
//         m_pCamera = nullptr;
//         MEM_FREED("CCamera");
//     }
    
    for (auto Cam : m_CamerasByName)
    {
        if (Cam.second != nullptr)
        {
            delete Cam.second;
            Cam.second = nullptr;
            MEM_FREED("CCamera")
        }
    }
    
    for (auto pObjVis : m_ObjectVisuals)
    {
        if (pObjVis != nullptr)
        {
            delete pObjVis;
            pObjVis = nullptr;
            MEM_FREED("IObjectVisuals")
        }
    }
    
    // for (auto it : m_UIDUserRef)
    // Do not delete objects here.
    // Object pointers are stored in two maps, referring to their name and
    // referring to their UID. Thus, they only need to be destroyed once.

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
    
    for (DebrisVisualsThrusterType::iterator it = m_DebrisVisualsThruster.begin();
        it != m_DebrisVisualsThruster.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
            MEM_FREED("CDebrisVisualsThruster")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CDebrisVisualsThruster", "Memory already freed."))
        }
    }
    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a camera
///
/// This method adds the given camera to the list of available cameras
///
/// \param _pCamera Camera to be added
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addCamera(CCamera* _pCamera)
{
    METHOD_ENTRY("CVisualsDataStorage::addCamera")
    
    m_CamerasByName.insert({_pCamera->getName(), _pCamera});
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
void CVisualsDataStorage::addDebrisVisuals(CDebrisVisuals* _pDebrisVisuals)
{
    METHOD_ENTRY("CVisualsDataStorage::addDebrisVisuals")
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
void CVisualsDataStorage::addDebrisVisualsThruster(CDebrisVisualsThruster* _pDebrisVisualsThruster)
{
    METHOD_ENTRY("CVisualsDataStorage::addDebrisVisualsThruster")
    m_DebrisVisualsThruster.push_back(_pDebrisVisualsThruster);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _WDS CVisualsDataStorage instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CVisualsDataStorage& _WDS)
{
    METHOD_ENTRY("CVisualsDataStorage::operator>>")
    
//     // Temporary reference to shape by UID to connect visuals with their shapes,
//     // after sequentually loading shapes and then visuals.
//     std::unordered_map<UIDType, IShape*> UIDShapeRef;
    
    for (auto it : _WDS.m_ObjectVisuals)
    {
        if (it != nullptr)
        {
            delete it;
            it = nullptr;
            MEM_FREED("IObjectVisuals")
        }
    }
    _WDS.m_ObjectVisuals.clear();

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
    
    // Thruster Visuals!!!
    
//     _WDS.m_UIDUserRef.clear();
    
//     //-------------------------------------------------------------------------
//     // Stream in object visuals
//     //-------------------------------------------------------------------------
//     {
//         ObjectVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("Visuals data storage", "Number of visuals to load: " << nSize))
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
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _WDS CVisualsDataStorage instance to stream
///
/// \return Stream with game state information of CVisualsDataStorage instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CVisualsDataStorage& _WDS)
{
    METHOD_ENTRY("CVisualsDataStorage::operator<<")

    _os << _WDS.m_ObjectVisuals.size() << std::endl;
    for (const auto ci : _WDS.m_ObjectVisuals)
    {
        _os << ci << std::endl;
    }

//     _os << _WDS.m_pCamera << std::endl;
    
    /// \todo Implement DebrisEmitter serialisation to make debris work
    return _os;
}
