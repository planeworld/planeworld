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
/// \file       com_interface.cpp
/// \brief      Implementation of class "CComInterface"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-10-22
///
////////////////////////////////////////////////////////////////////////////////

#include "com_interface.h"

#include <sstream>

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
        case ComIntExceptionType::NR_PARAMS:
            return "Wrong number of parameters";
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
                                    SignatureType::NONE_INT,
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
    
//     try{
    if (m_RegisteredFunctions.find(strName) != m_RegisteredFunctions.end())
    {
        switch (m_RegisteredSignatures[strName])
        {
            case SignatureType::DOUBLE:
            {
                oss << this->call<double>(strName);
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
            case SignatureType::NONE_STRING_DOUBLE:
            {
                std::string strS;
                iss >> strS;
                double fParam = 0.0;
                iss >> fParam;
                this->call<void,std::string, double>(strName, strS, fParam);
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
