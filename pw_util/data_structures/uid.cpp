////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2018 Torsten Büschenfeld
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
/// \file       uid.cpp
/// \brief      Implementation of class "CUID"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-24
///
////////////////////////////////////////////////////////////////////////////////

#include "uid.h"

/// Global counter for unique IDs. Reserve 0 for no reference
UIDType CUID::s_nUID = 1;

/// Global list for unused unique IDs
std::deque<UIDType> CUID::s_UnusedUIDs;

/// Global list for reference counting of uids
std::unordered_map<UIDType, std::uint32_t> CUID::s_ReferencedUIDs;

/// Global lock
CSpinlock CUID::s_Access;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
/// When the constructor is called, the specific ID is assigned. If there are
/// any unused IDs, these will be used first.
///
///////////////////////////////////////////////////////////////////////////////
CUID::CUID()
{
    METHOD_ENTRY("CUID::CUID")
    DTOR_CALL("CUID::CUID")
    
    s_Access.acquireLock();
    
    if (s_UnusedUIDs.empty())
    {
        m_nUID = s_nUID++;
    }
    else
    {
        m_nUID = s_UnusedUIDs.front();
        s_UnusedUIDs.pop_front();
    }
    s_ReferencedUIDs[m_nUID] = 1u;
    m_strName = "UID_"+std::to_string(m_nUID);
    
    s_Access.releaseLock();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// Copy constructor will exactly do the same as standard constructor, since
/// id has to be unique and thus, may not be copied 1:1.
///
/// \param _UID UID (won't be copied to ensure unique ids)
///
///////////////////////////////////////////////////////////////////////////////
CUID::CUID(const CUID& _UID)
{
    METHOD_ENTRY("CUID::CUID")
    DTOR_CALL("CUID::CUID")
    
    s_Access.acquireLock();
    
    this->copy(_UID);
    
    s_Access.releaseLock();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
/// When the destructor is called, the specific ID is released and thus added
/// to the list of unused IDs. These can be used by the next unique ID that is
/// constructed.
///
///////////////////////////////////////////////////////////////////////////////
CUID::~CUID()
{
    METHOD_ENTRY("CUID::~CUID")
    DTOR_CALL("CUID::~CUID")
    
    s_Access.acquireLock();
    
    s_ReferencedUIDs[m_nUID] -= 1u;
    if (s_ReferencedUIDs[m_nUID] == 0u)
    {
        s_UnusedUIDs.push_back(m_nUID);
        s_ReferencedUIDs.erase(m_nUID);
    }
    
    s_Access.releaseLock();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _UID UID to be copied and assigned data from
///
///////////////////////////////////////////////////////////////////////////////
CUID& CUID::operator=(const CUID& _UID)
{
    METHOD_ENTRY("CUID::operator=")
    
    if (this != &_UID)
    {
        s_Access.acquireLock();
        
        s_ReferencedUIDs[m_nUID] -= 1u;
        if (s_ReferencedUIDs[m_nUID] == 0u)
        {
            s_UnusedUIDs.push_back(m_nUID);
            s_ReferencedUIDs.erase(m_nUID);
        }
            
        this->copy(_UID);
        
        s_Access.releaseLock();
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a new value for this ID
///
////////////////////////////////////////////////////////////////////////////////
void CUID::setNewID()
{
    METHOD_ENTRY("CUID::setNewID")
    
    s_Access.acquireLock();
    
    UIDType nTmp;
    if (s_UnusedUIDs.empty())
    {
        nTmp = s_nUID++;
    }
    else
    {
        nTmp = s_UnusedUIDs.front();
        s_UnusedUIDs.pop_front();
    }
    s_ReferencedUIDs[m_nUID] -= 1u;
    if (s_ReferencedUIDs[m_nUID] == 0u)
    {
        s_UnusedUIDs.push_back(m_nUID);
        s_ReferencedUIDs.erase(m_nUID);
    }
    s_ReferencedUIDs[nTmp] = 1u;
    m_nUID = nTmp;
    m_strName = "UID_"+std::to_string(m_nUID);
    
    s_Access.releaseLock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _UID CUID instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CUID& _UID)
{
    METHOD_ENTRY("CUID::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    /// \todo Stream static members (queue and uid counter)
    _is >> _UID.m_nUID;
    _is >> _UID.s_nUID;
    _is >> _UID.m_strName;
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _UID CUID instance to stream
///
/// \return Stream with game state information of CUID instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CUID& _UID)
{
    METHOD_ENTRY("CUID::operator<<")
    
    _os << "UID:" << std::endl;
    
    /// \todo Stream static members (queue and uid counter)
    _os << _UID.m_nUID << std::endl;
    _os << _UID.s_nUID << std::endl;
    _os << _UID.m_strName << std::endl;
    
    return _os;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy data from given UID
///
/// \param _UID UID to copy data from
///
///////////////////////////////////////////////////////////////////////////////
void CUID::copy(const CUID& _UID)
{
    METHOD_ENTRY("CUID::copy")
    
    m_nUID = _UID.m_nUID;
    m_strName = _UID.m_strName;
    s_ReferencedUIDs[_UID.m_nUID] += 1u;
}

