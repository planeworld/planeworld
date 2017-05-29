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
/// \file       circular_buffer.tpp
/// \brief      Implementation of class "CCircularBuffer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-08
///
////////////////////////////////////////////////////////////////////////////////

#include "circular_buffer.h"

#include <eigen3/Eigen/Core>

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
CCircularBuffer<T>::CCircularBuffer()
{
    METHOD_ENTRY("CCircularBuffer::CCircularBuffer")
    CTOR_CALL("CCircularBuffer")
    m_Buffer.resize(m_nCapacity);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor, initialising buffer with given capacity
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
CCircularBuffer<T>::CCircularBuffer(const std::size_t& _nCapa)
{
    METHOD_ENTRY("CCircularBuffer::CCircularBuffer")
    CTOR_CALL("CCircularBuffer")
    this->reserve(_nCapa);
}    

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor, initialising from given buffer
///
/// \param _Buf Buffer to be copied
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
CCircularBuffer<T>::CCircularBuffer(const CCircularBuffer<T>& _Buf)
{
    METHOD_ENTRY("CCircularBuffer::CCircularBuffer")
    CTOR_CALL("CCircularBuffer")
    this->copy(_Buf);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _Buf Buffer to be copied
///
/// \return Copied buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
CCircularBuffer<T>& CCircularBuffer<T>::operator=(const CCircularBuffer<T>& _Buf)
{
    METHOD_ENTRY("CCircularBuffer::operator=")
  
    this->copy(_Buf);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the elment at given index
///
/// \param _nI Index
///
/// \return Element at given index
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
const T& CCircularBuffer<T>::operator[](const std::size_t& _nI) const
{
    METHOD_ENTRY("CCircularBuffer::operator[]")
  
    DEBUG_BLK(
        if (_nI >= m_nSize)
        {
            ERROR_MSG("Circular Buffer", "Trying to access element out of range.")
            return m_Buffer[0];
        }
    )
    std::size_t nI = _nI + m_nBegin;
    if (nI >= m_nCapacity) nI-=m_nCapacity;
    return m_Buffer[nI];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the elment at given index
///
/// This method is doing the same as the operator[].
///
/// \param _nI Index
///
/// \return Element at given index
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
const T& CCircularBuffer<T>::at(const std::size_t& _nI) const
{
    METHOD_ENTRY("CCircularBuffer::at")
  
    DEBUG_BLK(
        if (_nI >= m_nSize)
        {
            ERROR_MSG("Circular Buffer", "Trying to access element out of range.")
            return m_Buffer[0];
        }
    )
    std::size_t nI = _nI + m_nBegin;
    if (nI >= m_nCapacity) nI-=m_nCapacity;
    return m_Buffer[nI];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clone this buffer
///
/// \return New buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
CCircularBuffer<T>* CCircularBuffer<T>::clone() const
{
    METHOD_ENTRY("CCircularBuffer::clone")
  
    CCircularBuffer<T>* pBuf = new CCircularBuffer;
    MEM_ALLOC("CCircularBuffer")
    pBuf->copy(*this);
    
    return pBuf;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the elment at given index
///
/// \param _nI Index
///
/// \return Element at given index
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
T& CCircularBuffer<T>::operator[](const std::size_t& _nI)
{
    METHOD_ENTRY("CCircularBuffer::operator[]")
  
    DEBUG_BLK(
        if (_nI >= m_nSize)
        {
            ERROR_MSG("Circular Buffer", "Trying to access element out of range.")
            return m_Buffer[0];
        }
    )
    std::size_t nI = _nI + m_nBegin;
    if (nI >= m_nCapacity) nI-=m_nCapacity;
    return m_Buffer[nI];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the elment at given index
///
/// This method is doing the same as the operator[].
///
/// \param _nI Index
///
/// \return Element at given index
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
T& CCircularBuffer<T>::at(const std::size_t& _nI)
{
    METHOD_ENTRY("CCircularBuffer::at")
  
    DEBUG_BLK(
        if (_nI >= m_nSize)
        {
            ERROR_MSG("Circular Buffer", "Trying to access element out of range.")
            return m_Buffer[0];
        }
    )
    std::size_t nI = _nI + m_nBegin;
    if (nI >= m_nCapacity) nI-=m_nCapacity;
    return m_Buffer[nI];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds an element at the end of the buffer. If capacity is reached, the
///        first element will be overwritten.
///
/// \param _Elem Element to insert at the end of the buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
void CCircularBuffer<T>::push_back(const T& _Elem)
{
    METHOD_ENTRY("CCircularBuffer::push_back")
    
    if (++m_nEnd == m_nCapacity)
    {
        m_nEnd -= m_nCapacity;
    }
    m_Buffer[m_nEnd] = _Elem;
    
    if (m_nSize == m_nCapacity)
    {
        if (++m_nBegin >= m_nCapacity)
        {
            m_nBegin -= m_nCapacity;
        }
    }
    else
        ++m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Reserves given capacity for the buffer
///
/// \param _nCapa Element to insert at the end of the buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
void CCircularBuffer<T>::reserve(const std::size_t& _nCapa)
{
    METHOD_ENTRY("CCircularBuffer::reserve")
    m_Buffer.resize(_nCapa);
    m_nCapacity = _nCapa;
    m_nEnd = _nCapa - 1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _CB CCircularBuffer instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
template<class U>
std::istream& operator>>(std::istream& _is, CCircularBuffer<U>& _CB)
{
    METHOD_ENTRY("CCircularBuffer::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> _CB.m_nCapacity;
    _is >> _CB.m_nBegin;
    _is >> _CB.m_nEnd;
    _is >> _CB.m_nSize;
    
    /// \todo Test content of buffer against pointers and delete if neccessary
//     for (auto it : _CB.m_Buffer)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             MEM_FREED("IVisuals")
//             it = nullptr;
//         }
//     }
    _CB.m_Buffer.clear();
    
    _CB.m_Buffer.resize(_CB.m_nCapacity);
    U Value;
    for (auto i=0u; i<_CB.m_nCapacity; ++i)
    {
        _is >> Value;
        _CB.m_Buffer[i] = Value;
    }
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _CB CCircularBuffer instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
template<>
inline std::istream& operator>>(std::istream& _is, CCircularBuffer<Vector2d>& _CB)
{
    METHOD_ENTRY("CCircularBuffer::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    _is >> _CB.m_nCapacity;
    _is >> _CB.m_nBegin;
    _is >> _CB.m_nEnd;
    _is >> _CB.m_nSize;
    
    /// \todo Test content of buffer against pointers and delete if neccessary
//     for (auto it : _CB.m_Buffer)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             MEM_FREED("IVisuals")
//             it = nullptr;
//         }
//     }
    _CB.m_Buffer.clear();
    
    _CB.m_Buffer.resize(_CB.m_nCapacity);
    Vector2d Value;
    for (auto i=0u; i<_CB.m_nCapacity; ++i)
    {
        _is >> Value[0] >> Value[1];
        _CB.m_Buffer[i] = Value;
    }
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _CB CCircularBuffer instance to stream
///
/// \return Stream with game state information of CCircularBuffer instance
///
////////////////////////////////////////////////////////////////////////////////
template<class U>
std::ostream& operator<<(std::ostream& _os, const CCircularBuffer<U>& _CB)
{
    METHOD_ENTRY("CCircularBuffer::operator<<")
    
    _os << "CircularBuffer:" << std::endl;
    
    _os << _CB.m_nCapacity << std::endl;
    _os << _CB.m_nBegin << std::endl;
    _os << _CB.m_nEnd << std::endl;
    _os << _CB.m_nSize << std::endl;
    
    for (auto ci : _CB.m_Buffer)
    {
        _os << ci << " ";
    }
    _os << std::endl;
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _CB CCircularBuffer instance to stream
///
/// \return Stream with game state information of CCircularBuffer instance
///
////////////////////////////////////////////////////////////////////////////////
template<>
inline std::ostream& operator<<(std::ostream& _os, const CCircularBuffer<Vector2d>& _CB)
{
    METHOD_ENTRY("CCircularBuffer::operator<<")
    
    _os << "CircularBuffer:" << std::endl;
    
    _os << _CB.m_nCapacity << std::endl;
    _os << _CB.m_nBegin << std::endl;
    _os << _CB.m_nEnd << std::endl;
    _os << _CB.m_nSize << std::endl;
    
    for (auto ci : _CB.m_Buffer)
    {
        _os << ci[0] << " " << ci[1] << " ";
    }
    _os << std::endl;
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies contents of given buffer to this buffer
///
/// \param _Buf Buffer to copy contents from
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
void CCircularBuffer<T>::copy(const CCircularBuffer<T>& _Buf)
{
    METHOD_ENTRY("CCircularBuffer::copy")
    
    m_nCapacity = _Buf.m_nCapacity;
    m_nBegin = _Buf.m_nBegin;
    m_nEnd = _Buf.m_nEnd;
    m_nSize = _Buf.m_nSize;
    m_Buffer = _Buf.m_Buffer;
    m_Buffer.resize(m_nCapacity);
}
