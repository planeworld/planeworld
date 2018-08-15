////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017-2018 Torsten Büschenfeld
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
/// \file       com_interface_provider.h
/// \brief      Prototype of interface "IComInterfaceProvider"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-12-11
///
////////////////////////////////////////////////////////////////////////////////

#ifndef COM_INTERFACE_PROVIDER_H
#define COM_INTERFACE_PROVIDER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "com_interface.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that provide functions for the com interface
///
////////////////////////////////////////////////////////////////////////////////
class IComInterfaceProvider
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        IComInterfaceProvider() : m_pComInterface(nullptr){}

        //--- Methods --------------------------------------------------------//
        void initComInterface(CComInterface* const, const std::string&);
        
    protected:
        
        //--- Protected methods --------------------------------------------------------//
        virtual void myInitComInterface() = 0;
        
        CComInterface*  m_pComInterface;     ///< Pointer to com interface
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the com interface by registering functions
///
/// \param _pComInterface Instance of com interface to be used
/// \param _strWriterDomain Domain for functions with write access. Each domain
///                         will have a separate queue for writer functions.
///                         This allows for multi-threading.
///
////////////////////////////////////////////////////////////////////////////////
inline void IComInterfaceProvider::initComInterface(CComInterface* const _pComInterface,
                                                   const std::string& _strWriterDomain)
{
    METHOD_ENTRY("IComInterfaceProvider::initComInterface")

    DOM_DEV(    
        if (_pComInterface == nullptr)
        {
            WARNING_MSG("Com Interface Provider", "No valid com interface set")
            goto DomDevNoComInterface;
        }
    )
    
    if (m_pComInterface != nullptr)
    {
        NOTICE_MSG("Com Interface Provider", "Com interface instance already given, overwriting.")
    }
    
    m_pComInterface = _pComInterface;
    m_pComInterface->registerWriterDomain(_strWriterDomain);
    
    this->myInitComInterface();
    
    DOM_DEV(DomDevNoComInterface:)
}

#endif // COM_INTERFACE_PROVIDER_H
