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
CBody::CBody() : m_fAngle(0.0), m_fInertia(1.0), m_fTorque(0.0)
{
    METHOD_ENTRY("CBody::CBody");
    CTOR_CALL("CBody::CBody");
    
    m_pIntAng = new CAdamsMoultonIntegrator<double>;
    MEM_ALLOC("m_pIntAng");
    m_pIntAngVel = new CAdamsBashforthIntegrator<double>;
    MEM_ALLOC("m_pIntAngVel");

    // Default name for any body:
    m_strName = "Body";

    METHOD_EXIT("CBody::CBody");
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
        MEM_FREED("m_pIntAng")
    }

    if (m_pIntAngVel != 0)
    {
        delete m_pIntAngVel;
        m_pIntAngVel = 0;
        MEM_FREED("m_pIntAngVel")
    }

    METHOD_EXIT("CBody::~CBody")
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
    Rotation2Dd Rotation(m_fAngle);

    vecResult = Rotation * m_Anchors[_nID] + m_pIntPos->getValue();

    METHOD_EXIT("CBody::getAnchor")
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

    m_fAngle = 0.0;
    m_pIntAng->reset();
    m_pIntAngVel->reset();
    
    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        (*ci)->transform(m_fAngle, m_pIntPos->getValue());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }
    // Copy geometry to previous timestep
    for (std::list< IShape* >::const_iterator ci = m_Geometry.getPrevShapes()->begin();
        ci != m_Geometry.getPrevShapes()->end(); ++ci)
    {
        (*ci)->transform(m_fAngle, m_pIntPos->getValue());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }

    METHOD_EXIT("CBody::myInit")
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
        MEM_FREED("m_pIntAng");
        m_pIntAng = 0;
    }
    if (m_pIntAngVel != 0)
    {
        delete m_pIntAngVel;
        MEM_FREED("m_pIntAngVel");
        m_pIntAngVel = 0;
    }

    switch (_IntType)
    {
        case INTEGRATOR_EULER:
            m_pIntAng = new CEulerIntegrator<double>;
            m_pIntAngVel = new CEulerIntegrator<double>;
            MEM_ALLOC("m_pIntAng")
            MEM_ALLOC("m_pIntAngVel")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntAng = new CAdamsBashforthIntegrator<double>;
            m_pIntAngVel = new CAdamsBashforthIntegrator<double>;
            MEM_ALLOC("m_pIntAng")
            MEM_ALLOC("m_pIntAngVel")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntAng = new CAdamsMoultonIntegrator<double>;
            m_pIntAngVel = new CAdamsMoultonIntegrator<double>;
            MEM_ALLOC("m_pIntAng")
            MEM_ALLOC("m_pIntAngVel")
            break;
    }

    METHOD_EXIT("CBody::mySetNewIntegrator")
    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Move and rotate the body
///
///////////////////////////////////////////////////////////////////////////////
void CBody::myTransform()
{
    METHOD_ENTRY("CBody::myTransform")

    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        // Update bounding box of previous time step for continuous collision dection
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }
    m_Geometry.update();
    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes()->begin();
        ci != m_Geometry.getShapes()->end(); ++ci)
    {
        (*ci)->transform(m_fAngle, m_pIntPos->getValue());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }

    METHOD_EXIT("CBody::myTransform")
}
