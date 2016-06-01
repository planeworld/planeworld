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
                     m_fGroundResolution(1.0),
                     m_fHeightMax(10.0),
                     m_fRadius(1000.0),
                     m_fSeaLevel(0.0),
                     m_fSmoothness(1.0),
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
        
    pClone->copy(this);
    
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
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the planet
///
/// \param _vecC Center of the planet
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::setCenter(const Vector2d& _vecC)
{
    METHOD_ENTRY("CPlanet::setCenter")

    m_vecCenter0 = _vecC;
    m_vecCenter = _vecC;
    
    this->updateGeometry();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the planet
///
/// \param _fX Center-x of the planet
/// \param _fY Center-y of the planet
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::setCenter(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CPlanet::setCenter")

    m_vecCenter0[0] = _fX;
    m_vecCenter0[1] = _fY;
    m_vecCenter[0]  = _fX;
    m_vecCenter[1]  = _fY;
    
    this->updateGeometry();
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
    for (auto i=0u; i<m_RidgedMulti.size(); ++i)
        m_RidgedMulti[i].SetOctaveCountTmp(nOct);
    
    nOct = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
    if (nOct < 1) nOct = 1;
    for (auto i=0u; i<m_Billow.size(); ++i)
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
    
    for (auto i=0u; i<m_Billow.size(); ++i)
        m_Billow[i].SetOctaveCountTmp(m_nOctHlTr);
    for (auto i=0u; i<m_RidgedMulti.size(); ++i)
        m_RidgedMulti[i].SetOctaveCountTmp(m_nOctMtTr);
//     m_TerrainType.SetOctaveCountTmp(m_nOctTrTp);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecCOM Center of mass in local (object) coordinates
/// \param _vecV Translation vector
///
///////////////////////////////////////////////////////////////////////////////
void CPlanet::transform( const double& _fAngle,
                         const Vector2d& _vecCOM,
                         const Vector2d& _vecV )
{
    METHOD_ENTRY("CPlanet::transform")

    Rotation2Dd Rotation(_fAngle);

    m_vecCenter = Rotation * (m_vecCenter0-_vecCOM) + _vecCOM + _vecV;

    m_fAngle = _fAngle;

    // Update bounding box
    m_AABB.setLowerLeft( m_vecCenter - Vector2d(m_fRadius+m_fHeightMax,
                                                m_fRadius+m_fHeightMax));
    m_AABB.setUpperRight(m_vecCenter + Vector2d(m_fRadius+m_fHeightMax,
                                                m_fRadius+m_fHeightMax));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& CPlanet::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CPlanet::myStreamIn")
    
    std::string strTmp;
    _is >> strTmp;
    
    /// \todo Implement streaming of planet type
    
//     _is >> m_PlanetType;
    _is >> m_vecCenter[0];
    _is >> m_vecCenter[1];
    _is >> m_vecCenter0[0];
    _is >> m_vecCenter0[1];
    _is >> m_fAngle;
    _is >> m_fGroundResolution;
    _is >> m_fHeightMax;
    _is >> m_fRadius;
    _is >> m_fSeaLevel;
    _is >> m_fSmoothness;
    _is >> m_nSeed;
    
    _is >> m_fLacHlTr;
    _is >> m_fLacMtTr;
    _is >> m_fLacTrTp;
    _is >> m_nOctHlTr;
    _is >> m_nOctMtTr;
    _is >> m_nOctTrTp;
    
    // Terrain is initialised. This way, external noise modules don't need
    // to be saved and loaded (they do not store an internal state).
    this->initTerrain();
    
    return _is;
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
    
    _os << "Planet:" << std::endl;
    
    /// \todo Implement streaming of planet type
    
//     _os << m_PlanetType << std::endl;
    _os << m_vecCenter[0] << " " <<
           m_vecCenter[1] << std::endl;
    _os << m_vecCenter0[0] << " " <<
           m_vecCenter0[1] << std::endl;
    _os << m_fAngle << std::endl;
    _os << m_fGroundResolution << std::endl;
    _os << m_fHeightMax << std::endl;
    _os << m_fRadius << std::endl;
    _os << m_fSeaLevel << std::endl;
    _os << m_fSmoothness << std::endl;
    _os << m_nSeed << std::endl;
   
    // Noise modules don't need to be exported, since the planets will be
    // initialised from parameters when loaded.
    
    _os << m_fLacHlTr << std::endl;
    _os << m_fLacMtTr << std::endl;
    _os << m_fLacTrTp << std::endl;
    _os << m_nOctHlTr << std::endl;
    _os << m_nOctMtTr << std::endl;
    _os << m_nOctTrTp << std::endl;
    
    return _os;
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
void CPlanet::myCopy(const IShape* const _pShape)
{
    METHOD_ENTRY("CPlanet::myCopy");
    
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
            
            double fMinF = 1.0 / (m_fHeightMax*M_PI_2);
            double fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
            
            m_nOctHlTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
            if (m_nOctHlTr < 1) m_nOctHlTr = 1;
            
            m_nOctMtTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacMtTr));
            if (m_nOctMtTr < 1) m_nOctMtTr = 1;
            
            m_nOctTrTp = ceil(log2(fMaxF/(0.5*100.0/(2.0*M_PI*m_fRadius)))/log2(m_fLacTrTp));
            if (m_nOctTrTp < 1) m_nOctTrTp = 1;
            
            INFO_MSG("Planet", "Generating Terrain (Mountains)")
            DOM_VAR(DEBUG(
                double fNrOfPoints = 2.0*M_PI * m_fRadius / m_fGroundResolution;
                double fNrOfMountains = 2.0*M_PI * m_fRadius / (m_fHeightMax*M_PI_2);

                DEBUG_MSG("Planet", "Number of Mountains:       " << fNrOfMountains)
                DEBUG_MSG("Planet", "Number of Points:          " << fNrOfPoints)
                DEBUG_MSG("Planet", "Minimum Frequency:         " << fMinF << "/m")
                DEBUG_MSG("Planet", "Maximum Frequency:         " << fMaxF << "/m")
                DEBUG_MSG("Planet", "Maximum Octaves Mountains: " << m_nOctMtTr)
                DEBUG_MSG("Planet", "Maximum Octaves Hills:     " << m_nOctHlTr)
            ))
            
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
            
            double fMinF = 1.0 / (0.2*m_fRadius*2.0*M_PI);
            double fMaxF = 1.0 / (m_fGroundResolution*PLANET_DEFAULT_VERTICES_PER_PERIOD);
            
            m_nOctHlTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacHlTr));
            if (m_nOctHlTr < 1) m_nOctHlTr = 1;
            
            m_nOctMtTr = ceil(log2(fMaxF/fMinF)/log2(m_fLacMtTr));
            if (m_nOctMtTr < 1) m_nOctMtTr = 1;
            
            m_nOctTrTp = ceil(log2(fMaxF/(0.5*100.0/(2.0*M_PI*m_fRadius)))/log2(m_fLacTrTp));
            if (m_nOctTrTp < 1) m_nOctTrTp = 1;
            
            INFO_MSG("Planet", "Generating Terrain (Mountains)")
            DOM_VAR(DEBUG(
                double fNrOfPoints = 2.0*M_PI * m_fRadius / m_fGroundResolution;
                double fNrOfMountains = 2.0*M_PI * m_fRadius / (m_fHeightMax*M_PI_2);
            
              DEBUG_MSG("Planet", "Number of Mountains:       " << fNrOfMountains)
              DEBUG_MSG("Planet", "Number of Points:          " << fNrOfPoints)
              DEBUG_MSG("Planet", "Minimum Frequency:         " << fMinF << "/m")
              DEBUG_MSG("Planet", "Maximum Frequency:         " << fMaxF << "/m")
              DEBUG_MSG("Planet", "Maximum Octaves Mountains: " << m_nOctMtTr)
              DEBUG_MSG("Planet", "Maximum Octaves Hills:     " << m_nOctHlTr)
            ))

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
        case PLANET_TYPE_ICE:
        {
            /// \todo Implement different planet types, here: ice
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Update geometry relevant data, e.g. inertia, area, center of mass
///
////////////////////////////////////////////////////////////////////////////////
void CPlanet::myUpdateGeometry()
{
    METHOD_ENTRY("CPlanet::myUpdateGeometry")
    
    // Calculate COM, shape no longer valid
    m_vecCentroid = m_vecCenter0;
    m_fArea = M_PI * m_fRadius * m_fRadius;
    m_fInertia = 0.5 * m_fMass * m_fRadius * m_fRadius;
    DOM_VAR(DEBUG_MSG("Planet", "Inertia calculated: " << m_fInertia))
}

