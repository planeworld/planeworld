/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
        
        const std::size_t capacity() const;
        const std::size_t size() const;
        
        //--- Methods --------------------------------------------------------//
        T&   operator[](const std::size_t&);
        T&   at        (const std::size_t&);
        void push_back(const T&);
        void reserve(const std::size_t&);
                
    private:
        
        //--- Variables [private] --------------------------------------------//
        std::vector<T>      m_Buffer;           ///< Buffer to store U elements of type T
        size_t              m_nCapacity = 1;    ///< Capacity of the buffer
        size_t              m_nBegin = 0;       ///< Index for first buffer entry
        size_t              m_nEnd =  0;        ///< Index for last buffer entry
        size_t              m_nSize = 0;        ///< Size from begin to end of buffer
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline CCircularBuffer<T>::CCircularBuffer()
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
inline CCircularBuffer<T>::CCircularBuffer(const std::size_t& _nCapa)
{
    METHOD_ENTRY("CCircularBuffer::CCircularBuffer")
    CTOR_CALL("CCircularBuffer")
    this->reserve(_nCapa);
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
inline const T& CCircularBuffer<T>::operator[](const std::size_t& _nI) const
{
    METHOD_ENTRY("CCircularBuffer::operator[]")
  
    DEBUG(
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
inline const T& CCircularBuffer<T>::at(const std::size_t& _nI) const
{
    METHOD_ENTRY("CCircularBuffer::at")
  
    DEBUG(
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
/// \brief Returns the capacity of the buffer
///
/// \return Capacity of the buffer
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
inline const std::size_t CCircularBuffer<T>::capacity() const
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
inline const std::size_t CCircularBuffer<T>::size() const
{
    METHOD_ENTRY("CCircularBuffer::size")
    return m_nSize;
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
inline T& CCircularBuffer<T>::operator[](const std::size_t& _nI)
{
    METHOD_ENTRY("CCircularBuffer::operator[]")
  
    DEBUG(
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
inline T& CCircularBuffer<T>::at(const std::size_t& _nI)
{
    METHOD_ENTRY("CCircularBuffer::at")
  
    DEBUG(
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
inline void CCircularBuffer<T>::push_back(const T& _Elem)
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
inline void CCircularBuffer<T>::reserve(const std::size_t& _nCapa)
{
    METHOD_ENTRY("CCircularBuffer::reserve")
    m_Buffer.resize(_nCapa);
    m_nCapacity = _nCapa;
    m_nEnd = _nCapa - 1;
}

#endif // CIRCULAR_BUFFER_H
