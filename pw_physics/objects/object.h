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
/// \file       object.h
/// \brief      Prototype of interface "IObject"
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
typedef uint AnchorIDType;

/// Specifies the type of the object
enum class ObjectType
{
    OBJECT_NONE,
    OBJECT_BODY,
    OBJECT_POINTMASS,
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for general objects
///
/// This abstract class provides all basic methods and variables all object have
/// in common. For example, this is the position of the object and its mass.
/// Things like inertia are not included, since point masses and particals don't
/// need them. They are declared for a body, shape definition is declared there
/// too.
///
/// \todo Move depth information to shapes only. Then map information to object
/// 
////////////////////////////////////////////////////////////////////////////////
class IObject : public IUniqueIDUser,
                public IKinematicsStateUser,
                public IUniverseScaled
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IObject();
        IObject(const IObject* const);
        virtual ~IObject();

        //--- Constant methods -----------------------------------------------//
        virtual IObject*            clone() const = 0;                  ///< Clone object
        virtual const Vector2d      getAnchor(const int&) const = 0;    ///< Return anchor
        virtual const ObjectType    getObjectType() const;
        const Vector2d              getCOM() const;
        const int                   getDepths() const;
        const bool                  getDynamicsState() const;
        const bool                  getGravitationState() const;
        const double                getMass() const;
        const std::string           getName() const;
        const Vector2d              getForce() const {return m_vecForce;}
        const Vector2d              getOrigin() const;
        const Vector2d              getVelocity() const;
        const CTrajectory&          getTrajectory() const;

        //--- Methods --------------------------------------------------------//
        virtual void        addForce(const Vector2d&,  const Vector2d&) = 0;   ///< Add a force
        virtual void        addForceLC(const Vector2d&,  const Vector2d&) = 0; ///< Add a force (local coordinates)
        virtual void        clearForces() = 0;                                 ///< Clear forces acting on object
        
        void                addAcceleration(const Vector2d&);
        AnchorIDType        addAnchor(const Vector2d&);
        void                disableGravitation();
        void                enableGravitation();
        CGeometry* const    getGeometry();
        void                setCell(const Vector2i&);
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
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, IObject* const);
        friend std::ostream&    operator<<(std::ostream&, IObject* const);
        
    protected:

        //--- Abstract methods [protected] -----------------------------------//
        virtual void myDynamics(const double&) = 0;                 ///< Calculate dynamics from forces
        virtual void myInit() = 0;                                  ///< Initialise object -> total reset
        virtual void mySetNewIntegrator(const IntegratorType&) = 0; ///< Change type of integrator 
        virtual void myTransform() = 0;                             ///< Move and/or rotate a mass
        
        virtual std::istream& myStreamIn (std::istream&) = 0;       ///< Stream in from inherited class
        virtual std::ostream& myStreamOut(std::ostream&) = 0;       ///< Stream out from inherited class

        //-- Variables [protected] -------------------------------------------//
        bool                    m_bGravitation;                     ///< Does this object influence others by gravitation?
        bool                    m_bDynamics;                        ///< Dynamics calculations for object

        CTimer                  m_Lifetime;                         ///< Lifetime counter
        double                  m_fTimeFac;                         ///< Factor of realtime

        CGeometry               m_Geometry;                         ///< Geometry of object

        Vector2d                m_vecCOM;                           ///< Center of mass
        Vector2d                m_vecForce;                         ///< Resulting force applied
        
        int                     m_nDepthlayers;                     ///< Depths in which shape exists
        
        IIntegrator<Vector2d>*  m_pIntPos;                          ///< Position integrator
        IIntegrator<Vector2d>*  m_pIntVel;                          ///< Velocity integrator

//         std::string             m_strName;                          ///< Object's name

        std::vector<Vector2d>   m_Anchors;                          ///< Anchors to joints
        
        CTrajectory             m_Trajectory;                       ///< Trajectory of object
};

