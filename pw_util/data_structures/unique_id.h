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
/// \file       unique_id.h
/// \brief      Prototype of class "CUniqueID"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <deque>

//--- Program header ---------------------------------------------------------//
#include "log.h"

using UIDType = std::uint32_t;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that manages a unique ID to identify objects etc.
///
////////////////////////////////////////////////////////////////////////////////
class CUniqueID
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CUniqueID();
        CUniqueID(const CUniqueID&);
        ~CUniqueID();
        
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
        const UIDType&      getValue() const;
        
        //--- Methods --------------------------------------------------------//
        void setName(const std::string&);
                
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CUniqueID&);
        friend std::ostream&    operator<<(std::ostream&, CUniqueID&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
               UIDType             m_nUID;              ///< Unique ID for this instance
               std::string         m_strName;           ///< Name for this instance
               
        static UIDType             s_nUID;              ///< Unique ID counter
        static std::deque<UIDType> s_UnusedUIDs;        ///< Storage for unused / released IDs
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name as identifier
///
/// \return Name as identifier
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CUniqueID::getName() const
{
    METHOD_ENTRY("CUniqueID::getName")
    return m_strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the unique ID value
///
/// \return Unique ID value
///
////////////////////////////////////////////////////////////////////////////////
inline const UIDType& CUniqueID::getValue() const
{
    METHOD_ENTRY("CUniqueID::getValue")
    return m_nUID;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a name as identifier
///
/// \param _strName Name to be set as identifier
///
////////////////////////////////////////////////////////////////////////////////
inline void CUniqueID::setName(const std::string& _strName)
{
    METHOD_ENTRY("CUniqueID::setName")
    m_strName = _strName;
}

#endif // CUniqueID
