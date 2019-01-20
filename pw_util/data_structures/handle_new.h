////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2019 Torsten Büschenfeld
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
/// \date       2019-01-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HANDLE_H
#define HANDLE_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>

//--- Program header ---------------------------------------------------------//
#include "handle_manager.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Helper class for providing one handle manager instance for all handles
///
/// Since CHandle is a templated class, a static member would be instanciated
/// for each template parameter. Therefore, this helper instanciates exactly one
/// Handle manager that all CHandle instanciation will inherit from.
///
////////////////////////////////////////////////////////////////////////////////
class CHandleBase
{
    public:
        static const std::deque<std::uint32_t>*     getFreeHandles();
        static const std::vector<HandleMapEntry>*   getHandleMap();
    
    protected:
        //--- Variables [static, private] ------------------------------------//
        static CHandleManager   s_HandleManager; ///< Static handle manager instance
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class implementing a handle to templated objects
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
class CHandle : public CHandleBase
{

    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CHandle(T*);
        CHandle(void){}
   
        //--- Operators ------------------------------------------------------//
        T* operator->() const;
        T& operator*() const;
   
        //--- Constant Methods -----------------------------------------------//
        HandleID ID() const {return m_ID;}
        
        bool isValid() const;
        T*   ptr() const;
        
        //--- Methods --------------------------------------------------------//
        bool remove();
        void update(T* const);
        
    private:
        
        //--- Variables [private] --------------------------------------------//
        HandleID m_ID;   ///< Numeric value coding id (index) and internal meta data
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising handle with given pointer
///
/// \param _ptr Pointer to initialise data with
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline CHandle<T>::CHandle(T* _ptr)
{
    METHOD_ENTRY("CHandle::CHandle")
    m_ID = s_HandleManager.add(_ptr);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests handle for validity
///
/// \return Handle valid (true/false)?
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool CHandle<T>::isValid() const
{
    METHOD_ENTRY("CHandle::isValid")
    return s_HandleManager.isValid(m_ID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns pointer which is represented by handle
///
/// \return Pointer represented by handle
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* CHandle<T>::operator->() const
{
    METHOD_ENTRY("CHandle::operator->")
    return s_HandleManager.get<T>(m_ID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Derefences pointer returning the content/value
///
/// \return Content/value referenced by handle
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline T& CHandle<T>::operator*() const
{
    METHOD_ENTRY("CHandle::operator*")
    return *s_HandleManager.get<T>(m_ID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns pointer which is represented by handle
///
/// Since access is given by \ref operator-> and \ref operator* this method is
/// typically not needed. If used to copy the pointer, only use it locally and
/// with caution, if you definitely know about the pointers validity.
///
/// \return Pointer represented by handle
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* CHandle<T>::ptr() const
{
    METHOD_ENTRY("CHandle::ptr")
    return s_HandleManager.get<T>(m_ID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Removes handle from handle manager, all other instances of this 
///        handle will become invalid.
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool CHandle<T>::remove()
{
    METHOD_ENTRY("CHandle::remove")
    return CHandleBase::s_HandleManager.remove(m_ID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Updates handle with new pointer, all other instances of this handle 
///        will become invalid.
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void CHandle<T>::update(T* const _ptr)
{
    METHOD_ENTRY("CHandle::update")
    CHandleBase::s_HandleManager.update<T>(m_ID, _ptr);
}

#endif // HANDLE_H
