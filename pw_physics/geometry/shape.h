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
/// \file       shape.h
/// \brief      Prototype of interface "IShape"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
/// \todo Implement shape CDot
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SHAPE_H
#define SHAPE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "bounding_box.h"
#include "engine_common.h"
#include "graphics.h"

//--- Misc header ------------------------------------------------------------//

using namespace Eigen;

/// specifies the type of shape
typedef enum 
{
    SHAPE_NONE,
    SHAPE_CIRCLE,
    SHAPE_PLANET,
    SHAPE_POLYLINE,
    SHAPE_TERRAIN
} ShapeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Abstract class representing a simple shape
///
////////////////////////////////////////////////////////////////////////////////
class IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IShape() : m_nDepthlayers(SHAPE_DEPTH_ALL){}                       ///< Constructor
        virtual ~IShape(){}                                                 ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        virtual IShape*             clone() const = 0;
        const CBoundingBox&         getBoundingBox() const;
        int                         getDepths() const;
        virtual const ShapeType     getShapeType() const;
                
        //--- Methods --------------------------------------------------------//
        virtual void transform(const double&, const Vector2d&) = 0;         ///< Transforms the shape
        
        void setDepths(const int&);
        void unsetDepths(const int&);

    protected:

        //--- Protected Variables --------------------------------------------//
        CBoundingBox    m_AABB;                             ///< Bounding box of shape
        int             m_nDepthlayers;                     ///< Depths in which shape exists
};

//--- Implementation is done here for inline optimisation --------------------//

// ShapeType IShape::m_ShapeType = SHAPE_NONE;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the Axis Aligned Bounding Box 
///
/// \return Returns bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& IShape::getBoundingBox() const
{
    METHOD_ENTRY("IShape::getBoundingBox")

    METHOD_EXIT("IShape::getBoundingBox")
    return m_AABB;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the layers covered by shape
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline int IShape::getDepths() const
{
    METHOD_ENTRY("IShape::getDepths")

    METHOD_EXIT("IShape::getDepths")
    return m_nDepthlayers;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType IShape::getShapeType() const
{
    METHOD_ENTRY("IShape::getShapeType")

    METHOD_EXIT("IShape::getShapeType")
    return SHAPE_NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Set the layers covered by shape
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void IShape::setDepths(const int& _nD)
{
    METHOD_ENTRY("IShape::setDepths")

    m_nDepthlayers |= _nD;

    METHOD_EXIT("IShape::setDepths")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Unset the layers not covered by shape
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void IShape::unsetDepths(const int& _nD)
{
    METHOD_ENTRY("IShape::unsetDepths")

    m_nDepthlayers &= (!_nD);

    METHOD_EXIT("IShape::unsetDepths")
}

#endif
