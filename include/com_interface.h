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
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Base class for callback functions registered at com interface
///
////////////////////////////////////////////////////////////////////////////////
class IBaseComCallback
{
    public:
        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IBaseComCallback(){}
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Specialised callback functions registered at com interface
///
////////////////////////////////////////////////////////////////////////////////
template <class TRet, class... TArgs>
class CComCallback : public IBaseComCallback
{
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CComCallback(const std::function<TRet(TArgs...)>&);
        
        //--- Methods --------------------------------------------------------//
        TRet call(TArgs...);
        
    private:
        
        std::function<TRet(TArgs...)> m_Function; ///< Function to be registered at com interface
};

/// Map of functions, accessed by name
typedef std::unordered_map<std::string, IBaseComCallback*> RegisteredFunctionsType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing an interface to the engine
///
////////////////////////////////////////////////////////////////////////////////
class CComInterface
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        ~CComInterface();
        
        //--- Constant Methods -----------------------------------------------//
        void help() const;
        
        //--- Methods --------------------------------------------------------//
        template<class TRet, class... Args>
        TRet call(const std::string& _strName, Args...);
        
        bool registerFunction(const std::string&, IBaseComCallback* const);
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComInterface&);
        friend std::ostream& operator<<(std::ostream&, CComInterface&);
        
    private:
        
        RegisteredFunctionsType m_RegisteredFunctions; ///< Registered functions provided by modules
      
};

//--- Implementation is done here for inline optimisation --------------------//

#include "com_interface.tpp"

#endif // COM_INTERFACE_H
