/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
///
/// \file       engine_common.h
/// \brief      Common enumerations, constants etc for the planeworld engine
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_COMMON_H
#define ENGINE_COMMON_H

//--- Program header ---------------------------------------------------------//

//--- Standard header --------------------------------------------------------//

/// Default depth for drawing
const double SHAPE_DEFAULT_DEPTH = -15.0;

/// Specifies bitcodes for additional visuals
enum Visuals
{
    VISUALS_OBJECT_BBOXES = 1,
    VISUALS_OBJECT_INTERSECTIONS = 2,
    VISUALS_UNIVERSE_GRID = 4
};

/// Specifies depthlayer bitcodes
enum ShapeDepths
{
    SHAPE_DEPTH_1 = 1,
    SHAPE_DEPTH_2 = 2,
    SHAPE_DEPTH_3 = 4,
    SHAPE_DEPTH_4 = 8,
    SHAPE_DEPTH_5 = 16,
    SHAPE_DEPTH_6 = 32,
    SHAPE_DEPTH_7 = 64,
    SHAPE_DEPTH_8 = 128,
    SHAPE_DEPTH_ALL = 255
};

#endif
