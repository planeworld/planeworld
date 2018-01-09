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
#include "spinlock.h"
#include "uid_user.h"
#include "universe.h"

class CParticle;
class IEmitter;
class CObject;
class CObjectPlanet;
class IShape;
class CThruster;

typedef std::list<IJoint*>                      JointsType;                 ///< Specifies a list of joints

/// Map of particles, accessed by name
typedef std::unordered_map<std::string, CParticle*> ParticlesByNameType;
/// Buffered particles, accessed by name
typedef CMultiBuffer<BUFFER_QUADRUPLE, ParticlesByNameType, std::string, CParticle*> BufferedParticlesByNameType;
/// Map of particles, accessed by UID value
typedef std::unordered_map<UIDType, CParticle*> ParticlesByValueType;
/// Map of buffered particles, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, ParticlesByValueType, UIDType, CParticle*> BufferedParticlesByValueType;

/// Map of emitters, accessed by UID value
typedef std::unordered_map<UIDType, IEmitter*> EmittersByValueType;
/// Map of emitters, accessed by UID value
typedef std::unordered_map<UIDType, CThruster*> ThrustersByValueType;
// /// Map of buffered emitters, accessed by UID value
// typedef CMultiBuffer<BUFFER_TRIPLE, EmittersByValueType, UIDType, IEmitter*> BufferedEmittersByValueType;

/// Map of objects, accessed by UID value
typedef std::unordered_map<UIDType, CObject*> ObjectsByValueType;
/// Map of buffered objects, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, ObjectsByValueType, UIDType, CObject*> BufferedObjectsByValueType;
/// Map of shapes, accessed by UID value
typedef std::unordered_map<UIDType, IShape*> ShapesByValueType;

/// Map of objects, accessed by UID value
typedef std::unordered_map<UIDType, CObjectPlanet*> ObjectsPlanetsByValueType;
/// Map of buffered planetary objects, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, ObjectsPlanetsByValueType, UIDType, CObjectPlanet*> BufferedObjectsPlanetsByValueType;

