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
                m_nDepthlayers(SHAPE_DEPTH_ALL),
                m_strName("Object")
{
    METHOD_ENTRY("IObject::IObject")
    CTOR_CALL("IObject::IObject")
    
    m_pIntPos = new CEulerIntegrator<Vector2d>;
    MEM_ALLOC("CEulerIntegrator")
    m_pIntVel = new CEulerIntegrator<Vector2d>;
    MEM_ALLOC("CEulerIntegrator")

    m_vecOrigin0.setZero();
    m_vecCOM.setZero();
    m_vecForce.setZero();
    m_vecCell.setZero();

    m_Lifetime.start();
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
        MEM_FREED("IIntegrator")
    }

    if (m_pIntVel != 0)
    {
        delete m_pIntVel;
        m_pIntVel = 0;
        MEM_FREED("IIntegrator")
    }

    m_Lifetime.stop();
    DOM_STATS(
        DEBUG_MSG("Object", "Lifetime (" << m_strName << "): "
                << m_Lifetime.getTime())
    )
    DOM_STATS(
        DEBUG_MSG("Object", "Gametime (" << m_strName << "): "
                << m_Lifetime.getTime()*m_fTimeFac)
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Update the cell of this object
///
/// \todo Shouldn't this be a method of IUniverseScaled?!
///
////////////////////////////////////////////////////////////////////////////////
void IObject::updateCell()
{
    METHOD_ENTRY("IObject::updateCell")

    Vector2d vecUpdate;
    Vector2i vecUpdateCell;
    vecUpdate.setZero();
    vecUpdateCell.setZero();
    
    if      (m_pIntPos->getValue()[0] >  DEFAULT_CELL_SIZE)
    {
        vecUpdate[0]     -= DEFAULT_CELL_SIZE_2;
        vecUpdateCell[0] += 1;
    }
    else if (m_pIntPos->getValue()[0] < -DEFAULT_CELL_SIZE)
    {
        vecUpdate[0] += DEFAULT_CELL_SIZE_2;
        vecUpdateCell[0] -= 1;
    }
    else if (m_pIntPos->getValue()[1] >  DEFAULT_CELL_SIZE)
    {
        vecUpdate[1] -= DEFAULT_CELL_SIZE_2;
        vecUpdateCell[1] += 1;
    }
    else if (m_pIntPos->getValue()[1] < -DEFAULT_CELL_SIZE)
    {
        vecUpdate[1] += DEFAULT_CELL_SIZE_2;
        vecUpdateCell[1] -= 1;
    }
    m_pIntPos->init(m_pIntPos->getValue()+vecUpdate);
    /// \bug Bounding box must be updated with new relative position
    
    m_vecCell += vecUpdateCell;
    this->setCell(m_vecCell);

    DEBUG(
        if (std::abs(vecUpdateCell[0]) > 0 || std::abs(vecUpdateCell[1] > 0))
        {
            DOM_VAR(DEBUG_MSG("Object Interface","Cell update for " << m_strName << " is " <<
                            vecUpdateCell[0] << ", " << vecUpdateCell[1]))
        }
    )
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
/// begin. Specific init methods are called as well as transformation. Hence,
/// fixed objects don't need to be transformed more than once.
///
///////////////////////////////////////////////////////////////////////////////
void IObject::init()
{
    METHOD_ENTRY("IObject::init")

    // Initialise bounding box
    // Center of mass (position vector) is always inside AABB
    m_Geometry.getBoundingBox().setLowerLeft( m_pIntPos->getValue()+m_vecCOM);
    m_Geometry.getBoundingBox().setUpperRight(m_pIntPos->getValue()+m_vecCOM);
    this->setCell(m_vecCell);
   
    m_pIntPos->init(m_vecOrigin0);
    this->myInit();

    METHOD_EXIT("IObject::init")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to set cell, overloaded to hand cell to bounding box.
///
/// \param _vecCell Grid cell of the object
///
////////////////////////////////////////////////////////////////////////////////
void IObject::setCell(const Vector2i& _vecCell)
{
    METHOD_ENTRY("IObject::setCell")

    m_vecCell = _vecCell;
    m_Geometry.getBoundingBox().setCell(_vecCell);
    std::list<CDoubleBufferedShape*>::const_iterator ci = m_Geometry.getShapes()->begin();
    while (ci != m_Geometry.getShapes()->end())
    {
        (*ci)->getShapeCur()->getBoundingBox().setCell(m_vecCell);
        ++ci;
    }
    ci = m_Geometry.getShapes()->begin();
    while (ci != m_Geometry.getShapes()->end())
    {
        (*ci)->getShapeBuf()->getBoundingBox().setCell(m_vecCell);
        ++ci;
    }    
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
        MEM_FREED("IIntegrator")
        m_pIntPos = 0;
    }
    if (m_pIntVel != 0)
    {
        delete m_pIntVel;
        MEM_FREED("IIntegrator")
        m_pIntVel = 0;
    }

    switch (_IntType)
    {
        case INTEGRATOR_EULER:
            m_pIntPos = new CEulerIntegrator<Vector2d>;
            m_pIntVel = new CEulerIntegrator<Vector2d>;
            MEM_ALLOC("CEulerIntegrator")
            MEM_ALLOC("CEulerIntegrator")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntPos = new CAdamsBashforthIntegrator<Vector2d>;
            m_pIntVel = new CAdamsBashforthIntegrator<Vector2d>;
            MEM_ALLOC("CAdamsBashforthIntegrator")
            MEM_ALLOC("CAdamsBashforthIntegrator")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntPos = new CAdamsMoultonIntegrator<Vector2d>;
            m_pIntVel = new CAdamsMoultonIntegrator<Vector2d>;
            MEM_ALLOC("CAdamsMoultonIntegrator")
            MEM_ALLOC("CAdamsMoultonIntegrator")
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
