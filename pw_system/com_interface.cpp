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
    METHOD_ENTRY("CComInterfaceException::getMessage")
    
    switch (m_EType)
    {
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
    METHOD_ENTRY("CComInterface::CComInterface")
    CTOR_CALL("CComInterface::CComInterface")
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
    METHOD_ENTRY("CComInterface::~CComInterface")
    DTOR_CALL("CComInterface::~CComInterface")
    for (auto pFunction : m_RegisteredFunctions)
    {
        if (pFunction.second != nullptr)
        {
            delete pFunction.second;
            pFunction.second = nullptr;
            MEM_FREED("IBaseCommand")
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
    METHOD_ENTRY("CComInterface::call")
    
    std::istringstream iss(_strCommand);
    std::ostringstream oss("");
    std::string strName;
    
    iss >> strName;
    
    if (m_RegisteredFunctions.find(strName) != m_RegisteredFunctions.end())
    {
        switch (m_RegisteredFunctions[strName]->Signature)
        {
            case SignatureType::DOUBLE:
            {
                oss << this->call<double>(strName);
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
            case SignatureType::NONE_INT:
            {
                int nParam = 0;
                iss >> nParam;
                this->call<void,int>(strName, nParam);
                break;
            }
            case SignatureType::NONE_STRING:
            {
                std::string strS;
                iss >> strS;
                this->call<void,std::string>(strName, strS);
                break;
            }
            case SignatureType::NONE_STRING_4DOUBLE:
            {
                std::string strS;
                iss >> strS;
                double fParam[4] = {0.0, 0.0, 0.0, 0.0};
                iss >> fParam[0] >> fParam[1] >> fParam[2] >> fParam[3];
                this->call<void,std::string, double, double, double, double>(strName, strS, fParam[0], fParam[1], fParam[2], fParam[3]);
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
            case SignatureType::VEC2DDOUBLE_STRING:
            {
                std::string strParam = "";
                iss >> strParam;
                Vector2d vecRet; vecRet.setZero();
                vecRet = this->call<Vector2d,std::string>(strName, strParam);
                oss << vecRet[0] << " " << vecRet[1];
                break;
            }
            default:
                NOTICE_MSG("Com Interface", "Wrapper for " << strName << "'s signature not implemented.")
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
    METHOD_ENTRY("CComInterface::callWriters")
    
    IBaseCommand* pQueuedFunction = nullptr;
    
    while (m_WriterQueues[_strQueue].try_dequeue(pQueuedFunction))
    {
        switch (pQueuedFunction->Signature)
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
            case SignatureType::NONE_INT:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, int>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING_4DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, double, double, double, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()),
                                              std::get<1>(pQueuedFunctionConcrete->getParams()),
                                              std::get<2>(pQueuedFunctionConcrete->getParams()),
                                              std::get<3>(pQueuedFunctionConcrete->getParams()),
                                              std::get<4>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::NONE_STRING_DOUBLE:
            {
                auto pQueuedFunctionConcrete = static_cast<CCommandToQueueWrapper<void, std::string, double>*>(pQueuedFunction);
                pQueuedFunctionConcrete->call(std::get<0>(pQueuedFunctionConcrete->getParams()), std::get<1>(pQueuedFunctionConcrete->getParams()));
                break;
            }
            case SignatureType::DOUBLE:
            case SignatureType::DOUBLE_STRING:
            case SignatureType::DOUBLE_STRING_DOUBLE:
            case SignatureType::INT:
            case SignatureType::VEC2DDOUBLE_STRING:
            {
                WARNING_MSG("Com Interface", "Something went wrong, writing functions shouldn't have a return value.")
                break;
            }
            default:
            {
                NOTICE_MSG("Com Interface", "Queued writer call not implemented.")
            }
        }
        if (pQueuedFunction != nullptr)
        {
            delete pQueuedFunction;
            MEM_FREED("IBaseCommand")
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
    METHOD_ENTRY("CComInterface::help")
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
    METHOD_ENTRY("CComInterface::help")
    this->help(0);
}
