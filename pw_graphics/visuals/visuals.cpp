////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       visuals.cpp
/// \brief      Implementation of interface "IVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-23
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pVis IVisuals instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, IVisuals* const _pVis)
{
    METHOD_ENTRY("IVisuals::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IUniqueIDReferrer
    _is >> _pVis->m_UIDRef;
    
    return _pVis->myStreamIn(_is);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pVis IVisuals instance to stream
///
/// \return Stream with game state information of IVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, IVisuals* const _pVis)
{
    METHOD_ENTRY("IShape::operator<<")

    // ShapeVisualsType has to be the first information, since visuals creation
    // when loading depends on it.
    // Cast strongly typed enum ShapeVisualsType to streamable base type
    auto nShpVisType = static_cast<std::underlying_type<ShapeVisualsType>::type>(_pVis->getShapeVisualsType());
    _os << nShpVisType << std::endl;
    
    _os << "Visuals:" << std::endl;
    
    // From IUniqueIDReferrer
    _os << _pVis->m_UIDRef << std::endl;
    
    return _pVis->myStreamOut(_os);
}
