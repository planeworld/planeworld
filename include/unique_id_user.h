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
/// \file       unique_id_user.h
/// \brief      Prototype of interface "IUniqueIDUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-28
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIQUE_ID_USER_H
#define UNIQUE_ID_USER_H

//--- Standard header --------------------------------------------------------//
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "unique_id.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that use a engine wide unique id.
///
////////////////////////////////////////////////////////////////////////////////
class IUniqueIDUser
{

    public:
   
        //--- Constant Methods -----------------------------------------------//
        const UIDType   getUID() const;
        
    protected:
        
        //--- Protected variables --------------------------------------------//
        CUniqueID       m_UID; ///< Identifier
};

typedef std::unordered_map<UIDType, IUniqueIDUser*> UIDUserType; ///< Stores entities by UID

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the objects unique id
///
/// \return Unique id of object
///
////////////////////////////////////////////////////////////////////////////////
inline const UIDType IUniqueIDUser::getUID() const
{
    METHOD_ENTRY("IUniqueIDUser::getUID")
    return m_UID.value();
}

#endif // UNIQUE_ID_USER_H
