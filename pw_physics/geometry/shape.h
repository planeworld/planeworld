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
// #include <list>

//--- Program header ---------------------------------------------------------//
#include "bounding_box.h"
#include "engine_common.h"
#include "graphics.h"
#include "visuals.h"

//--- Misc header ------------------------------------------------------------//
#include <boost/thread.hpp> 

using namespace Eigen;

/// specifies the type of shape
typedef enum 
{
    SHAPE_NONE,
    SHAPE_CIRCLE,
    SHAPE_PLANET,
    SHAPE_POLYLINE,
    SHAPE_RECTANGLE,
    SHAPE_TERRAIN
} ShapeType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Abstract class representing a simple shape
///
/// \todo Implement clone method
/// 
////////////////////////////////////////////////////////////////////////////////
class IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IShape() : m_nDepthlayers(0){}                                      ///< Constructor
        virtual ~IShape(){}                                                 ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        virtual IShape*             clone() const = 0;
        const CBoundingBox&         getBoundingBox() const;
        int                         getDepths() const;
        virtual const ShapeType     getShapeType() const;
                
        //--- Methods --------------------------------------------------------//
        virtual void transform(const double&, const Vector2d&) = 0;         ///< Transforms the shape
        
        void setVisualsID(const VisualsIDType&);
        void setDepths(const int&);
        void unsetDepths(const int&);

    protected:

        //--- Protected Variables --------------------------------------------//
        CBoundingBox    m_AABB;                             ///< Bounding box of shape
        int             m_nDepthlayers;                     ///< Depths in which shape exists
        VisualsIDType   m_VisualsID;                        ///< ID to visual
        
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
/// \brief Sets the ID of shapes visuals to shape
///
/// The shape just stores the ID to a visual. The visual itself is handled by 
/// the visualsmanager.
///
/// \param _VisualsID ID of visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
inline void IShape::setVisualsID(const VisualsIDType& _VisualsID)
{
    METHOD_ENTRY("IShape::setVisualsID")
    
    m_VisualsID = _VisualsID;

    METHOD_EXIT("IShape::setVisualsID")
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
