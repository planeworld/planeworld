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
/// \file       unique_id_referrer.h
/// \brief      Prototype of interface "IUniqueIDReferrer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-01
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIQUE_ID_REFERRER_H
#define UNIQUE_ID_REFERRER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "unique_id.h"
#include "unique_id_user.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that refer to an unique id.
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
class IUniqueIDReferrer
{

    public:
   
        //--- Constant Methods -----------------------------------------------//
        T*              getRef() const;
        UIDType         getUIDRef() const;
        
        //--- Methods --------------------------------------------------------//
        void attachTo(T* const);
        
    protected:
        
        //--- Variables [protected] ------------------------------------------//
        UIDType         m_UIDRef = 0u;      ///< Reference to unique identifier 
        T*              m_pRef = nullptr;   ///< Pointer reference to UID user
        
        //--- Methods [protected] --------------------------------------------//
        virtual void myAttachTo();
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches to UID user
///
/// \param _pRef UID user to attach to
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline void IUniqueIDReferrer<T>::attachTo(T* const _pRef)
{
    METHOD_ENTRY("IUniqueIDReferrer::attachTo")
    m_pRef   = _pRef;
    m_UIDRef = _pRef->getUID();
    
    this->myAttachTo();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to UID user
///
/// \return Reference to UID user
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline T* IUniqueIDReferrer<T>::getRef() const
{
    METHOD_ENTRY("IUniqueIDReferrer::getRef")
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
inline UIDType IUniqueIDReferrer<T>::getUIDRef() const
{
    METHOD_ENTRY("IUniqueIDReferrer::getUIDRef")
    return m_UIDRef;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to give inherited classes the opportunity to do additional
///        things when attaching.
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline void IUniqueIDReferrer<T>::myAttachTo()
{
    METHOD_ENTRY("IUniqueIDReferrer::myAttachTo")
}

#endif // UNIQUE_ID_REFERRER_H
