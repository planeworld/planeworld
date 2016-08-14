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
#include "object_referrer.h"
#include "unique_id_user.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a camera for a view on the scene
///
/// \todo Clean up to just use kinematics state without wrapping methods like
///       setPosition.
///
////////////////////////////////////////////////////////////////////////////////
class CCamera : public CGraphicsBase,
                public IKinematicsStateUser,
                public IObjectReferrer,
                public IUniqueIDUser,
                public IUniverseScaled
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CCamera();
        CCamera(const CCamera&);
        ~CCamera();
        CCamera& operator=(const CCamera&);
        CCamera* clone() const;

        //--- Constant Methods -----------------------------------------------//
        const Vector2d      getCenter() const;
        double              getAngle() const;
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
        
        //--- friends --------------------------------------------------------//
        friend std::istream&    operator>>(std::istream&, CCamera* const);
        friend std::ostream&    operator<<(std::ostream&, CCamera* const);

    protected:
        
        //--- Methods --------------------------------------------------------//
        virtual void myAttachTo();
        
        void copy(const CCamera&);
        void updateWithHook();
        void updateWithoutHook();

        //--- Variables [protected] ------------------------------------------//
        std::array<Vector2d,4>  m_vecFrame0;        ///< Initial camera frame
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
    return m_KinematicsState.getOrigin() - IUniverseScaled::cellToDouble(m_vecCell);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the camera angle
///
/// \return The camera angle
///
////////////////////////////////////////////////////////////////////////////////
inline double CCamera::getAngle() const
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches camera to UID user
///
////////////////////////////////////////////////////////////////////////////////
inline void CCamera::myAttachTo()
{
    METHOD_ENTRY("CCamera::myAttachTo")
    m_KinematicsState.attachTo(&(m_pRef->getKinematicsState()));
}

#endif
