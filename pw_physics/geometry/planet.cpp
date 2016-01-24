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
CPlanet::CPlanet() : m_PlanetType(PLANET_TYPE_EARTHLIKE),
                     m_fAngle(0.0),
                     m_fHeightMax(10.0),
                     m_fRadius(1000.0),
                     m_fSeaLevel(0.0),
                     m_fSmoothness(1.0),
                     m_fGroundResolution(1.0),
                     m_nSeed(1),
                     m_pSurface(0),
                     m_pTerrainType(0),
                     m_fLacHlTr(1.937),
                     m_fLacMtTr(2.137),
                     m_fLacTrTp(2.0531),
                     m_nOctHlTr(5),
                     m_nOctMtTr(5),
                     m_nOctTrTp(5)
{
    METHOD_ENTRY("CPlanet::CPlanet")
    CTOR_CALL("CPlanet::CPlanet")
    
    m_vecCenter0.setZero();
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
    MEM_ALLOC("CPlanet")
        
    pClone->m_PlanetType        = m_PlanetType;
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies information of a given planet shape.
///
/// This method does not create a new planet shape, use clone in that case!
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::copy(const IShape* const _pShape)
{
    METHOD_ENTRY("CPlanet::copy");
    
    const CPlanet* const pPlanet = static_cast<const CPlanet* const>(_pShape);
        
    m_PlanetType        = pPlanet->m_PlanetType;
    m_fAngle            = pPlanet->m_fAngle;
    m_fGroundResolution = pPlanet->m_fGroundResolution;
    m_fHeightMax        = pPlanet->m_fHeightMax;
    m_fRadius           = pPlanet->m_fRadius;
    m_fSeaLevel         = pPlanet->m_fSeaLevel;
    m_fSmoothness       = pPlanet->m_fSmoothness;
    m_nSeed             = pPlanet->m_nSeed;
    m_vecCenter         = pPlanet->m_vecCenter;
    m_vecCenter0        = pPlanet->m_vecCenter0;
    m_fLacHlTr          = pPlanet->m_fLacHlTr;
    m_fLacMtTr          = pPlanet->m_fLacMtTr;
    m_fLacTrTp          = pPlanet->m_fLacTrTp;
    m_nOctHlTr          = pPlanet->m_nOctHlTr;
    m_nOctMtTr          = pPlanet->m_nOctMtTr;
    m_nOctTrTp          = pPlanet->m_nOctTrTp;
    m_AABB              = pPlanet->m_AABB;
    m_nDepthlayers      = pPlanet->m_nDepthlayers;
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
    nOct = ceil(log2(fMaxF/fMinF)/log2(m_fLacMtTr));
    if (nOct < 1) nOct = 1;
    for (int i=0; i<m_RidgedMulti.size(); ++i)
        m_RidgedMulti[i].SetOctaveCountTmp(nOct);
    
    nOct = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
    if (nOct < 1) nOct = 1;
    for (int i=0; i<m_Billow.size(); ++i)
        m_Billow[i].SetOctaveCountTmp(nOct);
    
//     nOct = ceil(log2(fMaxF/(0.5*100.0/(2.0*M_PI*m_fRadius)))/log2(m_fLacTrTp));
//     if (nOct < 1) nOct = 1;
//     m_TerrainType.SetOctaveCountTmp(nOct);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets the sampling to original value given by octave count
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::resetSampling()
{
    METHOD_ENTRY("CPlanet::resetSampling")
    
    for (int i=0; i<m_Billow.size(); ++i)
        m_Billow[i].SetOctaveCountTmp(m_nOctHlTr);
    for (int i=0; i<m_RidgedMulti.size(); ++i)
        m_RidgedMulti[i].SetOctaveCountTmp(m_nOctMtTr);
//     m_TerrainType.SetOctaveCountTmp(m_nOctTrTp);
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CPlanet instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CPlanet::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CPlanet::myStreamOut")
    
    _os << m_PlanetType;
    _os << m_vecCenter;
    _os << m_vecCenter0;
    _os << m_fAngle;
    _os << m_fGroundResolution;
    _os << m_fHeightMax;
    _os << m_fRadius;
    _os << m_fSeaLevel;
    _os << m_fSmoothness;
    _os << m_nSeed;
    
//         noise::module::Module*     m_pSurface;          ///< Final surface noise function
//         noise::module::Module*     m_pTerrainType;      ///< Final terrain type noise function
//         
//         std::vector<noise::module::Billow>      m_Billow;       ///< Billow modules
//         std::vector<noise::module::Clamp>       m_Clamp;        ///< Clamp modules
//         std::vector<noise::module::Perlin>      m_Perlin;       ///< Perlin modules
//         std::vector<noise::module::RidgedMulti> m_RidgedMulti;  ///< RidgedMulti modules
//         std::vector<noise::module::ScaleBias>   m_ScaleBias;    ///< Selector modules
//         std::vector<noise::module::Select>      m_Selector;     ///< Selector modules
//         std::vector<noise::module::Terrace>     m_Terrace;      ///< Terrace modules
    
    _os << m_fLacHlTr;
    _os << m_fLacMtTr;
    _os << m_fLacTrTp;
    _os << m_nOctHlTr;
    _os << m_nOctMtTr;
    _os << m_nOctTrTp;
    
    _os << "CPlanet::streamOut TEST";
    
    return _os;
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
    
    // Cleanup everything for the rare case that terrain is initialised more than once.
    m_Billow.clear();
    m_Clamp.clear();
    m_Perlin.clear();
    m_RidgedMulti.clear();
    m_Selector.clear();
    m_Terrace.clear();
    
    switch(m_PlanetType)
    {
        case PLANET_TYPE_EARTHLIKE:
        {
            m_fLacHlTr = 1.9371;
            m_fLacMtTr = 2.137;
            m_fLacTrTp = 2.317;
            
            double fNrOfPoints = 2.0*M_PI * m_fRadius / m_fGroundResolution;
            double fNrOfMountains = 2.0*M_PI * m_fRadius / (m_fHeightMax*M_PI_2);
            
            double fMinF = 1.0 / (m_fHeightMax*M_PI_2);
            double fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
            
            m_nOctHlTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
            if (m_nOctHlTr < 1) m_nOctHlTr = 1;
            
            m_nOctMtTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacMtTr));
            if (m_nOctMtTr < 1) m_nOctMtTr = 1;
            
            m_nOctTrTp = ceil(log2(fMaxF/(0.5*100.0/(2.0*M_PI*m_fRadius)))/log2(m_fLacTrTp));
            if (m_nOctTrTp < 1) m_nOctTrTp = 1;
            
            INFO_MSG("Planet", "Generating Terrain (Mountains)")
            DOM_VAR(DEBUG_MSG("Planet", "Number of Mountains:       " << fNrOfMountains))
            DOM_VAR(DEBUG_MSG("Planet", "Number of Points:          " << fNrOfPoints))
            DOM_VAR(DEBUG_MSG("Planet", "Minimum Frequency:         " << fMinF << "/m"))
            DOM_VAR(DEBUG_MSG("Planet", "Maximum Frequency:         " << fMaxF << "/m"))
            DOM_VAR(DEBUG_MSG("Planet", "Maximum Octaves Mountains: " << m_nOctMtTr))
            DOM_VAR(DEBUG_MSG("Planet", "Maximum Octaves Hills:     " << m_nOctHlTr))
            
            // Reserve memory for noise functions
            m_Billow.resize(1);
            m_Clamp.resize(1);
            m_Perlin.resize(1);
            m_RidgedMulti.resize(1);
            m_Selector.resize(1);
            m_Terrace.resize(1);

            m_RidgedMulti[0].SetSeed(m_nSeed);
            m_RidgedMulti[0].SetFrequency(fMinF);
            m_RidgedMulti[0].SetLacunarity(m_fLacMtTr);
            m_RidgedMulti[0].SetNoiseQuality(noise::QUALITY_BEST);
            m_RidgedMulti[0].SetOctaveCount(m_nOctMtTr);
            
            m_Clamp[0].SetSourceModule (0, m_RidgedMulti[0]);
            m_Clamp[0].SetBounds (-1.0, 0.8);
            
            m_Terrace[0].SetSourceModule (0, m_Clamp[0]);
            m_Terrace[0].AddControlPoint ( 0.0000);
            m_Terrace[0].AddControlPoint ( 0.2500);
            m_Terrace[0].AddControlPoint ( 0.5000);
            m_Terrace[0].AddControlPoint ( 0.7500);
            m_Terrace[0].AddControlPoint ( 0.8750);
            m_Terrace[0].AddControlPoint ( 1.0000);
            
            m_Billow[0].SetSeed(m_nSeed+3);
            m_Billow[0].SetFrequency(fMinF);
            m_Billow[0].SetLacunarity(m_fLacHlTr);
            m_Billow[0].SetNoiseQuality(noise::QUALITY_BEST);
            m_Billow[0].SetOctaveCount(m_nOctHlTr);
            
        //     m_FlatTerrain.SetSourceModule(0,m_BaseFlatTerrain);
        //     m_FlatTerrain.SetScale(0.25);
        //     m_FlatTerrain.SetBias(-0.75);
            
            m_Perlin[0].SetSeed(m_nSeed+7);
            m_Perlin[0].SetFrequency (0.5*100.0/(2.0*M_PI*m_fRadius));
            m_Perlin[0].SetPersistence (0.5);
            m_Perlin[0].SetLacunarity(m_fLacTrTp);
            m_Perlin[0].SetNoiseQuality(noise::QUALITY_BEST);
            m_Perlin[0].SetOctaveCount(m_nOctTrTp);

            m_Selector[0].SetSourceModule(0,m_Billow[0]);
            m_Selector[0].SetSourceModule(1,m_Terrace[0]);
            m_Selector[0].SetControlModule(m_Perlin[0]);
            m_Selector[0].SetBounds(0.0,1.0);
            m_Selector[0].SetEdgeFalloff(0.05);
            
            m_pTerrainType = &m_Perlin[0];
            m_pSurface = &m_Selector[0];
            break;
        }
        case PLANET_TYPE_ROCK:
        {
            m_fLacHlTr = 1.9371;
            m_fLacMtTr = 2.137;
            m_fLacTrTp = 2.317;
            
            m_fRadius = 120.0;
            m_fHeightMax = 60.0;
            
            double fNrOfPoints = 2.0*M_PI * m_fRadius / m_fGroundResolution;
            double fNrOfMountains = 2.0*M_PI * m_fRadius / (m_fHeightMax*M_PI_2);
            
            double fMinF = 1.0 / (0.2*m_fRadius*2.0*M_PI);
            double fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
            
            m_nOctHlTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
            if (m_nOctHlTr < 1) m_nOctHlTr = 1;
            
            m_nOctMtTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacMtTr));
            if (m_nOctMtTr < 1) m_nOctMtTr = 1;
            
            m_nOctTrTp = ceil(log2(fMaxF/(0.5*100.0/(2.0*M_PI*m_fRadius)))/log2(m_fLacTrTp));
            if (m_nOctTrTp < 1) m_nOctTrTp = 1;
            
            INFO_MSG("Planet", "Generating Terrain (Mountains)")
            DOM_VAR(INFO_MSG("Planet", "Number of Mountains:       " << fNrOfMountains))
            DOM_VAR(INFO_MSG("Planet", "Number of Points:          " << fNrOfPoints))
            DOM_VAR(INFO_MSG("Planet", "Minimum Frequency:         " << fMinF << "/m"))
            DOM_VAR(INFO_MSG("Planet", "Maximum Frequency:         " << fMaxF << "/m"))
            DOM_VAR(INFO_MSG("Planet", "Maximum Octaves Mountains: " << m_nOctMtTr))
            DOM_VAR(INFO_MSG("Planet", "Maximum Octaves Hills:     " << m_nOctHlTr))

            // Reserve memory for noise functions
            m_Perlin.resize(1);
            m_RidgedMulti.resize(1);
            
            m_RidgedMulti[0].SetSeed(m_nSeed);
            m_RidgedMulti[0].SetFrequency(fMinF);
            m_RidgedMulti[0].SetLacunarity(m_fLacMtTr);
            m_RidgedMulti[0].SetNoiseQuality(noise::QUALITY_BEST);
            m_RidgedMulti[0].SetOctaveCount(m_nOctMtTr);
                
            m_Perlin[0].SetSeed(m_nSeed+7);
            m_Perlin[0].SetFrequency (0.5*100.0/(2.0*M_PI*m_fRadius));
            m_Perlin[0].SetPersistence (0.5);
            m_Perlin[0].SetLacunarity(m_fLacTrTp);
            m_Perlin[0].SetNoiseQuality(noise::QUALITY_BEST);
            m_Perlin[0].SetOctaveCount(m_nOctTrTp);
            
            m_pTerrainType = &m_Perlin[0];
            m_pSurface = &m_RidgedMulti[0];
            
            break;
        }
    }
}
