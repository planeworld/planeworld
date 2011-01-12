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

#include "contact.h"
#include "geometry.h"
#include "visuals.h"

//--- Standard header --------------------------------------------------------//
#include <vector>

// Forward declarations
class CBody;
class CPointMass;

/// Type definition for the AnchorIDs
typedef uint AnchorIDType;

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
////////////////////////////////////////////////////////////////////////////////
class IObject
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IObject();
        IObject(const double&);
        virtual ~IObject();

        //--- Constant methods -----------------------------------------------//
        virtual const Vector2d      getAnchor(const int&) const = 0;    ///< Return anchor
        const CGeometry&            getGeometry() const;
        
        const Vector2d              getCOM() const;
        const int                   getDepths() const;
        const bool                  getGravitationState() const;
        const double                getMass() const;
        const std::string           getName() const;
        const Vector2d              getVelocity() const;

        //--- Methods --------------------------------------------------------//
        virtual void addForce(const Vector2d&,  const Vector2d&) = 0;   ///< Add a force
        virtual void clearForces() = 0;                                 ///< Clear forces acting on object
        
        void                addAcceleration(const Vector2d&);
        AnchorIDType        addAnchor(const Vector2d&);
        void                addVisualsID(const VisualsIDType&);
        void                addVisualsIDList(const VisualsIDListType&);
        void                disableGravitation();
        void                enableGravitation();
        CGeometry&          getGeometry();
        void                setCOM(const Vector2d&);
        void                setCOM(const double&, const double&);
        void                setDepths(const int&);
        void                setMass(const double&);
        void                setName(const std::string&);
        void                setTimeFac(const double&);
        void                setVelocity(const Vector2d&);
        void                unsetDepths(const int&);

        void                enableDynamics();
        void                disableDynamics();
        
        void                dynamics(const double&);
        void                init();
        void                setNewIntegrator(const IntegratorType&);
        void                transform();

    protected:

        //--- Abstract methods [protected] -----------------------------------//
        virtual void myDynamics(const double&) = 0;                 ///< Calculate dynamics from forces
        virtual void myInit() = 0;                                  ///< Initialise object -> total reset
        virtual void mySetNewIntegrator(const IntegratorType&) = 0; ///< Change type of integrator 
        virtual void myTransform() = 0;                             ///< Move and/or rotate a mass

        //-- Variables [protected] -------------------------------------------//
        bool                    m_bGravitation;                     ///< Does this object influence others by gravitation?
        bool                    m_bDynamics;                        ///< Dynamics calculations for object

        CTimer                  m_Lifetime;                         ///< Lifetime counter
        double                  m_fTimeFac;                         ///< Factor of realtime

        CGeometry               m_Geometry;                         ///< Geometry of object
        VisualsIDListType       m_VisualsIDs;                       ///< Visuals of object

        Vector2d                m_vecCOM0;                          ///< Initial center of mass
        Vector2d                m_vecForce;                         ///< Resulting force applied
        
        double                  m_fMass;                            ///< Mass of object in kg
        int                     m_nDepthlayers;                     ///< Depths in which shape exists
        
        IIntegrator<Vector2d>*  m_pIntPos;                          ///< Position integrator
        IIntegrator<Vector2d>*  m_pIntVel;                          ///< Velocity integrator

        std::string             m_strName;                          ///< Object's name

        std::vector<Vector2d>   m_Anchors;                          ///< Anchors to joints
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a specific acceleration to the body, for example gravitation
///
/// \param _vecV Acceleration vector
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::addAcceleration(const Vector2d& _vecV)
{
    METHOD_ENTRY("IObject::addAcceleration")

    this->addForce(_vecV*m_fMass, m_pIntPos->getValue());

    METHOD_EXIT("IObject::addAcceleration")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the geometry of the object
///
/// \return Geometry
///
////////////////////////////////////////////////////////////////////////////////
inline const CGeometry& IObject::getGeometry() const
{
    METHOD_ENTRY("IObject::getGeometry")

    METHOD_EXIT("IObject::getGeometry")
    return (m_Geometry);
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

    METHOD_EXIT("IObject::getCOM")
    return (m_pIntPos->getValue());
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

    METHOD_EXIT("IObject::getDepths")
    return (m_nDepthlayers);
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

    METHOD_EXIT("IObject::getGravitationState")
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

    METHOD_EXIT("IObject::getMass")
    return (m_fMass);
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

    METHOD_EXIT("IObject::getName")
    return (m_strName);
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

    METHOD_EXIT("IObject::getVelocity")
    return (m_pIntVel->getValue());
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

    METHOD_EXIT("IObject::addAnchor")
    return m_Anchors.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an ID of objects visuals to list
///
/// The object just stores ID's to visuals. The visuals itself are handled by 
/// the visualsmanager.
///
/// \param _VisualsID ID of visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::addVisualsID(const VisualsIDType& _VisualsID)
{
    METHOD_ENTRY("IObject::addVisualsID")
    
    m_VisualsIDs.push_back(_VisualsID);

    METHOD_EXIT("IObject::addVisualsID")
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

    METHOD_EXIT("IObject::disableGravitation")
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

    METHOD_EXIT("IObject::enableGravitation")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the geometry of the object
///
/// \return Geometry
///
////////////////////////////////////////////////////////////////////////////////
inline CGeometry& IObject::getGeometry()
{
    METHOD_ENTRY("IObject::getGeometry")

    METHOD_EXIT("IObject::getGeometry")
    return (m_Geometry);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the center of mass (COM)
///
/// \param _vecCOM Center of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setCOM(const Vector2d& _vecCOM)
{
    METHOD_ENTRY("IObject::setCOM")

    m_vecCOM0 = _vecCOM;
    m_pIntPos->init(_vecCOM);

    METHOD_EXIT("IObject::setCOM")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the center of mass (COM)
///
/// \param _fX X-position of center of mass 
/// \param _fY Y-position of center of mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setCOM(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("IObject::setCOM")

    m_vecCOM0[0] = _fX;
    m_vecCOM0[1] = _fY;

    m_pIntPos->init(m_vecCOM0);

    METHOD_EXIT("IObject::setCOM")
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

    METHOD_EXIT("IObject::setDepths")
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

    METHOD_EXIT("IObject::unsetDepths")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the mass
///
/// \param _fMass Mass 
///
////////////////////////////////////////////////////////////////////////////////
inline void IObject::setMass(const double& _fMass)
{
    METHOD_ENTRY("IObject::setMass")

    m_fMass = _fMass;

    METHOD_EXIT("IObject::setMass")
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

    METHOD_EXIT("IObject::setName")
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

    METHOD_EXIT("IObject::setTimeFac")
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

    m_pIntVel->init(_vecVel);

    METHOD_EXIT("IObject::setVelocity")
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

    METHOD_EXIT("IObject::enableDynamics")
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

    METHOD_EXIT("IObject::disableDynamics")
}

#endif
