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
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "bounding_box.h"

//--- Misc header ------------------------------------------------------------//
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
        const CBoundingBox           getBoundingBox() const;
        const std::vector<Vector2d>& getFrame() const;
        
        Vector2d    getPosition() const;
        double      getAngle() const;
        double      getZoom() const;
        
        //--- Methods --------------------------------------------------------//
        void setViewport(const double&, const double&);
        
        void reset();
        void rotateBy(const double&);
        void rotateTo(const double&);
        void translateBy(const Vector2d&);
        void translateTo(const Vector2d&);
        void zoomBy(const double&);
        void zoomTo(const double&);

    protected:
        
        //--- Methods [protected] --------------------------------------------//
        void update();

        //--- Variables [protected] ------------------------------------------//
        std::vector<Vector2d>  m_vecFrame0;         ///< Initial camera frame
        std::vector<Vector2d>  m_vecFrame;          ///< Camera frame
        CBoundingBox    m_BoundingBox;              ///< Cameras bounding box (for culling)
        Vector2d        m_vecPosition;              ///< Camera position
        double          m_fViewportWidth;           ///< Half of viewport width in m
        double          m_fViewportHeight;          ///< Half of viewport height in m
        double          m_fAngle;                   ///< Camera angle
        double          m_fZoom;                    ///< Camera zoom
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras bounding box
///
/// The bounding box of the camera is returned. It surrounds the viewport and
/// can be used for culling.
///
/// \return The bounding box
///
///////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox CCamera::getBoundingBox() const
{
    METHOD_ENTRY("CCamera::getBoundingBox")

    METHOD_EXIT("CCamera::getBoundingBox")
    return m_BoundingBox;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns camera frame (viewport) in world coordinates
///
/// \return The camera frame
///
///////////////////////////////////////////////////////////////////////////////
inline const std::vector<Vector2d>& CCamera::getFrame() const
{
    METHOD_ENTRY("CCamera::getFrame")

    METHOD_EXIT("CCamera::getFrame")
    return m_vecFrame;
}

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

#endif
