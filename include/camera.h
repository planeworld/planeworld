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
#include "graphics.h"
#include "hookable.h"

//--- Misc header ------------------------------------------------------------//
#include "eigen2/Eigen/Geometry"

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a camera for a view on the scene
///
/// \bug Camera reset positions camera to 0, instead of value in configuration.
///
////////////////////////////////////////////////////////////////////////////////
class CCamera : public CGraphicsBase
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CCamera();
        virtual ~CCamera();

        //--- Constant Methods -----------------------------------------------//
        const CBoundingBox           getBoundingBox() const;
        const double&                getBoundingCircleRadius() const;
        const std::vector<Vector2d>& getFrame() const;
        
        const Vector2i&     getCell() const;
        const Vector2d&     getCenter() const;
        const double        getAngle() const;
        const double&       getZoom() const;
        
        //--- Methods --------------------------------------------------------//
        void setCell(const Vector2i&);
        void setHook(IHookable*);
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
        
        //--- Methods [protected] --------------------------------------------//

        //--- Variables [protected] ------------------------------------------//
        std::vector<Vector2d>  m_vecFrame0;         ///< Initial camera frame
        std::vector<Vector2d>  m_vecFrame;          ///< Camera frame
        CBoundingBox    m_BoundingBox;              ///< Cameras bounding box (for culling)
        Vector2i        m_vecCell;                  ///< Cell in which the camera is located
        Vector2d        m_vecPosition;              ///< Camera position
        Vector2d        m_vecHook;                  ///< Hook position
        Vector2i        m_vecHookCell;              ///< Cell position of hook
        Vector2d        m_vecCenter;                ///< Center of camera
        double          m_fHookAng;                 ///< Hook angle
        double          m_fBoundingCircleRadius;    ///< Radius of Bounding circle
        double          m_fViewportWidth;           ///< Half of viewport width in m
        double          m_fViewportHeight;          ///< Half of viewport height in m
        double          m_fAngle;                   ///< Camera angle
        double          m_fZoom;                    ///< Camera zoom
        
        IHookable*      m_pHook;                    ///< Hook for the camera
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the cell the camera is located at.
///
/// \return Cell the camera is located at
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2i& CCamera::getCell() const
{
    METHOD_ENTRY("CCamera::getCell")
    return m_vecCell;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns center of camera
///
/// \return The camera's center
///
///////////////////////////////////////////////////////////////////////////////
inline const Vector2d& CCamera::getCenter() const
{
    METHOD_ENTRY("CCamera::getCenter")

    METHOD_EXIT("CCamera::getCenter")
    return m_vecCenter;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns radius of cameras bounding circle
///
/// The radius of the bounding circle of the camera is returned. The bounding
/// circle surrounds the viewport and can be used for culling like the bbox.
///
/// \return The bounding circle's radius
///
///////////////////////////////////////////////////////////////////////////////
inline const double& CCamera::getBoundingCircleRadius() const
{
    METHOD_ENTRY("CCamera::BoundingCircleRadius")

    METHOD_EXIT("CCamera::BoundingCircleRadius")
    return m_fBoundingCircleRadius;
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
/// \brief Returns the camera angle
///
/// \return The camera angle
///
///////////////////////////////////////////////////////////////////////////////
inline const double CCamera::getAngle() const
{
    METHOD_ENTRY("CCamera::getAngle")

    METHOD_EXIT("CCamera::getAngle")
    return (m_fAngle+m_fHookAng);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera zoom
///
/// \return The camera zoom
///
///////////////////////////////////////////////////////////////////////////////
inline const double& CCamera::getZoom() const
{
    METHOD_ENTRY("CCamera::getZoom")

    METHOD_EXIT("CCamera::getZoom")
    return m_fZoom;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the cell the camera is located at
///
/// \param _vecCell Cell of the camera
///
////////////////////////////////////////////////////////////////////////////////
inline void CCamera::setCell(const Vector2i& _vecCell)
{
    METHOD_ENTRY("CCamera::setCell")

    m_vecCell = _vecCell;
}

#endif
