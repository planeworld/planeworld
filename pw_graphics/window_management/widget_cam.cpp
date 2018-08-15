////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017-2018 Torsten Büschenfeld
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
/// \file       widget_cam.cpp
/// \brief      Implementation of class "CWidgetCam"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-08-11
///
////////////////////////////////////////////////////////////////////////////////

#include "widget_cam.h"

//--- Program header ---------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
/// \param _pFontManager Font manager to use for drawing
///
////////////////////////////////////////////////////////////////////////////////
CWidgetCam::CWidgetCam(CFontManager* const _pFontManager) :
                       IWidget(_pFontManager),
                       IVisualsDataStorageUser(),
                       m_fTransparency(0.5f)
{
    METHOD_ENTRY("CWidgetCam::CWidgetCam");
    CTOR_CALL("CWidgetCam::CWidgetCam");

    m_Type = WidgetTypeType::CAMERA;
    
    m_UID.setName("Widget_Cam_"+m_UID.getName());
    
    m_RenderMode.setRenderModeType(RenderModeType::VERT3COL4TEX2);
    m_Graphics.registerRenderMode(m_UID.getName(), &m_RenderMode);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deleting camera widget from data storage
///
////////////////////////////////////////////////////////////////////////////////
CWidgetCam::~CWidgetCam()
{
    METHOD_ENTRY("CWidgetCam::~CWidgetCam");
    DTOR_CALL("CWidgetCam::~CWidgetCam");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws widget displaying a scene using render to texture
///
////////////////////////////////////////////////////////////////////////////////
void CWidgetCam::draw()
{
    METHOD_ENTRY("CWidgetCam::draw")

    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
    
    m_Graphics.beginRenderBatch(m_UID.getName());
        m_Graphics.texturedRect(Vector2d(m_nFramePosX, m_nFramePosY+m_nFrameHeight),
                                Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY),
                                &m_TargetCam.getTexUV());
    m_Graphics.endRenderBatch();
    
    DOM_DEV(
        static bool bWarned = false;
        if (m_pUIDVisuals == nullptr)
        {
            if (!bWarned)
            {
                WARNING_MSG("Camera Widget", "UID visuals not set.")
                bWarned = true;
            }
            goto DomDev;
        })
    
    m_pUIDVisuals->draw(m_nFramePosX, m_nFramePosY, "Widget Camera", m_UID.getValue());
    if (m_hCamera.isValid())
    {
        m_pUIDVisuals->draw(m_nFramePosX, m_nFramePosY + m_pUIDVisuals->UIDText.getFontSize(), "Camera", m_hCamera->getUID());
    }
    
    DOM_DEV(DomDev:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resizes render target
///
/// \param _nX New size X
/// \param _nY New size Y
///
////////////////////////////////////////////////////////////////////////////////
void CWidgetCam::myResize(const int _nX, const int _nY)
{
    METHOD_ENTRY("CWidgetCam::myResize")

    m_TargetCam.init(_nX, _nY);
    m_RenderMode.getShaderProgram()->use();
    m_RenderMode.setTexture0("ScreenTexture", m_TargetCam.getIDTex());
    if (m_hCamera.isValid())
    {
        m_hCamera->setViewport(double(_nX) / GRAPHICS_PX_PER_METER, double(_nY) / GRAPHICS_PX_PER_METER);
    }
}
