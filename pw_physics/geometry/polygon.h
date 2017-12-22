////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       polygon.h
/// \brief      Prototype of class "CPolygon"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef POLYGON_H
#define POLYGON_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"
#include "shape_subtypes.h"

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing connected lines
///
////////////////////////////////////////////////////////////////////////////////
class CPolygon : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CPolygon() : m_PolygonType(PolygonType::FILLED){}
        virtual ~CPolygon(){};
        
        //--- Constant Methods -----------------------------------------------//
        CPolygon*              clone() const;
        
        const PolygonType&      getPolygonType() const;
              ShapeType         getShapeType() const;
        const VertexListType&   getVertices() const;

        //--- Methods --------------------------------------------------------//
        void addVertex(const Vector2d&);
        void addVertex(const double&, const double&);
        void transform(const double&, const Vector2d&, const Vector2d&);
        
        void setPolygonType(const PolygonType&);

    protected:
      
        //--- Protected methods ----------------------------------------------//
        std::istream&           myStreamIn(std::istream&);
        std::ostream&           myStreamOut(std::ostream&);
        
        void myCopy(const IShape* const);
        void myUpdateGeometry();

        //--- Protected variables --------------------------------------------//
        PolygonType             m_PolygonType;  ///< Type of polygon
        VertexListType          m_VertList;     ///< List of vertices
        VertexListType          m_VertList0;    ///< List of vertices

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the linetype
///
////////////////////////////////////////////////////////////////////////////////
inline const PolygonType& CPolygon::getPolygonType() const
{
    METHOD_ENTRY("CPolygon::getPolygonType()");
    return m_PolygonType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get pointer on array of vertices
///
////////////////////////////////////////////////////////////////////////////////
inline const VertexListType& CPolygon::getVertices() const
{
    METHOD_ENTRY("CPolygon::getVertices");
    return m_VertList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline ShapeType CPolygon::getShapeType() const
{
    METHOD_ENTRY("CPolygon::getShapeType")
    return ShapeType::POLYGON;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the polygon type
///
/// \param _PolygonType PolygonType
///
////////////////////////////////////////////////////////////////////////////////
inline void CPolygon::setPolygonType(const PolygonType& _PolygonType)
{
    METHOD_ENTRY("CPolygon::setPolygonType(const PolygonType&)");

    m_PolygonType = _PolygonType;
}

#endif

