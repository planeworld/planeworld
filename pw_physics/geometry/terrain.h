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
/// \file       terrain.h
/// \brief      Prototype of class "CTerrain"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TERRAIN_H
#define TERRAIN_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Geometry"
#include "noise/noise.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a procedural created terrain shape.
/// 
////////////////////////////////////////////////////////////////////////////////
class CTerrain : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CTerrain();
        virtual ~CTerrain();
        
        //--- Constant Methods -----------------------------------------------//
        const double&                   getAngle() const;
        const Vector2d&                 getCenter() const;
        const double&                   getGroundResolution() const;
        const double&                   getHeight() const;
        const int&                      getSeed() const;
        const double&                   getSmoothness() const;
        const noise::module::Select&    getSurface() const;
        const double&                   getWidth() const;
        
        const ShapeType                 getShapeType() const;

        //--- Methods --------------------------------------------------------//
        void init();
        
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setGroundResolution(const double&);
        void setHeight(const double&);
        void setSeed(const int&);
        void setSmoothness(const double&);
        void setWidth(const double&);
        
        void transform(const double&, const Vector2d&);
                        
    protected:

        Vector2d            m_vecCenter;                ///< Center of terrain
        Vector2d            m_vecCenter0;               ///< Initial center of terrain
        double              m_fAngle;                   ///< Angle, just for optical reasons
        double              m_fGroundResolution;        ///< Ground resolution in m
        double              m_fHeightMax;               ///< Maximum height of terrain
        double              m_fSmoothness;              ///< Smoothness of terrain landscape
        double              m_fWidth;                   ///< Width of terrain
        int                 m_nSeed;                    ///< Unique seed for terrain generation
        
        noise::module::Select      m_Surface;           ///< Final surface noise function
        noise::module::RidgedMulti m_MountainTerrain;   ///< Main mountain
        noise::module::Billow      m_BaseFlatTerrain;   ///< Base terrain
        noise::module::ScaleBias   m_FlatTerrain;       ///< Base terrain scaler
        noise::module::Perlin      m_TerrainType;       ///< Terrain type decision function
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return rotation angle of the terrain
///
/// \return Rotation angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CTerrain::getAngle() const
{
    METHOD_ENTRY("CTerrain::getAngle")

    METHOD_EXIT("CTerrain::getAngle")
    return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the center of the terrain
///
/// \return Vector, representing center of terrain
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CTerrain::getCenter() const
{
    METHOD_ENTRY("CTerrain::getCenter")

    METHOD_EXIT("CTerrain::getCenter")
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
inline const double& CTerrain::getGroundResolution() const
{
    METHOD_ENTRY("CTerrain::getNrOfSamplingPoints")

    METHOD_EXIT("CTerrain::getNrOfSamplingPoints")
    return m_fGroundResolution;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the height of terrain
///
/// \return Height
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CTerrain::getHeight() const
{
    METHOD_ENTRY("CTerrain::getHeight")

    METHOD_EXIT("CTerrain::getHeight")
    return m_fHeightMax;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the seed of the terrain
///
/// \return Seed
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CTerrain::getSeed() const
{
    METHOD_ENTRY("CTerrain::getSeed")

    METHOD_EXIT("CTerrain::getSeed")
    return m_nSeed;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the smoothness of terrain
///
/// \return Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CTerrain::getSmoothness() const
{
    METHOD_ENTRY("CTerrain::getSmoothness")

    METHOD_EXIT("CTerrain::getSmoothness")
    return m_fSmoothness;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return surface noise module of the terrain
///
/// \return Surface noise module
///
////////////////////////////////////////////////////////////////////////////////
inline const noise::module::Select& CTerrain::getSurface() const
{
    METHOD_ENTRY("CTerrain::getSmoothness")

    METHOD_EXIT("CTerrain::getSmoothness")
    return m_Surface;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the width of terrains
///
/// \return Width
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CTerrain::getWidth() const
{
    METHOD_ENTRY("CTerrain::getWidth")

    METHOD_EXIT("CTerrain::getWidth")
    return m_fWidth;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType CTerrain::getShapeType() const
{
    METHOD_ENTRY("CTerrain::getShapeType")

    METHOD_EXIT("CTerrain::getShapeType")
    return SHAPE_TERRAIN;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the terrain
///
/// \param _vecC Center of the terrain
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setCenter(const Vector2d& _vecC)
{
    METHOD_ENTRY("CTerrain::setCenter")

    m_vecCenter0 = _vecC;
    m_vecCenter = _vecC;

    METHOD_EXIT("CTerrain::setCenter")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the terrain
///
/// \param _fX Center-x of the terrain
/// \param _fY Center-y of the terrain
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setCenter(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CTerrain::setCenter")

    m_vecCenter0[0] = _fX;
    m_vecCenter0[1] = _fY;

    m_vecCenter[0] = _fX;
    m_vecCenter[1] = _fY;

    METHOD_EXIT("CTerrain::setCenter")
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
inline void CTerrain::setGroundResolution(const double& _fGRes)
{
    METHOD_ENTRY("CTerrain::setGroundResolution")

    m_fGroundResolution = _fGRes;

    METHOD_EXIT("CTerrain::setGroundResolution")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the maximum terrain height
///
/// \param _fH Maximum terrain height
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setHeight(const double& _fH)
{
    METHOD_ENTRY("CTerrain::setHeight")

    m_fHeightMax = _fH;

    METHOD_EXIT("CTerrain::setHeight")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets unique seed for procedural terrain generation
///
/// \param _nS Seed
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setSeed(const int& _nS)
{
    METHOD_ENTRY("CTerrain::setSeed")

    m_nSeed = _nS;

    METHOD_EXIT("CTerrain::setSeed")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets smoothness for procedural terrain generation
///
/// \param _fS Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setSmoothness(const double& _fS)
{
    METHOD_ENTRY("CTerrain::setSmoothness")

    m_fSmoothness = _fS;

    METHOD_EXIT("CTerrain::setSmoothness")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the terrain width
///
/// \param _fW Terrain width
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setWidth(const double& _fW)
{
    METHOD_ENTRY("CTerrain::setWidth")

    m_fWidth = _fW;

    METHOD_EXIT("CTerrain::setWidth")
}

#endif
