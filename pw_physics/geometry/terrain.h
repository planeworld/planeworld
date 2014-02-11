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
#include <eigen3/Eigen/Geometry>
#include "noise2d/noise.h"

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
        CTerrain*                       clone() const;
        const double&                   getAngle() const;
        const Vector2d&                 getCenter() const;
        const double&                   getDiversity() const;
        const double&                   getGroundResolution() const;
        const double&                   getHeight() const;
        const int&                      getSeed() const;
        const double&                   getSmoothness() const;
        const double                    getSurface(const double&) const;
        const double&                   getWidth() const;
                
        const ShapeType                 getShapeType() const;
        
        const double                    snapToTerrainGrid(const double&) const;

        //--- Methods --------------------------------------------------------//
        void copy(const IShape* const);
        
        void init();
        
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setDiversity(const double&);
        void setGroundResolution(const double&);
        void setHeight(const double&);
        void setSeed(const int&);
        void setSmoothness(const double&);
        void setWidth(const double&);
                
        void transform(const double&, const Vector2d&);
                        
    protected:

        std::vector<double> m_Cache;                    ///< Cached surface of terrain
        Vector2d            m_vecCenter;                ///< Center of terrain
        Vector2d            m_vecCenter0;               ///< Initial center of terrain
        double              m_fDiversity;               ///< Diversity of terrain
        double              m_fAngle;                   ///< Angle, just for optical reasons
        double              m_fGroundResolution;        ///< Ground resolution in m
        double              m_fHeightMax;               ///< Maximum height of terrain
        double              m_fSmoothness;              ///< Smoothness of terrain landscape
        double              m_fWidth;                   ///< Width of terrain
        int                 m_nSeed;                    ///< Unique seed for terrain generation
        
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
    return m_vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the diversity value of the terrain
///
/// The diversity represents the frequency of terrain changes reffering to width
/// of terrain.
///
/// \return Diversity (frequency of terrain changes)
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CTerrain::getDiversity() const
{
    METHOD_ENTRY("CTerrain::getDiversity")
    return m_fDiversity;
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
    return m_fSmoothness;
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
    return SHAPE_TERRAIN;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the given position snapped to terrain grid
///
/// The method returns the position snapped to the cached terrain which is left
/// to the given value (since the given value will simply be clipped).
///
/// \param _fX Point to get snapped position from
///
/// \return Snapped position
///
////////////////////////////////////////////////////////////////////////////////
inline const double CTerrain::snapToTerrainGrid(const double& _fX) const
{
    METHOD_ENTRY("CTerrain::snapToTerrainGrid")
    return floor((_fX+m_vecCenter[0])/m_fGroundResolution)*m_fGroundResolution-m_vecCenter[0];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the diversity value of the terrain
///
/// The diversity represents the frequency of terrain changes refering to width
/// of terrain.
///
/// \param _fDiversity Diversity (frequency of terrain changes)
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrain::setDiversity(const double& _fDiversity)
{
    METHOD_ENTRY("CTerrain::setDiversity")

    m_fDiversity = _fDiversity;
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
}

#endif
