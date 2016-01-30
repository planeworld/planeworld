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
/// \file       timer.h
/// \brief      Prototype of class "CTimer"
///
/// \date       2009-06-29
/// \author     bfeld
///
/// \note Timer does not use logging longer, since it is used by directly
///       logging class and would cause recursive calls.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H

//--- Standard header --------------------------------------------------------//
#include <chrono>
#include <istream>
#include <ostream>

//--- Program header ---------------------------------------------------------//

/// Factor to upscale time from u-seconds to seconds
constexpr double TIMER_OUTPUT_SEC_FACTOR = 0.000001;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing timer functions
/// 
////////////////////////////////////////////////////////////////////////////////
class CTimer
{

    public:

        //--- Constructor ----------------------------------------------------//
        CTimer();

        //--- Methods --------------------------------------------------------//
        void start();
        void stop();
        void sleepRemaining(const double&);

        //--- Constant Methods -----------------------------------------------//
        double          getSplitTime();
        inline double   getTime() const {return m_fDiffTime;}   ///< Returns time passed
        
        //--- Friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CTimer&);
        friend std::ostream& operator<<(std::ostream&, CTimer&);
        
    private:

        //--- Private Variables ----------------------------------------------//
        std::chrono::high_resolution_clock::time_point m_Start; ///< Starting time
        std::chrono::high_resolution_clock::time_point m_Stop;  ///< Stopping time

        double  m_fDiffTime = 0.0;                         ///< Time between start and stop

};

#endif
