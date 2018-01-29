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
/// \file       lua_manager.h
/// \brief      Prototype of class "CLuaManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

//--- Program header ---------------------------------------------------------//
#include "com_interface_provider.h"
#include "thread_module.h"

//--- Standard header --------------------------------------------------------//
#include <array>
#include <sstream>

//--- Misc. header -----------------------------------------------------------//
#define SOL_CHECK_ARGUMENTS
#include "sol.hpp"

// Constants
const std::string LUA_PACKAGE_PREFIX{"pw"};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class to handling Lua scripting
///
/// This class manages initialisation and running of Lua scripts. It provides
/// access to game entities and much more
///
////////////////////////////////////////////////////////////////////////////////
class CLuaManager : public IComInterfaceProvider,
                    public IThreadModule
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CLuaManager();

        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        bool init();
        bool processFrame();
        void setScript(const std::string&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CLuaManager&);
        friend std::ostream&    operator<<(std::ostream&, CLuaManager&);

    private:
        
        //--- Methods [private] ----------------------------------------------//
        void myInitComInterface();
        bool registerCallback(const std::string&,
                              const std::string&,
                              const std::string&);

        //--- Variables [private] --------------------------------------------//
        sol::state      m_LuaState;             ///< Current lua state
        
        std::string     m_strScript;            ///< Path and filename of main script
        bool            m_bPaused;              ///< Indicates if processing is paused, depends on physics
        
        CTimer          m_TimeProcessed;        ///< Counts processing time for one Lua frame
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set path and filename for Lua main script
///
/// \param _strScript Path and filename for Lua main script
///
////////////////////////////////////////////////////////////////////////////////
inline void CLuaManager::setScript(const std::string& _strScript)
{
    METHOD_ENTRY("CLuaManager::setScript")
    m_strScript = _strScript;
}

#endif // LUA_MANAGER_H
