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
/// \file       terrain.cpp
/// \brief      Implementation of class "CTerrain"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
/// The constructor creates a default terrain that is similar to earth in
/// dimensions and landscape.
///
///////////////////////////////////////////////////////////////////////////////
CTerrain::CTerrain() : m_fAngle(0.0),
                     m_fHeightMax(100.0),
                     m_fSmoothness(1.0),
                     m_fGroundResolution(1.0),
                     m_fWidth(200.0),
                     m_nSeed(2)
{
    METHOD_ENTRY("CTerrain::CTerrain()");
    CTOR_CALL("CTerrain::CTerrain()");
    
    m_vecCenter0.setZero();
    
    m_MountainTerrain.SetFrequency(0.000025);
    m_MountainTerrain.SetLacunarity(1.9737346);
    m_MountainTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    m_MountainTerrain.SetOctaveCount(16);
    
    m_BaseFlatTerrain.SetFrequency(0.00001);
    m_BaseFlatTerrain.SetLacunarity(1.793947);
    m_BaseFlatTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    
    m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
    m_FlatTerrain.SetScale(0.25);
    m_FlatTerrain.SetBias(-0.75);
    
    m_TerrainType.SetFrequency (0.000001);
    m_TerrainType.SetPersistence (0.25);
    m_TerrainType.SetLacunarity(2.12358986);
    m_TerrainType.SetNoiseQuality(noise::QUALITY_BEST);

    m_Surface.SetSourceModule(0,m_FlatTerrain);
    m_Surface.SetSourceModule(1,m_MountainTerrain);
    m_Surface.SetControlModule(m_TerrainType);
    m_Surface.SetBounds(0.0,100.0);
    m_Surface.SetEdgeFalloff(0.25);
    
    METHOD_EXIT("CTerrain::CTerrain()")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CTerrain::~CTerrain()
{
    METHOD_ENTRY("CTerrain::~CTerrain")
    DTOR_CALL("CTerrain::~CTerrain")

    METHOD_EXIT("CTerrain::~CTerrain")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the terrain
///
/// For initialisation, some parameters depend on others. The terrain width
/// is important for the maximum frequency, depending on the ground resolution.
///
///////////////////////////////////////////////////////////////////////////////
void CTerrain::init()
{
    METHOD_ENTRY("CTerrain::init")
    
    double fNrOfPoints = m_fWidth / m_fGroundResolution;
    double fNrOfMountains = m_fWidth / m_fHeightMax *2.0;
    
    double fMinF = fNrOfMountains/fNrOfPoints;
    double fMaxF = 1.0;
    
    int nMaxOctave = log2(fMaxF/fMinF)+1;
    if (nMaxOctave < 1) nMaxOctave = 1;
    
    INFO_MSG("Terrain", "Generating Terrain (Mountains)")
    DOM_VAR(INFO_MSG("Terrain", "Number of Mountains: " << fNrOfMountains))
    DOM_VAR(INFO_MSG("Terrain", "Number of Points:    " << fNrOfPoints))
    DOM_VAR(INFO_MSG("Terrain", "Minimum Frequency:   " << fMinF))
    DOM_VAR(INFO_MSG("Terrain", "Maximum Frequency:   " << fMaxF))
    DOM_VAR(INFO_MSG("Terrain", "Maximum Octaves:     " << nMaxOctave))

    m_MountainTerrain.SetFrequency(fMinF);
    m_MountainTerrain.SetLacunarity(1.937);
    m_MountainTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    m_MountainTerrain.SetOctaveCount(nMaxOctave);
    
    m_BaseFlatTerrain.SetFrequency(fMinF/*0.5*12/(2.0*M_PI*m_fRadius)*/);
    m_BaseFlatTerrain.SetLacunarity(1.93947);
    m_BaseFlatTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    m_BaseFlatTerrain.SetOctaveCount(nMaxOctave);
    m_BaseFlatTerrain.SetPersistence(0.5);
    
    m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
    m_FlatTerrain.SetScale(0.25);
    m_FlatTerrain.SetBias(-0.75);
    
    m_TerrainType.SetFrequency (1000.0/(m_fWidth));
    m_TerrainType.SetPersistence (0.5);
    m_TerrainType.SetLacunarity(2.137);
    m_TerrainType.SetNoiseQuality(noise::QUALITY_BEST);

    m_Surface.SetSourceModule(0,m_FlatTerrain);
    m_Surface.SetSourceModule(1,m_MountainTerrain);
    m_Surface.SetControlModule(m_TerrainType);
    m_Surface.SetBounds(0.0,100.0);
    m_Surface.SetEdgeFalloff(0.5);

    METHOD_EXIT("CTerrain::init")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CTerrain::transform( const double& _fAngle, const Vector2d& _vecV )
{
    METHOD_ENTRY("CTerrain::transform")

    Rotation2Dd Rotation(_fAngle);

    m_vecCenter = Rotation * m_vecCenter0 + _vecV;

    m_fAngle = _fAngle;

    // Update bounding box
    m_AABB.setLowerLeft( m_vecCenter - Vector2d(m_fWidth*0.5,0.0));
    m_AABB.setUpperRight(m_vecCenter + Vector2d(m_fWidth*0.5,m_fHeightMax));

    METHOD_EXIT("CTerrain::transform")
}
