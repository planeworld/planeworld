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
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "joint.h"
#include "multi_buffer.h"
#include "unique_id_user.h"

class CDebris;
class IEmitter;
class CObject;
class IShape;

typedef std::list<IJoint*>                      JointsType;                 ///< Specifies a list of joints

/// Map of debris, accessed by name
typedef std::unordered_map<std::string, CDebris*> DebrisByNameType;
/// Buffered debris, accessed by name
typedef CMultiBuffer<BUFFER_QUADRUPLE, DebrisByNameType, std::string, CDebris*> BufferedDebrisByNameType;
/// Map of debris, accessed by UID value
typedef std::unordered_map<UIDType, CDebris*> DebrisByValueType;
/// Map of buffered debris, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, DebrisByValueType, UIDType, CDebris*> BufferedDebrisByValueType;

/// Map of debris, accessed by UID value
typedef std::unordered_map<UIDType, IEmitter*> EmittersByValueType;
/// Map of buffered debris, accessed by UID value
typedef CMultiBuffer<BUFFER_TRIPLE, EmittersByValueType, UIDType, IEmitter*> BufferedEmittersByValueType;

/// Map of objects, accessed by name
typedef std::map<std::string, CObject*> ObjectsByNameType;
/// Map of buffered objects, accessed by name
typedef CMultiBuffer<BUFFER_QUADRUPLE, ObjectsByNameType, std::string, CObject*> BufferedObjectsByNameType;
/// Map of objects, accessed by UID value
typedef std::unordered_map<UIDType, CObject*> ObjectsByValueType;
/// Map of buffered objects, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, ObjectsByValueType, UIDType, CObject*> BufferedObjectsByValueType;
/// Map of shapes, accessed by UID value
typedef std::unordered_map<UIDType, IShape*> ShapesByValueType;

/// Vector of UID users, accessed by UID value
typedef std::vector<IUniqueIDUser*> UIDUsersByValueType;
/// Vector of buffered UID users, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, UIDUsersByValueType, IUniqueIDUser*> BufferedUIDUsersByValueType;

const std::uint16_t WDS_DEFAULT_UID_BUFFER_SIZE = 32768; ///< Default size of value buffer

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that stores all physics data
///
////////////////////////////////////////////////////////////////////////////////
class CWorldDataStorage
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CWorldDataStorage();
        ~CWorldDataStorage();
        
        //--- Constant Methods -----------------------------------------------//
//         const DebrisType&                   getDebris() const;
        const JointsType&                   getJoints() const;
        const double&                       getTimeScale() const;
        
        //--- Methods --------------------------------------------------------//
        bool addDebris(CDebris*);
        void addJoint(IJoint*);
        bool addObject(CObject*);
        void addShape(IShape*);
        bool addUIDUser(IUniqueIDUser*);
        
        void updateObject(const UIDType);
        
        BufferedObjectsByNameType&  getObjectsBuffer();
        DebrisByNameType*           getDebrisByNameBack();
        DebrisByNameType*           getDebrisByNameFront();
        DebrisByValueType*          getDebrisByValueFront();
        DebrisByValueType*          getDebrisByValueBack();
        ObjectsByNameType*          getObjectsByNameBack();
        ObjectsByValueType*         getObjectsByValueBack();
        ObjectsByValueType*         getObjectsByValueFront();
        UIDUsersByValueType*        getUIDUsersByValueFront();
        UIDUsersByValueType*        getUIDUsersByValueBack();
        
        ShapesByValueType*          getShapesByValue();
        
        void                        swapBack();
        void                        swapFront();
        
        void setTimeScale(const double&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CWorldDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CWorldDataStorage&);
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        bool addUIDUser(const std::array<IUniqueIDUser*,BUFFER_QUADRUPLE>&);
      
        BufferedDebrisByNameType    m_DebrisByName;             ///< Buffered debris, accessed by name
        BufferedDebrisByValueType   m_DebrisByValue;            ///< Buffered debris, accessed by value
        BufferedEmittersByValueType m_EmittersByValue;          ///< Buffered emitters, accessed by value
        BufferedObjectsByNameType   m_ObjectsByName;            ///< Buffered objects, accessed by name
        BufferedObjectsByValueType  m_ObjectsByValue;           ///< Buffered objects, accessed by UID value
        BufferedUIDUsersByValueType m_UIDUsersByValue;          ///< Buffered UID users, accessed by value
        
        ShapesByValueType           m_ShapesByValue;            ///< Shapes, accessed by UID value
        
        JointsType                  m_Joints;                   ///< List of joints
        UIDUserType                 m_UIDUserRef;               ///< Store objects referred by their UID
        
        std::mutex                  m_MutexFrontNew;            ///< Mutex for thread safety when swapping
        bool                        m_bFrontNew;                ///< Indicates new information for front buffer
        double                      m_fTimeScale;               ///< Factor for global acceleration of time
};

