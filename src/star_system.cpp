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
/// \file       star_system.cpp
/// \brief      Implementation of class "CStarSystem"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-18
///
////////////////////////////////////////////////////////////////////////////////

#include "star_system.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CStarSystem::CStarSystem() : m_nNumberOfPlanets(0),
                             m_StarType(STAR_CLASS_M)
{
    METHOD_ENTRY("CStarSystem::CStarSystem()");
    CTOR_CALL("CStarSystem::CStarSystem()");
    
    m_vecLocation.setZero();
}
