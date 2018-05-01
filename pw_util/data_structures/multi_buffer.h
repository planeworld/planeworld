////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2018 Torsten Büschenfeld
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
/// \file       multi_buffer.h
/// \brief      Prototype of class "CMultiBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-07-17
///
////////////////////////////////////////////////////////////////////////////////

#ifndef MULTI_BUFFER_H
#define MULTI_BUFFER_H

//--- Standard header --------------------------------------------------------//
#include <array>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "serializable.h"


//--- Constants for readable access ------------------------------------------//
constexpr std::uint8_t BUFFER_DOUBLE = 2u;          ///< Number of buffers in double buffer
constexpr std::uint8_t BUFFER_TRIPLE = 3u;          ///< Number of buffers in triple buffer
constexpr std::uint8_t BUFFER_QUADRUPLE = 4u;       ///< Number of buffers in quadruple buffer
constexpr std::uint8_t BUFFER_FRONT = 0u;           ///< Index front buffer
constexpr std::uint8_t BUFFER_DOUBLE_FRONT = 0u;    ///< Index front buffer in double buffer
constexpr std::uint8_t BUFFER_TRIPLE_FRONT = 0u;    ///< Index front buffer in triple buffer
constexpr std::uint8_t BUFFER_QUADRUPLE_FRONT = 0u; ///< Index front buffer in quadruple buffer
constexpr std::uint8_t BUFFER_DOUBLE_BACK = 1u;     ///< Index back buffer in double buffer
constexpr std::uint8_t BUFFER_TRIPLE_BACK = 2u;     ///< Index back buffer in triple buffer
constexpr std::uint8_t BUFFER_QUADRUPLE_BACK = 3u;  ///< Index back buffer in quadruple buffer
constexpr std::uint8_t BUFFER_TRIPLE_MIDDLE = 1u;   ///< Index middle buffer in triple buffer
constexpr std::uint8_t BUFFER_QUADRUPLE_MIDDLE_BACK = 2u;   ///< Index second middle buffer in quadruple buffer
constexpr std::uint8_t BUFFER_QUADRUPLE_MIDDLE_FRONT = 1u;  ///< Index first middle buffer in quadruple buffer

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a multi buffer
///
/// This is the base definition to be specialised for different container types
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class... TArgs>
class CMultiBuffer{};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a multi buffer
///
/// This is a specialisation for single classes
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
class CMultiBuffer<N,T> : public ISerializable
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CMultiBuffer();
        
        //--- Constant methods -----------------------------------------------//
        std::uint8_t getBufferSize() const {return N;}
        std::size_t  getContainerSize() const {return 1;}
        
        //--- Methods --------------------------------------------------------//
        template<std::uint8_t I>
        T* getBuffer();
        
        void add(const T&);
        void add(const std::array<T, N>&);
        
        void resizeBuffer(const std::size_t _nSize);
                
        void buffer(const T&);
        template<std::uint8_t I, std::uint8_t J>
        void swap();

    private:

        //--- Variables [private] --------------------------------------------//
        std::array<T*,N>    m_BufferRef;    ///< References to buffers
        std::array<T ,N>    m_Buffer;       ///< Buffers
        
        SERIALIZE_DECL
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a multi buffer
///
/// This is a specialisation for unary containers that implement push_back,
/// e.g. std::vector
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
class CMultiBuffer<N, TContainer, TVal> : public ISerializable
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CMultiBuffer();
        
        //--- Constant methods -----------------------------------------------//
        std::uint8_t        getBufferSize() const {return N;}
        std::size_t         getContainerSize() const {return m_Buffer[0].size();}
        
        //--- Methods --------------------------------------------------------//
        template<std::uint8_t I>
        TContainer*         getBuffer();
        std::array<TVal,N>  getElementAll(const std::uint32_t&);
                
        void add(const TVal&);
        void add(const std::array<TVal, N>&);
        template<std::uint8_t I, std::uint8_t J>
        void copyDeep();
        void fillBuffer(const TVal&);
        void resizeBuffer(const std::size_t _nSize);
        
        void setAt(const std::uint32_t, const TVal&);
        void setAt(const std::uint32_t, const std::array<TVal, N>&);
        template<std::uint8_t I, std::uint8_t J>
        void swap();
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        std::array<TContainer*,N>   m_BufferRef;    ///< References to buffers
        std::array<TContainer ,N>   m_Buffer;       ///< Buffers
        
        SERIALIZE_DECL
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a multi buffer
///
/// This is a specialisation for binary containers that implement insert,
/// e.g. std::map
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
class CMultiBuffer<N, TContainer, TKey, TVal> : public ISerializable
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CMultiBuffer();
        
        //--- Constant methods -----------------------------------------------//
        std::uint8_t getBufferSize() const {return N;}
        std::size_t  getContainerSize() const {return m_Buffer[0].size();}
        
        //--- Methods --------------------------------------------------------//
        template<std::uint8_t I>
        TContainer*         getBuffer();
        std::array<TVal,N>  getElementAll(const TKey&);
        
        void add(const TKey&, const TVal&);
        void add(const TKey&, const std::array<TVal, N>&);
        template<std::uint8_t I, std::uint8_t J>
        void copyDeep();
        
        void fillBuffer(const TVal&);
        void resizeBuffer(const std::size_t _nSize);
        
        template<std::uint8_t I, std::uint8_t J>
        void swap();

    private:
        
        //--- Variables [private] --------------------------------------------//
        std::array<TContainer*,N>   m_BufferRef;    ///< References to buffers
        std::array<TContainer ,N>   m_Buffer;       ///< Buffers
        
        SERIALIZE_DECL
};


//--- Implementation is done here for inline optimisation --------------------//

#include "multi_buffer.tpp"

#endif // MULTI_BUFFER_H
