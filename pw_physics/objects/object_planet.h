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
/// \file       object_planet.h
/// \brief      Prototype of interface "CObjectPlanet"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-01-03
///
////////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_PLANET_H
#define OBJECT_PLANET_H

//--- Program header ---------------------------------------------------------//
#include "object.h"
#include "planet.h"

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a specialised physical object, representing
///        planets
///
/// Planetary objects have some more attributes than bare physical objects, such
/// as a potential atmosphere and an area of influence.
///
/// Atmosphere
///
/// In planeworld, planetare atmosphere calculations use the barometric formula
/// which implies some
/// simplifications such as constant temperature and an atmosphere that has 
/// little mass compared to the planet itself. This is a basic model which
/// decreases atmospheric density exponentially:
/// \f$
///     p(h_1) = p(h_o) e^{-\frac{\Delta h}{h_s}}
/// \f$
/// with\n
/// \f$p\f$: Pressure\n
/// \f$h\f$: Height\n
/// \f$h_s\f$: Scale height (scale factor parameterising exponential function)\n
/// \f$\Delta h\f$: \f$h_1 - h_0\f$
/// 
////////////////////////////////////////////////////////////////////////////////
class CObjectPlanet : public CObject
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CObjectPlanet();
        CObjectPlanet(const CObjectPlanet&);        
        CObjectPlanet& operator=(const CObjectPlanet&);

        //--- Constant methods -----------------------------------------------//
        const CObjectPlanet* clone() const;
        
        double getPressureAtGround() const;
        double getPressureAtHeight(const double&) const;
        double getPressureAtPosition(const Vector2d&) const;
        double getPressureAtRadius(const double&) const;
        double getRadius() const;

        //--- Methods --------------------------------------------------------//
        void setPressureAtGround(const double&);
        void setRadius(const double&);
        void setScaleHeight(const double&);
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CObjectPlanet* const);
        friend std::ostream&    operator<<(std::ostream&, CObjectPlanet* const);
        
    protected:
        
        //--- Methods [protected] --------------------------------------------//
        void copy(const CObjectPlanet&);

        //-- Variables [protected] -------------------------------------------//
//         CHandle<CPlanet> m_hPlanetShape;        ///< Planetary surface, default shape
        
        double m_fAtmosphericPressure = 1.0e5;  ///< 100 kPa, approximatly atmospheric pressure on earth
        double m_fRadius = 1.0e6;               ///< Arbitrary number, much larger than that of non-celestial objects
        double m_fScaleHeight = 1.0e4;          ///< Arbitrary number, every km pressure is reduced by factor \f$e ~= 2.7\f$
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get atmospheric pressure at ground level
///
/// \return Atmospheric pressure at ground level
///
////////////////////////////////////////////////////////////////////////////////
inline double CObjectPlanet::getPressureAtGround() const
{
    METHOD_ENTRY("CObjectPlanet::getPressureAtGround")
    return m_fAtmosphericPressure;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get atmospheric pressure at given height w.r.t. ground level
///
/// \param _fHeight Height to obtain atmospheric pressure from
///
/// \return Atmospheric pressure at given height
///
////////////////////////////////////////////////////////////////////////////////
inline double CObjectPlanet::getPressureAtHeight(const double& _fHeight) const
{
    METHOD_ENTRY("CObjectPlanet::getPressureAtHeight")
    return m_fAtmosphericPressure * std::exp(-_fHeight / m_fScaleHeight);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get atmospheric pressure at given position in world coordinates
///
/// \param _vecPos Position in world coordinates to get pressure from
///
/// \return Atmospheric pressure at given position in world coordinates
///
////////////////////////////////////////////////////////////////////////////////
inline double CObjectPlanet::getPressureAtPosition(const Vector2d& _vecPos) const
{
    METHOD_ENTRY("CObjectPlanet::getPressureAtPosition")
    double fHeight = (_vecPos - this->getCOM()).norm() - m_fRadius;
    return m_fAtmosphericPressure * std::exp(-fHeight / m_fScaleHeight);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get atmospheric pressure at given radius w.r.t. center of planet
///
/// \param _fRadius Radius to obtain atmospheric pressure from
///
/// \return Atmospheric pressure at given radius
///
////////////////////////////////////////////////////////////////////////////////
inline double CObjectPlanet::getPressureAtRadius(const double& _fRadius) const
{
    METHOD_ENTRY("CObjectPlanet::getPressureAtRadius")
    return m_fAtmosphericPressure * std::exp(-(_fRadius - m_fRadius) / m_fScaleHeight);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get radius of planetary object
///
/// \return Radius of planetary object
///
////////////////////////////////////////////////////////////////////////////////
inline double CObjectPlanet::getRadius() const
{
    METHOD_ENTRY("CObjectPlanet::getRadius")
    return m_fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set pressure (at ground level) of planetary objects atmosphere
///
/// \param _fPressure Pressure (at ground level) of planetary objects atmosphere
///
////////////////////////////////////////////////////////////////////////////////
inline void CObjectPlanet::setPressureAtGround(const double& _fPressure)
{
    METHOD_ENTRY("CObjectPlanet::setPressureAtGround")
    m_fAtmosphericPressure = _fPressure;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set radius of planetary object
///
/// \param _fRadius Radius of planetary object
///
////////////////////////////////////////////////////////////////////////////////
inline void CObjectPlanet::setRadius(const double& _fRadius)
{
    METHOD_ENTRY("CObjectPlanet::setRadius")
    m_fRadius = _fRadius;
    IShape* pShp = m_Geometry.getShapes().front().ptr();
    if (pShp != nullptr && pShp->getShapeType() == ShapeType::PLANET)
    {
        static_cast<CPlanet*>(pShp)->setRadius(_fRadius);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set scale height normalisation of planetary objects atmosphere
///
/// \param _fScaleHeight Scale height of planetary objects atmosphere
///
////////////////////////////////////////////////////////////////////////////////
inline void CObjectPlanet::setScaleHeight(const double& _fScaleHeight)
{
    METHOD_ENTRY("CObjectPlanet::setScaleHeight")
    m_fScaleHeight = _fScaleHeight;
}

#endif // OBJECT_PLANET_H
