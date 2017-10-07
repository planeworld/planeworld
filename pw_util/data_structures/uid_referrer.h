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
/// \file       uid_referrer.h
/// \brief      Prototype of interface "IUIDReferrer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-01
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UID_REFERRER_H
#define UID_REFERRER_H

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
class IUIDReferrer
{

    public:
   
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getNameRef() const;
        T*                  getRef() const;
        UIDType             getUIDRef() const;
        bool                hasRef() const;
        
        //--- Methods --------------------------------------------------------//
        void setRef(T* const);
        
    protected:
        
        //--- Variables [protected] ------------------------------------------//
        std::string     m_strUIDName = "UID_0";  ///< Reference to named uid
        UIDType         m_UIDRef = 0u;           ///< Reference to unique identifier 
        T*              m_pRef = nullptr;        ///< Pointer reference to UID user
        
        //--- Methods [protected] --------------------------------------------//
        virtual void mySetRef();
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Refers to UID user
///
/// \param _pRef UID user to refer to
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline void IUIDReferrer<T>::setRef(T* const _pRef)
{
    METHOD_ENTRY("IUIDReferrer::setRef")
    m_strUIDName = _pRef->getName();
    m_pRef   = _pRef;
    m_UIDRef = _pRef->getUID();
    
    this->mySetRef();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to named uid of UID user
///
/// \return Reference to named of UID user
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline const std::string& IUIDReferrer<T>::getNameRef() const
{
    METHOD_ENTRY("IUIDReferrer::getNameRef")
    return m_strUIDName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to UID user
///
/// \return Reference to UID user
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline T* IUIDReferrer<T>::getRef() const
{
    METHOD_ENTRY("IUIDReferrer::getRef")
    return m_pRef;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to unique id of UID user
///
/// \return Reference to unique id of UID user
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline UIDType IUIDReferrer<T>::getUIDRef() const
{
    METHOD_ENTRY("IUIDReferrer::getUIDRef")
    return m_UIDRef;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Indicates if reference is given
///
/// \return Got reference?
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline bool IUIDReferrer<T>::hasRef() const
{
    METHOD_ENTRY("IUIDReferrer::hasRef")
    if (m_UIDRef == 0u) return false;
    else return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to give inherited classes the opportunity to do additional
///        things when attaching.
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline void IUIDReferrer<T>::mySetRef()
{
    METHOD_ENTRY("IUIDReferrer::mySetRef")
}

#endif // UID_REFERRER_H
