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
/// \file       body.h
/// \brief      Prototype of class "CBody"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef BODY_H
#define BODY_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "object.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a body
///
/// This class represents a body which mainly refers to an object that is defined
/// by a mass and shape.
///
////////////////////////////////////////////////////////////////////////////////
class CBody : public IObject
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CBody();
        virtual ~CBody();                               ///< Destructor

        //--- Constant Methods -----------------------------------------------//
        IObject*            clone() const = 0;
        
        const double        getHookAngle() const;
        
        const Vector2d      getAnchor(const int&) const;
        const double        getAngle() const;
        const double        getAngleVelocity() const;
        const double        getInertia() const;
        const ObjectType    getObjectType() const;
        
        //--- Methods --------------------------------------------------------//
        void setAngle(const double&);
        void setAngleVelocity(const double&);

    protected:

        //--- Methods [protected] --------------------------------------------//
        void myInit();
        void mySetNewIntegrator(const IntegratorType&);
        
        std::istream& myStreamIn (std::istream&);
        std::ostream& myStreamOut(std::ostream&);

        //--- Variables [protected] ------------------------------------------//
        double                  m_fTorque;              ///< Resulting torque on body
        
        IIntegrator<double>*    m_pIntAng;              ///< Angle integrator
        IIntegrator<double>*    m_pIntAngVel;           ///< Angle velocity integrator

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the angle of the hooked object
///
/// \return Angle of hooked object
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBody::getHookAngle() const
{
    METHOD_ENTRY("CBody::getHookAngle")
    return m_KinematicsState.getLocalAngle();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the angle of object
///
/// \return Angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBody::getAngle() const
{
    METHOD_ENTRY("CBody::getAngle")
    return m_KinematicsState.getLocalAngle();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the angle velocity
///
/// \return Angle velocity
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBody::getAngleVelocity() const
{
    METHOD_ENTRY("CBody::getAngleVelocity")
    return m_KinematicsState.getLocalAngleVelocity();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the inertia
///
/// \return inertia
///
////////////////////////////////////////////////////////////////////////////////
inline const double CBody::getInertia() const
{
    METHOD_ENTRY("CBody::getInertia")
    return m_Geometry.getInertia();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the objects type - RTTI
///
/// \return Type of object
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectType CBody::getObjectType() const
{
    METHOD_ENTRY("CBody::getObjectType")
    return ObjectType::OBJECT_BODY;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the angle of the body
///
/// \param _fAng Angle of body
///
////////////////////////////////////////////////////////////////////////////////
inline void CBody::setAngle(const double& _fAng)
{
    METHOD_ENTRY("CBody::setAngle")
    m_pIntAng->init(_fAng);
    m_KinematicsState.setAngle(_fAng);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the angle velocity of the body
///
/// \param _fV Angle velocity of body
///
////////////////////////////////////////////////////////////////////////////////
inline void CBody::setAngleVelocity(const double& _fV)
{
    METHOD_ENTRY("CBody::setAngleVelocity")
    m_pIntAngVel->init(_fV);
    m_KinematicsState.setAngleVelocity(_fV);
}

#endif
