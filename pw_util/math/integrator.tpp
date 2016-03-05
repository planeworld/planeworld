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
/// \file       integrator.cpp
/// \brief      Implementation of interface "IIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-16
///
////////////////////////////////////////////////////////////////////////////////

#include "integrator.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pInt IIntegrator interface to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
template <class U>
std::istream& operator>>(std::istream& _is, IIntegrator<U>* const _pInt)
{
    METHOD_ENTRY("IIntegrator::operator>>")
    
    return _pInt->myStreamIn(_is);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pInt IIntegrator interface to stream
///
/// \return Stream with game state information of IIntegrator instance
///
////////////////////////////////////////////////////////////////////////////////
template <class U>
std::ostream& operator<<(std::ostream& _os, IIntegrator<U>* const _pInt)
{
    METHOD_ENTRY("IIntegrator::operator<<")
    
    return _pInt->myStreamOut(_os);
}
