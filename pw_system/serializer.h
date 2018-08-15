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
/// \file       serializer.h
/// \brief      Prototype of interface "ISerializer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-04-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZER_H
#define SERIALIZER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "serialize_macros.h"

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Core>

//--- Forward declarations ---------------------------------------------------//

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for serializers
///
////////////////////////////////////////////////////////////////////////////////
class ISerializer
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        virtual ~ISerializer(){}

        //--- Methods --------------------------------------------------------//
        virtual void deserialize(const std::string&) {}
        virtual void deserialize(const std::string&, bool&) {}
        virtual void deserialize(const std::string&, double&) {}
        virtual void deserialize(const std::string&, int&) {}
        virtual void deserialize(const std::string&, unsigned int&) {}
        virtual void deserialize(const std::string&, std::size_t&) {}
        virtual void deserialize(const std::string&, std::string&) {}
        virtual void deserialize(const std::string&, Vector2d&) {}
        virtual void deserialize(const std::string&, Vector2i&) {}
        
        virtual void serialize(const std::string&) {}
        virtual void serialize(const std::string&, bool) {}
        virtual void serialize(const std::string&, double) {}
        virtual void serialize(const std::string&, int) {}
        virtual void serialize(const std::string&, unsigned int) {}
        virtual void serialize(const std::string&, std::size_t) {}
        virtual void serialize(const std::string&, const std::string&) {}
        virtual void serialize(const std::string&, const Vector2d&) {}
        virtual void serialize(const std::string&, const Vector2i&) {}
        
    protected:
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SERIALIZER_H
