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
/// \file       body.cpp
/// \brief      Implementation of class "CBody"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "body.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CBody::CBody() : m_fInertia(1.0), m_fTorque(0.0)
{
    METHOD_ENTRY("CBody::CBody");
    CTOR_CALL("CBody::CBody");
    
    m_pIntAng = new CEulerIntegrator<double>;
    MEM_ALLOC("CEulerIntegrator");
    m_pIntAngVel = new CEulerIntegrator<double>;
    MEM_ALLOC("CEulerIntegrator");

    // Default name for any body:
    m_strName = "Body";
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
CBody::~CBody()
{
    METHOD_ENTRY("CBody::~CBody")
    DTOR_CALL("CBody::~CBody")
    
    if (m_pIntAng != 0)
    {
        delete m_pIntAng;
        m_pIntAng = 0;
        MEM_FREED("IIntegrator")
    }

    if (m_pIntAngVel != 0)
    {
        delete m_pIntAngVel;
        m_pIntAngVel = 0;
        MEM_FREED("IIntegrator")
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns an anchor
///
/// \param _nID ID of anchor that should be returned
///
/// \return Demanded anchor
///
///////////////////////////////////////////////////////////////////////////////
const Vector2d CBody::getAnchor(const int& _nID) const
{
    METHOD_ENTRY("CBody::getAnchor")

    Vector2d vecResult;
    Rotation2Dd Rotation(m_KinematicsState.getLocalAngle());

    vecResult = Rotation * m_Anchors[_nID] + m_pIntPos->getValue();

    return vecResult;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Specific initialisation
///
/// This method initialises it's specific members.
///
///////////////////////////////////////////////////////////////////////////////
void CBody::myInit()
{
    METHOD_ENTRY("CBody::myInit")
    
    m_pIntAng->init(m_KinematicsState.getLocalAngle());
    m_pIntAngVel->init(m_KinematicsState.getLocalAngleVelocity());

    for (std::list< CDoubleBufferedShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        (*ci)->getShapeCur()->transform(m_KinematicsState.getAngle(), m_KinematicsState.getOrigin());
        (*ci)->getShapeBuf()->transform(m_KinematicsState.getAngle(), m_KinematicsState.getOrigin());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getShapeCur()->getDepths();

        // Update bounding box
        m_Geometry.updateBoundingBox((*ci)->getShapeCur()->getBoundingBox());
        m_Geometry.updateBoundingBox((*ci)->getShapeBuf()->getBoundingBox());
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a new integrator for this instance
///
/// \param _IntType Which integrator to use
///
///////////////////////////////////////////////////////////////////////////////
void CBody::mySetNewIntegrator(const IntegratorType& _IntType)
{
    METHOD_ENTRY("CBody::mySetNewIntegrator")

    if (m_pIntAng != 0)
    {
        delete m_pIntAng;
        MEM_FREED("IIntegrator");
        m_pIntAng = 0;
    }
    if (m_pIntAngVel != 0)
    {
        delete m_pIntAngVel;
        MEM_FREED("IIntegrator");
        m_pIntAngVel = 0;
    }

    switch (_IntType)
    {
        case INTEGRATOR_EULER:
            m_pIntAng = new CEulerIntegrator<double>;
            m_pIntAngVel = new CEulerIntegrator<double>;
            MEM_ALLOC("CEulerIntegrator")
            MEM_ALLOC("CEulerIntegrator")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntAng = new CAdamsBashforthIntegrator<double>;
            m_pIntAngVel = new CAdamsBashforthIntegrator<double>;
            MEM_ALLOC("CAdamsMoultonIntegrator")
            MEM_ALLOC("CAdamsMoultonIntegrator")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntAng = new CAdamsMoultonIntegrator<double>;
            m_pIntAngVel = new CAdamsMoultonIntegrator<double>;
            MEM_ALLOC("CAdamsBashforthIntegrator")
            MEM_ALLOC("CAdamsBashforthIntegrator")
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Move and rotate the body
///
///////////////////////////////////////////////////////////////////////////////
void CBody::myTransform()
{
    METHOD_ENTRY("CBody::myTransform")

    for (std::list< CDoubleBufferedShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        // Update bounding box of previous time step for continuous collision dection
        m_Geometry.updateBoundingBox((*ci)->getShapeCur()->getBoundingBox());
    }
    m_Geometry.update();
    for (std::list< CDoubleBufferedShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        (*ci)->getShapeCur()->transform(m_KinematicsState.getAngle(), m_KinematicsState.getOrigin());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getShapeCur()->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getShapeCur()->getBoundingBox());
    }
}
