////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2018 Torsten Büschenfeld
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
/// \file       spinlock.cpp
/// \brief      Implementation of class "CSpinlock"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-01-25
///
////////////////////////////////////////////////////////////////////////////////

#include "spinlock.h"

//--- Standard header --------------------------------------------------------//

//--- Misc header ------------------------------------------------------------//

#ifdef PW_MULTITHREADING
    std::uint64_t CSpinlock::s_Sleeps = 0u;
    std::uint64_t CSpinlock::s_Waits = 0u;
    std::uint64_t CSpinlock::s_Yields = 0u;
#endif

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Acquire lock using atomic flag variable. 
///
/// If lock is acquired try again SPINLOCK_MAX_ITER times while giving the
/// processor some hint (pause) to optimise pipelining, hyper threading
/// and bus frequency. If still blocked, sleep to free resources.
///
////////////////////////////////////////////////////////////////////////////////
void CSpinlock::acquireLock()
{
    METHOD_ENTRY("CSpinlock::acquireLock")
    #ifdef PW_MULTITHREADING
        int nIter = 0;
        while (isAccessed.test_and_set(std::memory_order_acquire))
        {
            if (nIter < SPINLOCK_MAX_ITER)
            {
                #if defined(_MSC_VER)
                    _mm_pause();
                #elif defined(__clang__) || defined(__GNUC__)
                    asm("pause");
                #endif
                ++nIter;
                DOM_STATS(DEBUG_BLK(++s_Waits;))
            }
            else if (nIter < SPINLOCK_MAX_ITER*2)
            {
                std::this_thread::yield();
                ++nIter;
                DOM_STATS(DEBUG_BLK(++s_Yields;))
            }
            else
            {
                using namespace std::chrono;
                std::this_thread::sleep_for(500us);
                DOM_STATS(DEBUG_BLK(++s_Sleeps;))
            }
        }
    #endif
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Release lock using atomic flag variable. 
///
////////////////////////////////////////////////////////////////////////////////
void CSpinlock::releaseLock()
{
    METHOD_ENTRY("CSpinlock::releaseLock")
    #ifdef PW_MULTITHREADING
        isAccessed.clear(std::memory_order_release);
    #endif
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Lock without waiting to acquire lock. 
///
////////////////////////////////////////////////////////////////////////////////
void CSpinlock::setLock()
{
    METHOD_ENTRY("CSpinlock::setLock")
    #ifdef PW_MULTITHREADING
        isAccessed.test_and_set(std::memory_order_release);
    #endif
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Wait for lock to be released 
///
////////////////////////////////////////////////////////////////////////////////
void CSpinlock::waitForRelease()
{
    METHOD_ENTRY("CSpinlock::waitForRelease")
    #ifdef PW_MULTITHREADING
        int nIter = 0;
        while (isAccessed.test_and_set(std::memory_order_acquire))
        {
            if (nIter < SPINLOCK_MAX_ITER)
            {
                #if defined(_MSC_VER)
                    _mm_pause();
                #elif defined(__clang__) || defined(__GNUC__)
                    asm("pause");
                #endif
                ++nIter;
                DOM_STATS(DEBUG_BLK(++s_Waits;))
            }
            else if (nIter < SPINLOCK_MAX_ITER*2)
            {
                std::this_thread::yield();
                ++nIter;
                DOM_STATS(DEBUG_BLK(++s_Yields;))
            }
            else
            {
                using namespace std::chrono;
                std::this_thread::sleep_for(500us);
                DOM_STATS(DEBUG_BLK(++s_Sleeps;))
            }
        }
        isAccessed.clear(std::memory_order_release);
    #endif
}

