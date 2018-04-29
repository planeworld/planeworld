////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2018 Torsten Büschenfeld
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
/// \file       serialize_macros.h
/// \brief      Manages some macros for serializing game state
///
/// \date       2018-04-27
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZE_MACROS_H
#define SERIALIZE_MACROS_H

#include "conf_log.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \def USE_SERIALIZATION
///         Macro to use overloaded method for serialisation within the header
/// \def SERIALIZE(a, b)
///         Macro serializing the given object
/// \def SERIALIZE_UNARY(a, b)
///         Macro serializing the given object (unary container)
///
////////////////////////////////////////////////////////////////////////////////

#define SERIALIZE_DECL void mySerialize(const std::string&) const override;
#define SERIALIZE(a, b) ISerializable::serialize(a, b);
#define SERIALIZE_UNARY(a, b) for (const auto& Elem : b) ISerializable::serialize(a, Elem);

#define SERIALIZE_IMPL(c, d) void c::mySerialize(const std::string& _strDescr) const {d}

#endif // SERIALIZE_MACROS_H
