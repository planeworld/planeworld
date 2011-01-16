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
/// \file       object.cpp
/// \brief      Implementation of interface "IObject"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "object.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
IObject::IObject(): m_bGravitation(true),
                m_bDynamics(true),
                m_fTimeFac(1.0),
                m_fMass(1.0),
                m_nDepthlayers(0),
                m_strName("Object")
{
    METHOD_ENTRY("IObject::IObject")
    CTOR_CALL("IObject::IObject")
    
    m_pIntPos = new CAdamsMoultonIntegrator<Vector2d>;
    MEM_ALLOC("m_pIntPos")
    m_pIntVel = new CAdamsBashforthIntegrator<Vector2d>;
    MEM_ALLOC("m_pIntVel")

    m_vecOrigin0.setZero();
    m_vecCOM.setZero();
    m_vecForce.setZero();
    m_vecGrid.setZero();

    m_Lifetime.start();

    METHOD_EXIT("IObject::IObject")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor, initialising mass
///
///////////////////////////////////////////////////////////////////////////////
IObject::IObject(const double& _fM):
                    m_bGravitation(true),
                    m_bDynamics(true),
                    m_fTimeFac(1.0),
                    m_fMass(_fM),
                    m_nDepthlayers(0),
                    m_strName("Object")
{
    METHOD_ENTRY("IObject::IObject")
    CTOR_CALL("IObject::IObject")
    
    m_pIntPos = new CAdamsMoultonIntegrator<Vector2d>;
    MEM_ALLOC("m_pIntPos")
    m_pIntVel = new CAdamsBashforthIntegrator<Vector2d>;
    MEM_ALLOC("m_pIntVel")

    m_vecOrigin0.setZero();
    m_vecCOM.setZero();
    m_vecForce.setZero();
    m_vecGrid.setZero();

    m_Lifetime.start();

    METHOD_EXIT("IObject::IObject")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////
IObject::~IObject()
{
    METHOD_ENTRY("IObject::~IObject")
    DTOR_CALL("IObject::~IObject")

    if (m_pIntPos != 0)
    {
        delete m_pIntPos;
        m_pIntPos = 0;
        MEM_FREED("m_pIntPos")
    }

    if (m_pIntVel != 0)
    {
        delete m_pIntVel;
        m_pIntVel = 0;
        MEM_FREED("m_pIntVel")
    }

    m_Lifetime.stop();
    DOM_VAR(
        DEBUG_MSG("Object", "Lifetime (" << m_strName << "): "
                << m_Lifetime.getTime())
    )
    DOM_VAR(
        DEBUG_MSG("Object", "Gametime (" << m_strName << "): "
                << m_Lifetime.getTime()*m_fTimeFac)
    )

    METHOD_EXIT("IObject::~IObject")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a list of IDs to objects list of visuals
///
/// The object just stores IDs to visuals. The visuals itself are handled by 
/// the visualsmanager.
///
/// \param _VisualsID ID of visuals that should be added to list
///
////////////////////////////////////////////////////////////////////////////////
void IObject::addVisualsIDList(const VisualsIDListType& _VisualsIDList)
{
    METHOD_ENTRY("IObject::addVisualsID")
    
    VisualsIDListType::const_iterator ci = _VisualsIDList.begin();
    while (ci != _VisualsIDList.end())
    {
        m_VisualsIDs.push_back((*ci));
        ++ci;
    }

    METHOD_EXIT("IObject::addVisualsID")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate dynamics of object
///
/// \param _fTimeStep Time between two frames
///
///////////////////////////////////////////////////////////////////////////////
void IObject::dynamics(const double& _fTimeStep)
{
    METHOD_ENTRY("IObject::dynamics")

    // Call object specific dynamics if enabled
    if (m_bDynamics)
        this->myDynamics(_fTimeStep);

    METHOD_EXIT("IObject::dynamics")
    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise object
///
/// When the object is initialised, it is set back to the state of simulation
/// begin. Specific init methods are calledas well as transformation. Hence,
/// fixed object don't need to be transformed more than once.
///
///////////////////////////////////////////////////////////////////////////////
void IObject::init()
{
    METHOD_ENTRY("IObject::init")

    // Initialise bounding box
    // Center of mass (position vector) is always inside AABB
    m_Geometry.getBoundingBox().setLowerLeft( m_pIntPos->getValue()+m_vecCOM);
    m_Geometry.getBoundingBox().setUpperRight(m_pIntPos->getValue()+m_vecCOM);
   
    m_pIntPos->init(m_vecOrigin0);
    m_pIntVel->reset();

    this->myInit();

    METHOD_EXIT("IObject::init")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a new integrator for this instance
///
/// \param _IntType Which integrator to use
///
///////////////////////////////////////////////////////////////////////////////
void IObject::setNewIntegrator(const IntegratorType& _IntType)
{
    METHOD_ENTRY("IObject::setNewIntegrator")

    if (m_pIntPos != 0)
    {
        delete m_pIntPos;
        MEM_FREED("m_pIntPos")
        m_pIntPos = 0;
    }
    if (m_pIntVel != 0)
    {
        delete m_pIntVel;
        MEM_FREED("m_pIntVel")
        m_pIntVel = 0;
    }

    switch (_IntType)
    {
        case INTEGRATOR_EULER:
            m_pIntPos = new CEulerIntegrator<Vector2d>;
            m_pIntVel = new CEulerIntegrator<Vector2d>;
            MEM_ALLOC("m_pIntPos")
            MEM_ALLOC("m_pIntVel")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntPos = new CAdamsBashforthIntegrator<Vector2d>;
            m_pIntVel = new CAdamsBashforthIntegrator<Vector2d>;
            MEM_ALLOC("m_pIntPos")
            MEM_ALLOC("m_pIntVel")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntPos = new CAdamsMoultonIntegrator<Vector2d>;
            m_pIntVel = new CAdamsMoultonIntegrator<Vector2d>;
            MEM_ALLOC("m_pIntPos")
            MEM_ALLOC("m_pIntVel")
            break;
    }

    // Call object specific method
    this->mySetNewIntegrator(_IntType);

    METHOD_EXIT("IObject::setNewIntegrator")
    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transform Object
///
///////////////////////////////////////////////////////////////////////////////
void IObject::transform()
{
    METHOD_ENTRY("IObject::transform")

    // Don't touch fixed objects
    // Otherwise:
    if (m_bDynamics)
    {
        // Initialise bounding box
        // Center of mass (position vector) is always inside AABB
        m_Geometry.getBoundingBox().setLowerLeft( m_pIntPos->getValue() + m_vecCOM);
        m_Geometry.getBoundingBox().setUpperRight(m_pIntPos->getValue() + m_vecCOM);

        // Call object specific transformation
        this->myTransform();
    }

    METHOD_EXIT("IObject::transform")
    
}
