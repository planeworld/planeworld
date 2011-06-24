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
#include "log.h"
#include "eigen2/Eigen/Core"

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Misc header ------------------------------------------------------------//
#include <boost/circular_buffer.hpp>

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
class CDebris
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CDebris();

        //--- Constant methods -----------------------------------------------//
        const int getDepths() const;

        //--- Methods --------------------------------------------------------//
        void addForce(const Vector2d&, const Vector2d&);
        
        boost::circular_buffer<Vector2d>* getPositions();
        boost::circular_buffer<Vector2d>* getVelocities();
        boost::circular_buffer<Vector2d>* getPreviousPositions();
        boost::circular_buffer<Vector2d>* getPreviousVelocities();

        void                setDamping(const double&);
        void                setDepths(const int&);
        void                setNumber(const int&);
        void                setTimeFac(const double&);
        
        void                dynamics(const double&);
        void                init();

    protected:

        //-- Variables [protected] -------------------------------------------//
        CTimer                  m_Lifetime;                         ///< Lifetime counter
        double                  m_fTimeFac;                         ///< Factor of realtime

        boost::circular_buffer<Vector2d> m_PosList;                 ///< Position of debris
        boost::circular_buffer<Vector2d> m_PosListPrev;             ///< Position of debris in previous time step
        boost::circular_buffer<Vector2d> m_VelList;                 ///< Velocity of derbis
        boost::circular_buffer<Vector2d> m_VelListPrev;             ///< Velocity of derbis in previous time step
        
        double                  m_fDamping;                         ///< Damping of debris
        int                     m_nDepthlayers;                     ///< Depths in which debris exists

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the debris
///
/// \return List of positions of debris
///
////////////////////////////////////////////////////////////////////////////////
inline boost::circular_buffer<Vector2d>* CDebris::getPositions()
{
    METHOD_ENTRY("CDebris::getPositions")

    METHOD_EXIT("CDebris::getPositions")
    return &m_PosList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns velocities of the debris
///
/// \return List of velocities of debris
///
////////////////////////////////////////////////////////////////////////////////
inline boost::circular_buffer<Vector2d>* CDebris::getVelocities()
{
    METHOD_ENTRY("CDebris::getVelocities")

    METHOD_EXIT("CDebris::getVelocities")
    return &m_VelList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of the debris from previous timestep
///
/// \return List of positions of debris from previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline boost::circular_buffer<Vector2d>* CDebris::getPreviousPositions()
{
    METHOD_ENTRY("CDebris::getPreviousPositions")

    METHOD_EXIT("CDebris::getPreviousPositions")
    return &m_PosListPrev;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns velocities of the debris from previous timestep
///
/// \return List of velocities of debris from previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline boost::circular_buffer<Vector2d>* CDebris::getPreviousVelocities()
{
    METHOD_ENTRY("CDebris::getPreviousVelocities")

    METHOD_EXIT("CDebris::getPreviousVelocities")
    return &m_VelListPrev;
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

    METHOD_EXIT("CDebris::getDepths")
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

    METHOD_EXIT("CDebris::setDamping")
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

    METHOD_EXIT("CDebris::setDepths")
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

    METHOD_EXIT("CDebris::setTimeFac")
}

#endif
