////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2012-2017 Torsten Büschenfeld
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
/// \file       star_system.h
/// \brief      Prototype of class "CStarSystem"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "grid_user.h"
#include "star.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a star system.
/// 
////////////////////////////////////////////////////////////////////////////////
class CStarSystem : public IGridUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CStarSystem();
        
        //--- Constant Methods -----------------------------------------------//
        const int&          getNumberOfPlanets() const;
        const int&          getSeed() const;

        //--- Methods --------------------------------------------------------//
        void setNumberOfPlanets(const int&);
        void setSeed(const int&);
        
        CStar& Star();
        
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        CStar       m_Star;                ///< The central star
        int         m_nNumberOfPlanets;    ///< Number of planets in this star system
        int         m_nSeed;               ///< Local seed of this star system

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the number of planets for this star system
///
/// \return Number of planets for this star system
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CStarSystem::getNumberOfPlanets() const
{
    METHOD_ENTRY("CStarSystem::getNumberOfPlanets")
    return m_nNumberOfPlanets;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the local seed of this star system
///
/// \return Local seed of star system
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CStarSystem::getSeed() const
{
    METHOD_ENTRY("CStarSystem::getSeed")
    return m_nSeed;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of planets in this star system
///
/// \param _nNumberOfPlanets Number of planets to set for this star system
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setNumberOfPlanets(const int& _nNumberOfPlanets)
{
    METHOD_ENTRY("CStarSystem::setNumberOfPlanets")

    m_nNumberOfPlanets = _nNumberOfPlanets;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the local seed for this star system
///
/// \param _nSeed Local seed for this star system
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setSeed(const int& _nSeed)
{
    METHOD_ENTRY("CStarSystem::setSeed")

    m_nSeed = _nSeed;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Delegate, return star reference
///
/// \return Star
///
////////////////////////////////////////////////////////////////////////////////
inline CStar& CStarSystem::Star()
{
    METHOD_ENTRY("CStarSystem::Star")
    return m_Star;
}

#endif
