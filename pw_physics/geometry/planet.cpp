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
/// \file       planet.cpp
/// \brief      Implementation of class "CPlanet"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-16
///
////////////////////////////////////////////////////////////////////////////////

#include "planet.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
/// The constructor creates a default planet that is similar to earth in
/// dimensions and landscape.
///
///////////////////////////////////////////////////////////////////////////////
CPlanet::CPlanet() : m_fAngle(0.0),
                     m_fHeightMax(8000.0),
                     m_fRadius(6378.16e3),
                     m_fSmoothness(1.0),
                     m_fGroundResolution(0.1),
                     m_nSeed(2)
{
    METHOD_ENTRY("CPlanet::CPlanet()");
    CTOR_CALL("CPlanet::CPlanet()");
    
    m_vecCenter0.setZero();
    
    m_MountainTerrain.SetFrequency(0.000025);
    m_MountainTerrain.SetLacunarity(1.9737346);
    m_MountainTerrain.SetNoiseQuality(QUALITY_BEST);
    m_MountainTerrain.SetOctaveCount(16);
    
    m_BaseFlatTerrain.SetFrequency(0.00001);
    m_BaseFlatTerrain.SetLacunarity(1.793947);
    m_BaseFlatTerrain.SetNoiseQuality(QUALITY_BEST);
    
    m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
    m_FlatTerrain.SetScale(0.25);
    m_FlatTerrain.SetBias(-0.75);
    
    m_TerrainType.SetFrequency (0.000001);
    m_TerrainType.SetPersistence (0.25);
    m_TerrainType.SetLacunarity(2.12358986);
    m_TerrainType.SetNoiseQuality(QUALITY_BEST);

    m_Surface.SetSourceModule(0,m_FlatTerrain);
    m_Surface.SetSourceModule(1,m_MountainTerrain);
    m_Surface.SetControlModule(m_TerrainType);
    m_Surface.SetBounds(0.0,100.0);
    m_Surface.SetEdgeFalloff(0.25);
    
    METHOD_EXIT("CPlanet::CPlanet()")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPlanet::~CPlanet()
{
    METHOD_ENTRY("CPlanet::~CPlanet")
    DTOR_CALL("CPlanet::~CPlanet")

    METHOD_EXIT("CPlanet::~CPlanet")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the planets terrain
///
/// For initialisation, some parameters depend on others. Radius times 2 Pi
/// is important for the maximum frequency, depending on the ground resolution.
///
///////////////////////////////////////////////////////////////////////////////
void CPlanet::initTerrain()
{
    METHOD_ENTRY("CPlanet::initTerrain")
    
    double fNrOfPoints = 2.0 * M_PI * m_fRadius / m_fGroundResolution;
    double fNrOfMountains = 2.0 * M_PI * m_fRadius / (m_fHeightMax*M_PI_2) / 2.0;
    
    double fMinF = 0.5*fNrOfMountains/(2.0*M_PI*m_fRadius);
    double fMaxF = 1.0;
    
    int nMaxOctave = std::log(fMaxF/(0.5 * fNrOfMountains / fNrOfPoints));
    
    INFO_MSG("Planet", "Generating Terrain (Mountains)")
    DOM_VAR(INFO_MSG("Planet", "Number of Mountains: " << fNrOfMountains))
    DOM_VAR(INFO_MSG("Planet", "Number of Points:    " << fNrOfPoints))
    DOM_VAR(INFO_MSG("Planet", "Minimum Frequency:   " << fMinF))
    DOM_VAR(INFO_MSG("Planet", "Maximum Frequency:   " << fMaxF))
    DOM_VAR(INFO_MSG("Planet", "Maximum Octaves:     " << nMaxOctave))

    m_MountainTerrain.SetFrequency(fMinF);
    m_MountainTerrain.SetLacunarity(1.937);
    m_MountainTerrain.SetNoiseQuality(QUALITY_BEST);
    m_MountainTerrain.SetOctaveCount(nMaxOctave);
    
    m_BaseFlatTerrain.SetFrequency(fMinF/*0.5*12/(2.0*M_PI*m_fRadius)*/);
    m_BaseFlatTerrain.SetLacunarity(1.93947);
    m_BaseFlatTerrain.SetNoiseQuality(QUALITY_BEST);
    m_BaseFlatTerrain.SetOctaveCount(nMaxOctave);
    
    m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
    m_FlatTerrain.SetScale(0.25);
    m_FlatTerrain.SetBias(-0.75);
    
    m_TerrainType.SetFrequency (0.5*80/(2.0*M_PI*m_fRadius));
    m_TerrainType.SetPersistence (0.5);
    m_TerrainType.SetLacunarity(2.137);
    m_TerrainType.SetNoiseQuality(QUALITY_BEST);

    m_Surface.SetSourceModule(0,m_FlatTerrain);
    m_Surface.SetSourceModule(1,m_MountainTerrain);
    m_Surface.SetControlModule(m_TerrainType);
    m_Surface.SetBounds(0.0,100.0);
    m_Surface.SetEdgeFalloff(0.5);

    METHOD_EXIT("CPlanet::initTerrain")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CPlanet::transform( const double& _fAngle, const Vector2d& _vecV )
{
    METHOD_ENTRY("CPlanet::transform")

    Rotation2Dd Rotation(_fAngle);

    m_vecCenter = Rotation * m_vecCenter0 + _vecV;

    m_fAngle = _fAngle;

    // Update bounding box
    m_AABB.setLowerLeft( m_vecCenter - Vector2d(m_fRadius+m_fHeightMax,
                                                m_fRadius+m_fHeightMax));
    m_AABB.setUpperRight(m_vecCenter + Vector2d(m_fRadius+m_fHeightMax,
                                                m_fRadius+m_fHeightMax));

    METHOD_EXIT("CPlanet::transform")
}
