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
#include <array>

//--- Program header ---------------------------------------------------------//
#include "bounding_box.h"
#include "graphics.h"
#include "kinematics_state_user.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a camera for a view on the scene
///
/// \bug Camera reset positions camera to 0, instead of value in configuration.
///
////////////////////////////////////////////////////////////////////////////////
class CCamera : public CGraphicsBase,
                public IKinematicsStateUser,
                public IUniverseScaled
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CCamera();
        virtual ~CCamera();

        //--- Constant Methods -----------------------------------------------//
        const Vector2d      getCenter() const;
        const double        getAngle() const;
        const double&       getZoom() const;
        
        //--- Methods --------------------------------------------------------//
        const CBoundingBox  getBoundingBox();
        const double&       getBoundingCircleRadius();
        
        void setPosition(const double&, const double&);
        void setViewport(const double&, const double&);
        
        void update();
        void reset();
        void rotateBy(const double&);
        void rotateTo(const double&);
        void translateBy(const Vector2d&);
        void translateTo(const Vector2d&);
        void zoomBy(const double&);
        void zoomTo(const double&);

    protected:
        
        //--- Methods --------------------------------------------------------//
        void myHook();
        void myUpdateFromHookable();
        void updateWithHook();
        void updateWithoutHook();

        //--- Variables [protected] ------------------------------------------//
        std::array<Vector2d,4>  m_vecFrame0;         ///< Initial camera frame
        CBoundingBox    m_BoundingBox;              ///< Cameras bounding box (for culling)
        Vector2d        m_vecCenter;                ///< Center of camera
        double          m_fBoundingCircleRadius;    ///< Radius of Bounding circle
        double          m_fViewportWidth;           ///< Half of viewport width in m
        double          m_fViewportHeight;          ///< Half of viewport height in m
        double          m_fZoom;                    ///< Camera zoom
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns center of camera
///
/// \return The camera's center
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d CCamera::getCenter() const
{
    METHOD_ENTRY("CCamera::getCenter")
    return m_KinematicsState.getOrigin();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
////////////////////////////////////////////////////////////////////////////////
inline const double CCamera::getAngle() const
{
    METHOD_ENTRY("CCamera::getAngle")
    return (m_KinematicsState.getAngle());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera zoom
///
/// \return The camera zoom
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CCamera::getZoom() const
{
    METHOD_ENTRY("CCamera::getZoom")
    return m_fZoom;
}

#endif
