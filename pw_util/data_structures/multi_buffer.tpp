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
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
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
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
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
    for (auto i=0u; i<N; ++i)
        m_BufferRef[i]=&m_Buffer[i];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
template<std::uint8_t I>
T* CMultiBuffer<N,T>::getBuffer()
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[I];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
template<std::uint8_t I>
TContainer* CMultiBuffer<N, TContainer, TVal>::getBuffer()
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[I];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns buffer
///
/// \return Buffer
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
template<std::uint8_t I>
TContainer* CMultiBuffer<N, TContainer, TKey, TVal>::getBuffer()
{
    METHOD_ENTRY("CMultiBuffer::getBuffer")
    return m_BufferRef[I];
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
    for (auto i=0u; i<N; ++i)
        aElems[i] = m_Buffer[i][_unElem];
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
    for (auto i=0u; i<N; ++i)
        aElems[i] = m_Buffer[i][_Elem];
    return aElems;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets class object to be buffered.
///
/// \param _Val Class object to be buffered
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
void CMultiBuffer<N,T>::add(const T& _Val)
{
    METHOD_ENTRY("CMultiBuffer::add")
    for (auto i=0u; i<N; ++i)
        m_Buffer[i] = _Val;
}

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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].push_back(_Val);
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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].push_back(_aVal[i]);
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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].insert(std::pair<TKey,TVal>(_Key,_Val));
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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i].insert(std::pair<TKey,TVal>(_Key,_aVal[i]));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deep copy of buffer content
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
template<std::uint8_t I, std::uint8_t J>
void CMultiBuffer<N, TContainer, TVal>::copyDeep()
{
    METHOD_ENTRY("CMultiBuffer::copyDeep")
    for (auto i=0u; i<m_BufferRef[I].size(); ++i)
    {
        (*(m_BufferRef[J])) = (*(m_BufferRef[I]));
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deep copy of buffer content
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
template<std::uint8_t I, std::uint8_t J>
void CMultiBuffer<N, TContainer, TKey, TVal>::copyDeep()
{
    METHOD_ENTRY("CMultiBuffer::copyDeep")
    auto it = m_BufferRef[J]->begin();
    for (auto ci  = m_BufferRef[I]->cbegin(); ci != m_BufferRef[J]->cend(); ++ci)
    {
        *(it->second) = *(ci->second);
        ++it;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Fill the buffer with given value
///
/// \param _Val Value to fill the buffer with
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::fillBuffer(const TVal& _Val)
{
    METHOD_ENTRY("CMultiBuffer::fillBuffer")
    for (auto i=0u; i<N; ++i)
        std::fill(m_Buffer[i].begin(), m_Buffer[i].end(), _Val);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Fill the buffer with given value
///
/// \param _Val Value to fill the buffer with
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::fillBuffer(const TVal& _Val)
{
    METHOD_ENTRY("CMultiBuffer::fillBuffer")
    for (auto i=0u; i<N; ++i)
        std::fill(m_Buffer[i].begin(), m_Buffer[i].end(), _Val);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resize the buffer to given size
///
/// \param _nSize New buffer size
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
void CMultiBuffer<N,T>::resizeBuffer(const std::size_t _nSize)
{
    METHOD_ENTRY("CMultiBuffer::resizeBuffer")
    for (auto i=0u; i<N; ++i) {m_Buffer[i].resize(_nSize);}
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resize the buffer to given size
///
/// \param _nSize New buffer size
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
void CMultiBuffer<N, TContainer, TVal>::resizeBuffer(const std::size_t _nSize)
{
    METHOD_ENTRY("CMultiBuffer::resizeBuffer")
    for (auto i=0u; i<N; ++i) {m_Buffer[i].resize(_nSize);}
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resize the buffer to given size
///
/// \param _nSize New buffer size
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
void CMultiBuffer<N, TContainer, TKey, TVal>::resizeBuffer(const std::size_t _nSize)
{
    METHOD_ENTRY("CMultiBuffer::resizeBuffer")
    for (auto i=0u; i<N; ++i) {m_Buffer[i].resize(_nSize);}
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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i][_nPos] = _Val;
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
    for (auto i=0u; i<N; ++i)
        m_Buffer[i][_nPos] = _aVal[i];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class T>
template<std::uint8_t I, std::uint8_t J>
void CMultiBuffer<N,T>::swap()
{
    METHOD_ENTRY("CMultiBuffer::swap")
    std::swap(m_BufferRef[I], m_BufferRef[J]);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TVal>
template<std::uint8_t I, std::uint8_t J>
void CMultiBuffer<N, TContainer, TVal>::swap()
{
    METHOD_ENTRY("CMultiBuffer::swap")
    std::swap(m_BufferRef[I], m_BufferRef[J]);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap buffers
///
////////////////////////////////////////////////////////////////////////////////
template<std::uint8_t N, class TContainer, class TKey, class TVal>
template<std::uint8_t I, std::uint8_t J>
void CMultiBuffer<N, TContainer, TKey, TVal>::swap()
{
    METHOD_ENTRY("CMultiBuffer::swap")
    std::swap(m_BufferRef[I], m_BufferRef[J]);
}


SERIALIZE_IMPL_T(template<std::uint8_t N COMMA class T>,
                 CMultiBuffer<N COMMA T>,
    for (const auto& Buf : m_Buffer)
    {
        SERIALIZE("buffer", Buf)
    }
)


SERIALIZE_IMPL_T(template<std::uint8_t N COMMA class TContainer COMMA class TVal>,
                 CMultiBuffer<N COMMA TContainer COMMA TVal>,
    for (const auto& Buf : m_Buffer)
    {
        SERIALIZE_UNARY("buffer", Buf)
    }
)


SERIALIZE_IMPL_T(template<std::uint8_t N COMMA class TContainer COMMA class TKey COMMA class TVal>,
                 CMultiBuffer<N COMMA TContainer COMMA TKey COMMA TVal>,
    for (const auto& Buf : m_Buffer)
    {
        SERIALIZE_BINARY("key", "value", Buf)
    }
)
