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
