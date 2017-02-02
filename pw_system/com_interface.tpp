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
                                               m_Params(_Args...),
                                               m_Function(_Function)
{
    METHOD_ENTRY("CCommandToQueueWrapper::CCommandToQueueWrapper")
    CTOR_CALL("CCommandToQueueWrapper")
    this->dispatchSignature();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
/// \param _Function Function with write access to register
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
CCommandWritable<TRet, TArgs...>::CCommandWritable(const std::function<void(TArgs...)>& _Function) : m_Function(_Function)
{
    METHOD_ENTRY("CCommandWritable::CCommandWritable")
    CTOR_CALL("CCommandWritable")
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
    DEBUG_MSG("Com Interface", "Command called")
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
        DEBUG_MSG("Com Interface", "Queued writer called.")
        m_Function(_Args...);
    }
    catch (CComInterfaceException ComIntEx)
    {
        WARNING_MSG("Com Interface", ComIntEx.getMessage())
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calls a function with write access with given arguments
///
/// This method does not actually call the registered function, but puts it
/// in a command queue with its parameters. 
///
/// \param _Args Arguments to call the function with
///
/// \return Return value of function
///
///////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
TRet CCommandWritable<TRet, TArgs...>::call(TArgs... _Args)
{
    METHOD_ENTRY("CCommandWritable::call")
    DEBUG_MSG("Com Interface", "Writer called for storage in command queue.")
    m_Function(_Args...);
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
            if (m_WriterFlags.at(_strName))
            {
                DEBUG_MSG("Com Interface", "Direct writer call: <" << _strName << ">")
                auto pFunction = dynamic_cast<CCommandWritable<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
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
                DEBUG_MSG("Com Interface", "Direct reader call: <" << _strName << ">")
                auto pFunction = dynamic_cast<CCommand<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
                if (pFunction != nullptr )
                {
                    return pFunction->call(_Args...);
                }
                else
                {
                    WARNING_MSG("Com Interface", "Known function with different signature <" << _strName << ">. ")
                    return TRet();
                }
            }
        #else
            if (m_WriterFlags.at(_strName))
            {
                auto pFunction = static_cast<CCommandWritable<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
                return pFunction->call(_Args...);
            }
            else
            {
                auto pFunction = static_cast<CCommand<TRet, Args...>*>(m_RegisteredFunctions.at(_strName));
                return pFunction->call(_Args...);
            }
        #endif
    }
    catch (CComInterfaceException ComIntEx)
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
/// \brief Register the given function with its arguments
///
/// \param _strName Name the function should be registered under
/// \param _Command Function to be registered
/// \param _strDescription Description of the function to be registered
/// \param _ParamList List of parameters for given function
/// \param _Domain Domain of function to be registeredm_RegisteredFunctions[_strName]
/// \param _strWriterDomain Indicates a function that writes data (will be
///                         queued for thread safety). Reader functions will
///                         have the default domain "Reader"
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
        if (m_WriterDomains.find(_strWriterDomain) != m_WriterDomains.end())
        {
            m_RegisteredFunctions[_strName] = new CCommandWritable<TRet, TArgs...>([this,_strName,_Command, _strWriterDomain](TArgs... _Args)
                                                {
                                                    auto pCommand = new CCommandToQueueWrapper<TRet, TArgs...>(_Command.getFunction(), _Args...);
                                                    m_WriterQueues[_strWriterDomain].enqueue(pCommand);
                                                    MEM_ALLOC("IBaseCommand")
                                                });
            m_WriterFlags[_strName] = true;
            MEM_ALLOC("IBaseCommand")
        }
        else
        {
            ERROR_MSG("Com Interface", "Unknown writer domain <" << _strWriterDomain <<
                                       ">. Registered writer domains are:")
            ERROR(
                for (auto Domain : m_WriterDomains) std::cout << " - " << Domain << std::endl;
            )
            return false;
        }
    }
    else
    {
        m_RegisteredFunctions[_strName] = new CCommand<TRet, TArgs...>(_Command);
        m_WriterFlags[_strName] = false;
        MEM_ALLOC("IBaseCommand")
    }
    
    m_RegisteredFunctionsDescriptions[_strName] = _strDescription;
    m_RegisteredFunctionsParams[_strName] = _ParamList;
    m_RegisteredFunctionsDomain[_strName] = _Domain;
    
    return true;
}

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

using namespace Eigen;

