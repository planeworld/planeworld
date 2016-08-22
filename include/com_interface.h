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

//--- Misc header ------------------------------------------------------------//

/// Map of functions, accessed by name
typedef std::unordered_map<std::string, std::function<void(void)>> RegisteredFunctionsType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing an interface to the engine
///
////////////////////////////////////////////////////////////////////////////////
class CComInterface
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        
        //--- Constant Methods -----------------------------------------------//
        void help() const
        {
            METHOD_ENTRY("CComInterface::help")
            for (auto Com : m_RegisteredFunctions) std::cout << "Command: " << Com.first << std::endl;
        }
        
        //--- Methods --------------------------------------------------------//
        void call(const std::string& _strName)
        {
            METHOD_ENTRY("CComInterface::call")
            if (m_RegisteredFunctions.count(_strName) != 0)
                m_RegisteredFunctions[_strName]();
            else
            {
                WARNING_MSG("Com Interface", "Unknown function <" << _strName << ">. ")
            }
        }
        bool registerFunction(const std::string& _strName, const std::function<void(void)>& _Function)
        {
            METHOD_ENTRY("CComInterface::registerFunction")
            m_RegisteredFunctions[_strName] = _Function;
            return true;
        }
        
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComInterface&);
        friend std::ostream& operator<<(std::ostream&, CComInterface&);
        
    private:
        
        RegisteredFunctionsType m_RegisteredFunctions; ///< Registered functions provided by modules
      
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // COM_INTERFACE_H
