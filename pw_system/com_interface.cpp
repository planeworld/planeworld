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
            MEM_FREED("IBaseComCallback")
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
/// \param _strName Registered name of the function that should be called
/// \return Return value of function as string
///
///////////////////////////////////////////////////////////////////////////////
const std::string& CComInterface::call(const std::string& _strName)
{
    METHOD_ENTRY("CComInterface::call")
    
    if (m_RegisteredFunctions.find(_strName) != m_RegisteredFunctions.end())
    {
        ParameterListType ParamList = m_RegisteredFunctionsParams[_strName];
        
        switch (ParamList.size())
        {
            case 1:
                if (ParamList[0].first == ParameterType::VOID)
                {
                    this->call<void>(_strName);
                }
                else
                {
                    NOTICE_MSG("Com Interface", "Wrapper for " << _strName << "'s signature not implemented.")
                }
                break;
            case 2:
                if (ParamList[0].first == ParameterType::VOID)
                {
                    if (ParamList[1].first == ParameterType::DOUBLE)
                    {
                        this->call<void,double>(_strName, std::stod(_strName));
                    }
                }
                break;
        }
        
//         for (auto Params : m_RegisteredFunctionsParams[_strName])
//         {
//             if (Params)
//         }
//         #ifdef LOGLEVEL_DEBUG
//             auto pFunction = dynamic_cast<CComCallback<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
//             if (pFunction != nullptr )
//             {
//                 return pFunction->call(_Args...);
//             }
//             else
//             {
//                 WARNING_MSG("Com Interface", "Known function with different signature <" << _strName << ">. ")
//                 return TRet();
//             }
//         #else
//             auto pFunction = static_cast<CComCallback<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
//             return pFunction->call(_Args...);
//         #endif
    }
    else
    {
        WARNING_MSG("Com Interface", "Unknown function <" << _strName << ">. ")
        return std::string("");
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register the given function with its arguments
///
/// \param _strName Name the function should be registered under
/// \param _pFunction Function to be registered
/// \param _strDescription Description of the function to be registered
/// \param _ParamList List of parameters for given function
/// \param _Domain Domain of function to be registered
///
///////////////////////////////////////////////////////////////////////////////
bool CComInterface::registerFunction(const std::string& _strName, IBaseComCallback* const _pFunction,
                                     const std::string& _strDescription,
                                     const ParameterListType& _ParamList,
                                     const DomainType& _Domain
                                    )
{
    METHOD_ENTRY("CComInterface::registerFunction")
    
    m_RegisteredFunctions[_strName] = _pFunction;
    MEM_ALLOC("IBaseComCallback")
    
    m_RegisteredFunctionsDescriptions[_strName] = _strDescription;
    m_RegisteredFunctionsParams[_strName] = _ParamList;
    m_RegisteredFunctionsDomain[_strName] = _Domain;
    
    return true;
}

