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
/// \file       shape_subtypes.h
/// \brief      Enumeration used by graphics and physical geometry
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-12-22
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SHAPE_SUBTYPES_H
#define SHAPE_SUBTYPES_H

#include <vector>

/// Specifies the type of circle to be drawn
enum class CircleType
{
    FILLED,
    OUTLINE
};

/// Specifies the type of polygon to be drawn
enum class PolygonType
{
    FILLED,
    LINE_LOOP,
    LINE_SINGLE,
    LINE_STRIP
};

/// Type definition for vertex list
typedef std::vector<Vector2d> VertexListType;

//--- Enum parser ------------------------------------------------------------//
static std::unordered_map<PolygonType, std::string> s_PolygonTypeToStringMap = {
    {PolygonType::FILLED, "filled"},
    {PolygonType::LINE_LOOP, "line_loop"},
    {PolygonType::LINE_SINGLE, "line_single"},
    {PolygonType::LINE_STRIP, "line_strip"}
}; ///< Map from PolygonType to string

const std::unordered_map<std::string, PolygonType> STRING_TO_POLYGON_TYPE_MAP = {
    {"filled", PolygonType::FILLED},
    {"line_loop", PolygonType::LINE_LOOP},
    {"line_single", PolygonType::LINE_SINGLE},
    {"line_strip", PolygonType::LINE_STRIP}
}; ///< Map from string to PolygonType

#endif // SHAPE_SUBTYPES_H
