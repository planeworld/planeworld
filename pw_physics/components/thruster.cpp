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
/// \file       thruster.cpp
/// \brief      Implementation of class "CThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-03-04
///
////////////////////////////////////////////////////////////////////////////////

#include "thruster.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
CThruster::CThruster() : m_pThrusterBody(nullptr),
                         m_bActive(true),
                         m_fThrust(1.0),
                         m_fThrustMax(1.0),
                         m_pEmitter(nullptr),
                         m_fEmitterVelocity(1.0),
                         m_fEmitterVelocityStd(0.0)
{
    METHOD_ENTRY("CThruster::CThruster")
    CTOR_CALL("CThruster::CThruster")
    
//     IHooker::m_strName += ": Thruster";
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Activate thruster
///
/// This method sets the thrust member variable that is applied within the
/// execute() method which is called by the physics manager periodically.
///
/// \param _fThrust Thrust (reactive force) to apply
///
///////////////////////////////////////////////////////////////////////////////
void CThruster::activate(const double& _fThrust)
{
    METHOD_ENTRY("CThruster::activate")
    
    m_fThrust = _fThrust;
    if (m_fThrust > m_fThrustMax)
    {
      NOTICE_MSG("Thruster", "Exceeding maximum thrust, limiting to " << m_fThrustMax)
      m_fThrust = m_fThrustMax;
    }
    if (m_fThrust < 0.0) m_fThrust = 0.0;
    if (m_fThrust == 0.0) this->deactivate();
    else
    {
      m_bActive = true;
      m_pEmitter->activate();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Apply thrust
///
/// This method applies a force depending on the orientation and position of
/// the object the thruster is hooked on. This is clipped by maximum thrust
/// value.
///
///////////////////////////////////////////////////////////////////////////////
void CThruster::execute()
{
    METHOD_ENTRY("CThruster::execute")

    if (m_bActive)
    {
        Rotation2Dd ThrusterRotation(m_KinematicsState.getAngle());
        /*if (m_pThrusterBody == nullptr) std::cout << "nullptr" << std::endl;
        else*/ 
        m_pThrusterBody->addForce(ThrusterRotation * Vector2d(0.0,-m_fThrust), m_KinematicsState.getOrigin());
        
        if (m_fThrustMax != 0.0)
        {
            m_pEmitter->setVelocity(m_fThrust/m_fThrustMax * m_fEmitterVelocity);
            m_pEmitter->setVelocityStd(m_fThrust/m_fThrustMax * m_fEmitterVelocityStd);
        }
    }
}
