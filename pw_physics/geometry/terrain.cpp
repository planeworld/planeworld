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
CTerrain::CTerrain() :  m_fDiversity(1.0),
                        m_fAngle(0.0),
                        m_fGroundResolution(1.0),
                        m_fHeightMax(100.0),
                        m_fSmoothness(1.0),
                        m_fWidth(200.0),
                        m_nSeed(2)
{
    METHOD_ENTRY("CTerrain::CTerrain");
    CTOR_CALL("CTerrain::CTerrain");
    
    m_vecCenter0.setZero();
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
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones terrain 
///
/// \return Pointer to cloned terrain
///
////////////////////////////////////////////////////////////////////////////////
CTerrain* CTerrain::clone() const
{
    METHOD_ENTRY("CTerrain::clone")
    
    CTerrain* pClone = new CTerrain();
    MEM_ALLOC("pClone")
    
    pClone->copy(this);
        
    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return surface value for given point in world coordinates
///
/// For a given point, the terrain height is returned. Values between cached
/// points are linearly interpolated.
///
/// \param _fX Point to get height from (world coordinates)
///
/// \return Height
///
////////////////////////////////////////////////////////////////////////////////
const double CTerrain::getSurface(const double& _fX) const
{
    METHOD_ENTRY("CTerrain::getSurface")
    
    std::uint32_t nX = static_cast<std::uint32_t>(
                       (_fX+m_fWidth*0.5-m_vecCenter[0])/m_fGroundResolution
                 );
    if ((nX < 0u) || (nX >= m_Cache.size()))
    {
        nX = 0u;
    }
    return m_Cache[nX];
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
    
    noise::module::Select      Surface;           ///< Final surface noise function
    noise::module::RidgedMulti MountainTerrain;   ///< Main mountain
    noise::module::Billow      BaseFlatTerrain;   ///< Base terrain
    noise::module::ScaleBias   FlatTerrain;       ///< Base terrain scaler
    noise::module::Perlin      TerrainType;       ///< Terrain type decision function
    
    double fNrOfPoints = m_fWidth / m_fGroundResolution;
    double fNrOfMountains = m_fWidth / m_fHeightMax *2.0;
    
    double fMinF = fNrOfMountains/fNrOfPoints;
    double fMaxF = 0.5;
    
    int nMaxOctave = log2(fMaxF/fMinF)+1;
    if (nMaxOctave < 1) nMaxOctave = 1;
    
    INFO_MSG("Terrain", "Generating Terrain (Mountains)")
    DOM_VAR(DEBUG_MSG("Terrain", "Number of Mountains: " << fNrOfMountains))
    DOM_VAR(DEBUG_MSG("Terrain", "Number of Points:    " << fNrOfPoints))
    DOM_VAR(DEBUG_MSG("Terrain", "Minimum Frequency:   " << fMinF))
    DOM_VAR(DEBUG_MSG("Terrain", "Maximum Frequency:   " << fMaxF))
    DOM_VAR(DEBUG_MSG("Terrain", "Maximum Octaves:     " << nMaxOctave))

    MountainTerrain.SetFrequency(fMinF);
    MountainTerrain.SetLacunarity(1.937);
    MountainTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    MountainTerrain.SetOctaveCount(nMaxOctave);
    
    BaseFlatTerrain.SetFrequency(fMinF);
    BaseFlatTerrain.SetLacunarity(1.93947);
    BaseFlatTerrain.SetNoiseQuality(noise::QUALITY_BEST);
    BaseFlatTerrain.SetOctaveCount(nMaxOctave);
    BaseFlatTerrain.SetPersistence(0.5);
    
    FlatTerrain.SetSourceModule(0,BaseFlatTerrain);
    FlatTerrain.SetScale(0.25);
    FlatTerrain.SetBias(-0.75);
    
    TerrainType.SetFrequency (m_fDiversity/(m_fWidth));
    TerrainType.SetPersistence (0.5);
    TerrainType.SetLacunarity(2.137);
    TerrainType.SetNoiseQuality(noise::QUALITY_BEST);

    Surface.SetSourceModule(0,FlatTerrain);
    Surface.SetSourceModule(1,MountainTerrain);
    Surface.SetControlModule(TerrainType);
    Surface.SetBounds(0.0,100.0);
    Surface.SetEdgeFalloff(0.2);
    
    m_Cache.resize(ceil(fNrOfPoints));
    double fX = 0.0;
    for (int i=0; i<ceil(fNrOfPoints); ++i)
    {
        m_Cache[i]=Surface.GetValue(fX,0.0) * m_fHeightMax + m_vecCenter[1]; 
        fX += m_fGroundResolution;
        m_AABB.update(Vector2d(fX+m_vecCenter[0]-m_fWidth*0.5,m_Cache[i]));
        
        Log.progressBar("Caching Terrain", i,ceil(fNrOfPoints));
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transforms the shape
///
/// \param _fAngle Rotation angle
/// \param _vecV Translation vector
///
/// \todo Maybe implement virtual init method for all shapes to call after
///       transform
///
///////////////////////////////////////////////////////////////////////////////
void CTerrain::transform( const double& _fAngle, const Vector2d& _vecV )
{
    METHOD_ENTRY("CTerrain::transform")

//  Transformation for static shape not needed
    
//     Rotation2Dd Rotation(_fAngle);
// 
//     m_vecCenter = Rotation * m_vecCenter0 + _vecV;
// 
//     m_fAngle = _fAngle;
//     
//     // Update bounding box
//     m_AABB.setLowerLeft( m_vecCenter - Vector2d(m_fWidth*0.5,0.0));
//     m_AABB.setUpperRight(m_vecCenter + Vector2d(m_fWidth*0.5,m_fHeightMax));
    
    this->init();
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
std::istream& CTerrain::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CTerrain::myStreamIn")
    
    std::string strTmp;
    _is >> strTmp;
    
    /// \todo Implement streaming
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CTerrain instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CTerrain::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CTerrain::myStreamOut")
    
    _os << "Terrain:" << std::endl;
    
    /// \todo Implement streaming
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies information of a given terrain
///
/// This method does not create a new terrain, use clone in that case!
///
/// \param _pShape Shape to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CTerrain::myCopy(const IShape* const _pShape)
{
    METHOD_ENTRY("CTerrain::myCopy");
    
    const CTerrain* const pTerrain = static_cast<const CTerrain* const>(_pShape);
        
    m_Cache             = pTerrain->m_Cache;
    m_fAngle            = pTerrain->m_fAngle;
    m_fDiversity        = pTerrain->m_fDiversity;
    m_fGroundResolution = pTerrain->m_fGroundResolution;
    m_fHeightMax        = pTerrain->m_fHeightMax;
    m_fSmoothness       = pTerrain->m_fSmoothness;
    m_fWidth            = pTerrain->m_fWidth;
    m_nSeed             = pTerrain->m_nSeed;
    m_vecCenter         = pTerrain->m_vecCenter;
    m_vecCenter0        = pTerrain->m_vecCenter0;
}
