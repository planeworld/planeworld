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
/// \file       pointmass.cpp
/// \brief      Implementation of class "CPointMass"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "pointmass.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPointMass::CPointMass()
{
    METHOD_ENTRY("CPointMass::CPointMass");
    CTOR_CALL("CPointMass::CPointMass");

    // Default name for any particle:
    m_strName = "Point mass";

    METHOD_EXIT("CPointMass::CPointMass");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPointMass::~CPointMass()
{
    METHOD_ENTRY("CPointMass::~CPointMass");
    DTOR_CALL("CPointMass::~CPointMass");

    METHOD_EXIT("CPointMass::~CPointMass");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a force to Particle
///
/// \param _vecF Force to be applied
/// \param _vecPOC Point of contact, will be ignored here, always applying to
///                 center of mass.
///
///////////////////////////////////////////////////////////////////////////////
void CPointMass::addForce(const Vector2d& _vecF, const Vector2d& _vecPOC)
{
    METHOD_ENTRY("CPointMass::addForce(const Vector2d&, const Vector2d&)");

    m_vecForce += _vecF;

    METHOD_EXIT("CPointMass::addForce(const Vector2d&, const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculates movement of particle
///
/// This method calculates the movement -- acceleration, velocity, position --
/// of the particle by integration of the force.
///
///////////////////////////////////////////////////////////////////////////////
void CPointMass::calcMovement(const double& _fStep)
{
    METHOD_ENTRY("CPointMass::calcMovement()");

    Vector2d vecAccel = m_vecForce / m_fMass;
    m_pIntVel->integrate(vecAccel, _fStep*m_fTimeFac);
    m_pIntPos->integrate(m_pIntVel->getValue(), _fStep*m_fTimeFac);

    METHOD_EXIT("CPointMass::calcMovement()");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clear all forces acting on object
///
////////////////////////////////////////////////////////////////////////////////
void CPointMass::clearForces()
{
    METHOD_ENTRY("CPointMass::clearForces()");

    m_vecForce.setZero();

    METHOD_EXIT("CPointMass::clearForces()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test objects for collision
///
/// \param _pObject Other object
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CPointMass::myCollidesWith(IObject* _pObject)
{
    METHOD_ENTRY("CPointMass::myCollidesWith(IObject*)");

    METHOD_EXIT("CPointMass::myCollidesWith(IObject*)");
    return _pObject->collidesWith(this);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test for collision with body
///
/// \param _pBody Body
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CPointMass::myCollidesWith(CBody* _pBody)
{
    METHOD_ENTRY("CPointMass::myCollidesWith(CBody*)");

    CPolyLine Dummyshape;
    CContact Contacts;
    std::list<Vector2d> Intersections;

    Dummyshape.setLineType(GRAPHICS_LINETYPE_STRIP);
    Dummyshape.setDepths(SHAPE_DEPTH_ALL);
    Dummyshape.addVertex(m_pIntPos->getValue());
    Dummyshape.addVertex(m_pIntPos->getPrevValue());

    for (std::list< IShape* >::const_iterator ci = _pBody->getGeometry().getShapes().begin();
        ci != _pBody->getGeometry().getShapes().end(); ++ci)
    {
//      PointsOfContact = Dummyshape.intersectsWith((*ci));
//      if (PointsOfContact.size() != 0)
        {
//             m_Graphics.beginLine(GRAPHICS_LINETYPE_SINGLE, -15.0);
//                 m_Graphics.addVertex(m_pIntPos->getPrevValue());
//                 m_Graphics.addVertex(m_pIntPos->getPrevValue());
//             m_Graphics.endLine();

            // Add intersections to list
//          m_Intersections.insert(m_Intersections.end(), Intersections.begin(),
//                                      Intersections.end());
        }
    }

    METHOD_EXIT("CPointMass::myCollidesWith(CBody*)");
    return Contacts;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Test point masses for collision
///
/// \param _pPointMass Other point mass
///
/// \return Contacts
///
///////////////////////////////////////////////////////////////////////////////
const CContact CPointMass::myCollidesWith(CPointMass* _pPointMass)
{
    METHOD_ENTRY("CPointMass::myCollidesWith(CPointMass*)");

    METHOD_EXIT("CPointMass::myCollidesWith(CPointMass*)");
    return CContact();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Move the point mass
///
/// This method is just for updating the bounding boxes. These are computed
/// from actual and previous position.
///
///////////////////////////////////////////////////////////////////////////////
void CPointMass::myTransform()
{
    METHOD_ENTRY("CPointMass::myTransform()");

    // Update bounding box for continuous collision detection
    m_Geometry.updateBoundingBox(m_pIntPos->getPrevValue());
    m_Geometry.updateBoundingBox(m_pIntPos->getValue());
    
    METHOD_EXIT("CPointMass::myTransform()");
}

