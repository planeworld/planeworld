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
/// \brief This struct defines the numeric handle value
///
/// The index is the actual handle id, the counter is used to manage stale
/// handles while the type is used for casting, defining the pointer type.
///
////////////////////////////////////////////////////////////////////////////////
struct HanldeID
{
    union
    {
        struct
        {
            std::uint32_t   Index;
            std::uint8_t    Counter;
            std::uint8_t    Type;
        };
        std::uint64_t       Value;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that refer to an unique id.
///
////////////////////////////////////////////////////////////////////////////////
class CHandle
{

    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CHandle() : m_strName("UID_0"), m_UID(0u), m_pRef(nullptr){}
        CHandle(T* _pT) {this->set(_pT);}
   
        //--- Operators ------------------------------------------------------//
        T* operator->() const {return this->ptr();}
        T& operator*() {return *m_pRef;}
   
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
        UIDType             getUID() const;
        bool                isValid() const;
        T*                  ptr() const;
        
        //--- Methods --------------------------------------------------------//
        void set(T* const);
        void setPtr(T* const _pPtr) {m_pRef = _pPtr;}
        
    private:
        
        //--- Variables [static, private] ------------------------------------//
        static CHandleManager   ms_Handlemanager; ///< Static handle manager instance
        
        //--- Variables [private] --------------------------------------------//
        
        
        UIDType         m_UID;        ///< Reference to unique identifier 
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // HANDLE_H
