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
/// \file       pointmass.h
/// \brief      Prototype of class "CPointMass"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef POINTMASS_H
#define POINTMASS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "object.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a single particle
///
/// This class represents a particle, which is mainly a single mass with all its
/// features.
///
////////////////////////////////////////////////////////////////////////////////
class CPointMass : public IObject
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPointMass();
        virtual ~CPointMass();                          ///< Destructor

        //--- Constant Methods -----------------------------------------------//
        const Vector2d      getAnchor(const int&) const {return Vector2d();};
        const ObjectType    getObjectType() const;

        //--- Methods --------------------------------------------------------//
        void addForce(const Vector2d&, const Vector2d&);
        void calcMovement(const double&);
        void clearForces();

    protected:

        void myTransform();

        //--- Protected Variables --------------------------------------------//

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the objects type - RTTI
///
/// \return Type of object
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectType CPointMass::getObjectType() const
{
    METHOD_ENTRY("CCircle::getObjectType")
    return ObjectType::OBJECT_POINTMASS;
}

#endif
