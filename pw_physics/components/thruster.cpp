////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2016 Torsten Büschenfeld
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
CThruster::CThruster() : m_bActive(true),
                         m_fThrust(1.0),
                         m_fThrustMax(1.0)

{
    METHOD_ENTRY("CThruster::CThruster")
    CTOR_CALL("CThruster::CThruster")
    
    m_UID.setName("Thruster_"+m_UID.getName());
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
/// \return Current thrust
///
///////////////////////////////////////////////////////////////////////////////
const double& CThruster::activate(const double& _fThrust)
{
    METHOD_ENTRY("CThruster::activate")
    
    m_fThrust = _fThrust;
    
    for (const auto Emitter : m_hEmitters)
    {
        if (Emitter.isValid())
        {
            Emitter.get()->setIntensity(_fThrust/m_fThrustMax);
        }
    }
    
    if (m_fThrust > m_fThrustMax)
    {
        m_fThrust = m_fThrustMax;
    }
    if (m_fThrust < 0.0) m_fThrust = 0.0;
    if (m_fThrust == 0.0 && m_bActive)
    {
        this->deactivate();
    }
    else if (!m_bActive)
    {
        m_bActive = true;
        for (const auto Emitter : m_hEmitters)
        {
            if (Emitter.isValid())
            {
                Emitter.get()->activate();
            }
        }
    }
    
    return m_fThrust;
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
        if (m_hObject.isValid())
        {
            m_hObject.get()->addForceLC(Vector2d(0.0,m_fThrust), m_KinematicsState.getLocalOrigin());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add an emitter to thruster
///
/// \param _pEmitter Emitter to be added to thruster
///
///////////////////////////////////////////////////////////////////////////////
void CThruster::addEmitter(IEmitter* const _pEmitter)
{
    METHOD_ENTRY("CThruster::addEmitter")
    m_hEmitters.push_back(CHandle<IEmitter>(_pEmitter));
    if (m_hObject.isValid())
    {
        _pEmitter->attachTo(m_hObject.get());
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set physical object the thruster is bound to
///
/// \param _pObj Physical object the thruster is bound to
///
///////////////////////////////////////////////////////////////////////////////
void CThruster::setObject(CObject* const _pObj)
{
    METHOD_ENTRY("CThruster::setObject")
    m_hObject.set(_pObj);
    m_KinematicsState.setRef(&(_pObj->getKinematicsState()));
    for (auto Emitter : m_hEmitters)
    {
        Emitter.get()->attachTo(_pObj);
    }
}
