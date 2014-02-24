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

//--- Program header ---------------------------------------------------------//
#include "debris.h"
#include "joint.h"
#include "object.h"


typedef std::list<IJoint*>      JointsType;
typedef std::list<CDebris*>     DebrisType;
typedef std::vector<IObject*>  ObjectsType;

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
        const DebrisType&     getDebris() const;
        const JointsType&     getJoints() const;
        const ObjectsType&    getDynamicObjects() const;
        const ObjectsType&    getStaticObjects() const;
        
        //--- Methods --------------------------------------------------------//
        void addDebris(CDebris*);
        void addJoint(IJoint*);
        void addObject(IObject*);
        void addObjects(ObjectsType);
        
    private:
                
        DebrisType          m_Debris;               ///< List of debris
        JointsType          m_Joints;               ///< List of joints
        ObjectsType         m_DynamicObjects;       ///< List of dynamic objects
        ObjectsType         m_StaticObjects;        ///< List of static objects
};

//--- Implementation is done here for inline optimisation --------------------//

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
inline const ObjectsType& CWorldDataStorage::getDynamicObjects() const
{
    METHOD_ENTRY("CWorldDataStorage::getDynamicObjects")
    return m_DynamicObjects;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a list of static objects.
///
/// \return List of static objects
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectsType& CWorldDataStorage::getStaticObjects() const
{
    METHOD_ENTRY("CWorldDataStorage::getStaticObjects")
    return m_StaticObjects;
}

#endif
