////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2019 Torsten Büschenfeld
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
/// \file       entity.h
/// \brief      Prototype of class "CEntity"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-01-21
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>

//--- Program header ---------------------------------------------------------//
#include "handle_mixin.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Basic entity class
///
////////////////////////////////////////////////////////////////////////////////
template<typename T>
using CEntity = CHandleMixin<T>;

#endif // ENTITY_H
