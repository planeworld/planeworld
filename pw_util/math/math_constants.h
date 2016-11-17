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
/// \file       math_constants.h
/// \brief      Common constants for the planeworld engine
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-11-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

//--- Program header ---------------------------------------------------------//

//--- Standard header --------------------------------------------------------//
#include <cmath>

//--- Misc header ------------------------------------------------------------//
const double MATH_PI = std::asin(1.0);
const double MATH_2PI = 2.0 * MATH_PI;
const double MATH_PI2 = MATH_PI * 0.5;
const double MATH_SQRT2 = std::sqrt(2.0);

#endif // MATH_CONSTANTS_H
