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
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "concurrentqueue.h"

/// Specifies a parameter type
enum class ParameterType
{
    UNDEFINED,
    NONE,
    BOOL,
    DOUBLE,
    INT,
    STRING,
    UID,
    VEC2DDOUBLE
};

/// Specifies the signature of the function
enum class SignatureType
{
    UNDEFINED,
    NONE,
    DOUBLE,
    DOUBLE_STRING,
    DOUBLE_STRING_DOUBLE,
    INT,
    INT_INT,
    NONE_BOOL,
    NONE_DOUBLE,
    NONE_2DOUBLE,
    NONE_INT,
    NONE_STRING,
    NONE_STRING_4DOUBLE,
    NONE_STRING_DOUBLE,
    NONE_UID,
    VEC2DDOUBLE_STRING,
    VEC2DDOUBLE_2STRING
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Specialised callback functions with writable access at com interface
///
/// This wrapper is specialised for functions with writable access. It stores
/// a function that will take the original function \ref CCommand which is
/// wrapped containing its parameters by \ref CCommandToQueueWrapper) and
/// puts them in a command queue.
/// Hence, those functions are not called immediately, but queued and then
/// executed, externally. Thus, given a thread save queue, commands can be 
/// excuted over different threads.
///
////////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
class CCommandWritable : public IBaseCommand
{
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CCommandWritable() = delete;
        CCommandWritable(const std::function<void(TArgs...)>&);
        
        //--- Constant methods -----------------------------------------------//
        std::function<void(TArgs...)> getFunction() const {return m_Function;}
        
        //--- Methods --------------------------------------------------------//
        TRet call(TArgs...);
        
    private:
        
        /// --- Methods [private] --------------------------------------------//
        void dispatchSignature();
        
        /// --- Variables [private] ------------------------------------------//
        std::function<void(TArgs...)> m_Function; ///< Function to be registered at com interface
        
};

/// Map of all functions, accessed by name
typedef std::map<std::string, IBaseCommand*> RegisteredFunctionsType;
/// Map of descriptions, accessed by name
typedef std::unordered_map<std::string, std::string> RegisteredFunctionsDescriptionType;
/// Parameter list for functions
typedef std::vector<std::pair<ParameterType, std::string>> ParameterListType;
/// Map of parameter lists, accessed by function name
typedef std::unordered_map<std::string, ParameterListType> RegisteredParameterListsType;
/// Map of writer flags, accessed by function name
typedef std::unordered_map<std::string, bool> WriterFlagsType;

/// Domain of function being registered
typedef std::string DomainType;
/// Map of domains, accessed by function name
typedef std::unordered_map<std::string, DomainType> RegisteredDomainsType;

/// List of writer domains
typedef std::unordered_set<std::string> WriterDomainsType;
/// Map of queues with one queue for each writer domain
typedef std::unordered_map<std::string, moodycamel::ConcurrentQueue<IBaseCommand*>> WriterQueuesType;

//--- Enum parser ------------------------------------------------------------//
static std::map<ParameterType, std::string> mapParameterToString = {
    {ParameterType::NONE, "<none>"},
    {ParameterType::BOOL, "<bool>"},
    {ParameterType::DOUBLE, "<double>"},
    {ParameterType::INT, "<int>"},
    {ParameterType::STRING, "<string>"},
    {ParameterType::UID, "<uid>"}
}; ///< Map from ParameterType to string

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing an interface to the engine
///
////////////////////////////////////////////////////////////////////////////////
class CComInterface
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CComInterface();
        virtual ~CComInterface();
        
        //--- Constant Methods -----------------------------------------------//
        RegisteredDomainsType*        getDomains()    {return &m_RegisteredFunctionsDomain;} 
        RegisteredFunctionsType*      getFunctions()  {return &m_RegisteredFunctions;} 
        WriterFlagsType*              getWriterFlags() {return &m_WriterFlags;}
        
        //--- Methods --------------------------------------------------------//
        template<class TRet, class... Args>
        TRet                call(const std::string&, Args...);
        const std::string   call(const std::string&);
        void                callWriters(const std::string&);
        void                help();
        void                help(int);
        
        template <class TRet, class... TArgs>
        bool registerFunction(const std::string&, const CCommand<TRet, TArgs...>&,
                              const std::string&,
                              const ParameterListType& = {},
                              const DomainType& = "",
                              const std::string& = "Reader"
        );

        void registerWriterDomain(const std::string&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComInterface&);
        friend std::ostream& operator<<(std::ostream&, CComInterface&);
        
    private:
        
        RegisteredFunctionsType             m_RegisteredFunctions;       ///< All registered functions provided by modules
        RegisteredFunctionsDescriptionType  m_RegisteredFunctionsDescriptions; ///< Descriptions of registered functions
        RegisteredParameterListsType        m_RegisteredFunctionsParams; ///< Parameter lists of registered functions      
        RegisteredDomainsType               m_RegisteredFunctionsDomain; ///< Domain of registered functions
        
        WriterDomainsType                   m_WriterDomains;             ///< Domains for queued functions
        WriterQueuesType                    m_WriterQueues;              ///< Command queues for write access
        WriterFlagsType                     m_WriterFlags;               ///< Flags for writer functions
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
    METHOD_ENTRY("CComInterface::registerWriterDomain")
    m_WriterDomains.emplace(_strWriterDomain);
}


#include "com_interface.tpp"

#endif // COM_INTERFACE_H
