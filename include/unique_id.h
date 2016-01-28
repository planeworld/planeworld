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
/// \file       unique_id.h
/// \brief      Prototype of class "CUniqueID"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-24
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <deque>

//--- Program header ---------------------------------------------------------//
#include "log.h"

using UIDType = std::uint32_t;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that manages a unique ID to identify objects etc.
///
////////////////////////////////////////////////////////////////////////////////
class CUniqueID
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CUniqueID();
        ~CUniqueID();
        
        //--- Constant Methods -----------------------------------------------//
        const UIDType& value() const;
                
        //--- Methods --------------------------------------------------------//
                
    private:
        
        CUniqueID(const CUniqueID&){};                  ///< Copying not allowed
      
        //--- Variables [private] --------------------------------------------//
               UIDType             m_nUID;              ///< Unique ID for this instance
               
        static UIDType             s_nUID;              ///< Unique ID counter
        static std::deque<UIDType> s_UnusedUIDs;        ///< Storage for unused / released IDs
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the unique ID value
///
/// \return Unique ID value
///
////////////////////////////////////////////////////////////////////////////////
inline const UIDType& CUniqueID::value() const
{
    METHOD_ENTRY("CUniqueID::value")
    return m_nUID;
}

#endif // CUniqueID
