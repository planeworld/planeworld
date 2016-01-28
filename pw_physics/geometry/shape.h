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
#include "graphics.h"
#include "unique_id_user.h"

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
class IShape : public IUniqueIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IShape() : m_nDepthlayers(SHAPE_DEPTH_ALL){}        ///< Constructor
        virtual ~IShape(){}                                 ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        virtual IShape*             clone() const = 0;
        int                         getDepths() const;
        virtual const ShapeType     getShapeType() const;
                
        //--- Methods --------------------------------------------------------//
        virtual void copy(const IShape* const) = 0; ///< Copies shape attributes
        virtual void transform(const double&, const Vector2d&) = 0;  ///< Transforms the shape

        CBoundingBox&   getBoundingBox();
        void            setDepths(const int&);
        void            unsetDepths(const int&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, IShape* const);
        friend std::ostream& operator<<(std::ostream&, IShape* const);

    protected:

        virtual std::istream& myStreamIn (std::istream& _is){return _is;}
        virtual std::ostream& myStreamOut(std::ostream&) = 0;
      
        //--- Protected Variables --------------------------------------------//
        CBoundingBox    m_AABB;                             ///< Bounding box of shape
        int             m_nDepthlayers;                     ///< Depths in which shape exists
};

//--- Implementation is done here for inline optimisation --------------------//

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
    return SHAPE_NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the Axis Aligned Bounding Box 
///
/// \return Returns bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline CBoundingBox& IShape::getBoundingBox()
{
    METHOD_ENTRY("IShape::getBoundingBox")
    return m_AABB;
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
}

#endif
