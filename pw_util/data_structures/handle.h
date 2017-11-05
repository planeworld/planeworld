////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
/// \file       handle.h
/// \brief      Prototype of class "CHandle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-11-01
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HANDLE_H
#define HANDLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "uid.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that refer to an unique id.
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CHandle
{

    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CHandle() : m_strName("UID_0"), m_UID(0u), m_pRef(nullptr){}
        CHandle(T* _pT) {this->set(_pT);}
   
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
        T*                  get() const;
        UIDType             getUID() const;
        bool                isValid() const;
        
        //--- Methods --------------------------------------------------------//
        void set(T* const);
        
    private:
        
        //--- Variables [protected] ------------------------------------------//
        std::string     m_strName;    ///< Reference to named uid
        UIDType         m_UID;        ///< Reference to unique identifier 
        T*              m_pRef;       ///< Pointer reference to UID user
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets handle
///
/// \param _pRef Reference of this handle
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline void CHandle<T>::set(T* const _pRef)
{
    METHOD_ENTRY("CHandle::set")
    m_pRef   = _pRef;
    m_strName = _pRef->getName();
    m_UID = _pRef->getUID();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns name
///
/// \return Name
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline const std::string& CHandle<T>::getName() const
{
    METHOD_ENTRY("CHandle::getName")
    return m_strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to UID user
///
/// \return Reference to UID user
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline T* CHandle<T>::get() const
{
    METHOD_ENTRY("CHandle::get")
    return m_pRef;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns UID
///
/// \return UID
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline UIDType CHandle<T>::getUID() const
{
    METHOD_ENTRY("CHandle::getUID")
    return m_UID;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Indicates if handle is valid
///
/// \return Valid handle?
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline bool CHandle<T>::isValid() const
{
    METHOD_ENTRY("CHandle::isValid")
    return (m_UID == 0u) ? false : true;
}

#endif // HANDLE_H
