////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2016 Torsten Büschenfeld
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
/// \file       visuals_data_storage_user.h
/// \brief      Prototype of interface "IVisualsDataStorageUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-28
///
////////////////////////////////////////////////////////////////////////////////

#ifndef VISUALS_DATA_STORAGE_USER_H
#define VISUALS_DATA_STORAGE_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals_data_storage.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that use the visuals data storage
///
////////////////////////////////////////////////////////////////////////////////
class IVisualsDataStorageUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        IVisualsDataStorageUser() : m_pVisualsDataStorage(nullptr){}

        //--- Methods --------------------------------------------------------//
        void setVisualsDataStorage(CVisualsDataStorage*);
        
    protected:
        
        CVisualsDataStorage*  m_pVisualsDataStorage;     ///< Pointer to storage for visuals data
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the instance of the visuals data storage.
///
/// \param _pDataStorage Instance of visuals data storage.
///
////////////////////////////////////////////////////////////////////////////////
inline void IVisualsDataStorageUser::setVisualsDataStorage(CVisualsDataStorage* _pDataStorage)
{
    METHOD_ENTRY("IVisualsDataStorageUser::setVisualsDataStorage")
    
    if (m_pVisualsDataStorage != nullptr)
    {
        NOTICE_MSG("Visuals Data Storage User", "Data storage instance already given, overwriting.")
    }
    
    m_pVisualsDataStorage = _pDataStorage;
}

#endif // VISUALS_DATA_STORAGE_USER_H
