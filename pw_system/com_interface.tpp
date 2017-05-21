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
CCommand<TRet, TArgs...>::CCommand(const std::function<TRet(TArgs...)>& _Function) : m_Function(_Function)
{
    METHOD_ENTRY("CCommand::CCommand")
    CTOR_CALL("CCommand")
    this->dispatchSignature();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor for a registered function called from a command queue.
///
/// \param _Function Function to register
/// \param _Args Parameters of registered function call
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
CCommandToQueueWrapper<TRet, TArgs...>::CCommandToQueueWrapper(const std::function<TRet(TArgs...)>& _Function,
                                               TArgs... _Args) : 
                                               m_Function(_Function),
                                               m_Params(_Args...)
{
    METHOD_ENTRY("CCommandToQueueWrapper::CCommandToQueueWrapper")
    CTOR_CALL("CCommandToQueueWrapper")
    this->dispatchSignature();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the function with given arguments
///
/// \param _Args Arguments to call the function with
/// \return Return value of function
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
TRet CCommand<TRet, TArgs...>::call(TArgs... _Args)
{
    METHOD_ENTRY("CCommand::call")
    DEBUG_MSG("Command", "Command called.")
    return m_Function(_Args...);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls the queued writer function with given arguments
///
/// \return Return value of function
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
TRet CCommandToQueueWrapper<TRet, TArgs...>::call(TArgs... _Args)
{
    METHOD_ENTRY("CCommandWriter::call")
    try
    {
        DEBUG_MSG("Queued Command", "Queued command called.")
        m_Function(_Args...);
    }
    catch (const CComInterfaceException& ComIntEx)
    {
        WARNING_MSG("Queued Command", ComIntEx.getMessage())
    }
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
            
            // Search for callbacks and execute if exist
            const auto Range = m_RegisteredCallbacks.equal_range(_strName);
            for_each(Range.first, Range.second, 
                [&](RegisteredCallbacksType::value_type& _Com)
                {
                    DEBUG_MSG("Com Interface", "Callback called.")
                    
                    auto pCallback = dynamic_cast<CCommand<TRet, Args...>*>(_Com.second);
                    if (pCallback != nullptr)
                    {
                        return pCallback->call(_Args...);
                    }
                    else
                    {
                        WARNING_MSG("Com Interface", "Known function with different signature <" << _strName << ">. ")
                        return TRet();
                    }
                }
            );
            
            // Execute function if existant
            const auto ci = m_RegisteredFunctions.find(_strName);
            if (ci != m_RegisteredFunctions.end())
            {
                DEBUG_MSG("Com Interface", "Command called: <" << _strName << ">")
                
                auto pFunction = dynamic_cast<CCommand<TRet, Args...>*>(ci->second);
                if (pFunction != nullptr)
                {
                    return pFunction->call(_Args...);
                }
                else
                {
                    WARNING_MSG("Com Interface", "Known function with different signature <" << _strName << ">. ")
                    return TRet();
                }
            }
            else
            {
                return TRet();
            }
        #else
            // Search for callbacks and execute if exist
            const auto Range = m_RegisteredCallbacks.equal_range(_strName);
            for_each(Range.first, Range.second, 
                [&](RegisteredCallbacksType::value_type& _Com)
                {
                    auto pCallback = static_cast<CCommand<TRet, Args...>*>(_Com.second);
                    return pCallback->call(_Args...);
                }
            );

            // Execute function if existant
            const auto ci = m_RegisteredFunctions.find(_strName);
            if (ci != m_RegisteredFunctions.end())
            {
                auto pFunction = static_cast<CCommand<TRet, Args...>*>(ci->second);
                return pFunction->call(_Args...);
            }
            else
            {
                return TRet();
            }
        #endif
    }
    catch (const CComInterfaceException& ComIntEx)
    {
        WARNING_MSG("Com Interface", ComIntEx.getMessage())
        throw; // To be caught bei com console
    }
    catch (const std::out_of_range& oor)
    {
        WARNING_MSG("Com Interface", "Unknown function <" << _strName << ">. " << oor.what())
        return TRet();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register the given callback to existing function
///
/// \param _strName Name the function the callback should listen to
/// \param _Func Callback function to be registered
/// \param _strWriterDomain Indicates a callback that writes data (will be
///                         queued for thread safety). Reader functions will
///                         have the default domain "Reader"
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
template<class TRet, class... TArgs> 
bool CComInterface::registerCallback(const std::string& _strName, const std::function<TRet(TArgs...)>& _Func,
                                     const std::string& _strWriterDomain)
{
    METHOD_ENTRY("CComInterface::registerCallback")
 
    if (_strWriterDomain != "Reader")
    {
        LOGIC_CHECK(
            if (m_WriterDomains.find(_strWriterDomain) == m_WriterDomains.end())
            {
                ERROR_MSG("Com Interface", "Unknown writer domain <" << _strWriterDomain <<
                                        ">. Registered writer domains are:")
                DEBUG_BLK(
                    for (auto Domain : m_WriterDomains) std::cout << " - " << Domain << std::endl;
                )
                return false;
            }
        ) // LOGIC_CHECK
        
        m_RegisteredCallbacks.insert({{_strName,
                                        new CCommand<TRet, TArgs...>([this, _strName, _Func, _strWriterDomain](TArgs... _Args) -> TRet
                                        {
                                            auto pCommand = new CCommandToQueueWrapper<TRet, TArgs...>(_Func, _Args...);
                                            m_WriterQueues[_strWriterDomain].enqueue(pCommand);
                                            MEM_ALLOC("IBaseCommand")
                                            return TRet();
                                        })}});
        MEM_ALLOC("IBaseCommand")
    }
    else
    {
        m_RegisteredCallbacks.insert({{_strName, new CCommand<TRet, TArgs...>(_Func)}});
        MEM_ALLOC("IBaseCommand")
    }    
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register the given event with its arguments
///
/// \param _strName Name of the event
/// \param _strDescription Description of the event to be registered
/// \param _ParamList List of parameters for given function
/// \param _Domain Domain of event to be registered
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
template<class... TArgs> 
bool CComInterface::registerEvent(const std::string& _strName,
                                  const std::string& _strDescription,
                                  const ParameterListType& _ParamList,
                                  const DomainType& _Domain)
{
    METHOD_ENTRY("CComInterface::registerEvent")
    
    DEBUG_MSG("Com Interface", "Registering event <" << _strName << ">.")

    // Events are always readers, since they only trigger callbacks which
    // might then be writers

    m_RegisteredFunctions[_strName] = new CCommand<void, TArgs...>([](const TArgs&...){});
    MEM_ALLOC("IBaseCommand")
    
    m_RegisteredFunctionsDescriptions[_strName] = _strDescription;
    m_RegisteredFunctionsParams[_strName] = _ParamList;
    m_RegisteredFunctionsDomain[_strName] = _Domain;
    m_RegisteredDomains.emplace(_Domain);
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Register the given function with its arguments
///
/// \param _strName Name the function should be registered under
/// \param _Command Function to be registered
/// \param _strDescription Description of the function to be registered
/// \param _ParamList List of parameters for given function
/// \param _Domain Domain of function to be registered
/// \param _strWriterDomain Indicates a function that writes data (will be
///                         queued for thread safety). Reader functions will
///                         have the default domain "Reader"
///
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
template<class TRet, class... TArgs> 
bool CComInterface::registerFunction(const std::string& _strName, const CCommand<TRet, TArgs...>& _Command,
                                     const std::string& _strDescription,
                                     const ParameterListType& _ParamList,
                                     const DomainType& _Domain,
                                     const std::string& _strWriterDomain
                                    )
{
    METHOD_ENTRY("CComInterface::registerFunction")
    
    DEBUG_MSG("Com Interface", "Registering function <" << _strName << ">.")

    if (_strWriterDomain != "Reader")
    {
        LOGIC_CHECK(
            if (m_WriterDomains.find(_strWriterDomain) == m_WriterDomains.end())
            {
                ERROR_MSG("Com Interface", "Unknown writer domain <" << _strWriterDomain <<
                                        ">. Registered writer domains are:")
                DEBUG_BLK(
                    for (auto Domain : m_WriterDomains) std::cout << " - " << Domain << std::endl;
                )
                return false;
            }
        ) // LOGIC_CHECK
        
        m_RegisteredFunctions[_strName] = new CCommand<TRet, TArgs...>([this,_strName,_Command, _strWriterDomain](TArgs... _Args) -> TRet
                                            {
                                                auto pCommand = new CCommandToQueueWrapper<TRet, TArgs...>(_Command.getFunction(), _Args...);
                                                m_WriterQueues[_strWriterDomain].enqueue(pCommand);
                                                MEM_ALLOC("IBaseCommand")
                                                return TRet();
                                            });
        MEM_ALLOC("IBaseCommand")
    }
    else
    {
        m_RegisteredFunctions[_strName] = new CCommand<TRet, TArgs...>(_Command);
        MEM_ALLOC("IBaseCommand")
    }
    
    m_RegisteredFunctionsDescriptions[_strName] = _strDescription;
    m_RegisteredFunctionsParams[_strName] = _ParamList;
    m_RegisteredFunctionsDomain[_strName] = _Domain;
    m_RegisteredDomains.emplace(_Domain);
    
    return true;
}

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

using namespace Eigen;

template<class TRet, class... TArgs> void CCommand<TRet, TArgs...>::dispatchSignature() {m_Signature = SignatureType::UNDEFINED;}
template<> inline void CCommand<void>::dispatchSignature() {m_Signature = SignatureType::NONE;}
template<> inline void CCommand<double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE;}
template<> inline void CCommand<double,int>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_INT;}
template<> inline void CCommand<double,std::string>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING;}
template<> inline void CCommand<double,std::string,double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING_DOUBLE;}
template<> inline void CCommand<int>::dispatchSignature() {m_Signature = SignatureType::INT;}
template<> inline void CCommand<int,int>::dispatchSignature() {m_Signature = SignatureType::INT_INT;}
template<> inline void CCommand<int,std::string>::dispatchSignature() {m_Signature = SignatureType::INT_STRING;}
template<> inline void CCommand<void, bool>::dispatchSignature() {m_Signature = SignatureType::NONE_BOOL;}
template<> inline void CCommand<void, double>::dispatchSignature() {m_Signature = SignatureType::NONE_DOUBLE;}
template<> inline void CCommand<void, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_2DOUBLE;}
template<> inline void CCommand<void, int>::dispatchSignature() {m_Signature = SignatureType::NONE_INT;}
template<> inline void CCommand<void, int, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_DOUBLE;}
template<> inline void CCommand<void, int, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_2DOUBLE;}
template<> inline void CCommand<void, int, double, double, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_4DOUBLE;}
template<> inline void CCommand<void, int, std::vector<double>>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_DYN_ARRAY;}
template<> inline void CCommand<void, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_2INT;}
template<> inline void CCommand<void, int, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_3INT;}
template<> inline void CCommand<void, int, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_STRING;}
template<> inline void CCommand<void, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING;}
template<> inline void CCommand<void, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_2STRING;}
template<> inline void CCommand<void, std::string, std::string, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_4STRING;}
template<> inline void CCommand<void, std::string, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_DOUBLE;}
template<> inline void CCommand<void, std::string, int>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_INT;}
template<> inline void CCommand<void, std::string, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_2INT;}
template<> inline void CCommand<std::string>::dispatchSignature() {m_Signature = SignatureType::STRING;}
template<> inline void CCommand<Vector2d>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE;}
template<> inline void CCommand<Vector2d, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_INT;}
template<> inline void CCommand<Vector2d, int, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2INT;}
template<> inline void CCommand<Vector2d, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_STRING;}
template<> inline void CCommand<Vector2d, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2STRING;}
template<> inline void CCommand<Vector2i, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DINT_INT;}

