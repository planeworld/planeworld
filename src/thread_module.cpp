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
/// \file       thread_module.cpp
/// \brief      Implementation of class "IThreadModule"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-01-25
///
////////////////////////////////////////////////////////////////////////////////

#include "thread_module.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
IThreadModule::IThreadModule() : m_fFrequency(THREAD_MODULE_DEFAULT_FREQUENCY),
                                 m_fTimeSlept(1.0),
                                 m_fTimeAccel(1.0)
{
    METHOD_ENTRY("IThreadModule::IThreadModule")
    CTOR_CALL("IThreadModule::IThreadModule")
    
    #ifdef PW_MULTITHREADING
        m_strModuleName = "Thread Module";
    #endif
}

#ifdef PW_MULTITHREADING
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Runs the visuals engine, called as a thread.
  ///
  ///////////////////////////////////////////////////////////////////////////////
  void IThreadModule::run()
  {
      METHOD_ENTRY("IThreadModule::run")
      
      INFO_MSG("Thread Module", m_strModuleName << " started.")
      
      this->preRun();
      m_bRunning = true;
      
      CTimer ThreadModuleTimer;
      
      ThreadModuleTimer.start();
      while (m_bRunning)
      {
          this->processFrame();
          m_fTimeSlept = ThreadModuleTimer.sleepRemaining(m_fFrequency*m_fTimeAccel);
          
          if (m_fTimeSlept < 0.0)
          {
              DEBUG_MSG("Thread Module", "Execution time of thread " << m_strModuleName << " is too large: " << 1.0/m_fFrequency - m_fTimeSlept << 
                                          "s of " << 1.0/m_fFrequency << "s max.")
          }
      }
      INFO_MSG("Thread Module", m_strModuleName << " stopped.")
  }
#endif

