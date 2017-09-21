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
/// \file       particle.h
/// \brief      Prototype of interface "CParticle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_H
#define PARTICLE_H

//--- Program header ---------------------------------------------------------//
#include "bounding_box.h"
#include "circular_buffer.h"
#include "grid_user.h"
#include "unique_id_user.h"

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

const uint32_t PARTICLE_DEFAULT_NUMBER = 100;
const uint8_t PARTICLE_STATE_ACTIVE = 0;
const uint8_t PARTICLE_STATE_INACTIVE = 1;

using namespace Eigen;

/// Specifies the type of particle
typedef enum
{
    PARTICLE_TYPE_NONE,
    PARTICLE_TYPE_DOT,
    PARTICLE_TYPE_THRUST
} ParticleTypeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for particle
///
/// Particle have a very limited physical accuracy. They are mainly for visual
/// purposes, do not have any real mass and thus, do not influence other objects.
///
/// \Note The state is represented by a uint8_t, since bool specilisation for
///       std::vector is a little restricted when accessing elements.
/// 
////////////////////////////////////////////////////////////////////////////////
class CParticle : public IGridUser,
                public IUniqueIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CParticle();
        CParticle(const CParticle&);
        CParticle& operator=(const CParticle&);
        CParticle* clone() const;

        //--- Constant methods -----------------------------------------------//
        CBoundingBox&         getBoundingBox();
        const ParticleTypeType& getParticleType()  const;
        int                   getDepths() const;

        //--- Methods --------------------------------------------------------//
        CCircularBuffer<Vector2d>* getPositions();
        CCircularBuffer<Vector2d>* getVelocities();
        CCircularBuffer<Vector2d>* getPreviousPositions();
        CCircularBuffer<std::uint8_t>*  getStates();
        
        void                setDamping(const double&);
        void                setParticleType(const ParticleTypeType&);
        void                setDepths(const int&);
        void                setForce(const Vector2d&);
        void                setNumber(const int&);
        void                setTimeFac(const double&);
        
        void                dynamics(const double&);
        void                generate(const Vector2d&, const Vector2d&);

        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CParticle* const);
        friend std::ostream&    operator<<(std::ostream&, CParticle* const);
        
    protected:
        
        //--- Methods [protected] --------------------------------------------//
        void                   copy(const CParticle&);

        //--- Variables [protected] ------------------------------------------//
        CCircularBuffer<Vector2d> m_PosList;                 ///< Position of particle
        CCircularBuffer<Vector2d> m_PosListPrev;             ///< Position of particle in previous time step
        CCircularBuffer<Vector2d> m_VelList;                 ///< Velocity of derbis
        CCircularBuffer<std::uint8_t>  m_StateList;          ///< Is the particle active or inactive
        
        CBoundingBox            m_BBox;                      ///< Bounding box of all particle
        
        ParticleTypeType          m_ParticleType;                ///< Type of particle
        CTimer                  m_Lifetime;                  ///< Lifetime counter
        double                  m_fTimeFac;                  ///< Factor of realtime
        double                  m_fDamping;                  ///< Damping of particle
        int                     m_nDepthlayers;              ///< Depths in which particle exists
        
        Vector2d                m_vecForce;                  ///< Gravitational force applied

};

typedef std::vector<CParticle*>   ParticleType;                  ///< Specifies a list of particle

//--- Enum parser ------------------------------------------------------------//
static std::map<ParticleTypeType, std::string> s_ParticleTypeToStringMap = {
    {PARTICLE_TYPE_DOT, "particle_dot"},
    {PARTICLE_TYPE_THRUST, "particle_thrust"}
}; ///< Map from ParticleTypeType to string

const std::map<std::string, ParticleTypeType> STRING_TO_PARTICLE_TYPE_MAP    = {
    {"particle_dot", PARTICLE_TYPE_DOT},
    {"particle_thrust", PARTICLE_TYPE_THRUST}
}; ///< Map from string to ParticleTypeType

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to particle type
///
/// \return Particle type
///
////////////////////////////////////////////////////////////////////////////////
static ParticleTypeType mapStringToParticleType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToParticleType")
    
    const auto ci = STRING_TO_PARTICLE_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_PARTICLE_TYPE_MAP.end())
        return ci->second;
    else
        return PARTICLE_TYPE_NONE;
}

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline CBoundingBox& CParticle::getBoundingBox()
{
    METHOD_ENTRY("CGeometry::getBoundingBox")
    return m_BBox;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the type of particle
///
/// \return Type of particle
///
////////////////////////////////////////////////////////////////////////////////
inline const ParticleTypeType& CParticle::getParticleType() const
{
    METHOD_ENTRY("CParticle::getParticleTypeType")
    return m_ParticleType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the particle
///
/// \return List of positions of particle
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CParticle::getPositions()
{
    METHOD_ENTRY("CParticle::getPositions")
    return &m_PosList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns velocities of the particle
///
/// \return List of velocities of particle
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CParticle::getVelocities()
{
    METHOD_ENTRY("CParticle::getVelocities")
    return &m_VelList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the particle from previous timestep
///
/// \return List of positions of particle from previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CParticle::getPreviousPositions()
{
    METHOD_ENTRY("CParticle::getPreviousPositions")
    return &m_PosListPrev;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns states of the particle, defining if a particle is active or not.
///
/// \return List of states of the particle
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<std::uint8_t>* CParticle::getStates()
{
    METHOD_ENTRY("CParticle::getStates")
    return &m_StateList;
}


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns depthlayers
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline int CParticle::getDepths() const
{
    METHOD_ENTRY("CParticle::getDepths")
    return (m_nDepthlayers);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets damping of particle particle
/// 
/// Daming is used when particle collide with other objects to simulate energy loss.
///
/// \param _fD Damping to be applied to all particle
///
////////////////////////////////////////////////////////////////////////////////
inline void CParticle::setDamping(const double& _fD)
{
    METHOD_ENTRY("CParticle::setDamping")
    m_fDamping = _fD;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the type of particle
///
/// \param _ParticleType Type of particle
///
///////////////////////////////////////////////////////////////////////////////
inline void CParticle::setParticleType(const ParticleTypeType& _ParticleType)
{
    METHOD_ENTRY("CParticle::setParticleTypeType")
    m_ParticleType = _ParticleType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set depthlayers for these particle
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CParticle::setDepths(const int& _nD)
{
    METHOD_ENTRY("CParticle::setDepths")
    m_nDepthlayers |= _nD;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets gravitational force that acts on particle.
///
/// \param _vecF Gravitational force
///
////////////////////////////////////////////////////////////////////////////////
inline void CParticle::setForce(const Vector2d& _vecF)
{
    METHOD_ENTRY("CParticle::setForce")
    m_vecForce = _vecF;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the time factor for these particle.
///
/// The time factor is a variable that is multiplied with the frametime. Thus,
/// the kinematics integration of the particle is faster or slower, leading to
/// effects like local slow-motion or local time-lapse.
///
/// \param _fTF Time factor to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CParticle::setTimeFac(const double& _fTF)
{
    METHOD_ENTRY("CParticle::setTimeFac")

    m_fTimeFac = _fTF;
}

#endif
