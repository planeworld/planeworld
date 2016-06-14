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
/// \file       circular_buffer.h
/// \brief      Prototype of class "CCircularBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-02-01
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a static circular buffer using std::vector
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CCircularBuffer
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CCircularBuffer();
        CCircularBuffer(const std::size_t&);
        
        //--- Constant Methods -----------------------------------------------//
        const T& operator[](const std::size_t&) const;
        const T& at        (const std::size_t&) const;
        
        std::size_t capacity() const;
        std::size_t size() const;
        
        //--- Methods --------------------------------------------------------//
        T&   operator[](const std::size_t&);
        T&   at        (const std::size_t&);
        void push_back(const T&);
        void reserve(const std::size_t&);
        
        //--- friends --------------------------------------------------------//
        template <class U>
        friend std::istream&    operator>>(std::istream&, CCircularBuffer<U>&);
        template <class U>
        friend std::ostream&    operator<<(std::ostream&, const CCircularBuffer<U>&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        size_t              m_nCapacity = 1;    ///< Capacity of the buffer
        size_t              m_nBegin = 0;       ///< Index for first buffer entry
        size_t              m_nEnd =  0;        ///< Index for last buffer entry
        size_t              m_nSize = 0;        ///< Size from begin to end of buffer
        std::vector<T>      m_Buffer;           ///< Buffer to store U elements of type T
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the capacity of the buffer
///
/// \return Capacity of the buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline std::size_t CCircularBuffer<T>::capacity() const
{
    METHOD_ENTRY("CCircularBuffer::capacity")
    return m_nCapacity;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the size of the buffer
///
/// \return Size of the buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline std::size_t CCircularBuffer<T>::size() const
{
    METHOD_ENTRY("CCircularBuffer::size")
    return m_nSize;
}

#include "circular_buffer.tpp"

#endif // CIRCULAR_BUFFER_H
