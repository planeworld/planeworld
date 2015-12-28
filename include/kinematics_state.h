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
#include "log.h"

//--- Standard header --------------------------------------------------------//

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
class CKinematicsState
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CKinematicsState();
        CKinematicsState(const CKinematicsState&);

        //--- Constant methods -----------------------------------------------//
        const Vector2d  getOrigin() const;
        const Vector2d  getVelocity() const;
        const double    getAngle() const;
        const double    getAngleVelocity() const;
        
        const Vector2d& getLocalOrigin() const;
        const Vector2d& getLocalVelocity() const;
        const double&   getLocalAngle() const;
        const double&   getLocalAngleVelocity() const;
        
        const double           getAngleReferredTo(const CKinematicsState&) const;
        const Vector2d         getOriginReferredTo(const CKinematicsState&) const;
        const Vector2d         getPositionReferredTo(const Vector2d&, const CKinematicsState&) const;
        const Vector2d         getVelocityReferredTo(const CKinematicsState&) const;
        
        const CKinematicsState referredTo(const CKinematicsState&) const;
        
        const Vector2d         getLocalPosition(const Vector2d&) const;
        const Vector2d         getPosition(const Vector2d&) const;
        
        const CKinematicsState* const getReference() const;
        const bool&            gotReference() const;
        
        //--- Methods --------------------------------------------------------//
        Vector2d& Origin();
        
        void increaseAngle(const double&);
        
        void setCell(const Vector2i&);
        void setOrigin(const Vector2d&);
        void setVelocity(const Vector2d&);
        void setAngle(const double&);
        void setAngleVelocity(const double&);
        
        void setReference(CKinematicsState* const);
        
        void referTo(const CKinematicsState&);

    private:

        //--- Variables ------------------------------------------------------//
        CKinematicsState* m_pReference; ///< Reference for this kinematic state
        
        bool        m_bGotReference;    ///< Flags if reference is given
      
        Vector2d    m_vecOrigin;        ///< Origin of local coordinates
        Vector2d    m_vecVelocity;      ///< Velocity
        double      m_fAngle;           ///< Orientation angle
        double      m_fAngleVelocity;   ///< Angle velocity
};

// typedef std::unordered_map<std::string, CKinematicsState*>  KinematicsStatesType;    ///< Specifies a list of kinematics states

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline CKinematicsState::CKinematicsState() : m_pReference(nullptr),
                                              m_bGotReference(false),
                                              m_fAngle(0.0),
                                              m_fAngleVelocity(0.0)
{
    METHOD_ENTRY("CKinematicsState::CKinematicsState")
    CTOR_CALL("CKinematicsState::CKinematicsState")
    m_vecOrigin.setZero();
    m_vecVelocity.setZero();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor, creating kinematics with reference to given
///        kinematics.
///
/// \param _Reference Kinematics to refer to
///
////////////////////////////////////////////////////////////////////////////////
inline CKinematicsState::CKinematicsState(const CKinematicsState& _Reference)
{
    METHOD_ENTRY("CKinematicsState::CKinematicsState")
    CTOR_CALL("CKinematicsState::CKinematicsState")
    
    this->referTo(_Reference);
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
/// \brief Returns reference of this kinematics state
///
/// \return Reference of this kinematics state
///
////////////////////////////////////////////////////////////////////////////////
inline const CKinematicsState* const CKinematicsState::getReference() const
{
    METHOD_ENTRY("CKinematicsState::getReference")
    return m_pReference;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns flag to indicate if reference is given
///
/// \return Reference given (true/false)
///
////////////////////////////////////////////////////////////////////////////////
inline const bool& CKinematicsState::gotReference() const
{
    METHOD_ENTRY("CKinematicsState::gotReference")
    return m_bGotReference;
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
/// \brief Increases the angle by given value
///
/// \param _fInc Value to increase the angel by
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::increaseAngle(const double& _fAngle)
{
    METHOD_ENTRY("CKinematicsState::increaseAngle")
    m_fAngle += _fAngle;
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
    m_vecOrigin = _vecOrigin;
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
/// \brief Sets the reference for this kinematics state
///
/// \param _pReference Kinematics state to refer to
///
////////////////////////////////////////////////////////////////////////////////
inline void CKinematicsState::setReference(CKinematicsState* const _pReference)
{
    METHOD_ENTRY("CKinematicsState::setReference")
    m_pReference = _pReference;
    m_bGotReference = true;
}

#endif // KINEMATICS_STATE_H