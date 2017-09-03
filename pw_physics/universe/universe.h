////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2012-2016 Torsten Büschenfeld
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
#include <atomic>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "star_system.h"

//--- Misc header ------------------------------------------------------------//

const double UNIVERSE_CELL_SIZE=1.0e12;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a universe. Mainly needed for procedural generation.
///
/// Some notes: A globular cluster, which is represented at the moment, consists
/// of 10000 to several million stars. Their density is 0.4 stars/parsec on
/// average and 100-1000 stars/parsec in the inner core.
/// One cell of the space grid in planeworld that avoids numerical problems is
/// 1.0e12m. One parsec is 30.857e15m. This means that the star density is
/// 0.4/30.857e15m which is 0.4e12/30.857e15=1.3e-5/cell. Moreover, given
/// m_nNrOfStars and using the 3-sigma value to cut the distribution, the
/// density is 0.4/30.857e15m = m_nNrOfStars / 3-Sigma.That is
/// Sigma = m_nNrOfStars*30.857e15/(3*0.4)
///
////////////////////////////////////////////////////////////////////////////////
class CUniverse
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CUniverse();
        ~CUniverse();
        
        //--- Constant Methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        void                             generate(const int&, const int&);
        const std::vector<CStarSystem*>* getStarSystems();
        
        //--- Variables ------------------------------------------------------//
        #ifdef PW_MULTITHREADING
            std::atomic_flag isAccessed = ATOMIC_FLAG_INIT;    ///< Indicates access, important for multithreading
        #endif
            
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        const std::string starClassToString(const int&) const;
        
        //--- Variables [private] --------------------------------------------//
        std::vector<CStarSystem*>       m_StarSystems;      ///< Star system in this universe
        int                             m_nNrOfPlanetsMax;  ///< Maximum number of planets

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the list of all star systems.
///
/// \return Vector (list), which holds the star systems
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<CStarSystem*>* CUniverse::getStarSystems()
{
    METHOD_ENTRY("CUniverse::getStarSystems")
    return &m_StarSystems;
}

#endif // UNIVERSE_H
