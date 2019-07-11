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
/// \file       camera_entity.h
/// \brief      Prototype of class "CCameraEntity"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-07-07
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_ENTITY_H
#define CAMERA_ENTITY_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "entity.h"
#include "camera.h"

/// Type definition for a camera entity
typedef CEntity<CCamera> CCameraEntity;

#endif // CAMERA_ENTITY_H
