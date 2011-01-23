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
///////////////////////////////////////////////////////////////////////////////
CPlanet::CPlanet() : m_fAngle(0.0),
                     m_fHeightMax(1.0e3),
                     m_fRadius(1.0e5),
                     m_fSmoothness(1.0),
                     m_nNrOfSamplingPoints(20),
                     m_nSeed(2)
{
    METHOD_ENTRY("CPlanet::CPlanet()");
    CTOR_CALL("CPlanet::CPlanet()");
    
    m_vecCenter0.setZero();
    
    module::RidgedMulti mountainTerrain;
    mountainTerrain.SetFrequency(0.000025);
    mountainTerrain.SetLacunarity(1.9737346);
    mountainTerrain.SetNoiseQuality(QUALITY_BEST);
    mountainTerrain.SetOctaveCount(10);

    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency (0.00001);
    baseFlatTerrain.SetLacunarity(1.793947);
    baseFlatTerrain.SetNoiseQuality(QUALITY_BEST);

    module::ScaleBias flatTerrain;
    flatTerrain.SetSourceModule (0, baseFlatTerrain);
    flatTerrain.SetScale (0.25);
    flatTerrain.SetBias (-0.75);
    
    module::Perlin terrainType;
    terrainType.SetFrequency (0.000001);
    terrainType.SetPersistence (0.25);
    terrainType.SetLacunarity(2.12358986);
    terrainType.SetNoiseQuality(QUALITY_BEST);

    module::Select m_Surface;
    m_Surface.SetSourceModule (0, flatTerrain);
    m_Surface.SetSourceModule (1, mountainTerrain);
    m_Surface.SetControlModule (terrainType);
    m_Surface.SetBounds (0.0, 100000.0);
    m_Surface.SetEdgeFalloff (0.125);
    
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
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
/// \todo Add maximum height to radius for bounding box calculation
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
