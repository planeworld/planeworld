////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2018 Torsten Büschenfeld
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
#include "shape_subtypes.h"
#include "uid_user.h"

//--- Misc header ------------------------------------------------------------//

using namespace Eigen;

/// specifies the type of shape
enum class ShapeType
{
    NONE,
    CIRCLE,
    PLANET,
    POLYGON,
    TERRAIN
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Abstract class representing a simple shape
///
////////////////////////////////////////////////////////////////////////////////
class IShape : public IUIDUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IShape();           ///< Constructor
        virtual ~IShape(){} ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        virtual IShape*             clone() const = 0;
        virtual ShapeType           getShapeType() const;

        const double&               getArea() const;
        const Vector2d&             getCentroid() const;
        int                         getDepths() const;
        const double&               getInertia() const;
        const double&               getMass() const;
        bool                        isValid() const;
                
        //--- Methods --------------------------------------------------------//
        virtual void transform(const double&,
                               const Vector2d&,
                               const Vector2d&) = 0;  ///< Transforms the shape

        CBoundingBox&   getBoundingBox();
        bool&           isValid();
        
        void            setMass(const double&);
        void            copy(const IShape* const);
        void            setDepths(const int&);
        void            unsetDepths(const int&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, IShape* const);
        friend std::ostream& operator<<(std::ostream&, IShape* const);

    protected:

        virtual std::istream& myStreamIn (std::istream&) = 0;
        virtual std::ostream& myStreamOut(std::ostream&) = 0;
        
        virtual void myCopy(const IShape* const) = 0; ///< Copies shape attributes
        virtual void myUpdateGeometry() = 0;
      
        void         updateGeometry();
        
        //--- Protected Variables --------------------------------------------//
        CBoundingBox    m_AABB;                     ///< Bounding box of shape
        bool            m_bIsValid;                 ///< Indicates if shape data is valid
        double          m_fArea;                    ///< Area this shape covers
        double          m_fInertia;                 ///< Inertia of this shape
        double          m_fMass;                    ///< Mass of object part, associated to this shape
        int             m_nDepthlayers;             ///< Depths in which shape exists
        Vector2d        m_vecCentroid;              ///< Centroid of this shape
};

//--- Enum parser ------------------------------------------------------------//
static std::unordered_map<ShapeType, std::string> s_ShapeTypeToStringMap = {
    {ShapeType::NONE, "shp_none"},
    {ShapeType::CIRCLE, "shp_circle"},
    {ShapeType::PLANET, "shp_planet"},
    {ShapeType::POLYGON, "shp_polygon"},
    {ShapeType::TERRAIN, "shp_terrain"}
}; ///< Map from ShapeType to string

const std::unordered_map<std::string, ShapeType> STRING_TO_SHAPE_TYPE_MAP = {
    {"shp_none", ShapeType::NONE},
    {"shp_circle", ShapeType::CIRCLE},
    {"shp_planet", ShapeType::PLANET},
    {"shp_polygon", ShapeType::POLYGON},
    {"shp_terrain", ShapeType::TERRAIN}
}; ///< Map from string to ShapeType

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to shape type
///
/// \return Shape type
///
////////////////////////////////////////////////////////////////////////////////
static ShapeType mapStringToShapeType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToShapeType")
    
    const auto ci = STRING_TO_SHAPE_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_SHAPE_TYPE_MAP.end())
        return ci->second;
    else
        return ShapeType::NONE;
}

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns area covered by this shape
///
/// The area is need e.g. when automatically calculating the centroid of a
/// polygon.
///
/// \return Area covered by this shape
///
////////////////////////////////////////////////////////////////////////////////
inline const double& IShape::getArea() const
{
    METHOD_ENTRY("IShape::getArea")
    return m_fArea;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns centroid, which is the geometric center of the shape
///
/// The geometric center (centroid) equals the center of mass for a constant
/// density i.e. a homogeneous mass distribution.
///
/// \return Centroid of the shape
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& IShape::getCentroid() const
{
    METHOD_ENTRY("IShape::getCentroid")
    return m_vecCentroid;
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
    return m_nDepthlayers;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns inertia
///
/// \return Inertia
///
////////////////////////////////////////////////////////////////////////////////
inline const double& IShape::getInertia() const
{
    METHOD_ENTRY("IShape::getInertia")
    return m_fInertia;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns mass
///
/// \return Mass
///
////////////////////////////////////////////////////////////////////////////////
inline const double& IShape::getMass() const
{
    METHOD_ENTRY("IShape::getMass")
    return m_fMass;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Indicates if shape is still valid.
///
/// Validity might be changed by changing geometry or center of shape, since
/// recalculation of center of mass is neccessary.
///
/// \return Valid?
///
////////////////////////////////////////////////////////////////////////////////
inline bool IShape::isValid() const
{
    METHOD_ENTRY("IShape::isValid")
    return m_bIsValid;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline ShapeType IShape::getShapeType() const
{
    METHOD_ENTRY("IShape::getShapeType")
    return ShapeType::NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief (In)Validate state from outside
///
/// Validity might be changed by changing geometry or center of shape, since
/// recalculation of center of mass is neccessary. If done, states can be
/// reset to valid
///
/// \return Valid?
///
////////////////////////////////////////////////////////////////////////////////
inline bool& IShape::isValid()
{
    METHOD_ENTRY("IShape::isValid")
    return m_bIsValid;
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
/// \brief  Associates given mass of object to this shape
///
/// \param _fM Mass associated to this shape
///
////////////////////////////////////////////////////////////////////////////////
inline void IShape::setMass(const double& _fM)
{
    METHOD_ENTRY("IShape::setMass")
    m_fMass = _fM;
    m_bIsValid = false;
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Update geometry relevant data, e.g. inertia, center of mass, area.
///
////////////////////////////////////////////////////////////////////////////////
inline void IShape::updateGeometry()
{
    METHOD_ENTRY("IShape::updateGeometry")
    m_bIsValid = false;
    this->myUpdateGeometry();
}

#endif
