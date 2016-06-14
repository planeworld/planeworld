////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       planet.h
/// \brief      Prototype of class "CPlanet"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PLANET_H
#define PLANET_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include "noise2d/noise.h"

/// Specifies the major type of the planet which defines basic noise functions
/// and colours.
typedef enum 
{
    PLANET_TYPE_EARTHLIKE,
    PLANET_TYPE_ICE,
    PLANET_TYPE_ROCK,
} PlanetType;

typedef enum
{
    PLANET_TERRAIN_TYPE_BADLANDS,   // With and without terraces
    PLANET_TERRAIN_TYPE_CRATER,     // Size and number depends on atmosphere and history
    PLANET_TERRAIN_TYPE_DUNES,      // Sand
    PLANET_TERRAIN_TYPE_HILLS,      // On earthlike, green
    PLANET_TERRAIN_TYPE_MOUNTAINS,  // Mountains
    PLANET_TERRAIN_TYPE_SUBSEA,     // Underwater
    PLANET_TERRAIN_TYPE_TERRACES,   // On earth and rocky structures, depends on water?
} TerrainType;

const double PLANET_DEFAULT_VERTICES_PER_PERIOD = 5.0; ///< Vertices per period for calculation of octaves

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a procedurally created planet shape.
///
////////////////////////////////////////////////////////////////////////////////
class CPlanet : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CPlanet();
        virtual ~CPlanet();
        
        //--- Constant Methods -----------------------------------------------//
        CPlanet*                     clone() const;
        
        const double&                getAngle() const;
        const Vector2d&              getCenter() const;
        const double&                getGroundResolution() const;
        const double&                getHeight() const;
        const double&                getRadius() const;
        const double&                getSeaLevel() const;
        const int&                   getSeed() const;
        const double&                getSmoothness() const;
        const noise::module::Module* getSurface() const;
        const noise::module::Module* getTerrainType() const;
        
              ShapeType              getShapeType() const;

        //--- Methods --------------------------------------------------------//
        void initTerrain();
        
        void setPlanetType(const PlanetType&);
        
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setGroundResolution(const double&);
        void setHeight(const double&);
        void setRadius(const double&);
        void setSampling(const double&);
        void setSeaLevel(const double&);
        void setSeed(const int&);
        void setSmoothness(const double&);
        
        void resetSampling();
        
        void transform(const double&, const Vector2d&, const Vector2d&);
                        
    protected:
        
        std::istream&       myStreamIn(std::istream&);
        std::ostream&       myStreamOut(std::ostream&);
        
        void                myCopy(const IShape* const);
        void                myInitTerrain();
        void                myUpdateGeometry();
        
        PlanetType          m_PlanetType;               ///< Identifies type of planet

        Vector2d            m_vecCenter;                ///< Center of planet
        Vector2d            m_vecCenter0;               ///< Initial center of planet
        double              m_fAngle;                   ///< Angle, just for optical reasons
        double              m_fGroundResolution;        ///< Ground resolution in m
        double              m_fHeightMax;               ///< Maximum height of terrain
        double              m_fRadius;                  ///< Minimum radius of planet
        double              m_fSeaLevel;                ///< Sea level in [-1,...,1], if there is water 
        double              m_fSmoothness;              ///< Smoothness of planet landscape
        int                 m_nSeed;                    ///< Unique seed for terrain generation
        
        noise::module::Module*     m_pSurface;          ///< Final surface noise function
        noise::module::Module*     m_pTerrainType;      ///< Final terrain type noise function
        
        std::vector<noise::module::Billow>      m_Billow;       ///< Billow modules
        std::vector<noise::module::Clamp>       m_Clamp;        ///< Clamp modules
        std::vector<noise::module::Perlin>      m_Perlin;       ///< Perlin modules
        std::vector<noise::module::RidgedMulti> m_RidgedMulti;  ///< RidgedMulti modules
        std::vector<noise::module::ScaleBias>   m_ScaleBias;    ///< Selector modules
        std::vector<noise::module::Select>      m_Selector;     ///< Selector modules
        std::vector<noise::module::Terrace>     m_Terrace;      ///< Terrace modules
        
        double              m_fLacHlTr;                 ///< Lacunarity for hilly terrain
        double              m_fLacMtTr;                 ///< Lacunarity for mountain terrain
        double              m_fLacTrTp;                 ///< Lacunarity of terrain type
        int                 m_nOctHlTr;                 ///< Maximum number of octaves for hilly terrain
        int                 m_nOctMtTr;                 ///< Maximum number of octaves for mountain terrain
        int                 m_nOctTrTp;                 ///< Maximum number of octaves for terrain type
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return rotation angle of the planet
///
/// \return Rotation angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getAngle() const
{
    METHOD_ENTRY("CPlanet::getAngle")

    METHOD_EXIT("CPlanet::getAngle")
    return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the center of the planet
///
/// \return Vector, representing center of planet
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CPlanet::getCenter() const
{
    METHOD_ENTRY("CPlanet::getCenter")

    METHOD_EXIT("CPlanet::getCenter")
    return m_vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the ground resolution of the terrain
///
/// The ground resolution defines the length of a minimum segment between two
/// support points.
///
/// \return Ground resolution in m
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getGroundResolution() const
{
    METHOD_ENTRY("CPlanet::getNrOfSamplingPoints")

    METHOD_EXIT("CPlanet::getNrOfSamplingPoints")
    return m_fGroundResolution;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the height of planets terrain
///
/// \return Height
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getHeight() const
{
    METHOD_ENTRY("CPlanet::getHeight")

    METHOD_EXIT("CPlanet::getHeight")
    return m_fHeightMax;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the radius of the planet
///
/// \return Radius
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getRadius() const
{
    METHOD_ENTRY("CPlanet::getRadius")

    return m_fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the sea level of the planet
///
/// \return SeaLevel
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getSeaLevel() const
{
    METHOD_ENTRY("CPlanet::getSeaLevel")
    return m_fSeaLevel;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the seed of the planet
///
/// \return Seed
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CPlanet::getSeed() const
{
    METHOD_ENTRY("CPlanet::getSeed")
    return m_nSeed;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the smoothness of planets terrain
///
/// \return Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getSmoothness() const
{
    METHOD_ENTRY("CPlanet::getSmoothness")
    return m_fSmoothness;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return surface noise module of the planet
///
/// \return Surface noise module
///
////////////////////////////////////////////////////////////////////////////////
inline const noise::module::Module* CPlanet::getSurface() const
{
    METHOD_ENTRY("CPlanet::getSurface")
    return m_pSurface;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return terrain type noise module of the planet
///
/// \return terrain type noise module
///
////////////////////////////////////////////////////////////////////////////////
inline const noise::module::Module* CPlanet::getTerrainType() const
{
    METHOD_ENTRY("CPlanet::getTerrainType")
    return m_pTerrainType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline ShapeType CPlanet::getShapeType() const
{
    METHOD_ENTRY("CPlanet::getShapeType")
    return ShapeType::SHAPE_PLANET;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the type of this planet
///
/// \param _PlanetType Type of this planet
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setPlanetType(const PlanetType& _PlanetType)
{
    METHOD_ENTRY("CPlanet::setPlanetType")

    m_PlanetType = _PlanetType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the ground resolution of the terrain
///
/// The ground resolution defines the length of a minimum segment between two
/// support points.
///
/// \param _fGRes Ground resolution in m
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setGroundResolution(const double& _fGRes)
{
    METHOD_ENTRY("CPlanet::setGroundResolution")
    m_fGroundResolution = _fGRes;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the maximum terrain height of the planet
///
/// \param _fH Maximum terrain height
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setHeight(const double& _fH)
{
    METHOD_ENTRY("CPlanet::setHeight")
    m_fHeightMax = _fH;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the minimum radius of the planet
///
/// \param _fR Radius
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setRadius(const double& _fR)
{
    METHOD_ENTRY("CPlanet::setRadius")
    m_fRadius = _fR;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets sea level of the planet
///
/// \param _fSeaLevel SeaLevel
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setSeaLevel(const double& _fSeaLevel)
{
    METHOD_ENTRY("CPlanet::setSeaLevel")
    m_fSeaLevel = _fSeaLevel;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets unique seed for procedural terrain generation of the planet
///
/// \param _nS Seed
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setSeed(const int& _nS)
{
    METHOD_ENTRY("CPlanet::setSeed")
    m_nSeed = _nS;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets smoothness for procedural terrain generation of the planet
///
/// \param _fS Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setSmoothness(const double& _fS)
{
    METHOD_ENTRY("CPlanet::setSmoothness")

    m_fSmoothness = _fS;
}

#endif
