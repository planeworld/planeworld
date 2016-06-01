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

    Vector2d vecAccel = m_vecForce / m_Geometry.getMass();
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

