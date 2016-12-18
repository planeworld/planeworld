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
        virtual ~IComInterfaceProvider() {}

        //--- Methods --------------------------------------------------------//
        virtual void initComInterface() = 0;
        
        void setComInterface(CComInterface*);
        
    protected:
        
        CComInterface*  m_pComInterface;     ///< Pointer to com interface
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the com interface to be used
///
/// \param _pComInterface Instance of com interface to be used
///
////////////////////////////////////////////////////////////////////////////////
inline void IComInterfaceProvider::setComInterface(CComInterface* _pComInterface)
{
    METHOD_ENTRY("IComInterfaceProvider::setComInterface")
    
    if (m_pComInterface != nullptr)
    {
        NOTICE_MSG("Com Interface Provider", "Com interface instance already given, overwriting.")
    }
    
    m_pComInterface = _pComInterface;
}

#endif // COM_INTERFACE_PROVIDER_H
