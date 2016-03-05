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
/// \file       rigidbody.cpp
/// \brief      Implementation of class "CRigidbody"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "rigidbody.h"

uint32_t CRigidBody::m_unNrOfRigidBodies = 0;

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
    m_strName = "Rigidbody_" + std::to_string(m_unNrOfRigidBodies++);
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
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones a rigid body
///
/// \return Pointer to cloned rigid body
///
////////////////////////////////////////////////////////////////////////////////
IObject* CRigidBody::clone() const
{
    METHOD_ENTRY("CRigidBody::clone")
    
    CRigidBody* pClone = new CRigidBody;
    MEM_ALLOC("CRigidBody")

    //--- Variables of IObject -----------------------------------------------//

    pClone->m_KinematicsState = m_KinematicsState;
    pClone->m_bGravitation = m_bGravitation;
    pClone->m_bDynamics    = m_bDynamics;
    // m_Lifetime: New individual object
    pClone->m_fTimeFac     = m_fTimeFac;
    pClone->m_Geometry     = m_Geometry;
    pClone->m_vecCOM       = m_vecCOM;
    // m_vecForce: No Forces on newly created Rigidbody;
    pClone->m_fMass        = m_fMass;       
    pClone->m_nDepthlayers = m_nDepthlayers;
    
    if (pClone->m_pIntPos != nullptr)
    {
        delete pClone->m_pIntPos;
        MEM_FREED("IIntegrator")
        pClone->m_pIntPos = nullptr;
    }
    pClone->m_pIntPos      = m_pIntPos->clone();
    
    if (pClone->m_pIntVel != nullptr)
    {
        delete pClone->m_pIntVel;
        MEM_FREED("IIntegrator")
        pClone->m_pIntVel = nullptr;
    }
    pClone->m_pIntVel      = m_pIntVel->clone();     
    
    // m_strName: Don't clone the name, this is an individual object
    pClone->m_Anchors      = m_Anchors;
    
    //--- Variables of CBody -------------------------------------------------//
    pClone->m_fInertia = m_fInertia;
    pClone->m_fTorque = m_fTorque;
    
    if (pClone->m_pIntAng != nullptr)
    {
        delete pClone->m_pIntAng;
        MEM_FREED("IIntegrator")
        pClone->m_pIntAng = nullptr;
    }
    pClone->m_pIntAng      = m_pIntAng->clone();
    
    if (pClone->m_pIntAngVel != nullptr)
    {
        delete pClone->m_pIntAngVel;
        MEM_FREED("IIntegrator")
        pClone->m_pIntAngVel = nullptr;
    }
    pClone->m_pIntAngVel      = m_pIntAngVel->clone();
    
    //--- Variables of IUniverseScaled ---------------------------------------//
    pClone->m_vecCell = m_vecCell;
    
    return pClone;
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

    /// \todo Check if COM is handeled correctly when object is rotated
    m_vecForce  +=  _vecF;
    m_fTorque   +=  (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[0] * _vecF[1] -
                    (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[1] * _vecF[0];
//     m_fTorque   +=  _vecPOC[0] * _vecF[1] - _vecPOC[1] * _vecF[0];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a force to rigidbody (local coordinates given)
///
/// \param _vecF Force to be applied
/// \param _vecPOC Point of contact
///
///////////////////////////////////////////////////////////////////////////////
void CRigidBody::addForceLC(const Vector2d& _vecF, const Vector2d& _vecPOC)
{
    METHOD_ENTRY("CRigidBody::addForceLC")

    /// \todo Check if COM is handeled correctly when object is rotated
    
//     m_fTorque   +=  (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[0] * _vecF[1] -
//                     (_vecPOC - (m_pIntPos->getValue()+m_vecCOM))[1] * _vecF[0];
    
    Rotation2Dd Rotation(m_KinematicsState.getLocalAngle());
    
    m_vecForce  +=  Rotation * _vecF;
    
    Vector2d vecTmp = Rotation * _vecPOC;
    Vector2d vecTmp2 = Rotation * _vecF;
    m_fTorque   +=  vecTmp[0] * vecTmp2[1] - vecTmp[1] * vecTmp2[0];
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
    m_KinematicsState.setAngleVelocity(fAngleVel);
    m_KinematicsState.setAngle(m_pIntAng->integrateClip(fAngleVel, _fStep*m_fTimeFac, 2.0*M_PI));
}
