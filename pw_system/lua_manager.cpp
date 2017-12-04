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
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CLuaManager::CLuaManager() : IComInterfaceProvider(),
                             IThreadModule(),
                             m_strScript(""),
                             m_bPaused(false)
{
    METHOD_ENTRY("CLuaManager::CLuaManager")
    CTOR_CALL("CLuaManager::CLuaManager")
    
    #ifdef PW_MULTITHREADING
        m_strModuleName = "Lua Manager";
    #endif
}

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
    METHOD_ENTRY("CLuaManager::init")
    
    // For Lua, calls are registered using the com interface. Functions could 
    // be directly registered by using Function.second->getFunction() with the
    // apropriate casting. This would result in direct access without using
    // the com interface after registration. But it would also disable the
    // possibility to register callbacks, since the com interface is bypassed.
    
    sol::table TablePW = m_LuaState.create_named_table(LUA_PACKAGE_PREFIX);
    for (const auto& Dom : *m_pComInterface->getDomains())
    {
        TablePW[Dom] = m_LuaState.create_table();
    }
    
    for (auto Function : *m_pComInterface->getFunctions())
    {
        std::string strDomain((*m_pComInterface->getDomainsByFunction())[Function.first]);
        
        switch (Function.second->getSignature())
        {
            case SignatureType::BOOL_INT:
            {   
                std::function<bool(int)> Func =
                    [=](const int _nN) -> bool {return m_pComInterface->call<bool, int>(Function.first, _nN);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::INT:
            {   
                std::function<int()> Func =
                    [=]() -> int {return m_pComInterface->call<int>(Function.first);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::INT_INT:
            {
                std::function<int(int)> Func =
                    [=](const int _nN) -> int {return m_pComInterface->call<int, int>(Function.first, _nN);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::INT_STRING:
            {  
                std::function<int(std::string)> Func =
                    [=](const std::string& _strS) -> int {return m_pComInterface->call<int, std::string>(Function.first, _strS);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                    
                break;
            }   
            case SignatureType::DOUBLE:
            {   
                std::function<double()> Func =
                    [=]() -> double {return m_pComInterface->call<double>(Function.first);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                    
                break;
            }   
            case SignatureType::DOUBLE_INT:
            {   
                std::function<double(int)> Func =
                    [=](const int _nN) -> double {return m_pComInterface->call<double, int>(Function.first, _nN);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::DOUBLE_STRING:
            {   
                std::function<double(std::string)> Func =
                    [=](const std::string& _strS) -> double {return m_pComInterface->call<double, std::string>(Function.first, _strS);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::DOUBLE_STRING_DOUBLE:
            {   
                std::function<double(std::string, double)> Func = 
                    [=](const std::string& _strS, const double& _fD) -> double
                    {return m_pComInterface->call<double, std::string, double>(Function.first, _strS, _fD);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE:
            {   
                std::function<void()> Func =
                    [=]() {m_pComInterface->call<void>(Function.first);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_BOOL:
            {   
                std::function<void(bool)> Func =
                    [=](const bool _bB) {m_pComInterface->call<void, bool>(Function.first, _bB);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_DOUBLE:
            {   
                std::function<void(double)> Func =
                    [=](const double& _fD) {m_pComInterface->call<void, double>(Function.first, _fD);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_2DOUBLE:
            {   
                std::function<void(double, double)> Func =
                    [=](const double& _f1, const double& _f2) {m_pComInterface->call<void, double, double>(Function.first, _f1, _f2);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_INT:
            {   
                std::function<void(int)> Func =
                    [=](const int _nN) {m_pComInterface->call<void, int>(Function.first, _nN);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_2INT:
            {   
                std::function<void(int, int)> Func =
                    [=](const int _n1, const int _n2) {m_pComInterface->call<void, int, int>(Function.first, _n1, _n2);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_3INT:
            {   
                std::function<void(int, int, int)> Func =
                    [=](const int _n1, const int _n2, const int _n3) {m_pComInterface->call<void, int, int>(Function.first, _n1, _n2, _n3);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_INT_DOUBLE:
            {   
                std::function<void(int, double)> Func =
                    [=](const int _n1, const double& _f1) {m_pComInterface->call<void, int, double>(Function.first, _n1, _f1);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_INT_2DOUBLE:
            {   
                std::function<void(int, double, double)> Func =
                    [=](const int _n1, const double& _f1, const double& _f2) {m_pComInterface->call<void, int, double, double>(Function.first, _n1, _f1, _f2);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }  
            case SignatureType::NONE_INT_4DOUBLE:
            {   
                std::function<void(int, double, double, double, double)> Func =
                    [=](const int _n1, const double& _f1, const double& _f2, const double& _f3, const double& _f4)
                        {m_pComInterface->call<void, int, double, double, double, double>(Function.first, _n1, _f1, _f2, _f3, _f4);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_INT_DYN_ARRAY:
            {   
                std::function<void(int, sol::table)> Func =
                    [=](const int _n1, const sol::table& _T)
                    {
                        std::vector<double> vecTable(_T.size());
                        for (auto i = 1u; i <= _T.size(); ++i)
                        {
                            vecTable[i-1] = _T[i];
                        }
                        m_pComInterface->call<void, int, std::vector<double>>(Function.first, _n1, vecTable);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_INT_STRING:
            {   
                std::function<void(int, std::string)> Func =
                    [=](const int _n1, const std::string& _str1) {m_pComInterface->call<void, int, std::string>(Function.first, _n1, _str1);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_STRING:
            {   
                std::function<void(std::string)> Func =
                    [=](const std::string& _str1) {m_pComInterface->call<void, std::string>(Function.first, _str1);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_2STRING:
            {   
                std::function<void(std::string, std::string)> Func =
                    [=](const std::string& _str1, const std::string& _str2)
                        {m_pComInterface->call<void, std::string, std::string>(Function.first, _str1, _str2);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }
            case SignatureType::NONE_4STRING:
            {   
                std::function<void(std::string, std::string, std::string, std::string)> Func =
                    [=](const std::string& _str1, const std::string& _str2, const std::string& _str3, const std::string& _str4)
                        {m_pComInterface->call<void, std::string, std::string, std::string, std::string>(Function.first, _str1, _str2, _str3, _str4);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }
            case SignatureType::NONE_STRING_INT:
            {   
                std::function<void(std::string, int)> Func =
                    [=](const std::string& _str1, const int _n1) {m_pComInterface->call<void, std::string, int>(Function.first, _str1, _n1);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_STRING_2INT:
            {   
                std::function<void(std::string, int, int)> Func =
                    [=](const std::string& _str1, const int _n1, const int _n2)
                        {m_pComInterface->call<void, std::string, int, int>(Function.first, _str1, _n1, _n2);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::NONE_STRING_DOUBLE:
            {   
                std::function<void(std::string, double)> Func =
                    [=](const std::string& _str1, const double& _f1) {m_pComInterface->call<void, std::string, double>(Function.first, _str1, _f1);};
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::VEC2DDOUBLE:
            {   
                std::function<std::tuple<double, double>()> Func =
                    [=]() -> std::tuple<double,double>
                    {
                        Vector2d vecV = m_pComInterface->call<Vector2d>(Function.first);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::VEC2DDOUBLE_INT:
            {   
                std::function<std::tuple<double, double>(int)> Func =
                    [=](const int _n1) -> std::tuple<double,double>
                    {
                        Vector2d vecV = m_pComInterface->call<Vector2d, int>(Function.first, _n1);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::VEC2DDOUBLE_2INT:
            {   
                std::function<std::tuple<double, double>(int, int)> Func =
                    [=](const int _n1, const int _n2) -> std::tuple<double,double>
                    {
                        Vector2d vecV = m_pComInterface->call<Vector2d, int, int>(Function.first, _n1, _n2);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::VEC2DDOUBLE_STRING:
            {   
                std::function<std::tuple<double, double>(std::string)> Func =
                    [=](const std::string& _str1) -> std::tuple<double,double>
                    {
                        Vector2d vecV = m_pComInterface->call<Vector2d, std::string>(Function.first, _str1);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            case SignatureType::VEC2DDOUBLE_2STRING:
            {   
                std::function<std::tuple<double, double>(std::string, std::string)> Func =
                    [=](const std::string& _str1, const std::string& _str2) -> std::tuple<double,double>
                    {
                        Vector2d vecV = m_pComInterface->call<Vector2d, std::string, std::string>(Function.first, _str1, _str2);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }
            case SignatureType::VEC2DINT:
            {   
                std::function<std::tuple<int, int>()> Func =
                    [=]() -> std::tuple<int, int>
                    {
                        Vector2i vecV = m_pComInterface->call<Vector2i>(Function.first);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }
            case SignatureType::VEC2DINT_INT:
            {   
                std::function<std::tuple<int, int>(int)> Func =
                    [=](const int _n1) -> std::tuple<int, int>
                    {
                        Vector2i vecV = m_pComInterface->call<Vector2i, int>(Function.first, _n1);
                        return std::tie(vecV[0],vecV[1]);
                    };
                TablePW[strDomain.c_str()][Function.first.c_str()] = Func;
                break;
            }   
            default:
                DOM_DEV(NOTICE_MSG("Lua Manager", "Wrapper for " << Function.first << "'s signature not implemented."))
                break;
        }
    }
    DOM_VAR(DEBUG_BLK(
        for (const auto& TablePWEntry : TablePW)
        {
            std::cout << TablePWEntry.first.as<std::string>() << std::endl;
            for (const auto& TableDOM : TablePWEntry.second.as<sol::table>())
            {
                std::cout << "--" << TableDOM.first.as<std::string>() << std::endl;
            }
        }
    ))
    m_LuaState.open_libraries(sol::lib::base,
                              sol::lib::package,
                              sol::lib::coroutine,
                              sol::lib::string,
                              sol::lib::os,
                              sol::lib::math,
                              sol::lib::table,
                              //sol::lib::debug,
                              //sol::lib::bit32,
                              sol::lib::io
                              //sol::lib::ffi,
                              //sol::lib::jit
    );

    if (!m_strScript.empty())
    {
        try
        {
            m_LuaState.script_file(m_strScript);
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

    if (!m_bPaused)
    {
        m_pComInterface->call<void>("e_lua_update");
    }
    m_pComInterface->callWriters("lua");
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
        // Events
        m_pComInterface->registerEvent("e_lua_update",
                                       "Update event of the lua main loop",
                                       {{ParameterType::NONE, "No return value"}},
                                       "system");
        
        // Callback to physics pause
        std::function<void(void)> FuncPause =
        [&]()
        {
            m_bPaused = true;
        };
        m_pComInterface->registerCallback("pause", FuncPause, "lua");
        
        // Callback to physics resume
        std::function<void(void)> FuncResume =
        [&]()
        {
            m_bPaused = false;
        };
        m_pComInterface->registerCallback("resume", FuncResume, "lua");
        
        // Callback to physics toggle pause
        std::function<void(void)> FuncTogglePause =
        [&]()
        {
            m_bPaused ^= 1;
        };
        m_pComInterface->registerCallback("toggle_pause", FuncTogglePause, "lua");
        
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
        m_pComInterface->registerFunction("register_lua_callback",
                                          CCommand<void, std::string, std::string>([&](const std::string& _strFunc,
                                                                                       const std::string& _strCallback)
                                          {
                                              this->registerCallback(_strFunc, _strCallback, "lua");
                                          }),
                                          "Register a Lua function as  callback.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::STRING, "Name of function to attach callback to"},
                                           {ParameterType::STRING, "Name of callback function"}},
                                           "system", "lua");
                                          // Callback registration has to be queued by com interface. Hence,
                                          // a "register_callback" command has to be implemented
        m_pComInterface->registerFunction("set_frequency_lua",
                                          CCommand<void, double>([&](const double& _fFrequency)
                                          {
                                              this->setFrequency(_fFrequency);
                                          }),
                                          "Sets the frequency of the Lua thread.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Frequency"}},
                                           "system", "lua");
    }
    else
    {
        WARNING_MSG("Lua Manager", "Com interface not set, cannot register functions.")
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register a Lua function as callback
///
/// This method takes a Lua function and wraps it in a \ref CCommand, using
/// a std::function. This command is then registered at the \ref CComInterface.
///
/// \note Callbacks do not have any return value, they just inherit the
///       parameters from function/event they are hooked on.
///
/// \param _strFunc Registered function/event the callback is listening to
/// \param _strCallback Callback function to be registered
/// \param _strWriterDomain Domain for callbacks that write data and are
///                         therefore queued for thread safety
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CLuaManager::registerCallback(const std::string& _strFunc,
                                   const std::string& _strCallback,
                                   const std::string& _strWriterDomain)
{
    METHOD_ENTRY("CLuaManager::registerCallback")
    
    const auto ci = m_pComInterface->getFunctions()->find(_strFunc);
    if (ci != m_pComInterface->getFunctions()->end())
    {
        switch (ci->second->getSignature())
        {
            case SignatureType::DOUBLE:
            case SignatureType::INT:
            case SignatureType::NONE:
            case SignatureType::VEC2DDOUBLE:
            {
                std::function<void(void)> Func = [=]() {m_LuaState[_strCallback]();};
                m_pComInterface->registerCallback<void>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::BOOL_INT:
            case SignatureType::DOUBLE_INT:
            case SignatureType::INT_INT:
            case SignatureType::NONE_INT:
            case SignatureType::VEC2DDOUBLE_INT:
            case SignatureType::VEC2DINT_INT:
            {
                std::function<void(int)> Func = [=](const int _nN){m_LuaState[_strCallback](_nN);};
                m_pComInterface->registerCallback<void, int>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::DOUBLE_STRING:
            case SignatureType::INT_STRING:
            case SignatureType::NONE_STRING:
            case SignatureType::VEC2DDOUBLE_STRING:
            {
                std::function<void(std::string)> Func = [=](const std::string& _strS){m_LuaState[_strCallback](_strS);};
                m_pComInterface->registerCallback<void, std::string>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::DOUBLE_STRING_DOUBLE:
            case SignatureType::NONE_STRING_DOUBLE:
            {
                std::function<void(std::string, double)> Func = [=](const std::string& _strS, const double& _fD){m_LuaState[_strCallback](_strS, _fD);};
                m_pComInterface->registerCallback<void, std::string, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_BOOL:
            {
                std::function<void(bool)> Func = [=](const bool _bB){m_LuaState[_strCallback](_bB);};
                m_pComInterface->registerCallback<void, bool>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_DOUBLE:
            {
                std::function<void(double)> Func = [=](const double& _fD){m_LuaState[_strCallback](_fD);};
                m_pComInterface->registerCallback<void, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_2DOUBLE:
            {
                std::function<void(double, double)> Func = [=](const double& _f1, const double& _f2){m_LuaState[_strCallback](_f1, _f2);};
                m_pComInterface->registerCallback<void, double, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_2INT:
            case SignatureType::VEC2DDOUBLE_2INT:
            {
                std::function<void(int, int)> Func = [=](const int _n1, const int _n2){m_LuaState[_strCallback](_n1, _n2);};
                m_pComInterface->registerCallback<void, int, int>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_3INT:
            {
                std::function<void(int, int, int)> Func = [=](const int _n1, const int _n2, const int _n3){m_LuaState[_strCallback](_n1, _n2, _n3);};
                m_pComInterface->registerCallback<void, int, int, int>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_INT_DOUBLE:
            {
                std::function<void(int, double)> Func = [=](const int _nN, const double& _fD){m_LuaState[_strCallback](_nN, _fD);};
                m_pComInterface->registerCallback<void, int, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_INT_2DOUBLE:
            {
                std::function<void(int, double, double)> Func = [=](const int _nN, const double& _f1, const double& _f2){m_LuaState[_strCallback](_nN, _f1, _f2);};
                m_pComInterface->registerCallback<void, int, double, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_INT_4DOUBLE:
            {
                std::function<void(int, double, double, double, double)> Func = [=](const int _nN, const double& _f1, const double& _f2,
                                                                                                   const double& _f3, const double& _f4)
                {m_LuaState[_strCallback](_nN, _f1, _f2, _f3, _f4);};
                m_pComInterface->registerCallback<void, int, double, double, double, double>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_INT_DYN_ARRAY:
            {
                std::function<void(int, std::vector<double>)> Func = [=](const int _nN, const std::vector<double>& _vecV)
                {m_LuaState[_strCallback](_nN, _vecV);};
                m_pComInterface->registerCallback<void, int, std::vector<double>>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_INT_STRING:
            {
                std::function<void(int, std::string)> Func = [=](const int _nN, const std::string& _strS)
                {m_LuaState[_strCallback](_nN, _strS);};
                m_pComInterface->registerCallback<void, int, std::string>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_2STRING:
            {
                std::function<void(std::string, std::string)> Func = [=](const std::string& _str1, const std::string& _str2)
                    {m_LuaState[_strCallback](_str1, _str2);};
                m_pComInterface->registerCallback<void, std::string>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_4STRING:
            {
                std::function<void(std::string, std::string, std::string, std::string)> Func = 
                    [=](const std::string& _str1, const std::string& _str2, const std::string& _str3, const std::string& _str4)
                        {m_LuaState[_strCallback](_str1, _str2, _str3, _str4);};
                m_pComInterface->registerCallback<void, std::string>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_STRING_INT:
            {
                std::function<void(std::string, int)> Func = [=](const std::string& _strS, const int _nN)
                {m_LuaState[_strCallback](_strS, _nN);};
                m_pComInterface->registerCallback<void, std::string, int>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::NONE_STRING_2INT:
            {
                std::function<void(std::string, int, int)> Func = [=](const std::string& _strS, const int _n1, const int _n2)
                {m_LuaState[_strCallback](_strS, _n1, _n2);};
                m_pComInterface->registerCallback<void, std::string, int, int>(_strFunc, Func, _strWriterDomain);
                break;
            }
            case SignatureType::VEC2DDOUBLE_2STRING:
            {
                std::function<void(std::string, std::string)> Func = [=](const std::string& _str1, const std::string& _str2)
                {m_LuaState[_strCallback](_str1, _str2);};
                m_pComInterface->registerCallback<void, std::string, std::string>(_strFunc, Func, _strWriterDomain);
                break;
            }
            default:
                DOM_DEV(NOTICE_MSG("Lua Manager", "Wrapper for " << ci->first << "'s signature not implemented."))
                break;
        }
    }
    else
    {
        WARNING_MSG("Lua Manager", "Can't register callback on <" << _strFunc << ">, function unknown.")
        return false;
    }
    
    return true;
}
