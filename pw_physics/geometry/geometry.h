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
#include "circle.h"
#include "planet.h"
#include "polyline.h"
#include "terrain.h"

//--- Standard header --------------------------------------------------------//

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
        virtual ~CGeometry();

        //--- Constant methods -----------------------------------------------//
        const CBoundingBox&             getBoundingBox() const;
        const std::list<IShape*>* const getShapes() const;
        const std::list<IShape*>* const getPrevShapes() const;
        
        //--- Methods --------------------------------------------------------//
        CBoundingBox&  getBoundingBox();
        
        void addShape(IShape*);
        void updateBoundingBox(const CBoundingBox&);
        void updateBoundingBox(const Vector2d&);
        void setShapes(const std::list<IShape*>);
        void update();
        
    protected:

        //--- Abstract methods [protected] -----------------------------------//

        //-- Variables [protected] -------------------------------------------//
        CBoundingBox m_AABB;                    ///< Bounding box
        
        std::list<IShape*>* m_pShapesCurrent;   ///< Shapes (current timestep)
        std::list<IShape*>* m_pShapesPrevious;  ///< Shapes (previous timestep)
        std::list<IShape*>  m_Shapes1;          ///< Shapes (current or previous timestep)
        std::list<IShape*>  m_Shapes2;          ///< Shapes (current or previous timestep)
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CGeometry::getBoundingBox() const
{
    METHOD_ENTRY("CGeometry::getBoundingBox")

    METHOD_EXIT("CGeometry::getBoundingBox")
    return (m_AABB);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the shapelist of the geometry
///
/// \return Shapelist
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list< IShape* >* const CGeometry::getShapes() const
{
    METHOD_ENTRY("CGeometry::getShapes")

    METHOD_EXIT("CGeometry::getShapes")
    return m_pShapesCurrent;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the shapelist of previous time step
///
/// \return Shapelist
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list< IShape* >* const CGeometry::getPrevShapes() const
{
    METHOD_ENTRY("CGeometry::getPrevShapes")

    METHOD_EXIT("CGeometry::getPrevShapes")
    return m_pShapesPrevious;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns bounding box
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline CBoundingBox& CGeometry::getBoundingBox()
{
    METHOD_ENTRY("CGeometry::getBoundingBox")

    METHOD_EXIT("CGeometry::getBoundingBox")
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

    METHOD_EXIT("CGeometry::updateBoundingBox")
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

    METHOD_EXIT("CGeometry::updateBoundingBox")
}

#endif
