////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017-2018 Torsten Büschenfeld
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
/// \file       spinlock.h
/// \brief      Prototype of class "CSpinloc"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-12-07
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SPINLOCK_H
#define SPINLOCK_H

//--- Standard header --------------------------------------------------------//
#include <atomic>
#include <chrono>

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "log.h"

//--- Misc header ------------------------------------------------------------//

//--- Constants --------------------------------------------------------------//
constexpr int SPINLOCK_MAX_ITER = 100;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing spinlock
///
/// \note Parts are loosely based on the blog of David Geier
///       (thanks for the insight!):
///       https://geidav.wordpress.com/ (visited 2017-12-08)
///
/// The spinlock uses an atomic_flag to signal access. If lock is hold for some
/// iterations a sleep is involved to reduce CPU load.
///
////////////////////////////////////////////////////////////////////////////////
class CSpinlock
{

    public:
        
        //--- Constructor/Destructor------------------------------------------//
        
        //--- Constant methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        void acquireLock();
        void releaseLock();
        void setLock();
        void waitForRelease();
        
        #ifdef PW_MULTITHREADING
            static std::uint64_t getSleeps(){return s_Sleeps;}
            static std::uint64_t getWaits(){return s_Waits;}
            static std::uint64_t getYields(){return s_Yields;}
        #endif
        
    private:
        
        //--- Variables [private] --------------------------------------------//
        #ifdef PW_MULTITHREADING
            std::atomic_flag isAccessed = ATOMIC_FLAG_INIT; ///< Indicates access, important for multithreading
            static std::uint64_t s_Sleeps;
            static std::uint64_t s_Waits;
            static std::uint64_t s_Yields;
        #endif
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SPINLOCK_H
