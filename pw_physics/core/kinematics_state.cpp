////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2018 Torsten Büschenfeld
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

double CKinematicsState::s_fWorldLimitX = 3.0e20;
double CKinematicsState::s_fWorldLimitY = 3.0e20;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return x coordinate limit for the simulated world
///
/// The world is two dimensional with finite extension. It will be infinitely
/// repeated behind world borders. The limit sets the postion of these borders.
///
/// \return World limit x coordinate (abs)
///
////////////////////////////////////////////////////////////////////////////////
const double& CKinematicsState::getWorldLimitX()
{
    METHOD_ENTRY("CKinematicsState::getWorldLimitX")
    return s_fWorldLimitX;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return y coordinate limit for the simulated world
///
/// The world is two dimensional with finite extension. It will be infinitely
/// repeated behind world borders. The limit sets the postion of these borders.
///
/// \return World limit y coordinate (abs)
///
////////////////////////////////////////////////////////////////////////////////
const double& CKinematicsState::getWorldLimitY()
{
    METHOD_ENTRY("CKinematicsState::getWorldLimitY")
    return s_fWorldLimitY;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clip given coordinates to limits for the simulated world
///
/// The world is two dimensional with finite extension. It will be infinitely
/// repeated behind world borders. The limit sets the postion of these borders.
///
/// \param _vecPos Position to be clipped to world limits
/// 
/// \return Clipped coordinates
///
////////////////////////////////////////////////////////////////////////////////
const Vector2d CKinematicsState::clipToWorldLimit(const Vector2d& _vecPos)
{
    METHOD_ENTRY("CKinematicsState::clipToWorldLimit")
    
    Vector2d vecReturn(_vecPos);
    
    if      (_vecPos[0] >  s_fWorldLimitX) vecReturn[0] -= 2.0*s_fWorldLimitX;
    else if (_vecPos[0] < -s_fWorldLimitX) vecReturn[0] += 2.0*s_fWorldLimitX;
    if      (_vecPos[1] >  s_fWorldLimitY) vecReturn[1] -= 2.0*s_fWorldLimitY;
    else if (_vecPos[1] < -s_fWorldLimitY) vecReturn[1] += 2.0*s_fWorldLimitY;
    
    return vecReturn;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set coordinate limits for the simulated world
///
/// The world is two dimensional with finite extension. It will be infinitely
/// repeated behind world borders. The limit sets the postion of these borders.
///
/// \param _fLimitX World limit x coordinate (abs)
/// \param _fLimitY World limit x coordinate (abs)
///
////////////////////////////////////////////////////////////////////////////////
void CKinematicsState::setWorldLimit(const double& _fLimitX, const double& _fLimitY)
{
    METHOD_ENTRY("CKinematicsState::setWorldLimit")
    s_fWorldLimitX = _fLimitX;
    s_fWorldLimitY = _fLimitY;
}


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
CKinematicsState CKinematicsState::referredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::referredTo")
    
    CKinematicsState Return;
    
    Matrix2d matRotRef = Rotation2Dd(_Reference.m_fAngle).toRotationMatrix();
    
    
    Return.m_vecOrigin      = _Reference.m_vecOrigin + matRotRef*m_vecOrigin;
    Return.m_vecVelocity    =  matRotRef * m_vecVelocity +
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
Vector2d CKinematicsState::getOrigin() const
{
    METHOD_ENTRY("CKinematicsState::getOrigin")
    
    if (m_hKinStateRef.isValid())
    {
        if (m_fAngleRef != m_hKinStateRef->m_fAngle)
        {
            m_fAngleRef = m_hKinStateRef->m_fAngle;
            m_matRotRef = Rotation2Dd(m_hKinStateRef->m_fAngle).toRotationMatrix();
        }
        return m_hKinStateRef->m_vecOrigin + m_matRotRef*m_vecOrigin;
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
Vector2d CKinematicsState::getVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getVelocity")
    
    if (m_hKinStateRef.isValid())
    {
        if (m_fAngleRef != m_hKinStateRef->m_fAngle)
        {
            m_fAngleRef = m_hKinStateRef->m_fAngle;
            m_matRotRef = Rotation2Dd(m_hKinStateRef->m_fAngle).toRotationMatrix();
        }
        return m_matRotRef * m_vecVelocity +
                            m_hKinStateRef->m_vecVelocity;
                            m_hKinStateRef->m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
                                                                    m_vecOrigin[0]);
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
double CKinematicsState::getAngle() const
{
    METHOD_ENTRY("CKinematicsState::getAngle")
    if (m_hKinStateRef.isValid())
        return m_hKinStateRef->m_fAngle + m_fAngle;
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
double CKinematicsState::getAngleVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getAngleVelocity")
    if (m_hKinStateRef.isValid())
        return m_hKinStateRef->m_fAngleVelocity + m_fAngleVelocity;
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
double CKinematicsState::getAngleReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getAngleReferredTo")
    return m_fAngle - _Reference.m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Create angle velocity referred to given kinematics without changing the
///        internal state.
///
/// \param _Reference Kinematics to refer to
///
/// \return Resulting angle velocity when referring to _Reference
///
////////////////////////////////////////////////////////////////////////////////
double CKinematicsState::getAngleVelocityReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getAngleVelocityReferredTo")
    return m_fAngleVelocity - _Reference.m_fAngleVelocity;
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
Vector2d CKinematicsState::getOriginReferredTo(const CKinematicsState& _Reference) const
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
Vector2d CKinematicsState::getPositionReferredTo(const Vector2d& _vecLocal,
                                               const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getPositionReferredTo")
    
    Rotation2Dd Rotation(_Reference.m_fAngle);
    Vector2d vecResult = Rotation * ((m_matRot*_vecLocal) + m_vecOrigin)
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
Vector2d CKinematicsState::getVelocityReferredTo(const CKinematicsState& _Reference) const
{
    METHOD_ENTRY("CKinematicsState::getVelocityReferredTo")
    
//     Vector2d vecResult = m_vecVelocity + _Reference.m_vecVelocity + _Reference.m_fAngleVelocity *
//                                                     Vector2d(-_Reference.m_vecOrigin[1],
//                                                               _Reference.m_vecOrigin[0]);
                                                    
    Matrix2d matRotRef = Rotation2Dd(-_Reference.m_fAngle).toRotationMatrix();
    Vector2d vecResult = matRotRef * m_vecVelocity - _Reference.m_vecVelocity;
    Vector2d vecTmp =    matRotRef * Vector2d(- m_vecOrigin[1],
                                                m_vecOrigin[0]);
             vecResult += _Reference.m_fAngleVelocity * vecTmp;
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
Vector2d CKinematicsState::getLocalPosition(const Vector2d& _vecLocal) const
{
    METHOD_ENTRY("CKinematicsState::getPosition")
    
    Vector2d vecResult = m_matRot*_vecLocal + m_vecOrigin;
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
Vector2d CKinematicsState::getPosition(const Vector2d& _vecLocal) const
{
    METHOD_ENTRY("CKinematicsState::getPosition")
    
    if (m_hKinStateRef.isValid())
    {
        if (m_fAngleRef != m_hKinStateRef->m_fAngle)
        {
            m_fAngleRef = m_hKinStateRef->m_fAngle;
            m_matRotRef = Rotation2Dd(m_hKinStateRef->m_fAngle).toRotationMatrix();
        }
        return m_matRotRef * ((m_matRot*_vecLocal) + m_vecOrigin) +
                             m_hKinStateRef->m_vecOrigin;
    }
    else
    {
        return m_matRot*_vecLocal + m_vecOrigin;
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
    
    Matrix2d matRotRef = Rotation2Dd(_Reference.m_fAngle).toRotationMatrix();
    
    m_vecOrigin       = _Reference.m_vecOrigin + matRotRef*m_vecOrigin;
    m_vecVelocity     =  matRotRef * m_vecVelocity +
                        _Reference.m_vecVelocity +
                        _Reference.m_fAngleVelocity * Vector2d(- m_vecOrigin[1],
                                                                 m_vecOrigin[0]);
    m_fAngle         += _Reference.m_fAngle;
    m_fAngleVelocity += _Reference.m_fAngleVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Transform local coordinate system, including translation and rotation
///
/// \param _vecAxisGlobal   Axis in global coordinates to rotate around
/// \param _vecAxisLocal    Axis in local coordinates to rotate around
///
////////////////////////////////////////////////////////////////////////////////
void CKinematicsState::transform(const Vector2d& _vecAxisGlobal,
                                 const Vector2d& _vecAxisLocal)
{
    METHOD_ENTRY("CKinematicsState::transform")
    
    m_vecOrigin = _vecAxisGlobal - m_matRot * _vecAxisLocal;
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
    
//     // From IUIDUser
//     _is >> _KS.m_UID;
    
    // From IUIDReferrer
//     _is >> _KS.m_hKinStateRef.setUID();
    
    _is >> _KS.s_fWorldLimitX;
    _is >> _KS.s_fWorldLimitY;
    
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
/// \return Stream with game state information of CObject instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CKinematicsState& _KS)
{
    METHOD_ENTRY("CKinematicsState::operator<<")
    
    /// \todo Stream reference by using UIDs
    
    _os << "KinematicsState:" << std::endl;
    
//     // From IUIDUser
//     _os << _KS.m_UID << std::endl;
    
    // From IUIDReferrer
    _os << _KS.m_hKinStateRef.getUID() << std::endl;
    
    
    _os << _KS.s_fWorldLimitX << std::endl;
    _os << _KS.s_fWorldLimitY << std::endl;
    _os << _KS.m_vecOrigin[0] << " " <<
           _KS.m_vecOrigin[1] << std::endl;
    _os << _KS.m_vecVelocity[0] << " " <<
           _KS.m_vecVelocity[1] << std::endl;
    _os << _KS.m_fAngle << std::endl;
    _os << _KS.m_fAngleVelocity << std::endl;

    return _os;
}

