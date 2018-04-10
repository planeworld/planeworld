////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2018 Torsten Büschenfeld
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
/// \file       object.h
/// \brief      Prototype of interface "CObject"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_H
#define OBJECT_H

//--- Program header ---------------------------------------------------------//
#include "adams_bashforth_integrator.h"
#include "adams_moulton_integrator.h"
#include "euler_integrator.h"
#include "geometry.h"
#include "kinematics_state_user.h"
#include "trajectory.h"

//--- Standard header --------------------------------------------------------//

/// Type definition for the AnchorIDs
typedef std::uint16_t AnchorIDType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a phyiscal object
///
/// \todo Move depth information to shapes only. Then map information to object
/// \todo Implement copy constructor and operator=
/// 
////////////////////////////////////////////////////////////////////////////////
class CObject : public IUIDUser,
                public IKinematicsStateUser,
                public IGridUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CObject();
        CObject(const CObject&);
        virtual ~CObject();
        
        CObject& operator=(const CObject&);

        //--- Constant methods -----------------------------------------------//
              CObject*      clone() const;
              Vector2d      getAnchor(const int&) const;
        const double&       getAngle() const;
        const double&       getAngleVelocity() const;
              Vector2d      getCOM() const;
              int           getDepths() const;
              bool          getDynamicsState() const;
              bool          getGravitationState() const;
        const double&       getInertia() const;
        const double&       getMass() const;
        const std::string&  getName() const;
        const Vector2d&     getForce() const {return m_vecForce;}
        const Vector2d&     getOrigin() const;
        const Vector2d&     getVelocity() const;
        const CTrajectory&  getTrajectory() const;

        //--- Methods --------------------------------------------------------//
        void                addForce(const Vector2d&,  const Vector2d&);
        void                addForceLC(const Vector2d&,  const Vector2d&);
        void                clearForces();
        
        void                addAcceleration(const Vector2d&);
        AnchorIDType        addAnchor(const Vector2d&);
        void                disableGravitation();
        void                enableGravitation();
        CGeometry*          getGeometry();
        void                setAngle(const double&);
        void                setAngleVelocity(const double&);
        void                setCell(const Vector2i&);
        void                setCell(const int&, const int&);
        void                setOrigin(const Vector2d&);
        void                setOrigin(const double&, const double&);
        void                setDepths(const int&);
        void                setName(const std::string&);
        void                setTimeFac(const double&);
        void                setVelocity(const Vector2d&);
        void                unsetDepths(const int&);
        void                updateCell();

        void                enableDynamics();
        void                disableDynamics();
        
        void                dynamics(const double&);
        void                init();
        void                setNewIntegrator(const IntegratorType&);
        void                transform();
        
        //--- Static Methods -------------------------------------------------//
        static uint32_t     getCount();
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CObject* const);
        friend std::ostream&    operator<<(std::ostream&, CObject* const);
        
    protected:
        
        //--- Methods [protected] --------------------------------------------//
        void copy(const CObject&);

        //-- Variables [protected] -------------------------------------------//
        bool                    m_bGravitation;                     ///< Does this object influence others by gravitation?
        bool                    m_bDynamics;                        ///< Dynamics calculations for object

        CTimer                  m_Lifetime;                         ///< Lifetime counter
        double                  m_fTimeFac;                         ///< Factor of realtime

        CGeometry               m_Geometry;                         ///< Geometry of object

        Vector2d                m_vecForce;                         ///< Resulting force applied
        double                  m_fTorque;                          ///< Resulting torque on object
        
        int                     m_nDepthlayers;                     ///< Depths in which shape exists
        
        IIntegrator<double>*    m_pIntAng;                          ///< Angle integrator
        IIntegrator<double>*    m_pIntAngVel;                       ///< Angle velocity integrator
        IIntegrator<Vector2d>*  m_pIntPos;                          ///< Position integrator
        IIntegrator<Vector2d>*  m_pIntVel;                          ///< Velocity integrator

        std::vector<Vector2d>   m_Anchors;                          ///< Anchors to joints
        
        CTrajectory             m_Trajectory;                       ///< Trajectory of object
        
        static uint32_t         m_unNrOfObjects;                    ///< Static counter for name initialisation and tracking
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a specific acceleration to the object, for example gravitation
///
/// \param _vecA Acceleration vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::addAcceleration(const Vector2d& _vecA)
{
    METHOD_ENTRY("CObject::addAcceleration")

    this->addForce(_vecA*m_Geometry.getMass(), m_pIntPos->getValue()+m_Geometry.getCOM());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the angle of object
///
/// \return Angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CObject::getAngle() const
{
    METHOD_ENTRY("CObject::getAngle")
    return m_KinematicsState.getLocalAngle();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the angle velocity
///
/// \return Angle velocity
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CObject::getAngleVelocity() const
{
    METHOD_ENTRY("CObject::getAngleVelocity")
    return m_KinematicsState.getLocalAngleVelocity();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the center of mass (COM)
///
/// \return Center of mass (COM)
///
////////////////////////////////////////////////////////////////////////////////
inline Vector2d CObject::getCOM() const
{
    METHOD_ENTRY("CObject::getCOM")
    return m_KinematicsState.getLocalPosition(m_Geometry.getCOM());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns depthlayers
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline int CObject::getDepths() const
{
    METHOD_ENTRY("CObject::getDepths")
    return (m_nDepthlayers);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the state for dynamics
///
/// \return Dynamics state, true=dynamic, false=static
///
////////////////////////////////////////////////////////////////////////////////
inline bool CObject::getDynamicsState() const
{
    METHOD_ENTRY("CObject::getDynamicsState")
    return (m_bDynamics);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the gravitational state
///
/// In reality, every object has a gravitional impact on all other object.
/// Computational amount of gravitational influence between particals would be
/// too high on the one hand, on the other hand, the effect wouldn't be
/// recognisable at all. The method returns this state.
///
/// \return The gravitational state
///
////////////////////////////////////////////////////////////////////////////////
inline bool CObject::getGravitationState() const
{
    METHOD_ENTRY("CObject::getGravitationState")
    return (m_bGravitation);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the inertia
///
/// \return Inertia
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CObject::getInertia() const
{
    METHOD_ENTRY("CObject::getInertia")
    return m_Geometry.getInertia();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the mass of the object
///
/// \return Mass
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CObject::getMass() const
{
    METHOD_ENTRY("CObject::getMass")
    return (m_Geometry.getMass());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name of the object
///
/// \return Name of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CObject::getName() const
{
    METHOD_ENTRY("CObject::getName")
    return (m_UID.getName());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the origin of the object
///
/// \return Origin of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CObject::getOrigin() const
{
    METHOD_ENTRY("CObject::getOrigin")
    return m_KinematicsState.getLocalOrigin();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the velocity of the object
///
/// \return Velocity of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CObject::getVelocity() const
{
    METHOD_ENTRY("CObject::getVelocity")
//     return (m_pIntVel->getValue());
    return m_KinematicsState.getLocalVelocity();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the trajectory of the object
///
/// \return Trajectory of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const CTrajectory& CObject::getTrajectory() const
{
    METHOD_ENTRY("CObject::getTrajectory")
    return m_Trajectory;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a connector for a joint
///
/// \param _vecV position for anchor
///
/// \return ID of anchor
///
////////////////////////////////////////////////////////////////////////////////
inline AnchorIDType CObject::addAnchor(const Vector2d& _vecV)
{
    METHOD_ENTRY("CObject::addAnchor")

    m_Anchors.push_back(_vecV);
    
    return m_Anchors.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Disables gravitational influences of the object
///
/// In reality, every object has a gravitional impact on all other object.
/// Computational amount of gravitational influence between particals would be
/// too high on the one hand, on the other hand, the effect wouldn't be
/// recognisable at all. Therefore, a variable can disable this influence for
/// certain objects, e.g. particals.
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::disableGravitation()
{
    METHOD_ENTRY("CObject::disableGravitation")

    m_bGravitation = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Enables gravitational influences of the object
///
/// In reality, every object has a gravitional impact on all other object.
/// Computational amount of gravitational influence between particals would be
/// too high on the one hand, on the other hand, the effect wouldn't be
/// recognisable at all. Therefore, a variable can enable this influence for
/// certain objects, e.g. particals.
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::enableGravitation()
{
    METHOD_ENTRY("CObject::enableGravitation")

    m_bGravitation = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the geometry of the object
///
/// \return Geometry
///
////////////////////////////////////////////////////////////////////////////////
inline CGeometry* CObject::getGeometry()
{
    METHOD_ENTRY("CObject::getGeometry")
    return &m_Geometry;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the angle of the object
///
/// \param _fAng Angle of object
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setAngle(const double& _fAng)
{
    METHOD_ENTRY("CObject::setAngle")
    m_pIntAng->init(_fAng);
    m_KinematicsState.setAngle(_fAng);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the angle velocity of the object
///
/// \param _fV Angle velocity of object
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setAngleVelocity(const double& _fV)
{
    METHOD_ENTRY("CObject::setAngleVelocity")
    m_pIntAngVel->init(_fV);
    m_KinematicsState.setAngleVelocity(_fV);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the origin of mass
///
/// \param _vecOrigin Origin of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CObject::setOrigin")

    m_KinematicsState.setOrigin(_vecOrigin);
    m_pIntPos->init(m_KinematicsState.getOrigin());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the origin of mass
///
/// \param _fX X-position of origin of mass 
/// \param _fY Y-position of origin of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setOrigin(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CObject::setOrigin")

    m_KinematicsState.setOrigin(Vector2d(_fX, _fY));
    m_pIntPos->init(m_KinematicsState.getOrigin());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set depthlayers for this object
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setDepths(const int& _nD)
{
    METHOD_ENTRY("CObject::setDepths")

    m_nDepthlayers |= _nD;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Unset depthlayers for this object
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::unsetDepths(const int& _nD)
{
    METHOD_ENTRY("CObject::unsetDepths")

    m_nDepthlayers &= (!_nD);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the name for corresponding object
///
/// \param _strName Name of mass
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setName(const std::string& _strName)
{
    METHOD_ENTRY("CObject::setName")

    m_UID.setName(_strName);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the time factor for this object.
///
/// The time factor is a variable that is multiplied with the frametime. Thus,
/// the kinematics integration of the object is faster or slower, leading to
/// effects like local slow-motion or local time-lapse.
///
/// \param _fTF Time factor to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setTimeFac(const double& _fTF)
{
    METHOD_ENTRY("CObject::setTimeFac")

    m_fTimeFac = _fTF;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the velocity of the mass
///
/// \param _vecVel Velocity of center of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::setVelocity(const Vector2d& _vecVel)
{
    METHOD_ENTRY("CObject::setVelocity")

    m_KinematicsState.setVelocity(_vecVel);
    m_pIntVel->init(_vecVel);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Enable dynamics calculations for object
///
/// This is the default state for each object. Dynamics calculations are done.
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::enableDynamics()
{
    METHOD_ENTRY("CObject::enableDynamics")

    m_bDynamics = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Disable dynamics caclulations for object
///
/// If dynamics calculations are disabled, object can only be influenced by
/// kinematics. If there is no kinematic controlled transformation the object
/// is fixed.
///
////////////////////////////////////////////////////////////////////////////////
inline void CObject::disableDynamics()
{
    METHOD_ENTRY("CObject::disableDynamics")

    m_bDynamics = false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the absolute count of object instances
///
/// \return Count of object instances
///
////////////////////////////////////////////////////////////////////////////////
inline uint32_t CObject::getCount()
{
    METHOD_ENTRY("CObject::getCount")
    return m_unNrOfObjects;
}

#endif
