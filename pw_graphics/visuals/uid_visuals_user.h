////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
/// \file       uid_visuals_user.h
/// \brief      Prototype of class "IUIDVisualsUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-05-28
///
////////////////////////////////////////////////////////////////////////////////
#ifndef UID_VISUALS_USER_H
#define UID_VISUALS_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "uid_visuals.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief User of UID visuals
///
////////////////////////////////////////////////////////////////////////////////
class IUIDVisualsUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        IUIDVisualsUser() : m_pUIDVisuals(nullptr){}
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        void setUIDVisuals(CUIDVisuals*);
        
    protected:
        
        //--- Variables [protected] ------------------------------------------//
        CUIDVisuals* m_pUIDVisuals;     ///< UID visuals
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the instance of the worlds data storage.
///
/// \param _pDataStorage Instance of worlds data storage.
///
////////////////////////////////////////////////////////////////////////////////
inline void IUIDVisualsUser::setUIDVisuals(CUIDVisuals* _pUIDVisuals)
{
    METHOD_ENTRY("IUIDVisualsUser::setUIDVisuals")
    
    if (m_pUIDVisuals != nullptr)
    {
        NOTICE_MSG("UID Visuals User", "Visuals User instance already given, overwriting.")
    }
    
    m_pUIDVisuals = _pUIDVisuals;
}

#endif // UID_VISUALS_USER_H
