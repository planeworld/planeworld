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
/// \file       thread_module.h
/// \brief      Prototype of class "IThreadModule"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-01-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef THREAD_MODULE_H
#define THREAD_MODULE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "log.h"

//--- Misc header ------------------------------------------------------------//
const double THREAD_MODULE_DEFAULT_FREQUENCY = 60.0;   ///< Default frequency for module

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface defining a general thread module
///
////////////////////////////////////////////////////////////////////////////////
class IThreadModule
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        IThreadModule();
        
        //--- Constant Methods -----------------------------------------------//
        const double&   getFrequency() const;
                
        //--- Methods --------------------------------------------------------//
        virtual void    processFrame() = 0;
        void            setFrequency(const double&);

        #ifdef PW_MULTITHREADING
          void run();
          void terminate();
        #endif
        
    protected:

        //--- Methods [private] ----------------------------------------------//
        #ifdef PW_MULTITHREADING
          virtual void  preRun() {}         ///< Everything that needs to be done before run()
          
          std::string   m_strModuleName;    ///< Name of module
          bool          m_bRunning = false; ///< Indicates if thread is running
        #endif
        
        double          m_fFrequency;       ///< Frequency of module update
        double          m_fTimeSlept;       ///< Sleep time of thread
        double          m_fTimeAccel;       ///< Time acceleration of module
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of module update
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double& IThreadModule::getFrequency() const
{
    METHOD_ENTRY("IThreadModule::getFrequency")
    return (m_fFrequency);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the frequency for module update
///
/// \param _fFrequency Frequency for input update
///
////////////////////////////////////////////////////////////////////////////////
inline void IThreadModule::setFrequency(const double& _fFrequency)
{
    METHOD_ENTRY("IThreadModule::setFrequency")
    m_fFrequency = _fFrequency;
}

#ifdef PW_MULTITHREADING
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Stops module
  ///
  ////////////////////////////////////////////////////////////////////////////////
  inline void IThreadModule::terminate()
  {
      METHOD_ENTRY("IThreadModule::terminate")
      m_bRunning = false;
  }
#endif

#endif
