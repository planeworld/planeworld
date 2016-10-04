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
/// \file       star.h
/// \brief      Prototype of class "CStar"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-09-06
///
////////////////////////////////////////////////////////////////////////////////

#ifndef STAR_H
#define STAR_H

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

constexpr const double SOLAR_RADIUS = 695000000.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a star.
/// 
////////////////////////////////////////////////////////////////////////////////
class CStar
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CStar();
        
        //--- Constant Methods -----------------------------------------------//
        const std::string&  getName() const;
        const Vector2d&     getOrigin() const;
        const double&       getRadius() const;
        const int&          getStarType() const;

        //--- Methods --------------------------------------------------------//
        void setOrigin(const Vector2d&);
        void setName(const std::string&);
        void setRadius(const double&);
        void setStarType(const int&);
        
    private:
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        std::string m_strName;              ///< Name of the star
        Vector2d    m_vecOrigin;            ///< Origin of star system in cell
        double      m_fRadius;              ///< Size of star
        int         m_nStarType;            ///< Type of star

};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
inline CStar::CStar() : m_strName("Star"),
                        m_fRadius(SOLAR_RADIUS),
                        m_nStarType(STAR_CLASS_M)
{
    METHOD_ENTRY("CStar::CStar")
    CTOR_CALL("CStar::CStar")
    m_vecOrigin.setZero();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the name of this star
///
/// \return Name of this star
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CStar::getName() const
{
    METHOD_ENTRY("CStar::getName")
    return m_strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the origin of the star
///
/// \return Origin of the star
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CStar::getOrigin() const
{
    METHOD_ENTRY("CStar::getOrigin")
    return m_vecOrigin;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the size of the star (radius)
///
/// \return Size of the star (radius)
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CStar::getRadius() const
{
    METHOD_ENTRY("CStar::getRadius")
    return m_fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the type of star (spectral class)
///
/// \return Type of star
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CStar::getStarType() const
{
    METHOD_ENTRY("CStar::getStarType")
    return m_nStarType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the origin of the star
///
/// \param _vecOrigin Origin of star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStar::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CStar::setOrigin")
    m_vecOrigin = _vecOrigin;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the name this star
///
/// \param _strName Name of the star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStar::setName(const std::string& _strName)
{
    METHOD_ENTRY("CStar::setName")
    m_strName = _strName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the radius ot the star
///
/// \param _fRadius Radius of the star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStar::setRadius(const double& _fRadius)
{
    METHOD_ENTRY("CStar::setRadius")
    m_fRadius = _fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the type of the star
///
/// \param _StarType Type of star
///
////////////////////////////////////////////////////////////////////////////////
inline void CStar::setStarType(const int& _StarType)
{
    METHOD_ENTRY("CStar::setStarType")

    m_nStarType = _StarType;
}

#endif // STAR_H
