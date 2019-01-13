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
#include <deque>
#include <vector>

//--- Program header ---------------------------------------------------------//

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Handle management ensuring unique id's and valid pointers
///
////////////////////////////////////////////////////////////////////////////////
class CHandleManager
{

    public:
        
        //--- Constructor/Destructor -----------------------------------------//
   
        //--- Operators ------------------------------------------------------//
   
        //--- Constant Methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        
    private:
        
        //--- Variables [protected] ------------------------------------------//
        std::vector<void*>  m_vecHandles;
        std::deque<int>     m_vecHandlesFree;
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // HANDLE_MANAGER_H
