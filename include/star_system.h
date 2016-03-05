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
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include <eigen3/Eigen/Geometry>

using namespace Eigen;

typedef enum
{
    STAR_CLASS_M = 0,
    STAR_CLASS_K = 1,
    STAR_CLASS_G = 2,
    STAR_CLASS_F = 3,
    STAR_CLASS_A = 4,
    STAR_CLASS_B = 5,
    STAR_CLASS_O = 6
} StarType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a star system.
/// 
////////////////////////////////////////////////////////////////////////////////
class CStarSystem
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CStarSystem();
        
        //--- Constant Methods -----------------------------------------------//
        const Vector2i&     getCell() const;
        const Vector2d&     getCenter() const;
        const std::string&  getName() const;
        const int&          getNumberOfPlanets() const;
        const int&          getStarType() const;
        const int&          getSeed() const;

        //--- Methods --------------------------------------------------------//
        void setCell(const Vector2i&);
        void setCenter(const Vector2d&);
        void setName(const std::string&);
        void setNumberOfPlanets(const int&);
        void setSeed(const int&);
        void setStarType(const int&);
        
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        std::string m_strName;             ///< Name of the central star
        Vector2i    m_vecCell;             ///< Cell in that the star system is located
        Vector2d    m_vecCenter;           ///< Center of star system in cell
        int         m_nNumberOfPlanets;    ///< Number of planets in this star system
        int         m_StarType;            ///< Type of star
        int         m_nSeed;               ///< Local seed of this star system

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the cell the star system is located at.
///
/// \return Cell the star system is located at
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2i& CStarSystem::getCell() const
{
    METHOD_ENTRY("CStarSystem::getCell")
    return m_vecCell;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the center of the star system within its grid
///
/// \return Center of the star system
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CStarSystem::getCenter() const
{
    METHOD_ENTRY("CStarSystem::getCenter")
    return m_vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name of this star system (i.e. the central star)
///
/// \return Name of this star system
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CStarSystem::getName() const
{
    METHOD_ENTRY("CStarSystem::getName")
    return m_strName;
}

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
/// \brief Returns the type of star (spectral class)
///
/// \return Type of star
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CStarSystem::getStarType() const
{
    METHOD_ENTRY("CStarSystem::getStarType")
    return m_StarType;
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
/// \brief Sets the cell the star system is located at
///
/// \param _vecCell Cell of star system
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setCell(const Vector2i& _vecCell)
{
    METHOD_ENTRY("CStarSystem::setCell")

    m_vecCell = _vecCell;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the center of the star system within its cell
///
/// \param _vecCenter Center of star system
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setCenter(const Vector2d& _vecCenter)
{
    METHOD_ENTRY("CStarSystem::setCenter")

    m_vecCenter = _vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the name this star system (i.e. its central star)
///
/// \param _strName Name of the central star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setName(const std::string& _strName)
{
    METHOD_ENTRY("CStarSystem::setName")

    m_strName = _strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of planets in this star system
///
/// \param _nNrOfPlanets Number of planets to set for this star system
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
/// \brief Sets the type of the star
///
/// \param _StarType Type of star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setStarType(const int& _StarType)
{
    METHOD_ENTRY("CStarSystem::setStarType")

    m_StarType = _StarType;
}

#endif
