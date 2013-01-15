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
/// \file       circle.h
/// \brief      Prototype of class "CCircle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CIRCLE_H
#define CIRCLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Geometry"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a simple dynamically detailed circle
/// 
////////////////////////////////////////////////////////////////////////////////
class CCircle : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CCircle();
        virtual ~CCircle();
        
        //--- Constant Methods -----------------------------------------------//
        CCircle*            clone() const;
        
        const double&       getAngle() const;
        const Vector2d&     getCenter() const;
        const double&       getRadius() const;
        const ShapeType     getShapeType() const;

        //--- Methods --------------------------------------------------------//
        void transform(const double&, const Vector2d&);
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setRadius(const double&);
        
    protected:

        Vector2d            m_vecCenter;                    ///< Center of circle
        Vector2d            m_vecCenter0;                   ///< Initial center of circle
        double              m_fAngle;                       ///< Angle, just for optical reasons
        double              m_fRadius;                      ///< Radius of circle
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return rotation angle of the circle
///
/// This method returns the rotation angle of the circle. Since it is not
/// perfectly round, one will see the difference between a turning and a still
/// circle.
///
/// \return Rotation angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CCircle::getAngle() const
{
    METHOD_ENTRY("CCircle::getAngle")
    return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the center of the circle
///
/// \return Vector, representing center of circle
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CCircle::getCenter() const
{
    METHOD_ENTRY("CCircle::getCenter")
    return m_vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the radius of the circle
///
/// \return Radius of circle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CCircle::getRadius() const
{
    METHOD_ENTRY("CCircle::getRadius")
    return m_fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType CCircle::getShapeType() const
{
    METHOD_ENTRY("CCircle::getShapeType")
    return SHAPE_CIRCLE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _vecC Center of the circle
///
////////////////////////////////////////////////////////////////////////////////
inline void CCircle::setCenter(const Vector2d& _vecC)
{
    METHOD_ENTRY("CCircle::setCenter")

    m_vecCenter0 = _vecC;
    m_vecCenter = _vecC;
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
    {
        static_cast<CCircle*>(m_pBuf)->m_vecCenter0 = _vecC;
        static_cast<CCircle*>(m_pBuf)->m_vecCenter  = _vecC;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _fX Center-x of the circle
/// \param _fY Center-y of the circle
///
////////////////////////////////////////////////////////////////////////////////
inline void CCircle::setCenter(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CCircle::setCenter")

    m_vecCenter0[0] = _fX;
    m_vecCenter0[1] = _fY;

    m_vecCenter[0] = _fX;
    m_vecCenter[1] = _fY;
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
    {
        static_cast<CCircle*>(m_pBuf)->m_vecCenter0[0] = _fX;
        static_cast<CCircle*>(m_pBuf)->m_vecCenter0[1] = _fY;
        static_cast<CCircle*>(m_pBuf)->m_vecCenter[0]  = _fX;
        static_cast<CCircle*>(m_pBuf)->m_vecCenter[1]  = _fY;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the circle
///
/// \param _fRadius Radius of the circle
///
////////////////////////////////////////////////////////////////////////////////
inline void CCircle::setRadius(const double& _fRadius)
{
    METHOD_ENTRY("CCircle::setRadius")

    m_fRadius = _fRadius;
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
        static_cast<CCircle*>(m_pBuf)->m_fRadius = _fRadius;
}

#endif
