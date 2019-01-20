////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2019 Torsten Büschenfeld
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
/// \file       handle.cpp
/// \brief      Implementation of class "CHandle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-01-13
///
////////////////////////////////////////////////////////////////////////////////

#include "handle_new.h"

CHandleManager CHandleBase::s_HandleManager;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns list (here: deque) of free handles
///
/// This method is mainly used for informatory purposes, debugging, or
/// statistics.
/// Delegate to \ref CHandleManager::getFreeHandles .
///
/// \return List of free handles
///
////////////////////////////////////////////////////////////////////////////////
const std::deque<std::uint32_t>* CHandleBase::getFreeHandles()
{
    METHOD_ENTRY("CHandleBase::getFreeHandles")
    return s_HandleManager.getFreeHandles();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns map (here: vector) of handled pointers
///
/// The handle index (id) refers to the vectors index (offset by 1 to mark
/// zero as invalid handle, so vector[0] => handle id 1.
/// This method is mainly used for informatory purposes, debugging, or
/// statistics.
/// Delegate to \ref CHandleManager::getHandleMap .
///
/// \return Map of handled pointers
///
////////////////////////////////////////////////////////////////////////////////
const std::vector<HandleMapEntry>* CHandleBase::getHandleMap()
{
    METHOD_ENTRY("CHandleBase::getHandleMap")
    return s_HandleManager.getHandleMap();
}
