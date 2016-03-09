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
#include "unique_id_user.h"
#include "universe_scaled.h"

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

const int DEBRIS_DEFAULT_NUMBER = 100;

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for debris
///
/// Debris have a very limited physical accuracy. They are mainly for visual
/// purposes, do not have any real mass and thus, do not influence other objects.
/// 
////////////////////////////////////////////////////////////////////////////////
class CDebris : public IUniverseScaled,
                public IUniqueIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CDebris();

        //--- Constant methods -----------------------------------------------//
        const int getDepths() const;

        //--- Methods --------------------------------------------------------//
        CCircularBuffer<Vector2d>* getPositions();
        CCircularBuffer<Vector2d>* getVelocities();
        CCircularBuffer<Vector2d>* getPreviousPositions();
        CCircularBuffer<Vector2d>* getPreviousVelocities();
        CCircularBuffer<bool>*     getStates();
        
        void                setDamping(const double&);
        void                setDepths(const int&);
        void                setForce(const Vector2d&);
        void                setNumber(const int&);
        void                setTimeFac(const double&);
        
        void                dynamics(const double&);
        void                generate(const Vector2d&, const Vector2d&);
        void                init();

        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CDebris* const);
        friend std::ostream&    operator<<(std::ostream&, CDebris* const);
        
    protected:

        //-- Variables [protected] -------------------------------------------//
        CTimer                  m_Lifetime;                  ///< Lifetime counter
        double                  m_fTimeFac;                  ///< Factor of realtime

        CCircularBuffer<Vector2d> m_PosList;                 ///< Position of debris
        CCircularBuffer<Vector2d> m_PosListPrev;             ///< Position of debris in previous time step
        CCircularBuffer<Vector2d> m_VelList;                 ///< Velocity of derbis
        CCircularBuffer<Vector2d> m_VelListPrev;             ///< Velocity of derbis in previous time step
        CCircularBuffer<bool>     m_StateList;               ///< Is the debris active or inactive
        
        double                  m_fDamping;                  ///< Damping of debris
        int                     m_nDepthlayers;              ///< Depths in which debris exists
        
        Vector2d                m_vecForce;                  ///< Gravitational force applied

};

typedef std::vector<CDebris*>   DebrisType;                  ///< Specifies a list of debris

//--- Implementation is done here for inline optimisation --------------------//


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
/// \brief Returns velocities of the debris from previous timestep
///
/// \return List of velocities of debris from previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<Vector2d>* CDebris::getPreviousVelocities()
{
    METHOD_ENTRY("CDebris::getPreviousVelocities")
    return &m_VelListPrev;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns states of the debris, defining if a debris is active or not.
///
/// \return List of states of the debris
///
////////////////////////////////////////////////////////////////////////////////
inline CCircularBuffer<bool>* CDebris::getStates()
{
    METHOD_ENTRY("CDebris::getStates")
    return &m_StateList;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns depthlayers
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline const int CDebris::getDepths() const
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
