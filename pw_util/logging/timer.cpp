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
/// \file       timer.cpp
/// \brief      Implementation of class "CTimer"
///
/// \date       2009-06-29
/// \author     bfeld
///
////////////////////////////////////////////////////////////////////////////////

#include "timer.h"


///////////////////////////////////////////////////////////////////////////////
///
/// \brief CTimer
///
///////////////////////////////////////////////////////////////////////////////
CTimer::CTimer():m_bFirstCall(true)
{
//  METHOD_ENTRY("CTimer::CTimer()");
//  CTOR_CALL("CTimer::CTimer()");

    gettimeofday(&m_Start, &m_TimeZone);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the timer
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::start()
{
//  METHOD_ENTRY("CTimer::start()");

    m_bFirstCall = false;
    gettimeofday(&m_Start, &m_TimeZone);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Stop the timer
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::stop()
{
//  METHOD_ENTRY("CTimer::stop()");

    gettimeofday(&m_Stop, &m_TimeZone);
    if (!m_bFirstCall)
        m_fDiffTime = (m_Stop.tv_sec - m_Start.tv_sec)+double(m_Stop.tv_usec - m_Start.tv_usec)*TIMER_OUTPUT_SEC_FACTOR;
    else
        m_fDiffTime = 0.0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sleeps for the time that remains between stop and start given a frequency.
///
/// If a frequency is given for a loop (or a thread) the timer stop, measures
/// the runtime between start and stop and sleeps for the rest of the time
/// according to the frequency.
///
/// \param _fFreq Frequency of the loop
///
///////////////////////////////////////////////////////////////////////////////
void CTimer::sleepRemaining(const double& _fFreq)
{
//  METHOD_ENTRY("CTimer::sleepRemaining")

    this->stop();
    double fFrametime = 1.0/_fFreq-this->getTime();
    if (fFrametime > 0.0)
    {
        unsigned int unFrametime = static_cast<unsigned int>(fFrametime*1e6);
        usleep(unFrametime);
    }
    this->start();
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
//  METHOD_ENTRY("CTimer::getSplitTime()");
    struct timeval tmpStop;

    gettimeofday(&tmpStop, &m_TimeZone);
    if (!m_bFirstCall)
        return ((tmpStop.tv_sec - m_Start.tv_sec) +
                double(tmpStop.tv_usec - m_Start.tv_usec)*TIMER_OUTPUT_SEC_FACTOR);
    else
        return 0.0;
}
