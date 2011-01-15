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
/// \file       camera.h
/// \brief      Prototype of class "CCamera"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-14
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_H
#define CAMERA_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Core"
#include "eigen2/Eigen/Geometry"

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a camera for a view on the scene
///
////////////////////////////////////////////////////////////////////////////////
class CCamera
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CCamera();
        virtual ~CCamera();                         ///< Destructor

        //--- Constant Methods -----------------------------------------------//
        Vector2d    getPosition() const;
        double      getAngle() const;
        double      getZoom() const;
        
        //--- Methods --------------------------------------------------------//
        void reset();
        void rotateBy(const double&);
        void rotateTo(const double&);
        void translateBy(const Vector2d&);
        void translateTo(const Vector2d&);
        void zoomBy(const double&);
        void zoomTo(const double&);

    protected:

        //--- Variables [protected] ------------------------------------------//
        Vector2d        m_vecPosition;             ///< Camera position
        double          m_fAngle;                  ///< Camera angle
        double          m_fZoom;                   ///< Camera zoom
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera position
///
/// \return The camera position
///
///////////////////////////////////////////////////////////////////////////////
inline Vector2d CCamera::getPosition() const
{
    METHOD_ENTRY("CCamera::getPosition")

    METHOD_EXIT("CCamera::getPosition")
    return m_vecPosition;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
///////////////////////////////////////////////////////////////////////////////
inline double CCamera::getAngle() const
{
    METHOD_ENTRY("CCamera::getAngle")

    METHOD_EXIT("CCamera::getAngle")
    return m_fAngle;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera zoom
///
/// \return The camera zoom
///
///////////////////////////////////////////////////////////////////////////////
inline double CCamera::getZoom() const
{
    METHOD_ENTRY("CCamera::getZoom")

    METHOD_EXIT("CCamera::getZoom")
    return m_fZoom;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera by a given angle
///
/// \param _fAngle Angle to rotate camera by
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::rotateBy(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateBy")

    m_fAngle += _fAngle;

    METHOD_EXIT("CCamera::rotateBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Rotates the camera to a given angle
///
/// \param _fAngle Angle to rotate camera to
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::rotateTo(const double& _fAngle)
{
    METHOD_ENTRY("CCamera::rotateTo")

    m_fAngle = _fAngle;

    METHOD_EXIT("CCamera::rotateTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Translates the camera by a given position
///
/// \param _vecV Vector to move camera
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::translateBy(const Vector2d& _vecV)
{
    METHOD_ENTRY("CCamera::translateBy")

    Rotation2Dd Rotation(-m_fAngle);

    m_vecPosition += Rotation * _vecV;

    METHOD_EXIT("CCamera::translateBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Translates the camera to a given position
///
/// \param _vecV Vector to move camera by
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::translateTo(const Vector2d& _vecV)
{
    METHOD_ENTRY("CCamera::translateTo")

    m_vecPosition = _vecV;

    METHOD_EXIT("CCamera::translateTo")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zooms the camera by a given factor
///
/// \param _fZoom Zoom factor
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::zoomBy(const double& _fZoom)
{
    METHOD_ENTRY("CCamera::zoomBy")

    m_fZoom *= _fZoom;

    METHOD_EXIT("CCamera::zoomBy")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Zooms the camera to a given factor
///
/// \param _fZoom Zoom factor
///
///////////////////////////////////////////////////////////////////////////////
inline void CCamera::zoomTo(const double& _fZoom)
{
    METHOD_ENTRY("CCamera::zoomTo")

    m_fZoom = _fZoom;

    METHOD_EXIT("CCamera::zoomTo")
}

#endif
