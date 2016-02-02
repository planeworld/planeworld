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
#include "circular_buffer.h"
#include "universe_scaled.h"

//--- Standard header --------------------------------------------------------//
#include <mutex>

//--- Misc header ------------------------------------------------------------//

typedef CCircularBuffer<Vector2d> TrajectoryType;        ///< Typedef for trajectory
typedef CCircularBuffer<Vector2i> TrajectoryCellType;    ///< Typedef for trajectory (cell part)

//--- Constants --------------------------------------------------------------//
const int TRAJECTORY_CAPACITY =  1000; ///< Default length of trajetory
const int TRAJECTORY_RESOLUTION =   1; ///< Default temporal resolution of trajectory

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a storing a trajectory
///
/// This class stores a trajectory for an object. It is aware of the cell grid.
/// It is therefore possible, to draw trajectories that expand over several grid
/// cells of the universe.
///
////////////////////////////////////////////////////////////////////////////////
class CTrajectory
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CTrajectory();

        //--- Constant methods -----------------------------------------------//
        const TrajectoryType&       getPositions() const;
        const TrajectoryCellType&   getCells() const;

        //--- Methods --------------------------------------------------------//
        void  init(const Vector2d&, const Vector2i&);
        void  update(const Vector2d&, const Vector2i&);
        
        void  lock() const;
        void  unlock() const;

    private:

        CCircularBuffer<Vector2d>    m_Positions;        ///< Trajectory of object
        CCircularBuffer<Vector2i>    m_Cells;            ///< Trajectory of object (cell part)
        uint8_t                      m_unUpdateCount;    ///< Counter for update of trajectory
        
        mutable std::mutex m_Mutex; ///< Mutex to lock access to trajectory
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline CTrajectory::CTrajectory() : m_unUpdateCount(0u)
{
    METHOD_ENTRY("CTrajectory::CTrajectory")
    
    m_Positions.reserve(TRAJECTORY_CAPACITY);
    m_Cells.reserve(TRAJECTORY_CAPACITY);
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
    return m_Positions;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cells of trajectory
///
/// \return Trajectory cells
///
////////////////////////////////////////////////////////////////////////////////
inline const TrajectoryCellType& CTrajectory::getCells() const
{
    METHOD_ENTRY("CTrajectory::getCells")
    return m_Cells;
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
    
    if (++m_unUpdateCount == TRAJECTORY_RESOLUTION)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Positions.push_back(_vecPos);
        m_Cells.push_back(_vecCell);
        m_unUpdateCount = 0;
    }
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
