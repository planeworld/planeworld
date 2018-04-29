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
        static void setSerializer(ISerializer* const _pSerializer)
        {
            METHOD_ENTRY("ISerializable::setSerializer")
            s_pSerializer = _pSerializer;
        }
        void serialize(const std::string& _strDescr) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr);
            this->mySerialize(_strDescr);
        }
        
    protected:
        
        virtual void mySerialize(const std::string&) const {}

        void serialize(const std::string& _strDescr, const ISerializable* const _pSerializable) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            _pSerializable->serialize(_strDescr);
        }
        void serialize(const std::string& _strDescr, bool _bB) const
        {
            METHOD_ENTRY("ISerializable::serialize")
//             DOM_DEV
            s_pSerializer->serialize(_strDescr, _bB);
        }
        void serialize(const std::string& _strDescr, double _fD) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _fD);
        }
        void serialize(const std::string& _strDescr, int _nI) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _nI);
        }
        void serialize(const std::string& _strDescr, unsigned int _unI) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _unI);
        }
        void serialize(const std::string& _strDescr, std::size_t _nI) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _nI);
        }
        void serialize(const std::string& _strDescr, const std::string& _strS) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _strS);
        }
        void serialize(const std::string& _strDescr, const Vector2d& _vecV) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _vecV);
        }
        void serialize(const std::string& _strDescr, const Vector2i& _vecV) const
        {
            METHOD_ENTRY("ISerializable::serialize")
            s_pSerializer->serialize(_strDescr, _vecV);
        }
    
        static ISerializer* s_pSerializer; ///< Serializer to be used
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SERIALIZABLE_H
