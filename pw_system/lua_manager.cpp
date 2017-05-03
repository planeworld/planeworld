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
/// \file       lua_manager.cpp
/// \brief      Implementation of class "CLuaManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-12-11
///
////////////////////////////////////////////////////////////////////////////////

#include "lua_manager.h"

//--- Standard header --------------------------------------------------------//
#include <string>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

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
    
    sol::table TablePW = m_LuaState.create_named_table(LUA_PACKAGE_PREFIX);
    for (const auto& Dom : *m_pComInterface->getDomains())
    {
        TablePW[Dom] = m_LuaState.create_table();
    }
    
    for (auto Function : *m_pComInterface->getFunctions())
    {
        std::string strDomain((*m_pComInterface->getDomainsByFunction())[Function.first]);
        
        if ((*m_pComInterface->getWriterFlags())[Function.first])
        {
            switch (Function.second->getSignature())
            {
                case SignatureType::INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::INT_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<int,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_BOOL:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,bool>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_2DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_3INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_2DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_4DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] =
                                static_cast<CCommandWritable<void,int,double, double, double, double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_DYN_ARRAY:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP, const sol::table& _Table)
                    {
                        auto pFunctionComInt = static_cast<CCommandWritable<void,int,std::vector<double>>*>(Function.second)->getFunction();
                        std::vector<double> vecTable(_Table.size());
                        for (auto i = 1u; i <= _Table.size(); ++i)
                        {
                            vecTable[i-1] = _Table[i];
                        }
                        pFunctionComInt(_nP, vecTable);
                    };
                    break;
                case SignatureType::NONE_INT_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [&]() -> std::string
                    {
                        std::string strRet = static_cast<CCommandWritable<std::string>*>(Function.second)->call();
                        return strRet;
                    };
                    break;
                case SignatureType::VEC2DDOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=]() -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommandWritable<Vector2d>*>(Function.second)->call();
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommandWritable<Vector2d,int>*>(Function.second)->call(_nP);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP1, const int _nP2) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommandWritable<Vector2d,int,int>*>(Function.second)->call(_nP1,_nP2);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommandWritable<Vector2d,std::string>*>(Function.second)->call(_strS);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS1, const std::string& _strS2) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommandWritable<Vector2d,std::string,std::string>*>(Function.second)->call(_strS1,_strS2);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DINT_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nUID) -> std::tuple<int,int>
                    {
                        Vector2i vecV = static_cast<CCommandWritable<Vector2i,int>*>(Function.second)->call(_nUID);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                default:
                    NOTICE_MSG("Lua Manager", "Wrapper for " << Function.first << "'s signature not implemented.")
                    break;
            }
        }
        else
        {
            switch (Function.second->getSignature())
            {
                case SignatureType::INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::INT_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::INT_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<int,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_BOOL:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,bool>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_2DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_3INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_2DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_4DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] =
                                static_cast<CCommand<void, int, double, double, double, double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT_DYN_ARRAY:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP, const sol::table& _Table)
                    {
                        auto pFunctionComInt = static_cast<CCommand<void,int,std::vector<double>>*>(Function.second)->getFunction();
                        std::vector<double> vecTable(_Table.size());
                        for (auto i = 1u; i <= _Table.size(); ++i)
                        {
                            vecTable[i-1] = _Table[i];
                        }
                        pFunctionComInt(_nP, vecTable);
                    };
                    break;
                case SignatureType::NONE_INT_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string,int,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_DOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::VEC2DDOUBLE:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=]() -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommand<Vector2d>*>(Function.second)->call();
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommand<Vector2d,int>*>(Function.second)->call(_nP);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nP1, const int _nP2) -> std::tuple<double,double>
                    {
                        Vector2d vecV = static_cast<CCommand<Vector2d,int,int>*>(Function.second)->call(_nP1,_nP2);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommand<Vector2d,std::string>*>(Function.second)->call(_strS);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2STRING:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS1, const std::string& _strS2) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommand<Vector2d,std::string,std::string>*>(Function.second)->call(_strS1,_strS2);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                case SignatureType::VEC2DINT_INT:
                    TablePW[strDomain.c_str()][Function.first.c_str()] = [=](const int _nUID) -> std::tuple<int,int>
                    {
                        Vector2i vecV = static_cast<CCommand<Vector2i,int>*>(Function.second)->call(_nUID);
                        return std::tie(vecV[0],vecV[1]);
                    };
                    break;
                default:
                    NOTICE_MSG("Lua Manager", "Wrapper for " << Function.first << "'s signature not implemented.")
                    break;
            }
        }
    }
    DOM_VAR(DEBUG(
        for (const auto& TablePWEntry : TablePW)
        {
            std::cout << TablePWEntry.first.as<std::string>() << std::endl;
            for (const auto& TableDOM : TablePWEntry.second.as<sol::table>())
            {
                std::cout << "--" << TableDOM.first.as<std::string>() << std::endl;
            }
        }
    ))
    m_LuaState.open_libraries(sol::lib::base, sol::lib::package,
                              sol::lib::io,
                              sol::lib::math,
                              sol::lib::os
    );

    if (!m_strPhysicsInterface.empty())
    {
        try
        {
            m_LuaState.script_file(m_strPhysicsInterface);
        }
        catch (const std::exception& _E)
        {
            ERROR_MSG("Lua Manager", _E.what())
            return false;
        }
    }
        
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Processes one single frame
///
////////////////////////////////////////////////////////////////////////////////
void CLuaManager::processFrame()
{
    METHOD_ENTRY("CLuaManager::processFrame")

    if (!m_strPhysicsInterface.empty())
    {
        m_LuaState["physics_interface"]();
        m_pComInterface->callWriters("lua");
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise the command interface
///
///////////////////////////////////////////////////////////////////////////////
void CLuaManager::myInitComInterface()
{
    METHOD_ENTRY("CLuaManager::myInitComInterface")

    INFO_MSG("Lua Manager", "Initialising com interace.")
    if (m_pComInterface != nullptr)
    {
        // System package
        m_pComInterface->registerFunction("get_lua_frequency",
                                          CCommand<double>([&]() -> double {return this->m_fFrequency;}),
                                          "Provides processing frequency of Lua module.",
                                          {{ParameterType::DOUBLE, "Processing frequency of Lua module"}},
                                           "system");
        m_pComInterface->registerFunction("execute_lua",
                                          CCommand<void, std::string>([&](const std::string& _strS)
                                          {
                                              try
                                              {
                                                 m_LuaState.script(_strS.c_str());
                                              }
                                              catch (const std::exception& _E)
                                              {
                                                  ERROR_MSG("Lua Manager", _E.what())
                                              }
                                          }),
                                          "Interpretes and executes given string in Lua.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::STRING, "String to be executed"}},
                                           "system", "lua");
    }
    else
    {
        WARNING_MSG("Lua Manager", "Com interface not set, cannot register functions.")
    }
}
