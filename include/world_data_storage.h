////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2016 Torsten Büschenfeld
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
/// \file       world_data_storage.h
/// \brief      Prototype of class "CWorldDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef WORLD_DATA_STORAGE_H
#define WORLD_DATA_STORAGE_H

//--- Standard header --------------------------------------------------------//
#include <mutex>
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "debris_visuals.h"
#include "debris_visuals_thruster.h"
#include "joint.h"
#include "object_visuals.h"

typedef std::list<IJoint*>                      JointsType;                 ///< Specifies a list of joints
typedef std::vector<CDebris*>                   DebrisType;                 ///< Specifies a list of debris
typedef std::vector<CDebrisVisuals*>            DebrisVisualsType;          ///< Specifies a list of debris visuals
typedef std::list<CDebrisVisualsThruster*>      DebrisVisualsThrusterType;  ///< Specifies a list of debris visuals
typedef std::vector<IObjectVisuals*>            ObjectVisualsType;          ///< Specifies a list of object visuals

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that stores all data, e.g. physics objects, visuals, etc.
///
////////////////////////////////////////////////////////////////////////////////
class CWorldDataStorage
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CWorldDataStorage();
        ~CWorldDataStorage();
        
        //--- Constant Methods -----------------------------------------------//
        CCamera* const                      getCamera() const;
        const DebrisType&                   getDebris() const;
        const DebrisVisualsType&            getDebrisVisuals() const;
        const DebrisVisualsThrusterType&    getDebrisVisualsThruster() const;
        const JointsType&                   getJoints() const;
        const ObjectsType&                  getDynamicObjects();
        const ObjectsType&                  getStaticObjects();
        const ObjectVisualsType&            getObjectVisuals();
        
        const ObjectsType::const_iterator recallDynamicObject(const std::string&);
        
        //--- Methods --------------------------------------------------------//
        void addDebris(CDebris*);
        void addDebrisVisuals(CDebrisVisuals*);
        void addDebrisVisualsThruster(CDebrisVisualsThruster*);
        void addJoint(IJoint*);
        void addObject(IObject*);
        void addObjects(ObjectsType);
        void addObjectVisuals(IObjectVisuals*);
        
        void memorizeDynamicObject(const std::string&,
                                   const ObjectsType::const_iterator);
        
        void lockObjects(){m_ObjectMutex.lock();}
        void unlockObjects(){m_ObjectMutex.unlock();}
        
        void setCamera(CCamera* const);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CWorldDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CWorldDataStorage&);
        
    private:
      
        CCamera*                    m_pCamera;                  ///< Active camera for visuals
      
        DebrisType                  m_Debris;                   ///< List of debris
        DebrisVisualsType           m_DebrisVisuals;            ///< List of debris visuals
        DebrisVisualsThrusterType   m_DebrisVisualsThruster;    ///< List of debris visuals
        JointsType                  m_Joints;                   ///< List of joints
//         KinematicsStatesType        m_KinematicsStates;         ///< List of kinematics states
        UIDUserType                 m_UIDUserRef;               ///< Store objects referred by their UID
        ObjectsType                 m_DynamicObjects;           ///< List of dynamic objects
        ObjectsType                 m_StaticObjects;            ///< List of static objects
        ObjectVisualsType           m_ObjectVisuals;            ///< List of object visuals
        
        std::unordered_map<std::string, ObjectsType::const_iterator> m_DynamicObjectsMemory; ///< Stores index to specific object
        
        std::mutex                  m_ObjectMutex;              ///< Mutex to lock object
        std::mutex                  m_ObjectVisualsMutex;       ///< Mutex to lock object visuals
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns active camera
///
/// \return Active camera
///
////////////////////////////////////////////////////////////////////////////////
inline CCamera* const CWorldDataStorage::getCamera() const
{
    METHOD_ENTRY("CWorldDataStorage::getCamera")
    return m_pCamera;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of debris.
///
/// \return List of debris
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisType& CWorldDataStorage::getDebris() const
{
    METHOD_ENTRY("CWorldDataStorage::getDebris")
    return m_Debris;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of debris visuals.
///
/// \return List of debris visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisVisualsType& CWorldDataStorage::getDebrisVisuals() const
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisVisuals")
    return m_DebrisVisuals;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of thruster debris visuals.
///
/// \return List of thruster debris visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisVisualsThrusterType& CWorldDataStorage::getDebrisVisualsThruster() const
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisVisualsThruster")
    return m_DebrisVisualsThruster;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of joints.
///
/// \return List of joints
///
////////////////////////////////////////////////////////////////////////////////
inline const JointsType& CWorldDataStorage::getJoints() const
{
    METHOD_ENTRY("CWorldDataStorage::getDebris")
    return m_Joints;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of dynamic objects.
///
/// \return List of dynamic objects
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectsType& CWorldDataStorage::getDynamicObjects() 
{
    METHOD_ENTRY("CWorldDataStorage::getDynamicObjects")
    std::lock_guard<std::mutex> lock(m_ObjectMutex);
    return m_DynamicObjects;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of static objects.
///
/// \return List of static objects
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectsType& CWorldDataStorage::getStaticObjects() 
{
    METHOD_ENTRY("CWorldDataStorage::getStaticObjects")
    std::lock_guard<std::mutex> lock(m_ObjectMutex);
    return m_StaticObjects;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of object visuals.
///
/// \return List of object visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectVisualsType& CWorldDataStorage::getObjectVisuals()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectVisuals")
    return m_ObjectVisuals;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the active camera
///
/// \param _pCam Active camera
///
////////////////////////////////////////////////////////////////////////////////
inline void CWorldDataStorage::setCamera(CCamera* const _pCam)
{
    METHOD_ENTRY("CWorldDataStorage::setCamera")
    m_pCamera = _pCam;
}

#endif // WORLD_DATA_STORAGE_H
