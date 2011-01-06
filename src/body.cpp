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
    
    m_pIntAng = new CAdamsMoultonIntegrator();
    MEM_ALLOC("m_pIntAng");
    m_pIntAngVel = new CAdamsBashforthIntegrator();
    MEM_ALLOC("m_pIntAngVel");

    // Default name for any body:
    m_strName = "Body";

    METHOD_EXIT("CBody::CBody");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
/// \todo Remove list element instead of setting pointer=0
/// \todo Properly displaying message of destruction
///
/// \bug Freeing memory not displayed
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
/// \brief Rotates the body
///
/// \param _fAngle Angle to rotate
///
///////////////////////////////////////////////////////////////////////////////
void CBody::rotBy(const double& _fAngle)
{
    METHOD_ENTRY("CBody::rotBy")

    m_fAngle += _fAngle;

    METHOD_EXIT("CBody::rotBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test bodies for collision
///
/// \param _pObject Other object
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CBody::myCollidesWith(IObject* _pObject)
{
    METHOD_ENTRY("CBody::myCollidesWith")

    METHOD_EXIT("CBody::myCollidesWith")
    return _pObject->collidesWith(this);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test bodies for collision
///
/// \param _pBody Other body
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CBody::myCollidesWith(CBody* _pBody)
{
    METHOD_ENTRY("CBody::myCollidesWith")

    CContact            Contact;
    IntersectionList    Intersections;

    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes().begin();
        ci != m_Geometry.getShapes().end(); ++ci)
    {
        for (std::list< IShape* >::const_iterator cj = _pBody->getGeometry().getShapes().begin();
        cj != _pBody->getGeometry().getShapes().end(); ++cj)
        {
            // Test for overlapping depthlayers
            if (((*ci)->getDepths() & (*cj)->getDepths()) > 0)
            {
                // Add Intersections to contact
                IntersectionList TmpList;// = (*ci)->intersectsWith((*cj));
                Intersections.splice(Intersections.end(),TmpList);
            }
        }
    }
    if (!Intersections.empty())
    {
        Contact.setOccurred(true);
        Contact.setPointsOfContact(Intersections);
    }

    METHOD_EXIT("CBody::myCollidesWith")
    return Contact;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test body and point mass for collision
///
/// \param _pPointMass Point mass
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CBody::myCollidesWith(CPointMass* _pPointMass)
{
    METHOD_ENTRY("CBody::myCollidesWith")

    METHOD_EXIT("CBody::myCollidesWith")
    return _pPointMass->collidesWith(this);
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
    
    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes().begin();
        ci != m_Geometry.getShapes().end(); ++ci)
    {
        (*ci)->transform(m_fAngle, m_pIntPos->getValue());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }
    // Copy geometry to previous timestep
    m_Geometry.update();

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
            m_pIntAng = new CEulerIntegrator;
            m_pIntAngVel = new CEulerIntegrator;
            MEM_ALLOC("m_pIntAng")
            MEM_ALLOC("m_pIntAngVel")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntAng = new CAdamsBashforthIntegrator;
            m_pIntAngVel = new CAdamsBashforthIntegrator;
            MEM_ALLOC("m_pIntAng")
            MEM_ALLOC("m_pIntAngVel")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntAng = new CAdamsMoultonIntegrator;
            m_pIntAngVel = new CAdamsMoultonIntegrator;
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

    m_Geometry.update();
    for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes().begin();
        ci != m_Geometry.getShapes().end(); ++ci)
    {
        // Update bounding box of previous time step for continuous collision dection
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
        
        (*ci)->transform(m_fAngle, m_pIntPos->getValue());

        // Update depthlayers
        m_nDepthlayers |= (*ci)->getDepths();

        // Update bounding box of current time step
        m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
    }

    METHOD_EXIT("CBody::myTransform")
}
