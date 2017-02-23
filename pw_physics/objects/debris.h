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
/// \file       debris.h
/// \brief      Prototype of interface "CDebris"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_H
#define DEBRIS_H

//--- Program header ---------------------------------------------------------//
#include "circular_buffer.h"
#include "grid_user.h"
#include "unique_id_user.h"

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

const uint32_t DEBRIS_DEFAULT_NUMBER = 100;
const uint8_t DEBRIS_STATE_ACTIVE = 0;
const uint8_t DEBRIS_STATE_INACTIVE = 1;

using namespace Eigen;

/// Specifies the type of debris
typedef enum
{
    DEBRIS_TYPE_DOT,
    DEBRIS_TYPE_THRUST
} DebrisTypeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for debris
///
/// Debris have a very limited physical accuracy. They are mainly for visual
/// purposes, do not have any real mass and thus, do not influence other objects.
///
/// \Note The state is represented by a uint8_t, since bool specilisation for
///       std::vector is a little restricted when accessing elements.
/// 
////////////////////////////////////////////////////////////////////////////////
class CDebris : public IGridUser,
                public IUniqueIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CDebris();
        CDebris(const CDebris&);
        CDebris& operator=(const CDebris&);
        CDebris* clone() const;

        //--- Constant methods -----------------------------------------------//
        
        const DebrisTypeType& getDebrisType()  const;
        int                   getDepths() const;

        //--- Methods --------------------------------------------------------//
        CCircularBuffer<Vector2d>* getPositions();
        CCircularBuffer<Vector2d>* getVelocities();
        CCircularBuffer<Vector2d>* getPreviousPositions();
        CCircularBuffer<std::uint8_t>*  getStates();
        
        void                setDamping(const double&);
        void                setDebrisType(const DebrisTypeType&);
        void                setDepths(const int&);
        void                setForce(const Vector2d&);
        void                setNumber(const int&);
        void                setTimeFac(const double&);
        
        void                dynamics(const double&);
        void                generate(const Vector2d&, const Vector2d&);

        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CDebris* const);
        friend std::ostream&    operator<<(std::ostream&, CDebris* const);
        
    protected:
        
        //--- Methods [protected] --------------------------------------------//
        void                   copy(const CDebris&);

        //--- Variables [protected] ------------------------------------------//
        CCircularBuffer<Vector2d> m_PosList;                 ///< Position of debris
        CCircularBuffer<Vector2d> m_PosListPrev;             ///< Position of debris in previous time step
        CCircularBuffer<Vector2d> m_VelList;                 ///< Velocity of derbis
        CCircularBuffer<std::uint8_t>  m_StateList;          ///< Is the debris active or inactive
        
        DebrisTypeType          m_DebrisType;                ///< Type of debris
        CTimer                  m_Lifetime;                  ///< Lifetime counter
        double                  m_fTimeFac;                  ///< Factor of realtime
        double                  m_fDamping;                  ///< Damping of debris
        int                     m_nDepthlayers;              ///< Depths in which debris exists
        
        Vector2d                m_vecForce;                  ///< Gravitational force applied

};

typedef std::vector<CDebris*>   DebrisType;                  ///< Specifies a list of debris

//--- Enum parser ------------------------------------------------------------//
const std::map<DebrisTypeType, std::string> mapDebrisTypeToString = {
    {DEBRIS_TYPE_DOT, "debris_dot"},
    {DEBRIS_TYPE_THRUST, "debris_thrust"}
}; ///< Map from DebrisTypeType to string

const std::map<std::string, DebrisTypeType> mapStringToDebrisType = {
    {"debris_dot", DEBRIS_TYPE_DOT},
    {"debris_thrust", DEBRIS_TYPE_THRUST}
}; ///< Map from string to DebrisTypeType

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the type of debris
///
/// \return Type of debris
///
////////////////////////////////////////////////////////////////////////////////
inline const DebrisTypeType& CDebris::getDebrisType() const
{
    METHOD_ENTRY("CDebris::getDebrisTypeType")
    return m_DebrisType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the debris
///
/// \return List of positions of debris
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CDebris::getPositions()
{
    METHOD_ENTRY("CDebris::getPositions")
    return &m_PosList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns velocities of the debris
///
/// \return List of velocities of debris
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CDebris::getVelocities()
{
    METHOD_ENTRY("CDebris::getVelocities")
    return &m_VelList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the debris from previous timestep
///
/// \return List of positions of debris from previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CDebris::getPreviousPositions()
{
    METHOD_ENTRY("CDebris::getPreviousPositions")
    return &m_PosListPrev;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns states of the debris, defining if a debris is active or not.
///
/// \return List of states of the debris
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<std::uint8_t>* CDebris::getStates()
{
    METHOD_ENTRY("CDebris::getStates")
    return &m_StateList;
}


///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns depthlayers
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline int CDebris::getDepths() const
{
    METHOD_ENTRY("CDebris::getDepths")
    return (m_nDepthlayers);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets damping of debris debris
/// 
/// Daming is used when debris collide with other objects to simulate energy loss.
///
/// \param _fD Damping to be applied to all debris
///
////////////////////////////////////////////////////////////////////////////////
inline void CDebris::setDamping(const double& _fD)
{
    METHOD_ENTRY("CDebris::setDamping")
    m_fDamping = _fD;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the type of debris
///
/// \param _DebrisType Type of debris
///
///////////////////////////////////////////////////////////////////////////////
inline void CDebris::setDebrisType(const DebrisTypeType& _DebrisType)
{
    METHOD_ENTRY("CDebris::setDebrisTypeType")
    m_DebrisType = _DebrisType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set depthlayers for these debris
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CDebris::setDepths(const int& _nD)
{
    METHOD_ENTRY("CDebris::setDepths")
    m_nDepthlayers |= _nD;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets gravitational force that acts on debris.
///
/// \param _vecF Gravitational force
///
////////////////////////////////////////////////////////////////////////////////
inline void CDebris::setForce(const Vector2d& _vecF)
{
    METHOD_ENTRY("CDebris::setForce")
    m_vecForce = _vecF;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the time factor for these debris.
///
/// The time factor is a variable that is multiplied with the frametime. Thus,
/// the kinematics integration of the debris is faster or slower, leading to
/// effects like local slow-motion or local time-lapse.
///
/// \param _fTF Time factor to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CDebris::setTimeFac(const double& _fTF)
{
    METHOD_ENTRY("CDebris::setTimeFac")

    m_fTimeFac = _fTF;
}

#endif
