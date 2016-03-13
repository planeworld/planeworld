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
/// \file       sim_timer.cpp
/// \brief      Implementation of class "CSimTimer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-13
///
////////////////////////////////////////////////////////////////////////////////

#include "sim_timer.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Increments counter by given value. Base is seconds, decimal places
///        are allowed.
///
/// \param _fSeconds Seconds to increment counter by
///
////////////////////////////////////////////////////////////////////////////////
void CSimTimer::inc(const double& _fSeconds)
{
    METHOD_ENTRY("CSimTimer::inc")
    
    constexpr double S_PER_Y = 365.0*24.0*60.0*60.0;
    if (m_bActive)
    {
        m_fSeconds += _fSeconds;
        while (m_fSeconds >= S_PER_Y)
        {
            m_fSeconds -= S_PER_Y;
            ++m_unYears;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Starts the timer, i.e. the timer accepts increments via @a inc()
///
////////////////////////////////////////////////////////////////////////////////
void CSimTimer::start()
{
    METHOD_ENTRY("CSimTimer::start")
    m_bActive = true;
    m_fSeconds = 0.0;
    m_unYears = 0u;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stops the timer, i.e. the timer ignores increments via @a inc()
///
////////////////////////////////////////////////////////////////////////////////
void CSimTimer::stop()
{
    METHOD_ENTRY("CSimTimer::stop")
    m_bActive = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Toggles the timer between @a start() and @a stop().
///
////////////////////////////////////////////////////////////////////////////////
void CSimTimer::toggle()
{
    METHOD_ENTRY("CSimTimer::toggle")
    if (m_bActive)
    {
        this->stop();
    }
    else
    {
        this->start();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _SimTimer CSimTimer instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CSimTimer& _SimTimer)
{
    METHOD_ENTRY("CSimTimer::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> _SimTimer.m_bActive;
    _is >> _SimTimer.m_fSeconds;
    _is >> _SimTimer.m_unYears;
    
    return _is;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _SimTimer CSimTimer instance to stream
///
/// \return Stream with game state information of CSimTimer instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CSimTimer& _SimTimer)
{
    METHOD_ENTRY("CSimTimer::operator<<")
    
    _os << "SimTimer:" << std::endl;
    
    _os << _SimTimer.m_bActive << std::endl;
    _os << _SimTimer.m_fSeconds << std::endl;
    _os << _SimTimer.m_unYears << std::endl;
    
    return _os;
}