template<class TRet, class... TArgs> void CCommand<TRet, TArgs...>::dispatchSignature() {m_Signature = SignatureType::UNDEFINED;}
template<> inline void CCommand<void>::dispatchSignature() {m_Signature = SignatureType::NONE;}
template<> inline void CCommand<double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE;}
template<> inline void CCommand<double,std::string>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING;}
template<> inline void CCommand<double,std::string,double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING_DOUBLE;}
template<> inline void CCommand<int>::dispatchSignature() {m_Signature = SignatureType::INT;}
template<> inline void CCommand<int,int>::dispatchSignature() {m_Signature = SignatureType::INT_INT;}
template<> inline void CCommand<void, bool>::dispatchSignature() {m_Signature = SignatureType::NONE_BOOL;}
template<> inline void CCommand<void, double>::dispatchSignature() {m_Signature = SignatureType::NONE_DOUBLE;}
template<> inline void CCommand<void, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_2DOUBLE;}
template<> inline void CCommand<void, int>::dispatchSignature() {m_Signature = SignatureType::NONE_INT;}
template<> inline void CCommand<void, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING;}
template<> inline void CCommand<void, std::string, double, double, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_4DOUBLE;}
template<> inline void CCommand<void, std::string, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_DOUBLE;}
template<> inline void CCommand<Vector2d, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_STRING;}
template<> inline void CCommand<Vector2d, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2STRING;}

template<class TRet, class... TArgs> void CCommandToQueueWrapper<TRet, TArgs...>::dispatchSignature() {m_Signature = SignatureType::UNDEFINED;}
template<> inline void CCommandToQueueWrapper<void>::dispatchSignature() {m_Signature = SignatureType::NONE;}
template<> inline void CCommandToQueueWrapper<double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE;}
template<> inline void CCommandToQueueWrapper<double,std::string>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING;}
template<> inline void CCommandToQueueWrapper<double,std::string,double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING_DOUBLE;}
template<> inline void CCommandToQueueWrapper<int>::dispatchSignature() {m_Signature = SignatureType::INT;}
template<> inline void CCommandToQueueWrapper<int,int>::dispatchSignature() {m_Signature = SignatureType::INT_INT;}
template<> inline void CCommandToQueueWrapper<void, bool>::dispatchSignature() {m_Signature = SignatureType::NONE_BOOL;}
template<> inline void CCommandToQueueWrapper<void, double>::dispatchSignature() {m_Signature = SignatureType::NONE_DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_2DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, int>::dispatchSignature() {m_Signature = SignatureType::NONE_INT;}
template<> inline void CCommandToQueueWrapper<void, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING;}
template<> inline void CCommandToQueueWrapper<void, std::string, double, double, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_4DOUBLE;}
template<> inline void CCommandToQueueWrapper<void, std::string, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_DOUBLE;}
template<> inline void CCommandToQueueWrapper<Vector2d, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_STRING;}
template<> inline void CCommandToQueueWrapper<Vector2d, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2STRING;}

template<class TRet, class... TArgs> void CCommandWritable<TRet, TArgs...>::dispatchSignature() {m_Signature = SignatureType::UNDEFINED;}
template<> inline void CCommandWritable<void>::dispatchSignature() {m_Signature = SignatureType::NONE;}
template<> inline void CCommandWritable<double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE;}
template<> inline void CCommandWritable<double,std::string>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING;}
template<> inline void CCommandWritable<double,std::string,double>::dispatchSignature() {m_Signature = SignatureType::DOUBLE_STRING_DOUBLE;}
template<> inline void CCommandWritable<int>::dispatchSignature() {m_Signature = SignatureType::INT;}
template<> inline void CCommandWritable<int,int>::dispatchSignature() {m_Signature = SignatureType::INT_INT;}
template<> inline void CCommandWritable<void, bool>::dispatchSignature() {m_Signature = SignatureType::NONE_BOOL;}
template<> inline void CCommandWritable<void, double>::dispatchSignature() {m_Signature = SignatureType::NONE_DOUBLE;}
template<> inline void CCommandWritable<void, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_2DOUBLE;}
template<> inline void CCommandWritable<void, int>::dispatchSignature() {m_Signature = SignatureType::NONE_INT;}
template<> inline void CCommandWritable<void, std::string>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING;}
template<> inline void CCommandWritable<void, std::string, double, double, double, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_4DOUBLE;}
template<> inline void CCommandWritable<void, std::string, double>::dispatchSignature() {m_Signature = SignatureType::NONE_STRING_DOUBLE;}
template<> inline void CCommandWritable<Vector2d, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_STRING;}
template<> inline void CCommandWritable<Vector2d, std::string, std::string>::dispatchSignature() {m_Signature = SignatureType::VEC2DDOUBLE_2STRING;}


