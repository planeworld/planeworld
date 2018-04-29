////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2018 Torsten Büschenfeld
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
/// \file       serializable.h
/// \brief      Prototype of interface "ISerializable"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-04-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "serializer.h"

//--- Misc header ------------------------------------------------------------//


//--- Forward declarations ---------------------------------------------------//

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for serializable classes
///
////////////////////////////////////////////////////////////////////////////////
class ISerializable
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        

        //--- Methods --------------------------------------------------------//
        static void setSerializer(ISerializer* const _pSerializer);

        void serialize(const std::string& _strDescr) const;
        
    protected:
        
        virtual void mySerialize(const std::string&) const {}

        void serialize(const std::string&, const ISerializable* const) const;
        void serialize(const std::string&, bool) const;
        void serialize(const std::string&, double) const;
        void serialize(const std::string&, int) const;
        void serialize(const std::string&, unsigned int) const;
        void serialize(const std::string&, std::size_t) const;
        void serialize(const std::string&, const std::string&) const;
        void serialize(const std::string&, const Vector2d&) const;
        void serialize(const std::string&, const Vector2i&) const;
    
        static ISerializer* s_pSerializer; ///< Serializer to be used
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set serializer
///
/// \param _pSerializer Serializer to use for serialization
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::setSerializer(ISerializer* const _pSerializer)
{
    METHOD_ENTRY("ISerializable::setSerializer")
    s_pSerializer = _pSerializer;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize this object
///
/// \param _strDescr Description of serialized object
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    
    s_pSerializer->serialize(_strDescr);
    this->mySerialize(_strDescr);
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given serializable
///
/// \param _strDescr Description of serialized object
/// \param _pSerializable Serializable to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, const ISerializable* const _pSerializable) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    _pSerializable->serialize(_strDescr);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given bool
///
/// \param _strDescr Description of serialized object
/// \param _bB Bool to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, bool _bB) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _bB);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given double
///
/// \param _strDescr Description of serialized object
/// \param _fD Double to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, double _fD) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _fD);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given integer
///
/// \param _strDescr Description of serialized object
/// \param _nI Integer to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, int _nI) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _nI);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given unsigned integer
///
/// \param _strDescr Description of serialized object
/// \param _unI Unsigned integer to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, unsigned int _unI) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _unI);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given size_t
///
/// \param _strDescr Description of serialized object
/// \param _nI size_t to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, std::size_t _nI) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _nI);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given string
///
/// \param _strDescr Description of serialized object
/// \param _strS String to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, const std::string& _strS) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _strS);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given vector
///
/// \param _strDescr Description of serialized object
/// \param _vecV Vector to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, const Vector2d& _vecV) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _vecV);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Serialize given vector
///
/// \param _strDescr Description of serialized object
/// \param _vecV Vector to serialize
///
////////////////////////////////////////////////////////////////////////////////
inline void ISerializable::serialize(const std::string& _strDescr, const Vector2i& _vecV) const
{
    METHOD_ENTRY("ISerializable::serialize")
    DOM_DEV
    (
        if (s_pSerializer == nullptr)
        {
            WARNING_MSG("Serializable", "Serializer not defined, use <setSerializer>.")
            return;
        }
    )
    s_pSerializer->serialize(_strDescr, _vecV);
}

#endif // SERIALIZABLE_H