/// Vector of UID users, accessed by UID value
typedef std::vector<IUIDUser*> UIDUsersByValueType;
/// Vector of buffered UID users, accessed by UID value
typedef CMultiBuffer<BUFFER_QUADRUPLE, UIDUsersByValueType, IUIDUser*> BufferedUIDUsersByValueType;
/// Map of UIDs, accessed by name
typedef std::unordered_map<std::string, UIDType> UIDsByNameType;

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
        const JointsType&           getJoints() const;
        const double&               getTimeScale() const;
        
        //--- Methods --------------------------------------------------------//
        bool addEmitter(IEmitter*);
        bool addParticle(CParticle*);
        void addJoint(IJoint*);
        bool addObject(CObject*);
        bool addObjectPlanet(CObjectPlanet*);
        void addShape(IShape*);
        bool addThruster(CThruster*);
        bool addUIDUser(IUIDUser*);
        
        void updateObject(const UIDType);
        
        CUniverse*                  getUniverse();
        
        IEmitter*                   getEmitterByValue(const UIDType);
        CObject*                    getObjectByValueBack(const UIDType);
        CObject*                    getObjectByValueFront(const UIDType);
        CObjectPlanet*              getObjectPlanetByValueBack(const UIDType);
        CObjectPlanet*              getObjectPlanetByValueFront(const UIDType);
        CParticle*                  getParticleByValueBack(const UIDType);
        IShape*                     getShapeByValue(const UIDType);
        CThruster*                  getThrusterByValue(const UIDType);
        
        EmittersByValueType*        getEmittersByValue();
        ParticlesByNameType*        getParticlesByNameBack();
        ParticlesByNameType*        getParticlesByNameFront();
        ParticlesByValueType*       getParticlesByValueFront();
        ParticlesByValueType*       getParticlesByValueBack();
        ObjectsByValueType*         getObjectsByValueBack();
        ObjectsByValueType*         getObjectsByValueFront();
        ObjectsPlanetsByValueType*  getObjectsPlanetsByValueBack();
        ObjectsPlanetsByValueType*  getObjectsPlanetsByValueFront();
        ThrustersByValueType*       getThrustersByValue();
        UIDUsersByValueType*        getUIDUsersByValueFront();
        UIDUsersByValueType*        getUIDUsersByValueBack();

        UIDsByNameType*             getUIDsByName();
        ShapesByValueType*          getShapesByValue();
        
        void                        setUniverse(CUniverse* const _pUniverse) {m_pUniverse = _pUniverse;}
        
        void                        swapBack();
        void                        swapFront();
        
        void setTimeScale(const double&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CWorldDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CWorldDataStorage&);
        
        //--- Variables ------------------------------------------------------//
        CSpinlock AccessNames;
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        bool addUIDUser(const std::array<IUIDUser*,BUFFER_QUADRUPLE>&);
      
        CUniverse*                      m_pUniverse;            ///< The procedurally generated universe
        
        // Buffered entities, used by graphical clients
        BufferedParticlesByNameType         m_ParticlesByName;          ///< Buffered particles, accessed by name
        BufferedParticlesByValueType        m_ParticlesByValue;         ///< Buffered particles, accessed by value
        BufferedObjectsByValueType          m_ObjectsByValue;           ///< Buffered objects, accessed by UID value+
        BufferedObjectsPlanetsByValueType   m_ObjectsPlanetsByValue;    ///< Buffered planetary objects, accessed by UID value+
        BufferedUIDUsersByValueType         m_UIDUsersByValue;          ///< Buffered UID users, accessed by value

        // Entities of physics engine
        EmittersByValueType         m_EmittersByValue;          ///< Emitters, accessed by value
        ShapesByValueType           m_ShapesByValue;            ///< Shapes, accessed by UID value
        ThrustersByValueType        m_ThrustersByValue;         ///< Thrusters, accessed by UID value
        UIDsByNameType              m_UIDsByName;               ///< UIDs of entities, accessed by name
        
        JointsType                  m_Joints;                   ///< List of joints
        
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
    METHOD_ENTRY("CWorldDataStorage::getParticle")
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
/// \brief Returns procedurally generated universe
///
/// \return Procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline CUniverse* CWorldDataStorage::getUniverse()
{
    METHOD_ENTRY("CWorldDataStorage::getUniverse")
    return m_pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns emitters, accessed by value
///
/// \return Emitters, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline EmittersByValueType* CWorldDataStorage::getEmittersByValue()
{
    METHOD_ENTRY("CWorldDataStorage::getEmittersByValue")
    return &m_EmittersByValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of particles, accessed by name
///
/// \return Particles front buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline ParticlesByNameType* CWorldDataStorage::getParticlesByNameFront()
{
    METHOD_ENTRY("CWorldDataStorage::getParticlesByNameFront")
    return m_ParticlesByName.getBuffer(BUFFER_QUADRUPLE_FRONT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of particles, accessed by name
///
/// \return Particles back buffer, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline ParticlesByNameType* CWorldDataStorage::getParticlesByNameBack()
{
    METHOD_ENTRY("CWorldDataStorage::getParticlesByNameBack")
    return m_ParticlesByName.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer of particles, accessed by value
///
/// \return Particles back buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ParticlesByValueType* CWorldDataStorage::getParticlesByValueBack()
{
    METHOD_ENTRY("CWorldDataStorage::getParticlesByValueBack")
    return m_ParticlesByValue.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of particles, accessed by value
///
/// \return Particles front buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ParticlesByValueType* CWorldDataStorage::getParticlesByValueFront()
{
    METHOD_ENTRY("CWorldDataStorage::getParticlesByValueFront")
    return m_ParticlesByValue.getBuffer(BUFFER_QUADRUPLE_FRONT);
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
/// \brief Returns back buffer of planetary objects, accessed by value
///
/// \return Planetary objects back buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ObjectsPlanetsByValueType* CWorldDataStorage::getObjectsPlanetsByValueBack()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsPlanetsByValueBack")
    return m_ObjectsPlanetsByValue.getBuffer(BUFFER_QUADRUPLE_BACK);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer of planetary objects, accessed by value
///
/// \return Planetary objects front buffer, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ObjectsPlanetsByValueType* CWorldDataStorage::getObjectsPlanetsByValueFront()
{
    METHOD_ENTRY("CWorldDataStorage::getObjectsPlanetsByValueFront")
    return m_ObjectsPlanetsByValue.getBuffer(BUFFER_QUADRUPLE_FRONT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns thrusters, accessed by value
///
/// \return Thrusters, accessed by value
///
////////////////////////////////////////////////////////////////////////////////
inline ThrustersByValueType* CWorldDataStorage::getThrustersByValue()
{
    METHOD_ENTRY("CWorldDataStorage::getThrustersByValue")
    return &m_ThrustersByValue;
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
/// \brief Returns UIDs, accessed by name
///
/// \return UIDs, accessed by name
///
////////////////////////////////////////////////////////////////////////////////
inline UIDsByNameType* CWorldDataStorage::getUIDsByName()
{
    METHOD_ENTRY("CWorldDataStorage::getUIDsByName")
    return &m_UIDsByName;
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