//--- Implementation is done here for inline optimisation --------------------//

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
/// \brief Returns the global time scale of the simulation
///
/// \return Global time scale of simulation
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CWorldDataStorage::getTimeScale() const
{
    METHOD_ENTRY("CWorldDataStorage::getTimeScale")
    return m_fTimeScale;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of debris, accessed by name
///
/// \return Debris front buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline DebrisByNameType* CWorldDataStorage::getDebrisByNameFront()
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisByNameFront")
    return m_DebrisByName.getBuffer(BUFFER_QUADRUPLE_FRONT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of debris, accessed by name
///
/// \return Debris back buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline DebrisByNameType* CWorldDataStorage::getDebrisByNameBack()
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisByNameBack")
    return m_DebrisByName.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of debris, accessed by value
///
/// \return Debris back buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline DebrisByValueType* CWorldDataStorage::getDebrisByValueBack()
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisByValueBack")
    return m_DebrisByValue.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of debris, accessed by value
///
/// \return Debris front buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline DebrisByValueType* CWorldDataStorage::getDebrisByValueFront()
{
    METHOD_ENTRY("CWorldDataStorage::getDebrisByValueFront")
    return m_DebrisByValue.getBuffer(BUFFER_QUADRUPLE_FRONT);
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer of objects, accessed by name
///
/// \return Objects buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline BufferedObjectsByNameType& CWorldDataStorage::getObjectsBuffer()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsBuffer")
    return m_ObjectsByName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of objects, accessed by name
///
/// \return Objects back buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline ObjectsByNameType* CWorldDataStorage::getObjectsByNameBack()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsByNameBack")
    return m_ObjectsByName.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of objects, accessed by value
///
/// \return Objects back buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ObjectsByValueType* CWorldDataStorage::getObjectsByValueBack()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsByValueBack")
    return m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of objects, accessed by value
///
/// \return Object front buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ObjectsByValueType* CWorldDataStorage::getObjectsByValueFront()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsByValueFront")
    return m_ObjectsByValue.getBuffer(BUFFER_QUADRUPLE_FRONT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of UID users, accessed by value
///
/// \return UID users back buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline UIDUsersByValueType* CWorldDataStorage::getUIDUsersByValueBack()
{
    METHOD_ENTRY("CWorldDataStorage::getUIDUsersByValueBack")
    return m_UIDUsersByValue.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns shapes, accessed by value
///
/// \return Shapes, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ShapesByValueType* CWorldDataStorage::getShapesByValue()
{
    METHOD_ENTRY("CWorldDataStorage::getShapesByValue")
    return &m_ShapesByValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of UID users, accessed by value
///
/// \return UID users front buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline UIDUsersByValueType* CWorldDataStorage::getUIDUsersByValueFront()
{
    METHOD_ENTRY("CWorldDataStorage::getUIDUsersByValueFront")
    return m_UIDUsersByValue.getBuffer(BUFFER_QUADRUPLE_FRONT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the global time scale of the simulation
///
/// \param _fTimeScale Global time scale of the simulation
///
////////////////////////////////////////////////////////////////////////////////
inline void CWorldDataStorage::setTimeScale(const double& _fTimeScale)
{
    METHOD_ENTRY("CWorldDataStorage::setTimeScale")
    m_fTimeScale = _fTimeScale;
}

#endif // WORLD_DATA_STORAGE_H
