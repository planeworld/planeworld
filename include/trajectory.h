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
/// \file       trajectory.h
/// \brief      Prototype of class "CTrajectory"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-05-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

//--- Program header ---------------------------------------------------------//
#include "universe_scaled.h"

//--- Standard header --------------------------------------------------------//
#include <mutex>

//--- Misc header ------------------------------------------------------------//
#include <boost/circular_buffer.hpp>

typedef boost::circular_buffer<Vector2d> TrajectoryType; ///< Typedef for trajectory

const int TRAJECTORY_CAPACITY = 1000; ///< Default length of trajetory

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a storing a trajectory
///
////////////////////////////////////////////////////////////////////////////////
class CTrajectory : public IUniverseScaled
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CTrajectory();

        //--- Constant methods -----------------------------------------------//
        const TrajectoryType& getPositions() const;

        //--- Methods --------------------------------------------------------//
        void  init(const Vector2d&, const Vector2i&);
        void  update(const Vector2d&, const Vector2i&);
        
        void  lock() const;
        void  unlock() const;

    private:

        boost::circular_buffer<Vector2d>    m_Trajectory;       ///< Trajectory of object
        
        mutable std::mutex m_Mutex; ///< Mutex to lock access to trajectory
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline CTrajectory::CTrajectory()
{
    METHOD_ENTRY("CTrajectory::CTrajectory")
    
    m_Trajectory.set_capacity(TRAJECTORY_CAPACITY);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns positions of trajectory
///
/// \return Trajectory positions
///
////////////////////////////////////////////////////////////////////////////////
inline const TrajectoryType& CTrajectory::getPositions() const
{
    METHOD_ENTRY("CTrajectory::getPositions")
    return m_Trajectory;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates trajectory with a new value
///
/// \param _vecPos Position
/// \param _vecCell Cell
///
////////////////////////////////////////////////////////////////////////////////
inline void CTrajectory::update(const Vector2d& _vecPos, const Vector2i& _vecCell)
{
    METHOD_ENTRY("CTrajectory::update")
    
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Trajectory.push_back(_vecPos);
    m_vecCell = _vecCell;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Locks access to trajectory
///
////////////////////////////////////////////////////////////////////////////////
inline void CTrajectory::lock() const
{
    METHOD_ENTRY("CTrajectory::lock")
    m_Mutex.lock();    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Locks access to trajectory
///
////////////////////////////////////////////////////////////////////////////////
inline void CTrajectory::unlock() const
{
    METHOD_ENTRY("CTrajectory::unlock")
    m_Mutex.unlock();    
}

#endif
