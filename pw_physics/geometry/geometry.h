////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
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
#include "shape.h"

//--- Standard header --------------------------------------------------------//
#include <list>

typedef std::list<IShape*> ShapesType; ///< Specifies a list of  shapes

/// Specifies the type of bounding box
enum class AABBType
{
    SINGLEFRAME,
    MULTIFRAME
};

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
        
        CGeometry& operator=(const CGeometry&);

        //--- Constant methods -----------------------------------------------//
        CGeometry*                clone() const;
        const Vector2d&           getCOM() const;
        const double&             getInertia() const;
        const double&             getMass() const;
        const ShapesType&         getShapes() const;
        
        //--- Methods --------------------------------------------------------//
        CBoundingBox& getBoundingBox(AABBType = AABBType::MULTIFRAME);
        
        void addShape(IShape* const);
        void updateBoundingBox(const CBoundingBox&);
        void updateBoundingBox(const Vector2d&);
        void setShapes(const ShapesType&);
        void transform(const double&, const Vector2d&);
        void update();
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CGeometry&);
        friend std::ostream& operator<<(std::ostream&, CGeometry&);
        
    protected:

        //--- Methods [protected] --------------------------------------------//
        void copy(const CGeometry&);

        //-- Variables [protected] -------------------------------------------//
        CBoundingBox    m_AABB;       ///< Bounding box, multiframe, covering movement
        CBoundingBox    m_AABBS;      ///< Bounding box, single frame
        ShapesType      m_Shapes;     ///< Shapes
        Vector2d        m_vecCOM;     ///< Center of mass
        double          m_fInertia;   ///< Inertia of whole geometry
        double          m_fMass;      ///< Mass of whole geometry
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return center of mass of the geometry
///
/// \return Center of mass
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CGeometry::getCOM() const
{
    METHOD_ENTRY("CGeometry::getCOM")
    return m_vecCOM;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return inertia of the geometry
///
/// \return Inertia of the geometry
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CGeometry::getInertia() const
{
    METHOD_ENTRY("CGeometry::getInertia")
    return m_fInertia;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return mass of the geometry
///
/// \return Mass of the geometry
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CGeometry::getMass() const
{
    METHOD_ENTRY("CGeometry::getMass")
    return m_fMass;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the shapelist of the geometry
///
/// \return Shapelist
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapesType& CGeometry::getShapes() const
{
    METHOD_ENTRY("CGeometry::getShapes")
    return m_Shapes;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box
///
/// \param _AABBType Type of bounding box, either single or multi frame
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline CBoundingBox& CGeometry::getBoundingBox(AABBType _AABBType)
{
    METHOD_ENTRY("CGeometry::getBoundingBox")
    if (_AABBType == AABBType::MULTIFRAME)
        return m_AABB;
    else
        return m_AABBS;
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
