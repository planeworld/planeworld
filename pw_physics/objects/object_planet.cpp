////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2018 Torsten Büschenfeld
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
/// \file       object_planet.cpp
/// \brief      Implementation of class "CObjectPlanet"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-01-05
///
////////////////////////////////////////////////////////////////////////////////

#include "object_planet.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
CObjectPlanet::CObjectPlanet() : CObject()
{
    METHOD_ENTRY("CObjectPlanet::CObjectPlanet")
    CTOR_CALL("CObjectPlanet::CObjectPlanet")

    m_bGravitation = true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor
///
/// \param _ObjPl Planetary object to be constructed from
///
////////////////////////////////////////////////////////////////////////////////
CObjectPlanet::CObjectPlanet(const CObjectPlanet& _ObjPl) : CObject(_ObjPl)
{
    METHOD_ENTRY("CObjectPlanet::CObjectPlanet")
    CTOR_CALL("CObjectPlanet::CObjectPlanet")
    
    this->copy(_ObjPl);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _ObjPl Planetary bject to be copied and assigned data from
///
///////////////////////////////////////////////////////////////////////////////
CObjectPlanet& CObjectPlanet::operator=(const CObjectPlanet& _ObjPl)
{
    METHOD_ENTRY("CObjectPlanet::operator=")
    
    if (this != &_ObjPl)
    {
        CObject::operator=(_ObjPl);
        this->copy(_ObjPl);
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone planetary object
///
/// \return Pointer to cloned planetary object
///
////////////////////////////////////////////////////////////////////////////////
const CObjectPlanet* CObjectPlanet::clone() const
{
    METHOD_ENTRY("CObjectPlanet::clone")
    
    CObjectPlanet* pClone = new CObjectPlanet(*this);
    MEM_ALLOC("CObject")
    
    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy data from given planetary object
///
/// \param _ObjPl Planetary object to copy data from
///
////////////////////////////////////////////////////////////////////////////////
void CObjectPlanet::copy(const CObjectPlanet& _ObjPl)
{
    METHOD_ENTRY("CObjectPlanet::copy")
    m_fAtmosphericPressure = _ObjPl.m_fAtmosphericPressure;
    m_fRadius = _ObjPl.m_fRadius;
    m_fScaleHeight = _ObjPl.m_fScaleHeight;
}

SERIALIZE_IMPL(CObjectPlanet,
    
)
