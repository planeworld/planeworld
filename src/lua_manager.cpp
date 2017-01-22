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
CLuaManager::CLuaManager() : m_fFrequency(LUA_DEFAULT_FREQUENCY)
{
    METHOD_ENTRY("CLuaManager::CLuaManager")
    CTOR_CALL("CLuaManager::CLuaManager")
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
    METHOD_ENTRY("CPhysicsManager::init")
    
    for (auto Function : *m_pComInterface->getFunctions())
    {
        std::string strDomain((*m_pComInterface->getDomains())[Function.first]);
        
        if ((*m_pComInterface->getWriterFlags())[Function.first])
        {
            switch (Function.second->getSignature())
            {
                case SignatureType::INT:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING_DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<double,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_BOOL:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,bool>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_2DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_4DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] =
                                static_cast<CCommandWritable<void,std::string,double, double, double, double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommandWritable<void,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::VEC2DDOUBLE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommandWritable<Vector2d,std::string>*>(Function.second)->call(_strS);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS1, const std::string& _strS2) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommandWritable<Vector2d,std::string,std::string>*>(Function.second)->call(_strS1,_strS2);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                default:
                    NOTICE_MSG("Lua Manager", "Wrapper for " << Function.first << "'s signature not implemented.")
            }
        }
        else
        {
            switch (Function.second->getSignature())
            {
                case SignatureType::INT:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::DOUBLE_STRING_DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<double,std::string,double>*>(Function.second)->getFunction();
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
                case SignatureType::NONE_2DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,double,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_INT:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,int>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_4DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] =
                                static_cast<CCommand<void,std::string,double, double, double, double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::NONE_STRING_DOUBLE:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = static_cast<CCommand<void,std::string,double>*>(Function.second)->getFunction();
                    break;
                case SignatureType::VEC2DDOUBLE_STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommand<Vector2d,std::string>*>(Function.second)->call(_strS);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                case SignatureType::VEC2DDOUBLE_2STRING:
                    m_LuaState[LUA_PACKAGE_PREFIX][strDomain.c_str()][Function.first.c_str()] = [=](const std::string& _strS1, const std::string& _strS2) -> std::tuple<double,double>
                    {
                        Vector2d vecVel = static_cast<CCommand<Vector2d,std::string,std::string>*>(Function.second)->call(_strS1,_strS2);
                        return std::tie(vecVel[0],vecVel[1]);
                    };
                    break;
                default:
                    NOTICE_MSG("Lua Manager", "Wrapper for " << Function.first << "'s signature not implemented.")
            }
        }
    }
    
    if (!m_LuaState.Load(m_strPhysicsInterface))
    {
        ERROR_MSG("Lua Manager", "Cannot load lua file " << m_strPhysicsInterface << ".")
        return false;
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

    m_LuaState["physics_interface"]();
    m_pComInterface->callWriters("lua");
}

#ifdef PW_MULTITHREADING
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Runs the lua engine, called as a thread.
  ///
  ///////////////////////////////////////////////////////////////////////////////
  void CLuaManager::run()
  {
      METHOD_ENTRY("CLuaManager::run")
      
      INFO_MSG("Lua Manager", "Lua thread started.")
      m_bRunning = true;
      
      CTimer LuaTimer;
      
      LuaTimer.start();
      while (m_bRunning)
      {
          this->processFrame();
          double fTimeSlept = LuaTimer.sleepRemaining(m_fFrequency);
          
          if (fTimeSlept < 0.0)
          {
              NOTICE_MSG("Lua Manager", "Execution time of Lua code is too large: " << 1.0/m_fFrequency - fTimeSlept << 
                                          "s of " << 1.0/m_fFrequency << "s max.")
          }
      }
      INFO_MSG("Lua Manager", "Lua thread stopped.")
  }
#endif

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
    }
    else
    {
        WARNING_MSG("Lua Manager", "Com interface not set, cannot register functions.")
    }
}
