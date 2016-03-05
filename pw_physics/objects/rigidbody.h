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
/// \file       rigidbody.h
/// \brief      Prototype of class "CRigidBody"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

// Standard-header

// Program-header
#include "body.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing rigid body
/// 
////////////////////////////////////////////////////////////////////////////////
class CRigidBody : public CBody
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CRigidBody();
        virtual ~CRigidBody();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IObject* clone() const;
        
        //--- Methods --------------------------------------------------------//
        virtual void addForce(const Vector2d&, const Vector2d&);
        virtual void addForceLC(const Vector2d&, const Vector2d&);
        virtual void clearForces();
        
        //--- Static Methods -------------------------------------------------//
        static uint32_t getCount();

    protected:

        //--- Methods [protected] --------------------------------------------//
        virtual void myDynamics(const double&);
        
        static uint32_t m_unNrOfRigidBodies;    ///< Static counter for name initialisation and tracking

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the absolute count of object instances
///
/// \return Count of object instances
///
////////////////////////////////////////////////////////////////////////////////
inline uint32_t CRigidBody::getCount()
{
    METHOD_ENTRY("CBody::getCount")
    return m_unNrOfRigidBodies;
}

#endif
