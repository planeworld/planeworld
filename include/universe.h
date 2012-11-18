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
/// \file       universe.h
/// \brief      Prototype of class "CUniverse"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-11
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIVERSE_H
#define UNIVERSE_H

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "star_system.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a universe. Mainly needed for procedural generation.
/// 
////////////////////////////////////////////////////////////////////////////////
class CUniverse
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
//         CUniverse();
        virtual ~CUniverse();
        
        //--- Constant Methods -----------------------------------------------//
        const std::vector<CStarSystem*>& getStarSystems() const;

        //--- Methods --------------------------------------------------------//
        void generate(const int&);
        
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        const std::string starClassToString(const int&) const;
        
        std::vector<CStarSystem*> m_StarSystems;  // Star system in this universe

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the list of all star systems.
///
/// \return Vector (list), which holds the star systems
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector< CStarSystem* >& CUniverse::getStarSystems() const
{
    METHOD_ENTRY("CUniverse::getStarSystems")
    return m_StarSystems;
}


#endif
