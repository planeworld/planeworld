////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2016 Torsten Büschenfeld
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
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//

/// Specifies a parameter type
enum class ParameterType
{
    NONE,
    CUSTOM_OBJ,
    BOOL,
    DOUBLE,
    INT,
    STRING,
    UID
};

/// Specifies the signature of the function
enum class SignatureType
{
    NONE,
    CUSTOM_OBJ,
    NONE_BOOL,
    NONE_DOUBLE,
    NONE_INT,
    NONE_STRING,
    NONE_STRING_DOUBLE,
    NONE_UID
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
        CCommand(const std::function<TRet(TArgs...)>&);
        
        //--- Constant methods -----------------------------------------------//
        std::function<TRet(TArgs...)> getFunction() const {return m_Function;}
        
        //--- Methods --------------------------------------------------------//
        TRet call(TArgs...);
        
    private:
        
        std::function<TRet(TArgs...)> m_Function; ///< Function to be registered at com interface
};

/// Map of functions, accessed by name
typedef std::map<std::string, IBaseCommand*> RegisteredFunctionsType;
/// Map of descriptions, accessed by name
typedef std::unordered_map<std::string, std::string> RegisteredFunctionsDescriptionType;
/// Parameter list for functions
typedef std::vector<std::pair<ParameterType, std::string>> ParameterListType;
/// Map of parameter lists, accessed by function name
typedef std::unordered_map<std::string, ParameterListType> RegisteredParameterListsType;
/// Map of signatures, accessed by function name
typedef std::unordered_map<std::string, SignatureType> RegisteredSignaturesType;
/// Domain of function being registered
typedef std::string DomainType;
/// Map of domains, accessed by function name
typedef std::unordered_map<std::string, DomainType> RegisteredDomainsType;

//--- Enum parser ------------------------------------------------------------//
static std::map<ParameterType, std::string> mapParameterToString = {
    {ParameterType::NONE, "<none>"},
    {ParameterType::CUSTOM_OBJ, "<custom_object>"},
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
        RegisteredSignaturesType*     getSignatures() {return &m_RegisteredSignatures;}
        
        //--- Methods --------------------------------------------------------//
        template<class TRet, class... Args>
        TRet                call(const std::string& _strName, Args...);
        const std::string&  call(const std::string&);
        void                help();
        void                help(int);
        
        template <class T>
        bool registerFunction(const std::string&, const T&,
                              const std::string&,
                              const SignatureType&,
                              const ParameterListType& = {},
                              const DomainType& = ""
        );
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComInterface&);
        friend std::ostream& operator<<(std::ostream&, CComInterface&);
        
    private:
        
        RegisteredFunctionsType             m_RegisteredFunctions;       ///< Registered functions provided by modules
        RegisteredFunctionsDescriptionType  m_RegisteredFunctionsDescriptions; // Descriptions of registered functions
        RegisteredParameterListsType        m_RegisteredFunctionsParams; ///< Parameter lists of registered functions      
        RegisteredSignaturesType            m_RegisteredSignatures;      ///< Signatures of the registered functions
        RegisteredDomainsType               m_RegisteredFunctionsDomain; ///< Domain of registered functions
};

//--- Implementation is done here for inline optimisation --------------------//

#include "com_interface.tpp"

#endif // COM_INTERFACE_H
