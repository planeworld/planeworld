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
                     m_fSeaLevel(0.0),
                     m_fSmoothness(1.0),
                     m_fGroundResolution(1.0),
                     m_nSeed(1),
                     m_fLacHlTr(1.937),
                     m_fLacMtTr(2.137),
                     m_fLacTrTp(2.0531),
                     m_nOctHlTr(5),
                     m_nOctMtTr(5),
                     m_nOctTrTp(5)
{
    METHOD_ENTRY("CPlanet::CPlanet()");
    CTOR_CALL("CPlanet::CPlanet()");
    
    m_vecCenter0.setZero();
    
//     m_MountainTerrain.SetFrequency(0.000025);
//     m_MountainTerrain.SetLacunarity(1.9737346);
//     m_MountainTerrain.SetNoiseQuality(noise::QUALITY_BEST);
//     m_MountainTerrain.SetOctaveCount(16);
//     
//     m_BaseFlatTerrain.SetFrequency(0.00001);
//     m_BaseFlatTerrain.SetLacunarity(1.793947);
//     m_BaseFlatTerrain.SetNoiseQuality(noise::QUALITY_BEST);
//     
//     m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
//     m_FlatTerrain.SetScale(0.25);
//     m_FlatTerrain.SetBias(-0.75);
//     
//     m_TerrainType.SetFrequency (0.000001);
//     m_TerrainType.SetPersistence (0.25);
//     m_TerrainType.SetLacunarity(2.12358986);
//     m_TerrainType.SetNoiseQuality(noise::QUALITY_BEST);
// 
//     m_Surface.SetSourceModule(0,m_FlatTerrain);
//     m_Surface.SetSourceModule(1,m_MountainTerrain);
//     m_Surface.SetControlModule(m_TerrainType);
//     m_Surface.SetBounds(0.0,100.0);
//     m_Surface.SetEdgeFalloff(0.25);
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
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones planet
///
/// \return Pointer to cloned planet
///
////////////////////////////////////////////////////////////////////////////////
CPlanet* CPlanet::clone() const
{
    METHOD_ENTRY("CPlanet::clone")
    
    CPlanet* pClone = new CPlanet();
    MEM_ALLOC("pClone")
        
    pClone->m_fAngle            = m_fAngle;
    pClone->m_fGroundResolution = m_fGroundResolution;
    pClone->m_fHeightMax        = m_fHeightMax;
    pClone->m_fRadius           = m_fRadius;
    pClone->m_fSeaLevel         = m_fSeaLevel;
    pClone->m_fSmoothness       = m_fSmoothness;
    pClone->m_nSeed             = m_nSeed;
    pClone->m_vecCenter         = m_vecCenter;
    pClone->m_vecCenter0        = m_vecCenter0;
    pClone->m_fLacHlTr          = m_fLacHlTr;
    pClone->m_fLacMtTr          = m_fLacMtTr;
    pClone->m_fLacTrTp          = m_fLacTrTp;
    pClone->m_nOctHlTr          = m_nOctHlTr;
    pClone->m_nOctMtTr          = m_nOctMtTr;
    pClone->m_nOctTrTp          = m_nOctTrTp;
    pClone->m_AABB              = m_AABB;
    pClone->m_nDepthlayers      = m_nDepthlayers;
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the planets terrain
///
/// For initialisation, some parameters depend on others. Radius times 2 Pi
/// is important for the maximum frequency, depending on the ground resolution.
/// It is also important that the one unit (double fD=1.0) corresponds to one
/// meter, because the frequency of noise methods is dependend on the unit,
/// too.
///
///////////////////////////////////////////////////////////////////////////////
void CPlanet::initTerrain()
{
    METHOD_ENTRY("CPlanet::initTerrain")

    this->myInitTerrain();
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
    {
        static_cast<CPlanet*>(m_pBuf)->myInitTerrain();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets sampling of planet surface by given zoom factor
///
/// The frequency is also calibrated for meter as unit. Thus, the initial maximum
/// frequency is 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD).
///
/// \param _fMaxF Maximum sampling frequency of planet surface
///
/// \todo setSampling and resetSampling are only used temporarily at the moment
///       when used by visuals. Therefore, no double buffering is needed here.
///       If used otherwise, double buffering must be incorporated. Apart from
///       this, the physics needs full precision, so there must be a mutex for
///       these cases or visuals need their own noise methods.
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::setSampling(const double& _fMaxF)
{
    METHOD_ENTRY("CPlanet::setSampling")
    
    double fMinF;
    double fMaxF;
    int nOct;

    fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
    fMinF = 1.0 / (m_fHeightMax*M_PI_2);
    
    if (_fMaxF < fMaxF) fMaxF = _fMaxF;
    nOct = log2(fMaxF/fMinF)/log2(m_fLacMtTr)+1;
    if (nOct < 1) nOct = 1;
    m_MountainTerrain.SetOctaveCountTmp(nOct);
    
    nOct = log2(fMaxF/fMinF)/log2(m_fLacHlTr)+1;
    if (nOct < 1) nOct = 1;
    m_BaseFlatTerrain.SetOctaveCountTmp(nOct);
    
    nOct = log2(100)/log2(m_fLacTrTp)+1;
    if (nOct < 1) nOct = 1;
    m_TerrainType.SetOctaveCountTmp(nOct);
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
    {
        static_cast<CPlanet*>(m_pBuf)->setSampling(_fMaxF);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets the sampling to original value given by octave count
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::resetSampling()
{
    METHOD_ENTRY("CPlanet::resetSampling")
    
    m_BaseFlatTerrain.SetOctaveCountTmp(m_nOctHlTr);
    m_MountainTerrain.SetOctaveCountTmp(m_nOctMtTr);
    m_TerrainType.SetOctaveCountTmp(m_nOctTrTp);
    
    // We have a buffer which must also be updated
    if (m_pBuf != 0)
    {
        static_cast<CPlanet*>(m_pBuf)->resetSampling();
    }
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the planets terrain
///
/// For initialisation, some parameters depend on others. Radius times 2 Pi
/// is important for the maximum frequency, depending on the ground resolution.
/// It is also important that the one unit (double fD=1.0) corresponds to one
/// meter, because the frequency of noise methods is dependend on the unit,
/// too.
///
/// This method is internal to make it usable in double buffering
///
///////////////////////////////////////////////////////////////////////////////
void CPlanet::myInitTerrain()
{
    METHOD_ENTRY("CPlanet::myInitTerrain")

    m_fLacHlTr = 1.9371;
    m_fLacMtTr = 2.137;
    m_fLacTrTp = 2.317;
    
    double fNrOfPoints = 2.0*M_PI * m_fRadius / m_fGroundResolution;
    double fNrOfMountains = 2.0*M_PI * m_fRadius / (m_fHeightMax*M_PI_2);
       
    double fMinF = 1.0 / (m_fHeightMax*M_PI_2);
    double fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
    
    m_nOctHlTr = log2(fMaxF/fMinF)/log2(m_fLacHlTr)+1;
    if (m_nOctHlTr < 1) m_nOctHlTr = 1;
    
    m_nOctMtTr = log2(fMaxF/fMinF)/log2(m_fLacMtTr)+1;
    if (m_nOctMtTr < 1) m_nOctMtTr = 1;
    
    INFO_MSG("Planet", "Generating Terrain (Mountains)")
    DOM_VAR(INFO_MSG("Planet", "Number of Mountains:       " << fNrOfMountains))
    DOM_VAR(INFO_MSG("Planet", "Number of Points:          " << fNrOfPoints))
    DOM_VAR(INFO_MSG("Planet", "Minimum Frequency:         " << fMinF << "/m"))
    DOM_VAR(INFO_MSG("Planet", "Maximum Frequency:         " << fMaxF << "/m"))
    DOM_VAR(INFO_MSG("Planet", "Maximum Octaves Mountains: " << m_nOctMtTr))
    DOM_VAR(INFO_MSG("Planet", "Maximum Octaves Hills:     " << m_nOctHlTr))

    m_MountainTerrain.SetSeed(m_nSeed);
    m_MountainTerrain.SetFrequency(fMinF);
    m_MountainTerrain.SetLacunarity(m_fLacMtTr);
    m_MountainTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    m_MountainTerrain.SetOctaveCount(m_nOctMtTr);
    
    m_ClampTerrain.SetSourceModule (0, m_MountainTerrain);
    m_ClampTerrain.SetBounds (-1.0, 0.8);
    
    m_TerraceTerrain.SetSourceModule (0, m_ClampTerrain);
    m_TerraceTerrain.AddControlPoint ( 0.0000);
    m_TerraceTerrain.AddControlPoint ( 0.2500);
    m_TerraceTerrain.AddControlPoint ( 0.5000);
    m_TerraceTerrain.AddControlPoint ( 0.7500);
    m_TerraceTerrain.AddControlPoint ( 0.8750);
    m_TerraceTerrain.AddControlPoint ( 1.0000);
    
    m_BaseFlatTerrain.SetSeed(m_nSeed+3);
    m_BaseFlatTerrain.SetFrequency(fMinF);
    m_BaseFlatTerrain.SetLacunarity(m_fLacHlTr);
    m_BaseFlatTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    m_BaseFlatTerrain.SetOctaveCount(m_nOctHlTr);
    
//     m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
//     m_FlatTerrain.SetScale(0.25);
//     m_FlatTerrain.SetBias(-0.75);
    
    m_TerrainType.SetSeed(m_nSeed+7);
    m_TerrainType.SetFrequency (0.5*100.0/(2.0*M_PI*m_fRadius));
    m_TerrainType.SetPersistence (0.5);
    m_TerrainType.SetLacunarity(m_fLacTrTp);
    m_TerrainType.SetNoiseQuality(noise::QUALITY_BEST);
    m_TerrainType.SetOctaveCount(15);

    m_Surface.SetSourceModule(0,m_BaseFlatTerrain);
    m_Surface.SetSourceModule(1,m_TerraceTerrain);
    m_Surface.SetControlModule(m_TerrainType);
    m_Surface.SetBounds(0.0,1.0);
    m_Surface.SetEdgeFalloff(0.05);
}