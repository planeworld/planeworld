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
/// \def COMMA
///         Define a comma which is needed for templates
/// \def SERIALIZE_DECL
///         Macro to use overloaded method for serialisation within the header
/// \def SERIALIZE_IMPL
///         Macro for implementation of serialisation method
/// \def SERIALIZE_IMPL_T
///         Macro for implementation of serialisation method using templates
/// \def SERIALIZE(a, b)
///         Macro serializing the given object
/// \def SERIALIZE_UNARY(a, b)
///         Macro serializing the given object (unary container)
/// \def SERIALIZE_BINARY(a, b)
///         Macro serializing the given object (binary container)
///
////////////////////////////////////////////////////////////////////////////////

#define COMMA ,
#define SERIALIZE_DECL void mySerialize() const override;
#define SERIALIZE_IMPL(a, b) void a::mySerialize() const {b}
#define SERIALIZE_IMPL_T(a, b, c) a void b::mySerialize() const {c}
#define SERIALIZE(a, b) ISerializable::serialize(a, b);
#define SERIALIZE_UNARY(a, b) for (const auto& Elem : b) ISerializable::serialize(a, Elem);
#define SERIALIZE_BINARY(a, b, c) for (const auto& Elem : c) { ISerializable::serialize(a, Elem.first); ISerializable::serialize(b, Elem.second); }

#endif // SERIALIZE_MACROS_H
