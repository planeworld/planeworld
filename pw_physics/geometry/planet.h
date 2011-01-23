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
/// \file       planet.h
/// \brief      Prototype of class "CPlanet"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PLANET_H
#define PLANET_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "shape.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Geometry"
#include "noise/noise.h"

using namespace noise;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a procedural created planet shape.
/// 
////////////////////////////////////////////////////////////////////////////////
class CPlanet : public IShape
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CPlanet();
        virtual ~CPlanet();
        
        //--- Constant Methods -----------------------------------------------//
        const double&           getAngle() const;
        const Vector2d&         getCenter() const;
        const double&           getHeight() const;
        const int&              getNrOfSamplingPoints() const;
        const double&           getRadius() const;
        const int&              getSeed() const;
        const double&           getSmoothness() const;
        const module::Select    getSurface() const;
        
        const ShapeType         getShapeType() const;

        //--- Methods --------------------------------------------------------//
        void transform(const double&, const Vector2d&);
        void setCenter(const Vector2d&);
        void setCenter(const double&, const double&);
        void setHeight(const double&);
        void setNrOfSamplingPoints(const int&);
        void setRadius(const double&);
        void setSeed(const int&);
        void setSmoothness(const double&);
                        
    protected:

        Vector2d            m_vecCenter;            ///< Center of planet
        Vector2d            m_vecCenter0;           ///< Initial center of planet
        double              m_fAngle;               ///< Angle, just for optical reasons
        double              m_fHeightMax;           ///< Maximum height of terrain
        double              m_fRadius;              ///< Minimum radius of planet
        double              m_fSmoothness;          ///< Smoothness of planet landscape
        int                 m_nNrOfSamplingPoints;  ///< Number of sampling points for terrain generation
        int                 m_nSeed;                ///< Unique seed for terrain generation
        module::Select      m_Surface;              ///< Surface noise function of planet
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return rotation angle of the planet
///
/// \return Rotation angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getAngle() const
{
    METHOD_ENTRY("CPlanet::getAngle")

    METHOD_EXIT("CPlanet::getAngle")
    return m_fAngle;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the center of the planet
///
/// \return Vector, representing center of planet
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CPlanet::getCenter() const
{
    METHOD_ENTRY("CPlanet::getCenter")

    METHOD_EXIT("CPlanet::getCenter")
    return m_vecCenter;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the height of planets terrain
///
/// \return Height
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getHeight() const
{
    METHOD_ENTRY("CPlanet::getHeight")

    METHOD_EXIT("CPlanet::getHeight")
    return m_fHeightMax;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the number of sampling points for terrain generation
///
/// \return Number of sampling points
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CPlanet::getNrOfSamplingPoints() const
{
    METHOD_ENTRY("CPlanet::getNrOfSamplingPoints")

    METHOD_EXIT("CPlanet::getNrOfSamplingPoints")
    return m_nNrOfSamplingPoints;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the radius of the planet
///
/// \return Radius
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getRadius() const
{
    METHOD_ENTRY("CPlanet::getRadius")

    METHOD_EXIT("CPlanet::getRadius")
    return m_fRadius;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the seed of the planet
///
/// \return Seed
///
////////////////////////////////////////////////////////////////////////////////
inline const int& CPlanet::getSeed() const
{
    METHOD_ENTRY("CPlanet::getSeed")

    METHOD_EXIT("CPlanet::getSeed")
    return m_nSeed;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the smoothness of planets terrain
///
/// \return Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CPlanet::getSmoothness() const
{
    METHOD_ENTRY("CPlanet::getSmoothness")

    METHOD_EXIT("CPlanet::getSmoothness")
    return m_fSmoothness;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return surface noise module of the planet
///
/// \return Surface noise module
///
////////////////////////////////////////////////////////////////////////////////
inline const module::Select CPlanet::getSurface() const
{
    METHOD_ENTRY("CPlanet::getSmoothness")

    METHOD_EXIT("CPlanet::getSmoothness")
    return m_Surface;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape type
///
/// \return Type of shape
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeType CPlanet::getShapeType() const
{
    METHOD_ENTRY("CPlanet::getShapeType")

    METHOD_EXIT("CPlanet::getShapeType")
    return SHAPE_PLANET;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the planet
///
/// \param _vecC Center of the planet
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setCenter(const Vector2d& _vecC)
{
    METHOD_ENTRY("CPlanet::setCenter")

    m_vecCenter0 = _vecC;
    m_vecCenter = _vecC;

    METHOD_EXIT("CPlanet::setCenter")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the center of the planet
///
/// \param _fX Center-x of the planet
/// \param _fY Center-y of the planet
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setCenter(const double& _fX, const double& _fY)
{
    METHOD_ENTRY("CPlanet::setCenter")

    m_vecCenter0[0] = _fX;
    m_vecCenter0[1] = _fY;

    m_vecCenter[0] = _fX;
    m_vecCenter[1] = _fY;

    METHOD_EXIT("CPlanet::setCenter")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the maximum terrain height of the planet
///
/// \param _fH Maximum terrain height
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setHeight(const double& _fH)
{
    METHOD_ENTRY("CPlanet::setHeight")

    m_fHeightMax = _fH;

    METHOD_EXIT("CPlanet::setHeight")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of sampling points for terrain generation
///
/// \param _nNrOSP Number of sampling points
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setNrOfSamplingPoints(const int& _nNrOSP)
{
    METHOD_ENTRY("CPlanet::setNrOfSamplingPoints")

    m_nNrOfSamplingPoints = _nNrOSP;

    METHOD_EXIT("CPlanet::setNrOfSamplingPoints")
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines the minimum radius of the planet
///
/// \param _fR Radius
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setRadius(const double& _fR)
{
    METHOD_ENTRY("CPlanet::setRadius")

    m_fRadius = _fR;

    METHOD_EXIT("CPlanet::setRadius")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets unique seed for procedural terrain generation of the planet
///
/// \param _nS Seed
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setSeed(const int& _nS)
{
    METHOD_ENTRY("CPlanet::setSeed")

    m_nSeed = _nS;

    METHOD_EXIT("CPlanet::setSeed")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets smoothness for procedural terrain generation of the planet
///
/// \param _fS Smoothness
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanet::setSmoothness(const double& _fS)
{
    METHOD_ENTRY("CPlanet::setSmoothness")

    m_fSmoothness = _fS;

    METHOD_EXIT("CPlanet::setSmoothness")
}

#endif
