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
/// \file       com_interface.tpp
/// \brief      Implementation of class "CComInterface" and helper classes
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-08-28
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
/// \param _Function Function to register
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
CComCallback<TRet, TArgs...>::CComCallback(const std::function<TRet(TArgs...)>& _Function) : m_Function(_Function)
{
    METHOD_ENTRY("CComCallback::CComCallback")
    CTOR_CALL("CComCallback")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the functions with given arguments
///
/// \param _Args Arguments to call the function with
/// \return Return value of function
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
inline TRet CComCallback<TRet, TArgs...>::call(TArgs... _Args)
{
    METHOD_ENTRY("CComCallback::call")
    return m_Function(_Args...);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the given function if registered
///
/// \param _strName Registered name of the function that should be called
/// \param _Args Arguments of the function to be called
/// \return Return value of function
///
///////////////////////////////////////////////////////////////////////////////
template<class TRet, class... Args>
inline TRet CComInterface::call(const std::string& _strName, Args... _Args)
{
    METHOD_ENTRY("CComInterface::call")
    
    try
    {
        #ifdef LOGLEVEL_DEBUG
            auto pFunction = dynamic_cast<CComCallback<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
            if (pFunction != nullptr )
            {
                return pFunction->call(_Args...);
            }
            else
            {
                WARNING_MSG("Com Interface", "Known function with different signature <" << _strName << ">. ")
                return TRet();
            }
        #else
            auto pFunction = static_cast<CComCallback<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
            return pFunction->call(_Args...);
        #endif
    }
    catch (const std::out_of_range& oor)
    {
        WARNING_MSG("Com Interface", "Unknown function <" << _strName << ">. " << oor.what())
        return TRet();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register the given function with its arguments
///
/// \param _strName Name the function should be registered under
/// \param _Command Function to be registered
/// \param _strDescription Description of the function to be registered
/// \param _Signature Signature of the function, containing return value and parameters
/// \param _ParamList List of parameters for given function
/// \param _Domain Domain of function to be registered
///
///////////////////////////////////////////////////////////////////////////////
template<class T>
bool CComInterface::registerFunction(const std::string& _strName, const T& _Command,
                                     const std::string& _strDescription,
                                     const SignatureType& _Signature,
                                     const ParameterListType& _ParamList,
                                     const DomainType& _Domain
                                    )
{
    METHOD_ENTRY("CComInterface::registerFunction")
    
    m_RegisteredFunctions[_strName] = new T(_Command);
    MEM_ALLOC("IBaseComCallback")
    
    m_RegisteredFunctionsDescriptions[_strName] = _strDescription;
    m_RegisteredFunctionsParams[_strName] = _ParamList;
    m_RegisteredFunctionsDomain[_strName] = _Domain;
    m_RegisteredSignatures[_strName] = _Signature;
    
    return true;
}
