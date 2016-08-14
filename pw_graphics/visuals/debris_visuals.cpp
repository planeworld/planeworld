////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       debris_visuals.cpp
/// \brief      Implementation of class "CDebrisVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-10
///
////////////////////////////////////////////////////////////////////////////////

#include "debris_visuals.h"

//--- Program header ---------------------------------------------------------//
#include "graphics.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisuals::CDebrisVisuals()
{
    METHOD_ENTRY("CDebrisVisuals::CDebrisVisuals")
    CTOR_CALL("CDebrisVisuals::CDebrisVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pDebris Debris to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisuals::CDebrisVisuals(CDebris* _pDebris)
{
    METHOD_ENTRY("CDebrisVisuals::CDebrisVisuals")
    CTOR_CALL("CDebrisVisuals::CDebrisVisuals")
    this->attachTo(_pDebris);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisuals::~CDebrisVisuals()
{
    METHOD_ENTRY("CDebrisVisuals::~CDebrisVisuals()");
    DTOR_CALL("CDebrisVisuals::~CDebrisVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Debris
///
/// \param _pCamera Active camera for drawing visuals
///
/// \todo Filter by debris state. Don't draw inactive debris.
/// \todo Single calling is not very performant (see commented call of "dots"
///       for better reference).
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CDebrisVisuals::draw()");
    CDebris* pDebris = static_cast<CDebris*>(m_pDataStorage->getUIDUsersByValueFront()->at(m_UIDRef));
    m_Graphics.dots((*pDebris->getPositions()),-_pCamera->getCenter()+
                      IUniverseScaled::cellToDouble(pDebris->getCell() - _pCamera->getCell()));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pDV CDebrisVisuals instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CDebrisVisuals* const _pDV)
{
    METHOD_ENTRY("CDebrisVisuals::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IUniqueIDReferrer
    _is >> _pDV->m_UIDRef;
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _pDV CDebrisVisuals instance to stream
///
/// \return Stream with game state information of CDebrisVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CDebrisVisuals* const _pDV)
{
    METHOD_ENTRY("CDebrisVisuals::operator<<")
    
    _os << "DebrisVisuals:" << std::endl;
    
    // From IUniqueIDReferrer
    _os << _pDV->m_UIDRef << std::endl;
    
    return _os;
}
