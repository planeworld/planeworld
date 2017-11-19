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
/// \file       com_interface.cpp
/// \brief      Implementation of class "CComInterface"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-10-22
///
////////////////////////////////////////////////////////////////////////////////

#include "com_interface.h"

//--- Standard header --------------------------------------------------------//
#include <sstream>

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return message according to exception type
///
/// \return Message according to exception type
///
////////////////////////////////////////////////////////////////////////////////
const std::string CComInterfaceException::getMessage() const
{
    METHOD_ENTRY_QUIET("CComInterfaceException::getMessage")
    
    switch (m_EType)
    {
        case ComIntExceptionType::INVALID_VALUE:
            return "Invalid parameter value";
        case ComIntExceptionType::UNKNOWN_COMMAND:
            return "Unknown command";
        case ComIntExceptionType::PARAM_ERROR:
            return "Parameter error. Wrong number or unknown value.";
        default:
            return "";
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, registeres its own functions
///
///////////////////////////////////////////////////////////////////////////////
CComInterface::CComInterface() 
{
    METHOD_ENTRY_QUIET("CComInterface::CComInterface")
    CTOR_CALL_QUIET("CComInterface::CComInterface")
    
    Log.addListener("com", this);
    
    this->registerEvent<std::string, std::string, std::string, std::string>
                          ("e_log_entry", "Indicates that a new log entry was made.",
                                    {{ParameterType::STRING,"Source of log entry"},
                                     {ParameterType::STRING,"Log message"},
                                     {ParameterType::STRING,"Log level"},
                                     {ParameterType::STRING,"Log domain"}
                                    },
                                    "system"
    );
    
    this->registerFunction("help",  CCommand<void, int>([&](int nVerboseLevel){this->help(nVerboseLevel);}),
                                    "Show command interface help",
                                    {{ParameterType::NONE,"No return value"},
                                     {ParameterType::INT,"Verbosity (0-1)"}},
                                    "system"
    );
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes callback functions
///
///////////////////////////////////////////////////////////////////////////////
CComInterface::~CComInterface()
{
    METHOD_ENTRY_QUIET("CComInterface::~CComInterface")
    DTOR_CALL_QUIET("CComInterface::~CComInterface")
    
    // Remove the listener, otherwise the already deleted com interface might
    // be called during destruction
    Log.removeListener("com");
    
    // Remove all remaining commands from queue
    for (WriterQueuesType::iterator it = m_WriterQueues.begin();
         it != m_WriterQueues.end(); ++it)
    {
        IBaseCommand* pCommand = nullptr;
        while (it->second.try_dequeue(pCommand))
        {
            if (pCommand != nullptr)
            {
                delete pCommand;
                MEM_FREED_QUIET("IBaseCommand")
                pCommand = nullptr;
            }
        }
    }
    
    for (auto pCallback : m_RegisteredCallbacks)
    {
        if (pCallback.second != nullptr)
        {
            delete pCallback.second;
            pCallback.second = nullptr;
            MEM_FREED_QUIET("IBaseCommand")
        }
    }

    for (auto pFunction : m_RegisteredFunctions)
    {
        if (pFunction.second != nullptr)
        {
            delete pFunction.second;
            pFunction.second = nullptr;
            MEM_FREED_QUIET("IBaseCommand")
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the given function if registered
///
/// This method is a wrapper function to call registered functions without the
/// need to use template parameters. Therefore, it's especially suited for
/// external calls. Internally, direct call should be preferred to avoid
/// the parsing.
///
/// \param _strCommand Command that should be called
/// \return Return value of function as string
///
///////////////////////////////////////////////////////////////////////////////
const std::string CComInterface::call(const std::string& _strCommand)
{
    METHOD_ENTRY_QUIET("CComInterface::call")
    
    std::istringstream iss(_strCommand);
    std::ostringstream oss("");
    std::string strName;
    
    iss >> strName;
    
    if (m_RegisteredFunctions.find(strName) != m_RegisteredFunctions.end())
    {
        switch (m_RegisteredFunctions[strName]->getSignature())
        {
            case SignatureType::BOOL_INT:
            {
                int nParam(0);
                iss >> nParam;
                oss << this->call<bool,int>(strName, nParam);
                break;
            }
            case SignatureType::DOUBLE:
            {
                oss << this->call<double>(strName);
                break;
            }
            case SignatureType::DOUBLE_INT:
            {
                int nParam(0);
                iss >> nParam;
                oss << this->call<double,int>(strName, nParam);
                break;
            }
            case SignatureType::DOUBLE_STRING:
            {
                std::string strParam = "";
                iss >> strParam;
                oss << this->call<double,std::string>(strName, strParam);
                break;
            }
            case SignatureType::DOUBLE_STRING_DOUBLE:
            {
                std::string strParam = "";
                double fParam = 0.0;
                iss >> strParam >> fParam;
                oss << this->call<double,std::string,double>(strName, strParam, fParam);
                break;
            }
            case SignatureType::INT:
            {
                oss << this->call<int>(strName);
                break;
            }
            case SignatureType::INT_INT:
            {
                int nParam = 0;
                iss >> nParam;
                oss << this->call<int,int>(strName, nParam);
                break;
            }
            case SignatureType::INT_STRING:
            {
                std::string strS("");
                iss >> strS;
                oss << this->call<int,std::string>(strName, strS);
                break;
            }
            case SignatureType::NONE:
            {
                this->call<void>(strName);
                break;
            }
            case SignatureType::NONE_BOOL:
            {
                bool bParam = 0;
                iss >> bParam;
                this->call<void,bool>(strName, bParam);
                break;
            }
            case SignatureType::NONE_DOUBLE:
            {
                double fParam = 0.0;
                iss >> fParam;
                this->call<void,double>(strName, fParam);
                break;
            }
            case SignatureType::NONE_2DOUBLE:
            {
                double fParam1 = 0.0;
                double fParam2 = 0.0;
                iss >> fParam1 >> fParam2;
                this->call<void,double>(strName, fParam1, fParam2);
                break;
            }
            case SignatureType::NONE_INT:
            {
                int nParam = 0;
                iss >> nParam;
                this->call<void,int>(strName, nParam);
                break;
            }
            case SignatureType::NONE_2INT:
            {
                int nParam1(0);
                int nParam2(0);
                iss >> nParam1 >> nParam2;
                this->call<void,int,int>(strName, nParam1, nParam2);
                break;
            }
            case SignatureType::NONE_3INT:
            {
                int nParam1(0);
                int nParam2(0);
                int nParam3(0);
                iss >> nParam1 >> nParam2 >> nParam3;
                this->call<void,int,int>(strName, nParam1, nParam2, nParam3);
                break;
            }
            case SignatureType::NONE_INT_DOUBLE:
            {
                int    nParam = 0;
                double fParam = 0.0;
                iss >> nParam >> fParam;
                this->call<void,int,double>(strName, nParam, fParam);
                break;
            }
            case SignatureType::NONE_INT_STRING:
            {
                int         nParam(0);
                std::string strParam("");
                iss >> nParam >> strParam;
                this->call<void,int,std::string>(strName, nParam, strParam);
                break;
            }
            case SignatureType::NONE_STRING:
            {
                std::string strS;
                iss >> strS;
                this->call<void,std::string>(strName, strS);
                break;
            }
            case SignatureType::NONE_2STRING:
            {
                std::string str1{""};
                std::string str2{""};
                iss >> str1 >> str2;
                this->call<void,std::string,std::string>(strName, str1, str2);
                break;
            }
            case SignatureType::NONE_4STRING:
            {
                std::string str1{""};
                std::string str2{""};
                std::string str3{""};
                std::string str4{""};
                iss >> str1 >> str2 >> str3 >> str4;
                this->call<void,std::string,std::string,std::string,std::string>(strName, str1, str2, str3, str4);
                break;
            }
            case SignatureType::NONE_INT_2DOUBLE:
            {
                int nParam(0);
                iss >> nParam;
                double fParam[2] = {0.0, 0.0};
                iss >> fParam[0] >> fParam[1];
                this->call<void, int, double, double>(strName, nParam, fParam[0], fParam[1]);
                break;
            }
            case SignatureType::NONE_INT_4DOUBLE:
            {
                int nParam(0);
                iss >> nParam;
                double fParam[4] = {0.0, 0.0, 0.0, 0.0};
                iss >> fParam[0] >> fParam[1] >> fParam[2] >> fParam[3];
                this->call<void, int, double, double, double, double>(strName, nParam, fParam[0], fParam[1], fParam[2], fParam[3]);
                break;
            }
            case SignatureType::NONE_INT_DYN_ARRAY:
            {
                int nParam(0);
                iss >> nParam;
                double fParam(0);
                std::vector<double> vecDynArray{};
                while (!iss.eof())
                {
                    iss >> fParam;
                    vecDynArray.push_back(fParam);
                }
                this->call<void, int, std::vector<double>>(strName, nParam, vecDynArray);
                break;
            }
            case SignatureType::NONE_STRING_DOUBLE:
            {
                std::string strS("");
                iss >> strS;
                double fParam = 0.0;
                iss >> fParam;
                this->call<void,std::string, double>(strName, strS, fParam);
                break;
            }
            case SignatureType::NONE_STRING_INT:
            {
                std::string strS("");
                iss >> strS;
                int nParam = 0;
                iss >> nParam;
                this->call<void,std::string,int>(strName, strS, nParam);
                break;
            }
            case SignatureType::NONE_STRING_2INT:
            {
                std::string strS("");
                iss >> strS;
                int nParam1 = 0;
                int nParam2 = 0;
                iss >> nParam1 >> nParam2;
                this->call<void,std::string,int,int>(strName, strS, nParam1, nParam2);
                break;
            }
            case SignatureType::STRING:
            {
                std::string strRet("");
                strRet = this->call<std::string>(strName);
                oss << strRet;
                break;
            }
            case SignatureType::VEC2DDOUBLE:
            {
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d>(strName);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            case SignatureType::VEC2DDOUBLE_INT:
            {
                int nParam(0);
                iss >> nParam;
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d,int>(strName, nParam);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            case SignatureType::VEC2DDOUBLE_2INT:
            {
                int nParam1(0);
                int nParam2(0);
                iss >> nParam1 >> nParam2;
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d,int,int>(strName, nParam1, nParam2);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            case SignatureType::VEC2DDOUBLE_STRING:
            {
                std::string strParam = "";
                iss >> strParam;
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d,std::string>(strName, strParam);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            case SignatureType::VEC2DDOUBLE_2STRING:
            {
                std::string strParam1 = "";
                std::string strParam2 = "";
                iss >> strParam1 >> strParam2;
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d,std::string,std::string>(strName, strParam1, strParam2);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            case SignatureType::VEC2DINT_INT:
            {
                int nParam(0);
                iss >> nParam;
                Vector2i vecRet; vecRet.setZero();
                vecRet = this->call<Vector2i,int>(strName, nParam);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            default:
                DOM_DEV(NOTICE_MSG_QUIET("Com Interface", "Wrapper for " << strName << "'s signature not implemented."))
                break;
        }
    }
    else
    {
        WARNING_MSG("Com Interface", "Unknown function <" << strName << ">. ");
        throw CComInterfaceException(ComIntExceptionType::UNKNOWN_COMMAND);
    }
    return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls all writing functions of given queue
///
/// \param _strQueue Queue of which functions should be called
///
///////////////////////////////////////////////////////////////////////////////
void CComInterface::callWriters(const std::string& _strQueue)
{
    METHOD_ENTRY_QUIET("CComInterface::callWriters")
    
    IBaseCommand* pQueuedFunction = nullptr;
    
    while (m_WriterQueues[_strQueue].try_dequeue(pQueuedFunction))
    {
        switch (pQueuedFunction->getSignature())
        {
            case SignatureType::NONE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call();
                break;
            }
            case SignatureType::NONE_BOOL:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, bool>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_2DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, double, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_2INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_3INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, int, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT_DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT_2DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, double, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT_4DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, double, double, double, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()),
                                              std::get<3>(pQueuedFunctionConcrete->getParams()),
                                              std::get<4>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT_DYN_ARRAY:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, std::vector<double>>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_INT_STRING:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int, std::string>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_2STRING:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, std::string>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_4STRING:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, std::string, std::string, std::string>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()),
                                              std::get<3>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING_DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()), std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING_INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING_2INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, int, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::BOOL_INT:
            case SignatureType::DOUBLE:
            case SignatureType::DOUBLE_INT:
            case SignatureType::DOUBLE_STRING:
            case SignatureType::DOUBLE_STRING_DOUBLE:
            case SignatureType::INT:
            case SignatureType::INT_INT:
            case SignatureType::INT_STRING:
            case SignatureType::STRING:
            case SignatureType::VEC2DDOUBLE:
            case SignatureType::VEC2DDOUBLE_INT:
            case SignatureType::VEC2DDOUBLE_2INT:
            case SignatureType::VEC2DDOUBLE_STRING:
            case SignatureType::VEC2DDOUBLE_2STRING:
            case SignatureType::VEC2DINT_INT:
            {
                DOM_DEV(WARNING_MSG("Com Interface", "Something went wrong, writing functions shouldn't have a return value."))
                break;
            }
            default:
            {
                DOM_DEV(NOTICE_MSG("Com Interface", "Queued writer call not implemented."))
            }
        }
        if (pQueuedFunction != nullptr)
        {
            delete pQueuedFunction;
            MEM_FREED_QUIET("IBaseCommand")
            pQueuedFunction = nullptr;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief List all known functions
///
/// \param nVerboseLevel Level of verbosity when listing registered functions
///
///////////////////////////////////////////////////////////////////////////////
void CComInterface::help(int nVerboseLevel)
{
    METHOD_ENTRY_QUIET("CComInterface::help")
    switch (nVerboseLevel)
    {
        case 0:
            for (auto Com : m_RegisteredFunctions)
            {
                std::cout << Com.first << std::endl;
            }
            break;
        case 1:
            for (auto Com : m_RegisteredFunctions)
            {
                std::cout << "Command: " << Com.first << " (" << m_RegisteredFunctionsDomain[Com.first] <<  ")" << std::endl;
                std::cout << "- Description: " << m_RegisteredFunctionsDescriptions[Com.first] << std::endl;
                std::cout << "- Params: " << std::endl;
                for (auto Param : m_RegisteredFunctionsParams[Com.first])
                {
                    std::cout << mapParameterToString[Param.first] << " " << Param.second << std::endl;
                }
                std::cout << std::endl;
            }
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief List all known functions
///
///////////////////////////////////////////////////////////////////////////////
void CComInterface::help()
{
    METHOD_ENTRY_QUIET("CComInterface::help")
    this->help(0);
}
