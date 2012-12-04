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
/// \note Timer does not use logging any longer, since it is used by
///       logging class.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H

//--- Standard header --------------------------------------------------------//
#include <unistd.h>
#include <sys/time.h>

//--- Program header ---------------------------------------------------------//

/// Factor to upscale time from u-seconds to seconds
const double TIMER_OUTPUT_SEC_FACTOR = 0.000001;

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
        inline double   getTime() const {return m_fDiffTime;}       ///< returns difftime
    private:

        //--- Private Variables ----------------------------------------------//
        struct timeval  m_Start;                        ///< Starting time
        struct timeval  m_Stop;                         ///< Stopping time
        struct timezone m_TimeZone;                     ///< Timezone

        bool    m_bFirstCall;                           ///< Flag for initialisation

        double  m_fDiffTime;                            ///< Time between start and stop

};

#endif
