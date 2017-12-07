////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
        
        ////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Lock using atomic flag variable. 
        ///
        /// If lock is block try again SPINLOCK_MAX_ITER times while giving the
        /// processor some hint (pause) to optimise pipelining, hyper threading
        /// and bus frequency. If still blocked, sleep to free resources.
        ///
        ////////////////////////////////////////////////////////////////////////
        void lock()
        {
            #ifdef PW_MULTITHREADING
                int nIter = 0;
                while (isAccessed.test_and_set(std::memory_order_acquire))
                {
                    if (nIter < SPINLOCK_MAX_ITER)
                    {
                        #if (COMPILER == MVCC)
                            _mm_pause();
                        #elif (COMPILER == GCC || COMPILER == LLVM)
                            asm("pause");
                        #endif
                        ++nIter;
                    }
                    else
                    {
                        using namespace std::chrono;
                        std::this_thread::sleep_for(500us);
                    }
                }
            #endif
        }
    
        ////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Unlock using atomic flag variable. 
        ///
        ////////////////////////////////////////////////////////////////////////
        void unlock()
        {
            #ifdef PW_MULTITHREADING
                isAccessed.clear(std::memory_order_release);
            #endif
        }
        
    private:
        
        //--- Variables [private] --------------------------------------------//
        #ifdef PW_MULTITHREADING
            std::atomic_flag isAccessed = ATOMIC_FLAG_INIT; ///< Indicates access, important for multithreading
        #endif
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SPINLOCK_H
