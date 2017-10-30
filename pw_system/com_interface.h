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
/// \file       com_interface.h
/// \brief      Prototype of class "CComInterface"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-08-21
///
////////////////////////////////////////////////////////////////////////////////

#ifndef COM_INTERFACE_H
#define COM_INTERFACE_H

//--- Standard header --------------------------------------------------------//
#include <atomic>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "log.h"
#include "log_listener.h"

//--- Misc header ------------------------------------------------------------//
#include "concurrentqueue.h"

/// Specifies a parameter type
enum class ParameterType
{
    UNDEFINED,
    NONE,
    BOOL,
    DOUBLE,
    DYN_ARRAY,
    INT,
    STRING,
    UID,
    VEC2DDOUBLE,
    VEC2DINT
};

/// Specifies the signature of the function
enum class SignatureType
{
    UNDEFINED,
    NONE,
    DOUBLE,
    DOUBLE_INT,
    DOUBLE_STRING,
    DOUBLE_STRING_DOUBLE,
    INT,
    INT_INT,
    INT_STRING,
    NONE_BOOL,
    NONE_DOUBLE,
    NONE_2DOUBLE,
    NONE_INT,
    NONE_2INT,
    NONE_3INT,
    NONE_INT_DOUBLE,
    NONE_INT_2DOUBLE,
    NONE_INT_4DOUBLE,
    NONE_INT_DYN_ARRAY,
    NONE_INT_STRING,
    NONE_STRING,
    NONE_2STRING,
    NONE_4STRING,
    NONE_STRING_DOUBLE,
    NONE_STRING_INT,
    NONE_STRING_2INT,
    NONE_UID,
    STRING,
    VEC2DDOUBLE,
    VEC2DDOUBLE_INT,
    VEC2DDOUBLE_2INT,
    VEC2DDOUBLE_STRING,
    VEC2DDOUBLE_2STRING,
    VEC2DINT_INT
};

/// Specifies type of possible exceptions in com interface
enum class ComIntExceptionType
{
    INVALID_VALUE,
    UNKNOWN_COMMAND,
    PARAM_ERROR
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class defining exceptions for the com interface
///
////////////////////////////////////////////////////////////////////////////////
class CComInterfaceException
{
    public:
        //--- Constructor/Destructor -----------------------------------------//
        CComInterfaceException(ComIntExceptionType _EType) : m_EType(_EType) {}
        
        //--- Constant methods -----------------------------------------------//
        const std::string getMessage() const;
            
    private:
        
        CComInterfaceException();
        
        ComIntExceptionType m_EType; ///< Type of exception

};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for callback functions registered at com interface
///
////////////////////////////////////////////////////////////////////////////////
class IBaseCommand
{
    public:
        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IBaseCommand(){}
        
        //--- Constant methods -----------------------------------------------//
        SignatureType getSignature() const {return m_Signature;}
        
    protected:
    
        SignatureType m_Signature = SignatureType::UNDEFINED;
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Specialised callback functions registered at com interface
///
////////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
class CCommand : public IBaseCommand
{
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CCommand() = delete;
        CCommand(const std::function<TRet(TArgs...)>&);
        
        //--- Constant methods -----------------------------------------------//
        std::function<TRet(TArgs...)> getFunction() const {return m_Function;}
        
        //--- Methods --------------------------------------------------------//
        TRet call(TArgs...);
        
    private:
        
        /// --- Methods [private] --------------------------------------------//
        void dispatchSignature();
        
        /// --- Variables [private] ------------------------------------------//
        std::function<TRet(TArgs...)> m_Function; ///< Function to be registered at com interface
        
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Specialised callback functions with parameters to store in queue
///
/// This wrapper encloses commands (\ref CCommand) which are stored in a queue.
/// In contrast to \ref CCommand, it also stores all parameters which are later
/// needed for actual execution of the command.
///
////////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
class CCommandToQueueWrapper : public IBaseCommand
{
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CCommandToQueueWrapper() = delete;
        CCommandToQueueWrapper(const std::function<TRet(TArgs...)>&, TArgs...);
        
        //--- Constant methods -----------------------------------------------//
        const std::tuple<TArgs...> getParams() const {return m_Params;}
        
        //--- Methods --------------------------------------------------------//
        TRet call(TArgs...);
        
    private:
        
        /// --- Methods [private] --------------------------------------------//
        void dispatchSignature();
        
