////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2016 Torsten Büschenfeld
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
    
    if (m_pRef != nullptr)
    {
        Rotation2Dd Rotation(m_pRef->m_fAngle);
        Vector2d vecResult = m_pRef->m_vecOrigin + Rotation*m_vecOrigin;
        
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
        Rotation2Dd Rotation(m_pRef->m_fAngle);
        Vector2d vecResult = Rotation * m_vecVelocity +
                                        m_pRef->m_vecVelocity +
                                        m_pRef->m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
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
        return m_pRef->m_fAngle + m_fAngle;
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
        return m_pRef->m_fAngleVelocity + m_fAngleVelocity;
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
        Rotation2Dd Rotation(m_pRef->m_fAngle);
        Vector2d vecResult = Rotation * ((RotationLocal*_vecLocal) + m_vecOrigin)
                            + m_pRef->m_vecOrigin;
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _KS CKinematicsState instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CKinematicsState& _KS)
{
    METHOD_ENTRY("CKinematicsState::operator>>")
    
    /// \todo Stream reference by using UIDs
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IUniqueIDUser
    _is >> _KS.m_UID;
    
    // From IUniqueIDReferrer
    _is >> _KS.m_UIDRef;
    
    _is >> _KS.m_bGotReference;
    _is >> _KS.m_vecOrigin[0];
    _is >> _KS.m_vecOrigin[1];
    _is >> _KS.m_vecVelocity[0];
    _is >> _KS.m_vecVelocity[1];
    _is >> _KS.m_fAngle;
    _is >> _KS.m_fAngleVelocity;

    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _KS CKinematicsState instance to stream
///
/// \return Stream with game state information of IObject instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CKinematicsState& _KS)
{
    METHOD_ENTRY("CKinematicsState::operator<<")
    
    /// \todo Stream reference by using UIDs
    
    _os << "KinematicsState:" << std::endl;
    
    // From IUniqueIDUser
    _os << _KS.m_UID << std::endl;
    
    // From IUniqueIDReferrer
    _os << _KS.m_UIDRef << std::endl;
    
    _os << _KS.m_bGotReference << std::endl;
    _os << _KS.m_vecOrigin[0] << " " <<
           _KS.m_vecOrigin[1] << std::endl;
    _os << _KS.m_vecVelocity[0] << " " <<
           _KS.m_vecVelocity[1] << std::endl;
    _os << _KS.m_fAngle << std::endl;
    _os << _KS.m_fAngleVelocity << std::endl;

    return _os;
}

