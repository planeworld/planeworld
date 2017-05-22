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
/// \file       log_common_types.h
/// \brief      Common types used by logging system
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-05-21
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_COMMON_TYPES
#define LOG_COMMON_TYPES

/// Represents logging level
typedef enum 
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG 
} LogLevelType;

/// Represents logging domain
typedef enum 
{
    LOG_DOMAIN_NONE,
    LOG_DOMAIN_METHOD_ENTRY,
    LOG_DOMAIN_METHOD_EXIT,
    LOG_DOMAIN_CONSTRUCTOR,
    LOG_DOMAIN_DESTRUCTOR,
    LOG_DOMAIN_MEMORY_ALLOCATED,
    LOG_DOMAIN_MEMORY_FREED,
    LOG_DOMAIN_DEV_LOGIC,
    LOG_DOMAIN_STATS,
    LOG_DOMAIN_VAR,
    LOG_DOMAIN_FILEIO
} LogDomainType;

/// Represents logging level
typedef enum 
{
    LOG_COLOUR_SCHEME_DEFAULT,
    LOG_COLOUR_SCHEME_MONOONBLACK,
    LOG_COLOUR_SCHEME_MONOONWHITE,
    LOG_COLOUR_SCHEME_ONBLACK,
    LOG_COLOUR_SCHEME_ONWHITE
} LogColourSchemeType;

#endif // LOG_COMMON_TYPES
