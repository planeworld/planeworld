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
#include "selene.h"

// Constants
const char   LUA_PACKAGE_PREFIX[2] = {'p','w'};

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
        void processFrame();
        void setPhysicsInterface(const std::string&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CLuaManager&);
        friend std::ostream&    operator<<(std::ostream&, CLuaManager&);

    private:
        
        //--- Methods [private] ----------------------------------------------//
        void myInitComInterface();

        //--- Variables [private] --------------------------------------------//
        sel::State  m_LuaState{true}; ///< Current lua state
        
        std::string     m_strPhysicsInterface; ///< Path and filename of physics interface
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
inline CLuaManager::CLuaManager()
{
    METHOD_ENTRY("CLuaManager::CLuaManager")
    CTOR_CALL("CLuaManager::CLuaManager")

    #ifdef PW_MULTITHREADING
        // Somehow, there is a problem with Selene and the module name if 
        // constructor is not inline. The error is pretty weird, lua 
        // registration does not recognize packages (nested tables).
        // The error only occurs when compiled in DEBUG mode, though it 
        // does not if DEBUG and CTOR_CALL is enabled.
        m_strModuleName = "Lua Manager";
    #endif
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set path and filename for Lua physics interface
///
/// \param _strPhysicsInterface Path and filename for Lua physics interface
///
////////////////////////////////////////////////////////////////////////////////
inline void CLuaManager::setPhysicsInterface(const std::string& _strPhysicsInterface)
{
    METHOD_ENTRY("CLuaManager::setPhysicsInterface")
    m_strPhysicsInterface = _strPhysicsInterface;
}

#endif // LUA_MANAGER_H
