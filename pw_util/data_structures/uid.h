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
/// \file       uid.h
/// \brief      Prototype of class "CUID"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UID_H
#define UID_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <deque>
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "spinlock.h"
#include "serializable.h"

using UIDType = std::uint32_t;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that manages a unique ID to identify objects etc.
///
////////////////////////////////////////////////////////////////////////////////
class CUID : public ISerializable
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CUID();
        CUID(const CUID&);
        ~CUID();
        
        CUID& operator=(const CUID&);
        
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
        const UIDType&      getValue() const;
        
        //--- Methods --------------------------------------------------------//
        void setName(const std::string&);
        void setNewID();
        
        //--- Static methods -------------------------------------------------//
        static const std::deque<UIDType>& getUnusedUIDs();
        static const std::unordered_map<UIDType, std::uint32_t>& getReferencedUIDs();
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        void copy(const CUID&);
        
        //--- Variables [private] --------------------------------------------//
               UIDType             m_nUID;              ///< Unique ID for this instance
               std::string         m_strName;           ///< Name for this instance
               
        static UIDType             s_nUID;              ///< Unique ID counter
        static std::deque<UIDType> s_UnusedUIDs;        ///< Storage for unused / released IDs
        static std::unordered_map<UIDType, std::uint32_t> s_ReferencedUIDs; ///< Storage for reference counting of uids
        
        static CSpinlock           s_Access;            ///< Global lock
        
        SERIALIZE_DECL
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name as identifier
///
/// \return Name as identifier
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CUID::getName() const
{
    METHOD_ENTRY("CUID::getName")
    return m_strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the unique ID value
///
/// \return Unique ID value
///
////////////////////////////////////////////////////////////////////////////////
inline const UIDType& CUID::getValue() const
{
    METHOD_ENTRY("CUID::getValue")
    return m_nUID;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a name as identifier
///
/// \param _strName Name to be set as identifier
///
////////////////////////////////////////////////////////////////////////////////
inline void CUID::setName(const std::string& _strName)
{
    METHOD_ENTRY("CUID::setName")
    m_strName = _strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns all unused UIDs
///
/// \return Unused UIDs
///
////////////////////////////////////////////////////////////////////////////////
inline const std::deque<UIDType>& CUID::getUnusedUIDs()
{
    METHOD_ENTRY("CUID::getUnusedUIDs")
    return s_UnusedUIDs;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns all referenced UIDs
///
/// UIDs are reference counted due to copy constructor and copy assignment
/// operator duplication of UIDs.
///
/// \return Referenced UIDs
///
////////////////////////////////////////////////////////////////////////////////
inline const std::unordered_map<UIDType, std::uint32_t>& CUID::getReferencedUIDs()
{
    METHOD_ENTRY("CUID::getReferencedUIDs")
    return s_ReferencedUIDs;
}

#endif // CUID
