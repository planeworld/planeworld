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

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

/// Default depth for drawing
const double   SHAPE_DEFAULT_DEPTH = -15.0;
const double   DEFAULT_CELL_SIZE = 1.0e12;
const double   DEFAULT_CELL_SIZE_2 = 2.0*DEFAULT_CELL_SIZE;
const Vector2d DEFAULT_CELL_SIZE_VEC(DEFAULT_CELL_SIZE,DEFAULT_CELL_SIZE);

/// Default precision for approximation of comparisons
const double DEFAULT_APPROX_PRECISION = 0.0001;

/// Specifies bitcodes for additional visuals
enum Visuals
{
    VISUALS_OBJECT_BBOXES = 1,
    VISUALS_UNIVERSE_GRID = 2,
    VISUALS_OBJECT_TRAJECTORIES = 4,
    VISUALS_NAMES = 8
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

inline bool isApproxZero(const double& _f1)
{
    return (std::abs(_f1) < DEFAULT_APPROX_PRECISION);
}

inline bool isApproxEqual(const double& _f1, const double& _f2)
{
    return (std::abs(_f1 - _f2) < DEFAULT_APPROX_PRECISION);
}

#endif
