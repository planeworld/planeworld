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
CTerrain::CTerrain() :  m_fDiversity(1.0),
                        m_fAngle(0.0),
                        m_fHeightMax(100.0),
                        m_fSmoothness(1.0),
                        m_fGroundResolution(1.0),
                        m_fWidth(200.0),
                        m_nSeed(2)
{
    METHOD_ENTRY("CTerrain::CTerrain()");
    CTOR_CALL("CTerrain::CTerrain()");
    
    m_vecCenter0.setZero();
        
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
    
    pClone->m_Cache             = m_Cache;
    pClone->m_fAngle            = m_fAngle;
    pClone->m_fDiversity        = m_fDiversity;
    pClone->m_fGroundResolution = m_fGroundResolution;
    pClone->m_fHeightMax        = m_fHeightMax;
    pClone->m_fSmoothness       = m_fSmoothness;
    pClone->m_fWidth            = m_fWidth;
    pClone->m_nSeed             = m_nSeed;
    pClone->m_vecCenter         = m_vecCenter;
    pClone->m_vecCenter0        = m_vecCenter0;
    pClone->m_AABB              = m_AABB;
    pClone->m_nDepthlayers      = m_nDepthlayers;
        
    METHOD_EXIT("CTerrain::clone")
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
    
    int      nX = static_cast<int>(
                       (_fX+m_fWidth*0.5-m_vecCenter[0])/m_fGroundResolution
                 );
    if ((nX < 0) || (nX >= m_Cache.size()))
    {
        nX = 0;
    }
    METHOD_EXIT("CTerrain::getSurface")
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

    METHOD_EXIT("CTerrain::init")
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

    METHOD_EXIT("CTerrain::transform")
}
