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
#include "log.h"
#include "serialize_macros.h"

//--- Program header ---------------------------------------------------------//

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
        

        //--- Methods --------------------------------------------------------//
        virtual void serialize(const std::string&) const {}
        virtual void serialize(const std::string&, bool) const {}
        virtual void serialize(const std::string&, double) const {}
        virtual void serialize(const std::string&, int) const {}
        virtual void serialize(const std::string&, unsigned int) const {}
        virtual void serialize(const std::string&, std::size_t) const {}
        virtual void serialize(const std::string&, const std::string&) const {}
        virtual void serialize(const std::string&, const Vector2d&) const {}
        virtual void serialize(const std::string&, const Vector2i&) const {}
        
    protected:
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SERIALIZER_H
