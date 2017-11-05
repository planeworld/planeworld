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
/// \file       kinematics_state.h
/// \brief      Prototype of class "CKinematicsState"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-09
///
////////////////////////////////////////////////////////////////////////////////

#ifndef KINEMATICS_STATE_H
#define KINEMATICS_STATE_H

//--- Program header ---------------------------------------------------------//
#include "handle.h"
#include "log.h"
#include "uid_user.h"

//--- Standard header --------------------------------------------------------//
#include <array>

//--- Misc. header -----------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for kinematics state
///
/// This class stores the kinematics state of an entity, i.e. position, velocity
/// angle, and angle velocity. Every entity moving in the world has a kinematics
/// state. The class allows for referring the state of one entity to another.
/// This might be simply a local coordinate system when referring to position
/// and angle but also manages initialisation of objects referring to another
/// kinematics state (e.g. a building on a planet is initialised not only within
/// the planets local coordinate system, but also sets initial velocity and
/// angle velocity with respect to it's reference to not move away from the
/// planet.
/// 
////////////////////////////////////////////////////////////////////////////////
class CKinematicsState  : public IUIDUser
{
    
    public:
    
        //--- Static Methods -------------------------------------------------//
        static const double&  getWorldLimitX();
        static const double&  getWorldLimitY();
        static const Vector2d clipToWorldLimit(const Vector2d&);
        static void           setWorldLimit(const double&, const double&);
            
        //--- Constructor/Destructor -----------------------------------------//
        CKinematicsState();

        //--- Constant methods -----------------------------------------------//
        Vector2d  getOrigin() const;
        Vector2d  getVelocity() const;
        double    getAngle() const;
        double    getAngleVelocity() const;
        
        const Vector2d&     getLocalOrigin() const;
        const Vector2d&     getLocalVelocity() const;
        const double&       getLocalAngle() const;
        const double&       getLocalAngleVelocity() const;
        
        double              getAngleReferredTo(const CKinematicsState&) const;
        double              getAngleVelocityReferredTo(const CKinematicsState&) const;
        Vector2d            getOriginReferredTo(const CKinematicsState&) const;
        Vector2d            getPositionReferredTo(const Vector2d&, const CKinematicsState&) const;
        Vector2d            getVelocityReferredTo(const CKinematicsState&) const;
        
        CKinematicsState    referredTo(const CKinematicsState&) const;
        
        Vector2d            getLocalPosition(const Vector2d&) const;
        Vector2d            getPosition(const Vector2d&) const;
        
        template<int T>
        void                getPositions(const std::array<Vector2d, T>&,
                                               std::array<Vector2d, T>&,
                                         const double& = 1.0) const;
        
        //--- Methods --------------------------------------------------------//
        void setRef(CKinematicsState* const);
                                         
        Vector2d& Origin();
        
        void increaseAngle(const double&);
        
        void setOrigin(const Vector2d&);
        void setVelocity(const Vector2d&);
        void setAngle(const double&);
        void setAngleVelocity(const double&);
        
        void referTo(const CKinematicsState&);
        void transform(const Vector2d&, const Vector2d&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CKinematicsState&);
        friend std::ostream&    operator<<(std::ostream&, CKinematicsState&);

    private:
        
        //--- Static variables -----------------------------------------------//
        static double s_fWorldLimitX;   ///< Maximum world coordinate abs(x) before repetition
        static double s_fWorldLimitY;   ///< Maximum world coordinate abs(x) before repetition

        //--- Variables ------------------------------------------------------//
        CHandle<CKinematicsState>   m_hKinStateRef; ///< kinematics state reference
        
        Matrix2d    m_matRot;           ///< Rotation
        Vector2d    m_vecOrigin;        ///< Origin of local coordinates
        Vector2d    m_vecVelocity;      ///< Velocity
        double      m_fAngle;           ///< Orientation angle
        double      m_fAngleVelocity;   ///< Angle velocity

