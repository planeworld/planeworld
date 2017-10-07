////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2017 Torsten Büschenfeld
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
/// \file       uid_user.h
/// \brief      Prototype of interface "IUIDUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-28
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UID_USER_H
#define UID_USER_H

//--- Standard header --------------------------------------------------------//
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "uid.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that use a engine wide unique id.
///
////////////////////////////////////////////////////////////////////////////////
class IUIDUser
{

    public:
   
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
              UIDType       getUID() const;
        
        //--- Methods --------------------------------------------------------//
        void setName(const std::string&);
        void setNewID();
        
    protected:
        
        //--- Protected variables --------------------------------------------//
        CUID       m_UID; ///< Identifier
};

typedef std::unordered_map<UIDType, IUIDUser*> UIDUserType; ///< Stores entities by UID

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the entity's name
///
/// \return Name of entity
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& IUIDUser::getName() const
{
    METHOD_ENTRY("IUIDUser::getName")
    return m_UID.getName();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the objects unique id
///
/// \return Unique id of object
///
////////////////////////////////////////////////////////////////////////////////
inline UIDType IUIDUser::getUID() const
{
    METHOD_ENTRY("IUIDUser::getUID")
    return m_UID.getValue();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the entities name
///
/// \param _strName Name to be set for entity
///
////////////////////////////////////////////////////////////////////////////////
inline void IUIDUser::setName(const std::string& _strName)
{
    METHOD_ENTRY("IUIDUser::setName")
    m_UID.setName(_strName);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a new ID
///
////////////////////////////////////////////////////////////////////////////////
inline void IUIDUser::setNewID()
{
    METHOD_ENTRY("IUIDUser::setNewID")
    m_UID.setNewID();
}

#endif // UID_USER_H