typedef std::unordered_map<std::string, IObject*>  ObjectsType;    ///< Specifies a list of objects

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the objects type - RTTI
///
/// \return Type of object
///
////////////////////////////////////////////////////////////////////////////////
inline const ObjectType IObject::getObjectType() const
{
    METHOD_ENTRY("IObject::getObjectType")
    return ObjectType::OBJECT_NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a specific acceleration to the body, for example gravitation
///
/// \param _vecA Acceleration vector
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::addAcceleration(const Vector2d& _vecA)
{
    METHOD_ENTRY("IObject::addAcceleration")

    this->addForce(_vecA*m_Geometry.getMass(), m_pIntPos->getValue()+m_vecCOM);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the center of mass (COM)
///
/// \return Center of mass (COM)
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d IObject::getCOM() const
{
    METHOD_ENTRY("IObject::getCOM")
//     return (m_vecCOM+m_pIntPos->getValue());
    return m_KinematicsState.getLocalOrigin() + m_vecCOM;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns depthlayers
///
/// \return Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline const int IObject::getDepths() const
{
    METHOD_ENTRY("IObject::getDepths")
    return (m_nDepthlayers);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the state for dynamics
///
/// \return Dynamics state, true=dynamic, false=static
///
////////////////////////////////////////////////////////////////////////////////
inline const bool IObject::getDynamicsState() const
{
    METHOD_ENTRY("IObject::getDynamicsState")
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
inline const bool IObject::getGravitationState() const
{
    METHOD_ENTRY("IObject::getGravitationState")
    return (m_bGravitation);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the mass of the object
///
/// \return Mass
///
////////////////////////////////////////////////////////////////////////////////
inline const double IObject::getMass() const
{
    METHOD_ENTRY("IObject::getMass")
    return (m_Geometry.getMass());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name of the object
///
/// \return Name of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string IObject::getName() const
{
    METHOD_ENTRY("IObject::getName")
    return (m_strName);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the origin of the object
///
/// \return Origin of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d IObject::getOrigin() const
{
    METHOD_ENTRY("IObject::getOrigin")
    return m_KinematicsState.getLocalOrigin();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the velocity of the object
///
/// \return Velocity of the object
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d IObject::getVelocity() const
{
    METHOD_ENTRY("IObject::getVelocity")
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
inline const CTrajectory& IObject::getTrajectory() const
{
    METHOD_ENTRY("IObject::getTrajectory")
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
inline AnchorIDType IObject::addAnchor(const Vector2d& _vecV)
{
    METHOD_ENTRY("IObject::addAnchor")

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
inline void IObject::disableGravitation()
{
    METHOD_ENTRY("IObject::disableGravitation")

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
inline void IObject::enableGravitation()
{
    METHOD_ENTRY("IObject::enableGravitation")

    m_bGravitation = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the geometry of the object
///
/// \return Geometry
///
////////////////////////////////////////////////////////////////////////////////
inline CGeometry* const IObject::getGeometry()
{
    METHOD_ENTRY("IObject::getGeometry")
    return &m_Geometry;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the origin of mass
///
/// \param _vecOrigin Origin of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("IObject::setOrigin")

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
inline void IObject::setOrigin(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("IObject::setOrigin")

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
inline void IObject::setDepths(const int& _nD)
{
    METHOD_ENTRY("IObject::setDepths")

    m_nDepthlayers |= _nD;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Unset depthlayers for this object
///
/// \param _nD Depthlayers as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::unsetDepths(const int& _nD)
{
    METHOD_ENTRY("IObject::unsetDepths")

    m_nDepthlayers &= (!_nD);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the name for corresponding object
///
/// \param _strName Name of mass
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setName(const std::string& _strName)
{
    METHOD_ENTRY("IObject::setName")

    m_strName = _strName;
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
inline void IObject::setTimeFac(const double& _fTF)
{
    METHOD_ENTRY("IObject::setTimeFac")

    m_fTimeFac = _fTF;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the velocity of the mass
///
/// \param _vecVel Velocity of center of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setVelocity(const Vector2d& _vecVel)
{
    METHOD_ENTRY("IObject::setVelocity")

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
inline void IObject::enableDynamics()
{
    METHOD_ENTRY("IObject::enableDynamics")

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
inline void IObject::disableDynamics()
{
    METHOD_ENTRY("IObject::disableDynamics")

    m_bDynamics = false;
}

#endif
