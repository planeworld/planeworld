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
/// \file       key_map.h
/// \brief      Prototype of class "CKeyMap"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-05-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef KEY_MAP_H
#define KEY_MAP_H

//--- Standard header --------------------------------------------------------//
#include <map>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//

/// Type definition for the Keys
typedef int KeyType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class implementing a key-value map
///
/// This class implements a key-value map. Removed keys are stored in a list and
/// will be reused.
/// 
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CKeyMap
{
    
    public:
        
        /// Implementing a constant iterator on container
        class const_iterator
        {
            
                friend class CKeyMap<T>;
                typedef typename std::map<KeyType, T>::const_iterator ci_type;
                
            public:
                
                const_iterator() : ci(0){}
                const_iterator(const ci_type& _ci) : ci(_ci){}
                const_iterator(const const_iterator& _ci) : ci(_ci.ci){}
                const const_iterator& operator++() {++ci; return *this;}
                const T& operator*() {return (const T&)(*ci).second; }
                bool operator!=(const const_iterator& _ci) {return (ci != _ci.ci);}
                
            private:
                
                ci_type ci; ///< Constant iterator type of wrapped container
            
        };
        
        /// Implementing an iterator on container
        class iterator
        {
                friend class CKeyMap<T>;
                typedef typename std::map<KeyType, T>::iterator it_type;
                        
            public:
                
                iterator() : it(0){}
                iterator(it_type _it) : it(_it){}
                iterator(const iterator& _it) : it(_it.it){}
                iterator& operator++() {++it; return *this;}
                T& operator*() {return (T&)(*it).second; }
                bool operator!=(iterator _it) {return (it != _it.it);}
                
            private:
                
                it_type it; ///< Iterator type of wrapped container
            
        };
    
        //--- Constructor/Destructor -----------------------------------------//
        CKeyMap():m_Key(0){}
        
        //--- Constant Methods -----------------------------------------------//
        
        ////////////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Get constant iterator pointing at begin of data
        ///
        /// \return Constant iterator on begin
        ///
        ////////////////////////////////////////////////////////////////////////////////
        const const_iterator begin() const
        {
            METHOD_ENTRY("CKeyMap<T>::begin")
            
            METHOD_EXIT("CKeyMap<T>::begin")
            return const_iterator(m_Values.begin());
        }

        ////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Get constant iterator pointing at end of data
        ///
        /// \return Constant iterator on end
        ///
        ////////////////////////////////////////////////////////////////////////
        const const_iterator end() const
        {
            METHOD_ENTRY("CKeyMap<T>::end")
            
            METHOD_EXIT("CKeyMap<T>::end")
            return const_iterator(m_Values.end());
        }

        //--- Methods --------------------------------------------------------//
        const KeyType           insert(const T&);
        
        ////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Get iterator pointing at begin of data
        ///
        /// \return Iterator on begin
        ///
        ////////////////////////////////////////////////////////////////////////
        iterator begin()
        {
            METHOD_ENTRY("CKeyMap<T>::begin")
            
            METHOD_EXIT("CKeyMap<T>::begin")
            return iterator(m_Values.begin());
        }

        ////////////////////////////////////////////////////////////////////////
        ///
        /// \brief Get iterator pointing at end of data
        ///
        /// \return Iterator on end
        ///
        ////////////////////////////////////////////////////////////////////////
        iterator end()
        {
            METHOD_ENTRY("CKeyMap<T>::end")
            
            METHOD_EXIT("CKeyMap<T>::end")
            return iterator(m_Values.end());
        }

    private:

        //--- Private Variables ----------------------------------------------//
        std::map<KeyType,T> m_Values;       ///< Map of values and keys
        std::list<KeyType>  m_KeysFree;     ///< Free keys due to value removal
        KeyType             m_Key;          ///< Current key
        
};

//--- Implementation is done here for inline optimisation --------------------//

//--- Implementation of template members -------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Insert a value and get a unique key
///
/// \param _T Value to insert in map
///
/// \return Key
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
const KeyType CKeyMap<T>::insert(const T& _Value)
{
    METHOD_ENTRY("CKeyMap<T>::insert")
    
    KeyType Key;

    if (m_KeysFree.size() > 0)
    {
        Key = m_KeysFree.front();
        m_KeysFree.pop_front();
    }
    else
    {
        Key = m_Key++;
    }
    m_Values.insert(std::pair<KeyType,T>(Key,_Value));
    
    METHOD_EXIT("CKeyMap<T>::insert")
    return Key;
}

#endif