        /// --- Variables [private] ------------------------------------------//
        std::function<TRet(TArgs...)> m_Function; ///< Function to be registered at com interface
        std::tuple<TArgs...>          m_Params;   ///< Parameter function is called with
        
};

/// Map of all functions, accessed by name
typedef std::map<std::string, IBaseCommand*> RegisteredFunctionsType;
/// Map of descriptions, accessed by name
typedef std::unordered_map<std::string, std::string> RegisteredFunctionsDescriptionType;
/// Parameter list for functions
typedef std::vector<std::pair<ParameterType, std::string>> ParameterListType;
/// Map of parameter lists, accessed by function name
typedef std::unordered_map<std::string, ParameterListType> RegisteredParameterListsType;

/// Domain of function being registered
typedef std::string DomainType;
/// Map of domains, accessed by function name
typedef std::unordered_map<std::string, DomainType> RegisteredDomainsType;

/// Multimap of callback functions, accessed by name
typedef std::unordered_multimap<std::string, IBaseCommand*> RegisteredCallbacksType;

/// List of writer domains
typedef std::set<std::string> DomainsType;
/// Map of queues with one queue for each writer domain
typedef std::unordered_map<std::string, moodycamel::ConcurrentQueue<IBaseCommand*>> WriterQueuesType;

//--- Enum parser ------------------------------------------------------------//
static std::map<ParameterType, std::string> mapParameterToString = {
    {ParameterType::UNDEFINED, "<undefined>"},
    {ParameterType::NONE, "<none>"},
    {ParameterType::BOOL, "<bool>"},
    {ParameterType::DOUBLE, "<double>"},
    {ParameterType::DYN_ARRAY, "<dyn_array>"},
    {ParameterType::INT, "<int>"},
    {ParameterType::STRING, "<string>"},
    {ParameterType::UID, "<uid>"},
    {ParameterType::VEC2DDOUBLE, "<vec2ddouble>"},
    {ParameterType::VEC2DINT, "<vec2dint>"}
}; ///< Map from ParameterType to string

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing an interface to the engine
///
////////////////////////////////////////////////////////////////////////////////
class CComInterface : public ILogListener
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CComInterface();
        virtual ~CComInterface();
        
        //--- Constant Methods -----------------------------------------------//
        DomainsType*                  getDomains() {return &m_RegisteredDomains;} 
        RegisteredDomainsType*        getDomainsByFunction() {return &m_RegisteredFunctionsDomain;}
        RegisteredFunctionsType*      getFunctions()  {return &m_RegisteredFunctions;} 
        
        //--- Methods --------------------------------------------------------//
        template<class TRet, class... Args>
        TRet                call(const std::string&, Args...);
        const std::string   call(const std::string&);
        void                callWriters(const std::string&);
        void                help();
        void                help(int);

        template <class TRet, class... TArgs>
        bool registerCallback(const std::string&, const std::function<TRet(TArgs...)>&,
                              const std::string& = "Reader");
        
        template <class... TArgs>
        bool registerEvent(const std::string&,
                           const std::string&,
                           const ParameterListType& = {},
                           const DomainType& = "");
        
        template <class TRet, class... TArgs>
        bool registerFunction(const std::string&, const CCommand<TRet, TArgs...>&,
                              const std::string&,
                              const ParameterListType& = {},
                              const DomainType& = "",
                              const std::string& = "Reader"
        );
        void registerWriterDomain(const std::string&);
        
        void logEntry(const std::string&, const std::string&,
                      const LogLevelType&, const LogDomainType&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComInterface&);
        friend std::ostream& operator<<(std::ostream&, CComInterface&);
        
    private:
        
        #ifdef PW_MULTITHREADING
            std::atomic_flag isAccessed = ATOMIC_FLAG_INIT;              ///< Indicates access, important for multithreading
        #endif
        
        
        RegisteredCallbacksType             m_RegisteredCallbacks;       ///< Callbacks attached to registered functions
        
        RegisteredFunctionsType             m_RegisteredFunctions;       ///< All registered functions provided by modules
        RegisteredFunctionsDescriptionType  m_RegisteredFunctionsDescriptions; ///< Descriptions of registered functions
        RegisteredParameterListsType        m_RegisteredFunctionsParams; ///< Parameter lists of registered functions      
        RegisteredDomainsType               m_RegisteredFunctionsDomain; ///< Domain of registered functions
        DomainsType                         m_RegisteredDomains;         ///< All domains registered
        DomainsType                         m_WriterDomains;             ///< Domains for queued functions
        WriterQueuesType                    m_WriterQueues;              ///< Command queues for write access
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the com interface by registering functions
///
/// \param _strWriterDomain Domain for functions with write access. Each domain
///                         will have a separate queue for writer functions.
///                         This allows for multi-threading.
///
////////////////////////////////////////////////////////////////////////////////
inline void CComInterface::registerWriterDomain(const std::string& _strWriterDomain)
{
    METHOD_ENTRY_QUIET("CComInterface::registerWriterDomain")
    m_WriterDomains.emplace(_strWriterDomain);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Called in case of log entry, creating an event
///
/// \param _strSrc Source of log entry
/// \param _strMessage Log message
/// \param _Level Log level
/// \param _Domain Log domain
///
////////////////////////////////////////////////////////////////////////////////
inline void CComInterface::logEntry(const std::string& _strSrc, const std::string& _strMessage,
                                    const LogLevelType& _Level, const LogDomainType& _Domain)
{
    METHOD_ENTRY_QUIET("CComInterface::logEntry")
    
    this->call<void, std::string, std::string, std::string, std::string>("e_log_entry",
                _strSrc, _strMessage, s_LogLevelTypeToStringMap[_Level], s_LogDomainTypeToStringMap[_Domain]);
}

#include "com_interface.tpp"

#endif // COM_INTERFACE_H
