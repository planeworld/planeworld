////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       emitter_referrer.h
/// \brief      Prototype of interface "IEmitterReferrer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef EMITTER_REFERRER_H
#define EMITTER_REFERRER_H

//--- Program header ---------------------------------------------------------//
#include "emitter.h"
#include "unique_id_referrer.h"

/// Type definition for specialised interface referring to emitters
typedef IUniqueIDReferrer<IEmitter> IEmitterReferrer;

#endif // EMITTER_REFERRER_H
