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
/// \file       handle_manager.cpp
/// \brief      Implementation of class "CHandleManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-01-15
///
////////////////////////////////////////////////////////////////////////////////

#include "handle_manager.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add new pointer to map and create unique handle id
///
/// \param _ptr Pointer to be handled by unique handle id
///
/// \return Unique handle id
///
////////////////////////////////////////////////////////////////////////////////
HandleID CHandleManager::add(void* const _ptr)
{
    METHOD_ENTRY("CHandleManager::add")
    
    PW_ASSERT(_ptr != nullptr);
    
    HandleMapEntry HMap;
    HMap.pEntry = _ptr;
    
    if (m_HandlesFree.size() > 0)
    {
        std::uint32_t IndexFree = m_HandlesFree.front();
        m_HandlesFree.pop_front();
        HMap.ID.Index = IndexFree;
        HMap.ID.Counter = m_HandleMap[IndexFree-1].ID.Counter+1;
        m_HandleMap[IndexFree-1] = HMap;
    }
    else
    {
        HMap.ID.Index = m_HandleMap.size()+1;
        HMap.ID.Counter = 1;
        m_HandleMap.push_back(HMap);
    }
    
    return HMap.ID;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Removes handle from handle manager, all other instances of this 
///        handle will become invalid.
///
/// \param _ID Handle Id to be removed
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CHandleManager::remove(const HandleID _ID)
{
    METHOD_ENTRY("CHandleManager::remove")
    
    if (_ID.Index <= m_HandleMap.size() &&
        _ID.Index != 0)
    {
        m_HandleMap[_ID.Index-1].ID.Index = 0;
        m_HandleMap[_ID.Index-1].ID.Counter += 1;
        m_HandleMap[_ID.Index-1].pEntry = nullptr;
        m_HandlesFree.push_back(_ID.Index);
        return true;
    }
    else
    {
        WARNING_MSG("Handle Manager", "Handle " << _ID.Index << " not valid.")
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns list (here: deque) of free handles
///
/// This method is mainly used for informatory purposes, debugging, or
/// statistics.
///
/// \return List of free handles
///
////////////////////////////////////////////////////////////////////////////////
const std::deque<std::uint32_t>* CHandleManager::getFreeHandles()
{
    METHOD_ENTRY("CHandleManager::getFreeHandles")
    return &m_HandlesFree;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns map (here: vector) of handled pointers
///
/// The handle index (id) refers to the vectors index (offset by 1 to mark
/// zero as invalid handle, so vector[0] => handle id 1.
/// This method is mainly used for informatory purposes, debugging, or
/// statistics.
///
/// \return Map of handled pointers
///
////////////////////////////////////////////////////////////////////////////////
const std::vector<HandleMapEntry>* CHandleManager::getHandleMap()
{
    METHOD_ENTRY("CHandleManager::getHandleMap")
    return &m_HandleMap;
}
