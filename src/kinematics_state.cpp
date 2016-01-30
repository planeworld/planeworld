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
/// \file       kinematics_state.cpp
/// \brief      Implementation of class "CKinematicsState"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-10
///
////////////////////////////////////////////////////////////////////////////////

//--- Program header ---------------------------------------------------------//
#include "kinematics_state.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create kinematics referred to given ones without changing the
///        internal state.
///
/// \param _Reference Kinematics to refer to
///
/// \return Resulting kinematics when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
const CKinematicsState CKinematicsState::referredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::referredTo")
    
    CKinematicsState Return;
    
    Rotation2Dd Rotation(_Reference.m_fAngle);
    
    Return.m_vecOrigin      = _Reference.m_vecOrigin + Rotation*m_vecOrigin;
    Return.m_vecVelocity    = Rotation * m_vecVelocity +
                              _Reference.m_vecVelocity +
                              _Reference.m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
                                                                       m_vecOrigin[0]);
    Return.m_fAngle         = _Reference.m_fAngle + m_fAngle;
    Return.m_fAngleVelocity = _Reference.m_fAngleVelocity + m_fAngleVelocity;
    
    return Return;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return origin in global coordinates based on reference
///
/// \return Resulting origin in global coordinates based on reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getOrigin() const
{
    METHOD_ENTRY("CKinematicsState::getOrigin")
    
    if (m_pReference != nullptr)
    {
        Rotation2Dd Rotation(m_pReference->m_fAngle);
        Vector2d vecResult = m_pReference->m_vecOrigin + Rotation*m_vecOrigin;
        
        return vecResult;
    }
    else
        return m_vecOrigin;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return velocity in global coordinates based on reference
///
/// \return Resulting velocity in global coordinates based on reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getVelocity")
    
    if (m_bGotReference)
    {
        Rotation2Dd Rotation(m_pReference->m_fAngle);
        Vector2d vecResult = Rotation * m_vecVelocity +
                                        m_pReference->m_vecVelocity +
                                        m_pReference->m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
                                                                                    m_vecOrigin[0]);
        return vecResult;
    }
    else
        return m_vecVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return angle in global coordinates based on reference
///
/// \return Resulting angle in global coordinates based on reference
///
////////////////////////////////////////////////////////////////////////////////
const double CKinematicsState::getAngle() const
{
    METHOD_ENTRY("CKinematicsState::getAngle")
    if (m_bGotReference)
        return m_pReference->m_fAngle + m_fAngle;
    else
        return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return angle velocity in global coordinates based on reference
///
/// \return Resulting angle velocity in global coordinates based on reference
///
////////////////////////////////////////////////////////////////////////////////
const double CKinematicsState::getAngleVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getAngleVelocity")
    if (m_bGotReference)
        return m_pReference->m_fAngleVelocity + m_fAngleVelocity;
    else
        return m_fAngleVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create angle referred to given kinematics without changing the
///        internal state.
///
/// \param _Reference Kinematics to refer to
///
/// \return Resulting angle when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
const double CKinematicsState::getAngleReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getAngleReferredTo")
    return m_fAngle - _Reference.m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create positon referred to given kinematics without changing the
///        internal state.
///
/// \param _Reference Kinematics to refer to
///
/// \return Resulting position when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getOriginReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getOriginReferredTo")
    
    Rotation2Dd Rotation(-_Reference.m_fAngle);
    Vector2d vecResult = Rotation*m_vecOrigin - _Reference.m_vecOrigin;
    
    return vecResult;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create positon referred to given kinematics without changing the
///        internal state.
///
/// \param _vecLocal  Position in local coordinates
/// \param _Reference Kinematics to refer to
///
/// \return Resulting position when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getPositionReferredTo(const Vector2d& _vecLocal,
                                               const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getPositionReferredTo")
    
    Rotation2Dd RotationLocal(m_fAngle);
    Rotation2Dd Rotation(_Reference.m_fAngle);
    Vector2d vecResult = Rotation * ((RotationLocal*_vecLocal) + m_vecOrigin)
                         + _Reference.m_vecOrigin;
    return vecResult;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create velocity referred to given kinematics without changing the
///        internal state.
///
/// \param _Reference Kinematics to refer to
///
/// \return Resulting velocity when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getVelocityReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getVelocityReferredTo")
    
//     Vector2d vecResult = m_vecVelocity + _Reference.m_vecVelocity + _Reference.m_fAngleVelocity *
//                                                     Vector2d(-_Reference.m_vecOrigin[1],
//                                                               _Reference.m_vecOrigin[0]);
                                                    
    Rotation2Dd Rotation(-_Reference.m_fAngle);
    Vector2d vecResult = Rotation * m_vecVelocity -
                                    _Reference.m_vecVelocity +
                                    _Reference.m_fAngleVelocity * Rotation * Vector2d(- m_vecOrigin[1],
                                                                                        m_vecOrigin[0]);
    return vecResult;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get positon referring to local coordinate system
///
/// \param _vecLocal  Position in local coordinates
///
/// \return Resulting coordinates referring to local coordinate system
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getLocalPosition(const Vector2d& _vecLocal) const
{
    METHOD_ENTRY("CKinematicsState::getPosition")
    
    Rotation2Dd RotationLocal(m_fAngle);
    Vector2d vecResult = RotationLocal*_vecLocal + m_vecOrigin;
    return vecResult;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Convert local positon to global position referring to reference
///
/// \param _vecLocal  Position in local coordinates
///
/// \return Resulting global coordinates when referring to reference
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::getPosition(const Vector2d& _vecLocal) const
{
    METHOD_ENTRY("CKinematicsState::getPosition")
    
    Rotation2Dd RotationLocal(m_fAngle);
    if (m_bGotReference)
    {
        Rotation2Dd Rotation(m_pReference->m_fAngle);
        Vector2d vecResult = Rotation * ((RotationLocal*_vecLocal) + m_vecOrigin)
                            + m_pReference->m_vecOrigin;
        return vecResult;
    }
    else
    {
        Vector2d vecResult = RotationLocal*_vecLocal + m_vecOrigin;
        return vecResult;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create kinematics with reference to given kinematics
///
/// \param _Reference Kinematics to refer to
///
////////////////////////////////////////////////////////////////////////////////
void CKinematicsState::referTo(const CKinematicsState& _Reference)
{
    METHOD_ENTRY("CKinematicsState::referTo")
    
    Rotation2Dd Rotation(_Reference.m_fAngle);
    
    m_vecOrigin       = _Reference.m_vecOrigin + Rotation*m_vecOrigin;
    m_vecVelocity     = Rotation * m_vecVelocity +
                        _Reference.m_vecVelocity +
                        _Reference.m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
                                                                 m_vecOrigin[0]);
    m_fAngle         += _Reference.m_fAngle;
    m_fAngleVelocity += _Reference.m_fAngleVelocity;
}