        mutable Matrix2d    m_matRotRef;     ///< Rotation of reference
        mutable double      m_fAngleRef;     ///< Angle of reference if attached
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline CKinematicsState::CKinematicsState() : IUIDUser(),
                                              m_fAngle(0.0),
                                              m_fAngleVelocity(0.0),
                                              m_fAngleRef(0.0)
{
    METHOD_ENTRY("CKinematicsState::CKinematicsState")
    CTOR_CALL("CKinematicsState::CKinematicsState")
    m_matRot.setIdentity();
    m_vecOrigin.setZero();
    m_vecVelocity.setZero();
    m_matRotRef.setIdentity();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the origin in local coordinates
///
/// \return Origin in local coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CKinematicsState::getLocalOrigin() const
{
    METHOD_ENTRY("CKinematicsState::getPosition")
    return m_vecOrigin;
}

//////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the velocity in local coordinates
///
/// \return Velocity in local coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CKinematicsState::getLocalVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getVelocity")
    return m_vecVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the angle in local coordinates
///
/// \return Angle in local coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CKinematicsState::getLocalAngle() const
{
    METHOD_ENTRY("CKinematicsState::getAngle")
    return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the angle velocity in local coordinates
///
/// \return Angle velocity in local coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CKinematicsState::getLocalAngleVelocity() const
{
    METHOD_ENTRY("CKinematicsState::getAngleVelocity")
    return m_fAngleVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns reference to origin in local coordinates
///
/// \return Reference origin in local coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline Vector2d& CKinematicsState::Origin()
{
    METHOD_ENTRY("CKinematicsState::Origin")
    return m_vecOrigin;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets reference for this kinematic state
///
/// \param _pRef Kinematics to refer to
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setRef(CKinematicsState* const _pRef)
{
    METHOD_ENTRY("CKinematicsState::setRef")
    m_hKinStateRef.set(_pRef);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Increases the angle by given value
///
/// \param _fAngle Value to increase the angle by
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::increaseAngle(const double& _fAngle)
{
    METHOD_ENTRY("CKinematicsState::increaseAngle")
    m_fAngle += _fAngle;
    m_matRot = Rotation2Dd(m_fAngle).toRotationMatrix();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the local coordinate systems origin within cell
///
/// \param _vecOrigin Local coordinate systems origin within cell
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CKinematicsState::setOrigin")
    m_vecOrigin = CKinematicsState::clipToWorldLimit(_vecOrigin);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the velocity
///
/// \param _vecVelocity Velocity
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setVelocity(const Vector2d& _vecVelocity)
{
    METHOD_ENTRY("CKinematicsState::setVelocity")
    m_vecVelocity = _vecVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the angle
///
/// \param _fAngle Angle
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setAngle(const double& _fAngle)
{
    METHOD_ENTRY("CKinematicsState::setAngle")
    m_fAngle = _fAngle;
    m_matRot = Rotation2Dd(_fAngle).toRotationMatrix();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the angle velocity
///
/// \param _fAngleVelocity Angle velocity
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setAngleVelocity(const double& _fAngleVelocity)
{
    METHOD_ENTRY("CKinematicsState::setAngleVelocity")
    m_fAngleVelocity = _fAngleVelocity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Convert local positons to global position referring to reference
///
/// \param _Pos0 Array of positions in local coordinates
/// \param _Pos1 Resulting global coordinates when referring to reference
/// \param _fZoom Zoom factor
///
////////////////////////////////////////////////////////////////////////////////
template<int T>
void CKinematicsState::getPositions(const std::array<Vector2d, T>& _Pos0,
                                          std::array<Vector2d, T>& _Pos1,
                                    const double& _fZoom) const
{
    METHOD_ENTRY("CKinematicsState::getPositions")
    
    if (m_hKinStateRef.isValid())
    {
        if (m_fAngleRef != m_hKinStateRef.get()->m_fAngle)
        {
            m_fAngleRef = m_hKinStateRef.get()->m_fAngle;
            m_matRotRef = Rotation2Dd(m_hKinStateRef.get()->m_fAngle).toRotationMatrix();
        }
        for (auto i=0u; i<T; ++i)
        {
            _Pos1[i] = m_matRotRef * ((m_matRot * (_Pos0[i] / _fZoom)) + m_vecOrigin) +
                       m_hKinStateRef.get()->m_vecOrigin;
        }
    }
    else
    {
        for (auto i=0u; i<T; ++i)
        {
            _Pos1[i] = m_matRot * (_Pos0[i] / _fZoom) + m_vecOrigin;
        }
    }
}

#endif // KINEMATICS_STATE_H
