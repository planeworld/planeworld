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
              ShapeType     getShapeType() const;

        //--- Methods --------------------------------------------------------//
        void transform(const double&, const Vector2d&, const Vector2d&);
        void setCircleType(const CircleType&);
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setRadius(const double&);
        
    protected:

        //--- Protected methods ----------------------------------------------//
        std::istream&       myStreamIn(std::istream&);
        std::ostream&       myStreamOut(std::ostream&);
        
        void myCopy(const IShape* const);
        void myUpdateGeometry();

        //--- Protected variables --------------------------------------------//
        CircleType          m_CircleType;                   ///< Type of circle
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
inline ShapeType CCircle::getShapeType() const
{
    METHOD_ENTRY("CCircle::getShapeType")
    return ShapeType::SHAPE_CIRCLE;
}

#endif
