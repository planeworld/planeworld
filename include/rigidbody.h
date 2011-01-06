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

        //--- Methods --------------------------------------------------------//
        virtual void addForce(const Vector2d&, const Vector2d&);
        virtual void clearForces();

    protected:

        //--- Methods [protected] --------------------------------------------//
        virtual void myDynamics(const double&);

};

#endif
