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
/// \file       rigidbody.cpp
/// \brief      Implementation of class "CRigidbody"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "rigidbody.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CRigidBody::CRigidBody()
{
    METHOD_ENTRY("CRigidBody::CRigidBody")
    CTOR_CALL("CRigidBody::CRigidBody")

    // Default name for any rigidbody:
    m_strName = "Rigidbody";

    METHOD_EXIT("CRigidBody::CRigidBody")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CRigidBody::~CRigidBody()
{
    METHOD_ENTRY("CRigidBody::~CRigidBody")
    DTOR_CALL("CRigidBody::~CRigidBody")

    METHOD_EXIT("CRigidBody::~CRigidBody")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a force to rigidbody
///
/// \param _vecF Force to be applied
/// \param _vecPOC Point of contact
///
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::addForce(const Vector2d& _vecF, const Vector2d& _vecPOC)
{
    METHOD_ENTRY("CRigidBody::addForce")

    m_vecForce  +=  _vecF;
    m_fTorque   +=  (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[0] * _vecF[1] -
                    (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[1] * _vecF[0];
                    
    METHOD_EXIT("CRigidBody::addForce")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clear all forces acting on object
///
////////////////////////////////////////////////////////////////////////////////
void CRigidBody::clearForces()
{
    METHOD_ENTRY("CRigidBody::clearForces")

    m_fTorque = 0.0;
    m_vecForce.setZero();

    METHOD_EXIT("CRigidBody::clearForces")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculates dynamics of the object
///
/// This method calculates the dynamics -- acceleration, velocity, position --
/// of the body by integration of force and torque.
///
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::myDynamics(const double& _fStep)
{
    METHOD_ENTRY("CRigidBody::myDynamics")

    Vector2d vecAccel = m_vecForce / m_fMass;
    m_pIntVel->integrate(vecAccel, _fStep*m_fTimeFac);
    m_pIntPos->integrate(m_pIntVel->getValue(),_fStep*m_fTimeFac);
    
    double fAngleAccel = m_fTorque / m_fInertia;
    double fAngleVel = m_pIntAngVel->integrate(fAngleAccel, _fStep*m_fTimeFac);
    m_fAngle = m_pIntAng->integrateClip(fAngleVel, _fStep*m_fTimeFac, 2.0*std::acos(-1.0));
    /// \todo Replace 2 Pi by constant
    
    METHOD_EXIT("CRigidBody::myDynamics")
}
