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
#include "eigen2/Eigen/Geometry"

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
        const Vector2d& getLocation() const;
        const int&      getStarType() const;

        //--- Methods --------------------------------------------------------//
        void setLocation(const Vector2d&);
        void setStarType(const int&);
        
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        Vector2d m_vecLocation;     ///< Location of star system
        int      m_StarType;        ///< Type of star

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the location of the star system
///
/// \return Location of the star system
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CStarSystem::getLocation() const
{
    METHOD_ENTRY("CStarSystem::getLocation")
    return m_vecLocation;
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
/// \brief Sets the location of the star system
///
/// \param _vecLocation Location of star system
///
////////////////////////////////////////////////////////////////////////////////
inline void CStarSystem::setLocation(const Vector2d& _vecLocation)
{
    METHOD_ENTRY("CStarSystem::setLocation")

    m_vecLocation = _vecLocation;
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
