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
/// \file       handle_manager.h
/// \brief      Prototype of class "CHandleManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-01-07
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HANDLE_MANAGER_H
#define HANDLE_MANAGER_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <deque>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//

constexpr std::uint32_t MAX_HANDLES = 32768;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Struct defining components of the numeric handle value
///
/// The Index is the actual handle id, the Counter is used to manage stale
/// handles while the Free component is not used, yet.
/// A bitset is used instead of a union provide more flexibility and avoid
/// another indirection when accessing the components. 32bit were used
/// to make sure external modules like Lua can handle the Raw handle value.
///
////////////////////////////////////////////////////////////////////////////////
struct HandleID
{
    std::uint32_t   Index :  16,    ///< Actual handle id
                    Counter : 8,    ///< Counter to manage stale handles
                    Free :    8;    ///< Not used, yet
    
    std::uint32_t Raw() const {return std::uint32_t(Counter) << 16 | std::uint32_t(Index);}
    
    HandleID() : Index(0u), Counter(0u), Free(0u) {}
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Handle map entry for actual mapping from handle to pointer
///
////////////////////////////////////////////////////////////////////////////////
struct HandleMapEntry
{
    HandleID ID;        ///< Numeric handle id consisting of index and internal data
    void*    pEntry;    ///< Pointer represented by handle
    
    HandleMapEntry() : pEntry(nullptr) {}
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Handle management ensuring unique id's and valid pointers
///
////////////////////////////////////////////////////////////////////////////////
class CHandleManager
{

    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CHandleManager() {m_HandleMap.reserve(MAX_HANDLES);}
   
        //--- Constant Methods -----------------------------------------------//
        bool isValid(const HandleID) const;

        //--- Methods --------------------------------------------------------//
        HandleID                add(void* const);
        template<class T> T*    get(const HandleID);
        bool                    remove(const HandleID);
        template<class T> void  update(HandleID&, T* const);
        
        const std::deque<std::uint32_t>* getFreeHandles();
        const std::vector<HandleMapEntry>* getHandleMap();
        
        
    private:
        
        //--- Variables [protected] ------------------------------------------//
        std::vector<HandleMapEntry> m_HandleMap;    ///< Handle map, mapping handle id's to pointers
        std::deque<std::uint32_t>   m_HandlesFree;  ///< Free handles to be reused
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests given handle for validity
///
/// \param _ID Handle id to test for validity
///
/// \return Handle valid (true/false)?
///
////////////////////////////////////////////////////////////////////////////////
inline bool CHandleManager::isValid(const HandleID _ID) const
{
    METHOD_ENTRY("CHandleManager::isValid")
    return (_ID.Index && (_ID.Counter == m_HandleMap[_ID.Index-1].ID.Counter));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns pointer which is represented by handle
///
/// \param _ID Handle id of pointer to be returned
///
/// \return Pointer represented by handle
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* CHandleManager::get(const HandleID _ID)
{
    METHOD_ENTRY("CHandleManager::get")
    
    PW_ASSERT(_ID.Index > 0u);
    PW_ASSERT(m_HandleMap[_ID.Index-1].pEntry != nullptr);
    
    return static_cast<T*>(m_HandleMap[_ID.Index-1].pEntry);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates handle with new pointer, all other instances of this handle 
///        will become invalid.
///
/// \param _ID Handle id of pointer to be updated
/// \param _ptr Pointer to update handle with
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void CHandleManager::update(HandleID& _ID, T* const _ptr)
{
    METHOD_ENTRY("CHandleManager::update")
    
    PW_ASSERT(_ptr != nullptr);
    if (this->isValid(_ID))
    {
        m_HandleMap[_ID.Index-1].pEntry = _ptr;
    }
}

#endif // HANDLE_MANAGER_H
