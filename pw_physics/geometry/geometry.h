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
/// \file       geometry.h
/// \brief      Prototype of interface "CGeometry"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_H
#define GEOMETRY_H

//--- Program header ---------------------------------------------------------//
#include "double_buffered_shape.h"

//--- Standard header --------------------------------------------------------//

typedef std::list<CDoubleBufferedShape*> DBShapesType; ///< Specifies a list of double buffered shapes

// Forward declarations

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for general geometrys
///
////////////////////////////////////////////////////////////////////////////////
class CGeometry
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CGeometry();
        CGeometry(const CGeometry&);
        virtual ~CGeometry();

        //--- Constant methods -----------------------------------------------//
        CGeometry*                clone() const;
        const DBShapesType* const getShapes() const;
        
        //--- Methods --------------------------------------------------------//
        CBoundingBox& getBoundingBox();
        
        void addShape(CDoubleBufferedShape* const);
        void updateBoundingBox(const CBoundingBox&);
        void updateBoundingBox(const Vector2d&);
        void setShapes(std::list<CDoubleBufferedShape*>* const);
        void update();
        
        CGeometry& operator=(const CGeometry&);
        
    protected:

        //--- Abstract methods [protected] -----------------------------------//

        //-- Variables [protected] -------------------------------------------//
        CBoundingBox    m_AABB;       ///< Bounding box
        DBShapesType*   m_pShapes;    ///< Double buffered shapes
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the shapelist of the geometry
///
/// \return Shapelist
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list<CDoubleBufferedShape*>* const CGeometry::getShapes() const
{
    METHOD_ENTRY("CGeometry::getShapes")
    return m_pShapes;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline CBoundingBox& CGeometry::getBoundingBox()
{
    METHOD_ENTRY("CGeometry::getBoundingBox")
    return (m_AABB);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates the bounding box
///
/// \param _BBox Bounding box to update with
///
///////////////////////////////////////////////////////////////////////////////
inline void CGeometry::updateBoundingBox(const CBoundingBox& _BBox)
{
    METHOD_ENTRY("CGeometry::updateBoundingBox")
    m_AABB.update(_BBox);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates the bounding box
///
/// \param _vecV Vector to update with
///
///////////////////////////////////////////////////////////////////////////////
inline void CGeometry::updateBoundingBox(const Vector2d& _vecV)
{
    METHOD_ENTRY("CGeometry::updateBoundingBox")
    m_AABB.update(_vecV);
}

#endif
