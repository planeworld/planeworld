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
/// \file       polyline.h
/// \brief      Prototype of class "CPolyLine"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef POLYLINE_H
#define POLYLINE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing connected lines
///
////////////////////////////////////////////////////////////////////////////////
class CPolyLine : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CPolyLine() : m_LineType(LineType::GRAPHICS_LINETYPE_STRIP){}
        virtual ~CPolyLine(){};
        
        //--- Constant Methods -----------------------------------------------//
        CPolyLine*              clone() const;
        
        const LineType&         getLineType() const;
        const ShapeType         getShapeType() const;
        const VertexListType&   getVertices() const;

        //--- Methods --------------------------------------------------------//
        void addVertex(const Vector2d&);
        void addVertex(const double&, const double&);
        void copy(const IShape* const);
        void finish();
        void transform(const double&, const Vector2d&);
        
        void setLineType(const LineType&);


    protected:
      
        //--- Protected methods ----------------------------------------------//
        std::istream&           myStreamIn(std::istream&);
        std::ostream&           myStreamOut(std::ostream&);

        //--- Protected variables --------------------------------------------//
        LineType                m_LineType;     ///< Type of polyline
        VertexListType          m_VertList;     ///< List of vertices
        VertexListType          m_VertList0;    ///< List of vertices

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the linetype
///
////////////////////////////////////////////////////////////////////////////////
inline const LineType& CPolyLine::getLineType() const
{
    METHOD_ENTRY("CPolyLine::getLineType()");
    return m_LineType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get pointer on array of vertices
///
////////////////////////////////////////////////////////////////////////////////
inline const VertexListType& CPolyLine::getVertices() const
{
    METHOD_ENTRY("CPolyLine::getVertices");
    return m_VertList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType CPolyLine::getShapeType() const
{
    METHOD_ENTRY("CPolyLine::getShapeType")
    return ShapeType::SHAPE_POLYLINE;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a vertex to the current list of vertices
///
/// \param _vecV Vertex to be added to list
///
///////////////////////////////////////////////////////////////////////////////
inline void CPolyLine::addVertex(const Vector2d& _vecV)
{
    METHOD_ENTRY("CPolyLine::addVertex");

    m_VertList0.push_back(_vecV);
    m_VertList.push_back(_vecV);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a vertex to the current list of vertices
///
/// \param _fX Vertex x-coordinate to be added to list
/// \param _fY Vertex y-coordinate to be added to list
///
///////////////////////////////////////////////////////////////////////////////
inline void CPolyLine::addVertex(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CPolyLine::addVertex");

    m_VertList0.push_back(Vector2d(_fX, _fY));
    m_VertList.push_back(Vector2d(_fX, _fY));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the linetype
///
/// \param _LT LineType
///
////////////////////////////////////////////////////////////////////////////////
inline void CPolyLine::setLineType(const LineType& _LT)
{
    METHOD_ENTRY("CPolyLine::setLineType(const LineType&)");

    m_LineType = _LT;
}

#endif

