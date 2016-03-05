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
/// \file       unique_id_referrer.h
/// \brief      Prototype of interface "IUniqueIDReferrer"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-03-01
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIQUE_ID_REFERRER_H
#define UNIQUE_ID_REFERRER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "unique_id.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that refer to an unique id.
///
////////////////////////////////////////////////////////////////////////////////
class IUniqueIDReferrer
{

    public:
   
        //--- Constant Methods -----------------------------------------------//
        const UIDType   getUIDRef() const;
        
    protected:
        
        //--- Protected variables --------------------------------------------//
        UIDType       m_UIDRef = 0u; ///< Reference to unique identifier 
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the reference to unique id
///
/// \return Reference to unique id
///
////////////////////////////////////////////////////////////////////////////////
inline const UIDType IUniqueIDReferrer::getUIDRef() const
{
    METHOD_ENTRY("IUniqueIDReferrer::getUIDRef")
    return m_UIDRef;
}

#endif // UNIQUE_ID_REFERRER_H