template<class TRet, class... TArgs> void CCommandToQueueWrapper<TRet, TArgs...>::dispatchSignature() {m_Signature = SignatureType::UNDEFINED;}
template<> inline void CCommandToQueueWrapper<void>::dispatchSignature() {m_Signature = SignatureType::NONE;}
template<> inline void CCommandToQueueWrapper<double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE;}
template<> inline void CCommandToQueueWrapper<double,int>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_INT;}
template<> inline void CCommandToQueueWrapper<double,std::string>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING;}
template<> inline void CCommandToQueueWrapper<double,std::string,double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING_DOUBLE;}
template<> inline void CCommandToQueueWrapper<int>::dispatchSignature() {m_Signature = SignatureType::INT;}
template<> inline void CCommandToQueueWrapper<int,int>::dispatchSignature() {m_Signature = SignatureType::INT_INT;}
template<> inline void CCommandToQueueWrapper<int,std::string>::dispatchSignature() {m_Signature = SignatureType::INT_STRING;}
template<> inline void CCommandToQueueWrapper<void, bool>::dispatchSignature() {m_Signature = SignatureType::NONE_BOOL;}
template<> inline void CCommandToQueueWrapper<void, double>::dispatchSignature() {m_Signature = SignatureType::NONE_DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_2DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, int>::dispatchSignature() {m_Signature = SignatureType::NONE_INT;}
template<> inline void CCommandToQueueWrapper<void, int, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, int, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_2DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, int, double, double, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_4DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, int, std::vector<double>>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_DYN_ARRAY;}
template<> inline void CCommandToQueueWrapper<void, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_2INT;}
template<> inline void CCommandToQueueWrapper<void, int, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_3INT;}
template<> inline void CCommandToQueueWrapper<void, int, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_INT_STRING;}
template<> inline void CCommandToQueueWrapper<void, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING;}
template<> inline void CCommandToQueueWrapper<void, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_2STRING;}
template<> inline void CCommandToQueueWrapper<void, std::string, std::string, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_4STRING;}
template<> inline void CCommandToQueueWrapper<void, std::string, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, std::string, int>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_INT;}
template<> inline void CCommandToQueueWrapper<void, std::string, int, int>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_2INT;}
template<> inline void CCommandToQueueWrapper<std::string>::dispatchSignature() {m_Signature = SignatureType::STRING;}
template<> inline void CCommandToQueueWrapper<Vector2d>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE;}
template<> inline void CCommandToQueueWrapper<Vector2d, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_INT;}
template<> inline void CCommandToQueueWrapper<Vector2d, int, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2INT;}
template<> inline void CCommandToQueueWrapper<Vector2d, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_STRING;}
template<> inline void CCommandToQueueWrapper<Vector2d, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2STRING;}
template<> inline void CCommandToQueueWrapper<Vector2i, int>::dispatchSignature() {m_Signature = SignatureType::VEC2DINT_INT;}
