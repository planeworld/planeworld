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
/// \file       unique_id.cpp
/// \brief      Implementation of class "CUniqueID"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-24
///
////////////////////////////////////////////////////////////////////////////////

#include "unique_id.h"

/// Global counter for unique IDs.
UniqueIDType CUniqueID::s_nID = 0;

/// Global list for unused unique IDs
std::deque<UniqueIDType> CUniqueID::s_UnusedIDs;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
/// When the constructor is called, the specific ID is assigned. If there are
/// any unused IDs, these will be used first.
///
///////////////////////////////////////////////////////////////////////////////
CUniqueID::CUniqueID()
{
    METHOD_ENTRY("CUniqueID::CUniqueID")
    DTOR_CALL("CUniqueID::CUniqueID")
    
    if (s_UnusedIDs.empty())
        m_nID = s_nID++;
    else
    {
        m_nID = s_UnusedIDs.front();
        s_UnusedIDs.pop_front();
    }
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
CUniqueID::~CUniqueID()
{
    METHOD_ENTRY("CUniqueID::~CUniqueID")
    DTOR_CALL("CUniqueID::~CUniqueID")
    
    s_UnusedIDs.push_back(m_nID);
}
