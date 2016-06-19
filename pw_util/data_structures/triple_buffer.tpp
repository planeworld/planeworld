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
/// \file       triple_buffer.tpp
/// \brief      Implementation of class "CTripleBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-14
///
////////////////////////////////////////////////////////////////////////////////

#include "triple_buffer.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising pointers
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
CTripleBuffer<TContainer, TVal>::CTripleBuffer()
{
    METHOD_ENTRY("CTripleBuffer::CTripleBuffer")
    CTOR_CALL("CTripleBuffer::CTripleBuffer")
    
    m_pFront     = &m_Buffer0;
    m_pBackFront = &m_Buffer1;
    m_pBackBack  = &m_Buffer2;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising pointers
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
CTripleBuffer<TContainer, TKey, TVal>::CTripleBuffer()
{
    METHOD_ENTRY("CTripleBuffer::CTripleBuffer")
    CTOR_CALL("CTripleBuffer::CTripleBuffer")
    
    m_pFront     = &m_Buffer0;
    m_pBackFront = &m_Buffer1;
    m_pBackBack  = &m_Buffer2;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Buffer given container to back buffers
///
/// \param _Container Container to be buffered
///
/// \note At the moment, this does not involve deep copying. This might later
///       be implemented by using std::transform
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
void CTripleBuffer<TContainer, TVal>::buffer(const TContainer& _Container)
{
    METHOD_ENTRY("CTripleBuffer::buffer")
    m_Buffer0 = _Container;
    m_Buffer1 = _Container;
    m_Buffer2 = _Container;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Buffer given container to back buffers
///
/// \param _Container Container to be buffered
///
/// \note At the moment, this does not involve deep copying. This might later
///       be implemented by using std::transform
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
void CTripleBuffer<TContainer, TKey, TVal>::buffer(const TContainer& _Container)
{
    METHOD_ENTRY("CTripleBuffer::buffer")
    m_Buffer0 = _Container;
    m_Buffer1 = _Container;
    m_Buffer2 = _Container;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer
///
/// \return Back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
TContainer* CTripleBuffer<TContainer, TVal>::getBack()
{
    METHOD_ENTRY("CTripleBuffer::getBack")
    return m_pBackBack;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns back buffer
///
/// \return Back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
TContainer* CTripleBuffer<TContainer, TKey, TVal>::getBack()
{
    METHOD_ENTRY("CTripleBuffer::getBack")
    return m_pBackBack;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer
///
/// \return Front buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
TContainer* CTripleBuffer<TContainer, TVal>::getFront()
{
    METHOD_ENTRY("CTripleBuffer::getFront")
    return m_pFront;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns front buffer
///
/// \return Front buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
TContainer* CTripleBuffer<TContainer, TKey, TVal>::getFront()
{
    METHOD_ENTRY("CTripleBuffer::getFront")
    return m_pFront;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an element to buffer.
///
/// \param _Val Element to add to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
void CTripleBuffer<TContainer, TVal>::add(const TVal& _Val)
{
    METHOD_ENTRY("CTripleBuffer::add")
    
    m_Buffer0.push_back(_Val);
    m_Buffer1.push_back(_Val);
    m_Buffer2.push_back(_Val);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a key-value pair to buffer.
///
/// \param _Key Key for insertion to buffer
/// \param _Val Value to add to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
void CTripleBuffer<TContainer, TKey, TVal>::add(const TKey& _Key, const TVal& _Val)
{
    METHOD_ENTRY("CTripleBuffer::add")
    
    m_Buffer0.insert(std::pair<TKey,TVal>(_Key,_Val));
    m_Buffer1.insert(std::pair<TKey,TVal>(_Key,_Val));
    m_Buffer2.insert(std::pair<TKey,TVal>(_Key,_Val));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap front buffer with active back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
void CTripleBuffer<TContainer, TVal>::swapBack()
{
    METHOD_ENTRY("CTripleBuffer::swapBack")
    m_Mutex.lock();
    std::swap(m_pBackFront, m_pBackBack);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap front buffer with active back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
void CTripleBuffer<TContainer, TKey, TVal>::swapBack()
{
    METHOD_ENTRY("CTripleBuffer::swapBack")
    m_Mutex.lock();
    std::swap(m_pBackFront, m_pBackBack);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap front buffer with active back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TVal>
void CTripleBuffer<TContainer, TVal>::swapFront()
{
    METHOD_ENTRY("CTripleBuffer::swapFront")
    m_Mutex.lock();
    std::swap(m_pFront, m_pBackFront);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap front buffer with active back buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class TContainer, class TKey, class TVal>
void CTripleBuffer<TContainer, TKey, TVal>::swapFront()
{
    METHOD_ENTRY("CTripleBuffer::swapFront")
    m_Mutex.lock();
    std::swap(m_pFront, m_pBackFront);
    m_Mutex.unlock();
}
