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
/// \file       multi_buffer.tpp
/// \brief      Implementation of class "CMultiBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-07-17
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising pointers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
CMultiBuffer<N,T>::CMultiBuffer()
{
    METHOD_ENTRY("CMultiBuffer::CMultiBuffer")
    CTOR_CALL("CMultiBuffer::CMultiBuffer")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising pointers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
CMultiBuffer<N, TContainer, TVal>::CMultiBuffer()
{
    METHOD_ENTRY("CMultiBuffer::CMultiBuffer")
    CTOR_CALL("CMultiBuffer::CMultiBuffer")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising pointers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
CMultiBuffer<N, TContainer, TKey, TVal>::CMultiBuffer()
{
    METHOD_ENTRY("CMultiBuffer::CMultiBuffer")
    CTOR_CALL("CMultiBuffer::CMultiBuffer")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
    m_Mutex.unlock();
}

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Buffer given class object to back buffers
// ///
// /// \param _CObj Class object to be buffered
// ///
// /// \note At the moment, this does not involve deep copying. This might later
// ///       be implemented by using std::transform
// ///
// ////////////////////////////////////////////////////////////////////////////////
// template<std::uint8_t N, class T>
// void CMultiBuffer<N,T>::buffer(const T& _CObj)
// {
//     METHOD_ENTRY("CMultiBuffer::buffer")
//     m_Buffer0 = _CObj;
//     m_Buffer1 = _CObj;
//     m_Buffer2 = _CObj;
// }
// 
// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Buffer given container to back buffers
// ///
// /// \param _Container Container to be buffered
// ///
// /// \note At the moment, this does not involve deep copying. This might later
// ///       be implemented by using std::transform
// ///
// ////////////////////////////////////////////////////////////////////////////////
// template<std::uint8_t N, class TContainer, class TVal>
// void CMultiBuffer<N, TContainer, TVal>::buffer(const TContainer& _Container)
// {
//     METHOD_ENTRY("CMultiBuffer::buffer")
//     m_Buffer0 = _Container;
//     m_Buffer1 = _Container;
//     m_Buffer2 = _Container;
// }
// 
// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Buffer given container to back buffers
// ///
// /// \param _Container Container to be buffered
// ///
// /// \note At the moment, this does not involve deep copying. This might later
// ///       be implemented by using std::transform
// ///
// ////////////////////////////////////////////////////////////////////////////////
// template<std::uint8_t N, class TContainer, class TKey, class TVal>
// void CMultiBuffer<N, TContainer, TKey, TVal>::buffer(const TContainer& _Container)
// {
//     METHOD_ENTRY("CMultiBuffer::buffer")
//     m_Buffer0 = _Container;
//     m_Buffer1 = _Container;
//     m_Buffer2 = _Container;
// }

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \param _unBuf Buffer to be returned
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
T* CMultiBuffer<N,T>::getBuffer(const std::uint8_t _unBuf)
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[_unBuf];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \param _unBuf Buffer to be returned
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
TContainer* CMultiBuffer<N, TContainer, TVal>::getBuffer(const std::uint8_t _unBuf)
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[_unBuf];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \param _unBuf Buffer to be returned
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
TContainer* CMultiBuffer<N, TContainer, TKey, TVal>::getBuffer(const std::uint8_t _unBuf)
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[_unBuf];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns element and its buffered elements
///
/// \param _unElem Element to be returned
///
/// \return Element and its buffered elements
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
std::array<TVal, N> CMultiBuffer<N, TContainer, TVal>::getElementAll(const std::uint32_t& _unElem)
{
    METHOD_ENTRY("CMultiBuffer::getElementAll")
    std::array<TVal, N> aElems;
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        aElems[i] = m_Buffer[i][_unElem];
    m_Mutex.unlock();
    return aElems;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns element and its buffered elements
///
/// \param _Elem Element to be returned
///
/// \return Element and its buffered elements
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
std::array<TVal, N> CMultiBuffer<N, TContainer, TKey, TVal>::getElementAll(const TKey& _Elem)
{
    METHOD_ENTRY("CMultiBuffer::getElementAll")
    std::array<TVal, N> aElems;
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        aElems[i] = m_Buffer[i][_Elem];
    m_Mutex.unlock();
    return aElems;
}

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Sets class object to be buffered.
// ///
// /// \param _CObj Class object to be buffered
// ///
// ////////////////////////////////////////////////////////////////////////////////
// template<std::uint8_t N, class T>
// void CMultiBuffer<N,T>::set(const T& _CObj)
// {
//     METHOD_ENTRY("CMultiBuffer::add")
//     
//     m_Buffer0.push_back(_CObj);
//     m_Buffer1.push_back(_CObj);
//     m_Buffer2.push_back(_CObj);
// }

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an element to buffer, elements will be copied
///
/// \param _Val Element to add to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::add(const TVal& _Val)
{
    METHOD_ENTRY("CMultiBuffer::add")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].push_back(_Val);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an element to buffer, all elements are given
///
/// \param _aVal Element to add to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::add(const std::array<TVal, N>& _aVal)
{
    METHOD_ENTRY("CMultiBuffer::add")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].push_back(_aVal[i]);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a key-value pair to buffer, elements will be copied
///
/// \param _Key Key for insertion to buffer
/// \param _Val Value to be added to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::add(const TKey& _Key,
                                                  const TVal& _Val)
{
    METHOD_ENTRY("CMultiBuffer::add")

    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].insert(std::pair<TKey,TVal>(_Key,_Val));
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a key-value pair to buffer, all elements are given
///
/// \param _Key Key for insertion to buffer
/// \param _aVal Value array to be added to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::add(const TKey& _Key,
                                                  const std::array<TVal, N>& _aVal
                                               )
{
    METHOD_ENTRY("CMultiBuffer::add")

    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].insert(std::pair<TKey,TVal>(_Key,_aVal[i]));
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deep copy of buffer content
///
/// \param _unBuf0 Buffer to copy content from
/// \param _unBuf1 Buffer to copy content to
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::copyDeep(const std::uint8_t _unBuf0, const std::uint8_t _unBuf1)
{
    METHOD_ENTRY("CMultiBuffer::copyDeep")
    m_Mutex.lock();
    for (auto i=0u; i<m_BufferRef[_unBuf0].size(); ++i)
    {
        (*(m_BufferRef[_unBuf1])) = (*(m_BufferRef[_unBuf0]));
    }
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deep copy of buffer content
///
/// \param _unBuf0 Buffer to copy content from
/// \param _unBuf1 Buffer to copy content to
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::copyDeep(const std::uint8_t _unBuf0, const std::uint8_t _unBuf1)
{
    METHOD_ENTRY("CMultiBuffer::copyDeep")
    m_Mutex.lock();
    auto it = m_BufferRef[_unBuf1]->begin();
    for (auto ci  = m_BufferRef[_unBuf0]->cbegin(); ci != m_BufferRef[_unBuf0]->end(); ++ci)
    {
        *(it->second) = *(ci->second);
        ++it;
    }
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an element to buffer at given position, elements will be copied
///
/// \param _nPos Position to set element at
/// \param _Val Element to be added to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::setAt(const std::uint32_t _nPos, const TVal& _Val)
{
    METHOD_ENTRY("CMultiBuffer::setAt")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i][_nPos] = _Val;
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets an element to buffer at given position, all elements are given
///
/// \param _nPos Position to set element at
/// \param _aVal Element array to be added to buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::setAt(const std::uint32_t _nPos, 
                                              const std::array<TVal, N>& _aVal)
{
    METHOD_ENTRY("CMultiBuffer::setAt")
    
    m_Mutex.lock();
    for (auto i=0u; i<N; ++i)
        m_Buffer[i][_nPos] = _aVal[i];
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
/// \param _unBuf0 First buffer to swap
/// \param _unBuf1 Second buffer to swap
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
void CMultiBuffer<N,T>::swap(const std::uint8_t _unBuf0, const std::uint8_t _unBuf1)
{
    METHOD_ENTRY("CMultiBuffer::swap")
    m_Mutex.lock();
    std::swap(m_BufferRef[_unBuf0], m_BufferRef[_unBuf1]);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
/// \param _unBuf0 First buffer to swap
/// \param _unBuf1 Second buffer to swap
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::swap(const std::uint8_t _unBuf0, const std::uint8_t _unBuf1)
{
    METHOD_ENTRY("CMultiBuffer::swap")
    m_Mutex.lock();
    std::swap(m_BufferRef[_unBuf0], m_BufferRef[_unBuf1]);
    m_Mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
/// \param _unBuf0 First buffer to swap
/// \param _unBuf1 Second buffer to swap
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::swap(const std::uint8_t _unBuf0, const std::uint8_t _unBuf1)
{
    METHOD_ENTRY("CMultiBuffer::swap")
    m_Mutex.lock();
    std::swap(m_BufferRef[_unBuf0], m_BufferRef[_unBuf1]);
    m_Mutex.unlock();
}
