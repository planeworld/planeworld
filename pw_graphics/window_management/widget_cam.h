////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017, 2019 Torsten Büschenfeld
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
/// \file       widget_cam.h
/// \brief      Prototype of "CWidgetCam"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-08-11
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WIDGET_CAM_H
#define WIDGET_CAM_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "render_mode.h"
#include "render_target.h"
#include "visuals_data_storage_user.h"
#include "widget.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a camera widget, using render to texture to display a scene
///
////////////////////////////////////////////////////////////////////////////////
class CWidgetCam : public IWidget,
                   public IVisualsDataStorageUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWidgetCam() = delete;
        CWidgetCam(CFontManager* const);
        ~CWidgetCam() override;
        
        //--- Constant methods -----------------------------------------------//
        const CHandle<CCamera>* getCamera() const {return &m_hCamera;}
        const CRenderMode*      getRenderMode() const {return &m_RenderMode;}
        const CRenderTarget*    getRenderTarget() const {return &m_TargetCam;}
        
        //--- Methods --------------------------------------------------------//
        CRenderTarget* getRenderTarget() {return &m_TargetCam;}
        
        void draw() override;
        void setCamera(CCamera* const _pCamera) {m_hCamera.update(_pCamera);}
        void setShaderProgram(CShaderProgram* const _pShaderProgram) 
        {
            METHOD_ENTRY("CWidgetCam::setShaderProgram")
            m_RenderMode.setShaderProgram(_pShaderProgram);
            m_RenderMode.addUniform("transparency", &m_fTransparency);
        }
        void setTransparency(const double& _fTransparency)
        {
            METHOD_ENTRY("CWidgetCam::setTransparency")
            m_fTransparency = _fTransparency;
        }
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        void myResize(const int, const int) override;
        
        //--- Variables [private] --------------------------------------------//
        CHandle<CCamera> m_hCamera;         ///< Camera attached to this widget
        
        CRenderMode     m_RenderMode;       ///< Render mode to use for rendering
        CRenderTarget   m_TargetCam;        ///< Rendertarget for virtual camera
        
        float           m_fTransparency;    ///< Transparency of virtual camera display    
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // WIDGET_CAM_H
