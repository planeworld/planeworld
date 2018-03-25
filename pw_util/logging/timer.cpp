////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2018 Torsten Büschenfeld
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
/// \file       timer.cpp
/// \brief      Implementation of class "CTimer"
///
/// \date       2009-06-29
/// \author     bfeld
///
////////////////////////////////////////////////////////////////////////////////

#include "timer.h"

#include <string>
#include <thread>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief CTimer
///
///////////////////////////////////////////////////////////////////////////////
CTimer::CTimer() : m_fCountAbsolute(0.0), m_fFrequency(-1.0)
{
    // Initialise m_Start to be sure to have a valid time if stop() is called
    // without calling start() first
    m_Start = std::chrono::high_resolution_clock::now();
    m_StartAbsolute = std::chrono::high_resolution_clock::now();

}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the timer
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::start()
{
    m_Start = std::chrono::high_resolution_clock::now();
    m_fCountAbsolute += 1.0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Stop the timer
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::stop()
{
    m_Stop = std::chrono::high_resolution_clock::now();
    
    m_fDiffTime = std::chrono::duration<double>(m_Stop - m_Start).count();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Restart the timer
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::restart()
{
    this->stop();
    this->start();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sleeps for the time that remains between stop and start given a frequency.
///
/// If a frequency is given for a loop (or a thread) the timer stop, measures
/// the runtime between start and stop and sleeps for the rest of the time
/// according to the frequency.
/// Sleep remaining automatically calls the start and stop method of timer.
///
/// \param _fFreq Frequency of the loop
/// \return Sleep time in seconds, might be negative if no time left
///
///////////////////////////////////////////////////////////////////////////////
double CTimer::sleepRemaining(const double& _fFreq)
{
    using namespace std::chrono_literals;
    
    if (m_fFrequency == -1.0)
    {
        m_fFrequency = _fFreq;
    }
    else if (m_fFrequency != _fFreq)
    {
        m_fFrequency = _fFreq;
        m_fCountAbsolute = 1.0;
        m_StartAbsolute = std::chrono::high_resolution_clock::now();
    }
    
    this->stop();
    double fFrametime = (1.0/_fFreq-m_fDiffTime);
    
    std::unique_lock<std::mutex> lk(m_MutexCV);
    if (m_CV.wait_until(lk, m_StartAbsolute + std::chrono::duration<double>(m_fCountAbsolute/_fFreq), [](){return false;}))
    m_MutexCV.lock();
    
    this->start();
    
    return fFrametime;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Get split time
///
/// This method returns the time between now and start.
///
///////////////////////////////////////////////////////////////////////////////
double CTimer::getSplitTime()
{
    return std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - m_Start
        ).count();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _Timer CTimer instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CTimer& _Timer)
{
    std::string strTmp;
    _is >> strTmp;
    _is >> _Timer.m_fDiffTime;

    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _Timer CTimer instance to stream
///
/// \return Stream with game state information of CTimer instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CTimer& _Timer)
{
    _os << "Timer:" << std::endl;
    _os << _Timer.m_fDiffTime << std::endl;

    return _os;
}
