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
/// \file       rectangle.h
/// \brief      prototype of class "CRectangle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RECTANGLE_H
#define RECTANGLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Geometry"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief class representing a rectangle
/// 
////////////////////////////////////////////////////////////////////////////////
class CRectangle : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CRectangle();
        virtual ~CRectangle();
        
        //--- Const methods --------------------------------------------------//
        const ShapeType         getShapeType() const;
        const Vector2d* const   getVertices() const;

        //--- Methods --------------------------------------------------------//
        void transform(const double&, const Vector2d&);
        void setUL(const Vector2d&);
        void setLR(const Vector2d&);
        
    protected:
    
        Vector2d    m_vecUL0;                               ///< upper left corner (init value)
        Vector2d    m_vecLR0;                               ///< lower right corner (init value)
        Vector2d    m_vecUR0;                               ///< upper right corner (init value)
        Vector2d    m_vecLL0;                               ///< lower left corner (init value)

        Vector2d    m_vecVerts[4];                          ///< Vertices of rectangle

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType CRectangle::getShapeType() const
{
    METHOD_ENTRY("CRectangle::getShapeType")

    METHOD_EXIT("CRectangle::getShapeType")
    return SHAPE_RECTANGLE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get array of vertices
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d* const CRectangle::getVertices() const
{
    METHOD_ENTRY("CRectangle::getVertices");

    METHOD_EXIT("CRectangle::getVertices");
    return m_vecVerts;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines upper left corner of rectangle
///
/// \param _vecUL Upper left corner
///
////////////////////////////////////////////////////////////////////////////////
inline void CRectangle::setUL(const Vector2d& _vecUL)
{
    METHOD_ENTRY("CRectangle::setUL(const Vector2d&)");

    m_vecUL0 = _vecUL;
    m_vecVerts[1] = _vecUL;

    m_vecLL0[0] = _vecUL[0];
    m_vecUR0[1] = _vecUL[1];

    m_vecVerts[0][0] = _vecUL[0];
    m_vecVerts[2][1] = _vecUL[1];

    METHOD_EXIT("CRectangle::setUL(const Vector2d&)");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines lower right corner of rectangle
///
/// \param _vecUL Lower right corner
///
////////////////////////////////////////////////////////////////////////////////
inline void CRectangle::setLR(const Vector2d& _vecLR)
{
    METHOD_ENTRY("CRectangle::setLR(const Vector2d&)");

    m_vecLR0 = _vecLR;
    m_vecVerts[3] = _vecLR;

    m_vecLL0[1] = _vecLR[1];
    m_vecUR0[0] = _vecLR[0];
    m_vecVerts[0][1] = _vecLR[1];
    m_vecVerts[2][0] = _vecLR[0];

    METHOD_EXIT("CRectangle::setLR(const Vector2d&)");
}

#endif
