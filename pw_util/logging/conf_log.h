////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2017 Torsten Büschenfeld
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
/// \file		conf_log.h
/// \brief		Configuration for logging class "CLog"
///
/// \date		2009-06-29
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONF_LOG_H
#define CONF_LOG_H

// Version of the configuration
#define LOGGING_VERSION 2

//--- Configuration of loglevel: ---------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \def LOGLEVEL_DEBUG
///			Sets the actual loglevel (level debug)
/// \def LOGLEVEL_INFO
///			Sets the actual loglevel (level info)
/// \def LOGLEVEL_NOTICE
///			Sets the actual loglevel (level notice)
/// \def LOGLEVEL_WARNING
///			Sets the actual loglevel (level warning)
/// \def LOGLEVEL_ERROR
///			Sets the actual loglevel (level error)
/// \def LOGLEVEL_NONE
///			Sets the actual loglevel (level none)
///
/// \def DOMAIN_NONE
///			Default, if no domain is specified
/// \def DOMAIN_METHOD_ENTRY
///			Defines if method entries should be logged
/// \def DOMAIN_METHOD_EXIT
///			Defines if method exits should be logged
/// \def DOMAIN_CONSTRUCTOR
///			Defines if constructor calls should be logged
/// \def DOMAIN_DESTRUCTOR
///			Defines if constructor calls should be logged
/// \def DOMAIN_MEMORY_ALLOCATED
///			Defines if memory allocation should be logged
/// \def DOMAIN_MEMORY_FREED
///			Defines if memory deallocation should be logged
/// \def DOMAIN_DEV_LOGIC
///         Defines if development logic should be checked
/// \def DOMAIN_STATS 
///         Defines if statistics should be printed out
/// \def DOMAIN_VAR	
///			Defines if variables should be printed out
/// \def DOMAIN_FILEIO
///			Defines if file input/output operations should be printed out
///
////////////////////////////////////////////////////////////////////////////////

//--- Allow locking, e.g. when using progress bars ---//
//====================================================//
#define LOG_LOCKING_ON

//--- Indention of output on/off ---//
//==================================//
#define OUTPUT_INDENTION

//--- Otherwise use custom loglevel 
//--- Uncomment one (only one!) level to be used for displaying ---//
//=================================================================//

// #define LOGLEVEL_DEBUG
#define LOGLEVEL_INFO
// #define LOGLEVEL_NOTICE
// #define LOGLEVEL_WARNING
// #define LOGLEVEL_ERROR
// #define LOGLEVEL_NONE

//--- Uncomment all domains that should be used for logging ---//
//=============================================================//

#define DOMAIN_NONE
#define DOMAIN_METHOD_ENTRY
#define DOMAIN_METHOD_EXIT
#define DOMAIN_CONSTRUCTOR
#define DOMAIN_DESTRUCTOR
#define DOMAIN_MEMORY_ALLOCATED
#define DOMAIN_MEMORY_FREED
#define DOMAIN_DEV_LOGIC
#define DOMAIN_STATS
#define DOMAIN_VAR
#define DOMAIN_FILEIO

//--- Allow global DEBUG definition typically passed by compiler ---//
//==================================================================//

#ifdef DEBUG
 #define LOGLEVEL_DEBUG
 #undef LOGLEVEL_INFO
 #undef LOGLEVEL_NOTICE
 #undef LOGLEVEL_WARNING
 #undef LOGLEVEL_ERROR
 #undef LOGLEVEL_NONE
#endif

//--- End of configuration ---------------------------------------------------//

#endif
