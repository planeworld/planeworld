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
/// \file       lua_manager.cpp
/// \brief      Implementation of class "CLuaManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-12-11
///
////////////////////////////////////////////////////////////////////////////////

#include "lua_manager.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise Lua scripting engine
///
/// Lua uses the com interface of planeworld. Therefore, all functions
/// registered at the com interface will be registered for Lua. 
///
/// \return Initialisation succesful?
///
///////////////////////////////////////////////////////////////////////////////
bool CLuaManager::init()
{
    METHOD_ENTRY("CPhysicsManager::init")
    
    for (auto Function : *m_pComInterface->getFunctions())
    {
        std::string strDomain((*m_pComInterface->getDomains())[Function.first]);
        
        switch ((*m_pComInterface->getSignatures())[Function.first])
        {
            case SignatureType::INT:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<int>*>(Function.second)->getFunction();
                break;
            case SignatureType::DOUBLE:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double>*>(Function.second)->getFunction();
                break;
            case SignatureType::NONE:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void>*>(Function.second)->getFunction();
                break;
            case SignatureType::NONE_BOOL:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,bool>*>(Function.second)->getFunction();
                break;
            case SignatureType::NONE_DOUBLE:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,double>*>(Function.second)->getFunction();
                break;
            case SignatureType::NONE_INT:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int>*>(Function.second)->getFunction();
                break;
            case SignatureType::NONE_STRING_DOUBLE:
                m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string,double>*>(Function.second)->getFunction();
                break;
        }
    }
    return true;
}

void CLuaManager::test()
{
    if (!m_LuaState.Load("test.lua"))
    {
        ERROR_MSG("Lua Manager", "Cannot load lua file.")
    };
}
