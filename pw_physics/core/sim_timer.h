////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       sim_timer.h
/// \brief      Prototype of class "CSimTimer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-13
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SIM_TIMER_H
#define SIM_TIMER_H

//--- Standard header --------------------------------------------------------//
#include <cmath>

//--- Program header ---------------------------------------------------------//
#include "log.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a timer which runs in simulation time by
///        counting simulation time of single frames.
///
////////////////////////////////////////////////////////////////////////////////
class CSimTimer
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        
        //--- Constant Methods -----------------------------------------------//
        const uint16_t   getSecondsPart() const;
        const uint16_t   getMinutesPart() const;
        const uint16_t   getHoursPart() const;
        const uint16_t   getDaysPart() const;
        const uint32_t   getYears() const;
        
        const double&   getSecondsRaw() const;
        
        const bool isActive() const;
        
        //--- Methods --------------------------------------------------------//
        void inc(const double&);
        void start();
        void stop();
        void toggle();
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CSimTimer&);
        friend std::ostream& operator<<(std::ostream&, CSimTimer&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        bool        m_bActive  = false; ///< Flag if timer is active
        double      m_fSeconds = 0.0;   ///< Counts seconds (with decimal places for ms etc.) up to one year
        uint32_t    m_unYears  = 0u;    ///< Counts years        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the seconds part (residual) of the time passed 
///
/// \return Seconds part (residual) of the time passed
///
////////////////////////////////////////////////////////////////////////////////
inline const uint16_t CSimTimer::getSecondsPart() const
{
    METHOD_ENTRY("CSimTimer::getSecondsPart")
    
    constexpr double S_PER_M = 60.0;
    return static_cast<uint16_t>(((m_fSeconds / S_PER_M) - std::floor(m_fSeconds / S_PER_M)) * S_PER_M);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the minutes part (residual) of the time passed 
///
/// \return Minutes part (residual) of the time passed
///
////////////////////////////////////////////////////////////////////////////////
inline const uint16_t CSimTimer::getMinutesPart() const
{
    METHOD_ENTRY("CSimTimer::getMinutesPart")
    
    constexpr double M_PER_H = 60.0;
    constexpr double S_PER_H = 60.0*M_PER_H;
    return static_cast<uint16_t>(((m_fSeconds / S_PER_H) - std::floor(m_fSeconds / S_PER_H)) * M_PER_H);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the hours part (residual) of the time passed 
///
/// \return Hours part (residual) of the time passed
///
////////////////////////////////////////////////////////////////////////////////
inline const uint16_t CSimTimer::getHoursPart() const
{
    METHOD_ENTRY("CSimTimer::getHoursPart")
    
    constexpr double H_PER_D = 24.0;
    constexpr double S_PER_D = 60.0*60.0*H_PER_D;
    return static_cast<uint16_t>(((m_fSeconds / S_PER_D) - std::floor(m_fSeconds / S_PER_D)) * H_PER_D);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the days part (residual) of the time passed 
///
/// \return Days part (residual) of the time passed
///
////////////////////////////////////////////////////////////////////////////////
inline const uint16_t CSimTimer::getDaysPart() const
{
    METHOD_ENTRY("CSimTimer::getDaysPart")
    
    constexpr double D_PER_Y = 365.0;
    constexpr double S_PER_Y = 60.0*60.0*24.0*D_PER_Y;
    return static_cast<uint16_t>(((m_fSeconds / S_PER_Y) - std::floor(m_fSeconds / S_PER_Y)) * D_PER_Y);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the number of years passed since starting of timer
///
/// \return Number of years that passed since starting of timer
///
////////////////////////////////////////////////////////////////////////////////
inline const uint32_t CSimTimer::getYears() const
{
    METHOD_ENTRY("CSimTimer::getYears")
    return m_unYears;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the number of seconds passed since starting of timer
///
/// Compared to method @a getSecondsPart() this method does not only return the
/// residual value, but the internal floating point counter that counts seconds
/// up to one year.
///
/// \return Number of seconds that passed since starting of timer
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CSimTimer::getSecondsRaw() const
{
    METHOD_ENTRY("CSimTimer::getSecondsRaw")
    return m_fSeconds;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns if timer is active
///
/// \return Is timer active?
///
////////////////////////////////////////////////////////////////////////////////
inline const bool CSimTimer::isActive() const
{
    METHOD_ENTRY("CSimTimer::isActive")
    return m_bActive;
}

#endif // SIM_TIMER_H
