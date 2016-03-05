////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2016 Torsten Büschenfeld
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
/// \file       world_data_storage_user.h
/// \brief      Prototype of interface "IWorldDataStorageUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef WORLD_DATA_STORAGE_USER_H
#define WORLD_DATA_STORAGE_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "world_data_storage.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that use the world data storage
///
////////////////////////////////////////////////////////////////////////////////
class IWorldDataStorageUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        IWorldDataStorageUser() : m_pDataStorage(nullptr){}

        //--- Methods --------------------------------------------------------//
        void setWorldDataStorage(CWorldDataStorage*);
        
    protected:
        
        CWorldDataStorage*  m_pDataStorage;     ///< Pointer to storage for worlds data
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the instance of the worlds data storage.
///
/// \param _pDataStorage Instance of worlds data storage.
///
////////////////////////////////////////////////////////////////////////////////
inline void IWorldDataStorageUser::setWorldDataStorage(CWorldDataStorage* _pDataStorage)
{
    METHOD_ENTRY("IWorldDataStorageUser::setWorldDataStorage")
    
    if (m_pDataStorage != nullptr)
    {
        NOTICE_MSG("World Data Storage User", "Data storage instance already given, overwriting.")
    }
    
    m_pDataStorage = _pDataStorage;
}

#endif
