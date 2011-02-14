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
#include "eigen2/Eigen/Geometry"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing connected lines
///
/// \todo Remove m_VertList0 if random line handles array.
/// 
////////////////////////////////////////////////////////////////////////////////
class CPolyLine : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CPolyLine();
        virtual ~CPolyLine();
        
        //--- Constant Methods -----------------------------------------------//
        const LineType&             getLineType() const;
        const ShapeType             getShapeType() const;
        const std::list<Vector2d>&  getVertices() const;

        //--- Methods --------------------------------------------------------//
        void addVertex(const Vector2d&);
        void addVertex(const double&, const double&);
        void finish();
        void transform(const double&, const Vector2d&);
        
        void setLineType(const LineType&);


    protected:

        LineType                m_LineType;     ///< Type of polyline
        std::list<Vector2d>     m_VertList;     ///< List of vertices
        std::list<Vector2d>     m_VertList0;    ///< List of vertices

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

    METHOD_EXIT("CPolyLine::getLineType()");
    return m_LineType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get pointer on array of vertices
///
////////////////////////////////////////////////////////////////////////////////
inline const std::list<Vector2d>& CPolyLine::getVertices() const
{
    METHOD_ENTRY("CPolyLine::getVertices");

    METHOD_EXIT("CPolyLine::getVertices");
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

    METHOD_EXIT("CPolyLine::getShapeType")
    return SHAPE_POLYLINE;
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
    METHOD_ENTRY("CPolyLine::addVertex(const Vector2d&)");

    m_VertList0.push_back(_vecV);
    m_VertList.push_back(_vecV);

    METHOD_EXIT("CPolyLine::addVertex(const Vector2d&)");
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
    METHOD_ENTRY("CPolyLine::addVertex(const double&, const double&)");

    m_VertList0.push_back(Vector2d(_fX, _fY));
    m_VertList.push_back(Vector2d(_fX, _fY));

    METHOD_EXIT("CPolyLine::addVertex(const double&, const double&)");
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

    METHOD_EXIT("CPolyLine::setLineType(const LineType&)");
}

#endif
