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
/// \file       log_listener.h
/// \brief      Prototype of interface "ILogListener"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-05-21
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_LISTENER_H
#define LOG_LISTENER_H

//--- Standard header --------------------------------------------------------//
#include <string>

//--- Program header ---------------------------------------------------------//
#include "log_common_types.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes track log entries
///
////////////////////////////////////////////////////////////////////////////////
class ILogListener
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//

        //--- Methods --------------------------------------------------------//
        virtual void logEntry(const std::string&, const std::string&,
                              const LogLevelType&, const LogDomainType&) = 0;
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // LOG_LISTENER_H
