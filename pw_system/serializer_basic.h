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
/// \file       serializer_basic.h
/// \brief      Prototype of class "CSerializerBasic"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-04-26
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZER_BASIC_H
#define SERIALIZER_BASIC_H

//--- Standard header --------------------------------------------------------//
#include "serializer.h"

//--- Program header ---------------------------------------------------------//

//--- Misc header ------------------------------------------------------------//


//--- Forward declarations ---------------------------------------------------//

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class implementing a basic serializer
///
////////////////////////////////////////////////////////////////////////////////
class CSerializerBasic : public ISerializer
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        

        //--- Methods --------------------------------------------------------//
        void serialize(const std::string& _strDescr) const override
        {
            std::cout << "--- " << _strDescr << " ---" << std::endl;
        }
        void serialize(const std::string& _strDescr, bool _bB) const override
        {
            std::cout << "bool: " << _strDescr << " = " << _bB << std::endl;
        }
        void serialize(const std::string& _strDescr, double _fD) const override
        {
            std::cout << "double: " << _strDescr << " = " << _fD << std::endl;
        }
        void serialize(const std::string& _strDescr, int _nI) const override
        {
            std::cout << "int: " << _strDescr << " = " << _nI << std::endl;
        }
        void serialize(const std::string& _strDescr, unsigned int _unI) const override
        {
            std::cout << "unsigned int: " << _strDescr << " = " << _unI << std::endl;
        }
        void serialize(const std::string& _strDescr, std::size_t _nI) const override
        {
            std::cout << "size_t: " << _strDescr << " = " << _nI << std::endl;
        }
        void serialize(const std::string& _strDescr, const std::string& _strS) const override
        {
            std::cout << "string: " << _strDescr << " = " << _strS << std::endl;
        }
        void serialize(const std::string& _strDescr, const Vector2d& _vecV) const override
        {
            std::cout << "vector2d: " << _strDescr << " = " << _vecV[0] << ", " << _vecV[1] << std::endl;
        }
        void serialize(const std::string& _strDescr, const Vector2i& _vecV) const override
        {
            std::cout << "vector2i: " << _strDescr << " = " << _vecV[0] << ", " << _vecV[1] << std::endl;
        }
        
    protected:
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SERIALIZER_BASIC_H
