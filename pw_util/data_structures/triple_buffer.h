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
/// \file       triple_buffer.h
/// \brief      Prototype of class "CTripleBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-14
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIPLE_BUFFER_H
#define TRIPLE_BUFFER_H

//--- Standard header --------------------------------------------------------//
#include <mutex>

//--- Program header ---------------------------------------------------------//
#include "log.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a triple buffer
///
/// This is the base definition to be specialised for different container types
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class... TArgs>
class CTripleBuffer{};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a triple buffer
///
/// This is a specialisation for unary containers that implement push_back,
/// e.g. std::vector
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
class CTripleBuffer<TContainer, TVal>
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CTripleBuffer();
        
        //--- Methods --------------------------------------------------------//
        TContainer* getFront();
        
        void add(const TVal&);
        void buffer(const TContainer&);
        void swapBack();
        void swapFront();

//         //--- friends --------------------------------------------------------//
//         template <class U>
//         friend std::istream&    operator>>(std::istream&, CTripleBuffer<U>&);
//         template <class U>
//         friend std::ostream&    operator<<(std::ostream&, const CTripleBuffer<U>&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        TContainer*   m_pFront;       ///< Pointer to front buffer
        TContainer*   m_pBackFront;   ///< Pointer to back buffer to be swapped with front buffer
        TContainer*   m_pBackBack;    ///< Backbuffer, save for update operations
        TContainer    m_Buffer0;      ///< Buffer 1/3
        TContainer    m_Buffer1;      ///< Buffer 2/3
        TContainer    m_Buffer2;      ///< Buffer 3/3
        
        std::mutex    m_Mutex;        ///< Mutex for thread safety
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that implements a triple buffer
///
/// This is a specialisation for binary containers that implement insert,
/// e.g. std::map
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
class CTripleBuffer<TContainer, TKey, TVal>
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CTripleBuffer();
        
        //--- Methods --------------------------------------------------------//
        TContainer* getFront();
        
        void add(const TKey&, const TVal&);
        void buffer(const TContainer&);
        void swapBack();
        void swapFront();

//         //--- friends --------------------------------------------------------//
//         template <class U>
//         friend std::istream&    operator>>(std::istream&, CTripleBuffer<U>&);
//         template <class U>
//         friend std::ostream&    operator<<(std::ostream&, const CTripleBuffer<U>&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        TContainer*   m_pFront;       ///< Pointer to front buffer
        TContainer*   m_pBackFront;   ///< Pointer to back buffer to be swapped with front buffer
        TContainer*   m_pBackBack;    ///< Backbuffer, save for update operations
        TContainer    m_Buffer0;      ///< Buffer 1/3
        TContainer    m_Buffer1;      ///< Buffer 2/3
        TContainer    m_Buffer2;      ///< Buffer 3/3
        
        std::mutex    m_Mutex;        ///< Mutex for thread safety
};


//--- Implementation is done here for inline optimisation --------------------//

#include "triple_buffer.tpp"

#endif // TRIPLE_BUFFER_H
