////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
#include "unique_id_referrer.h"
#include "visuals_data_storage_user.h"
#include "widget.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a camera widget, using render to texture to display a scene
///
////////////////////////////////////////////////////////////////////////////////
class CWidgetCam : public IWidget,
                   public IUniqueIDReferrer<CCamera>,
                   public IVisualsDataStorageUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWidgetCam() = delete;
        CWidgetCam(CFontManager* const);
        ~CWidgetCam() override;
        
        //--- Constant methods -----------------------------------------------//
        const CRenderTarget* getRenderTarget() const {return &m_TargetCam;}
        
        //--- Methods --------------------------------------------------------//
        void draw() override;
        
        void setShaderProgram(CShaderProgram* const _pShaderProgram) {m_RenderMode.setShaderProgram(_pShaderProgram);}
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        void myResize(const int, const int) override;
        
        //--- Variables [private] --------------------------------------------//
        CRenderMode   m_RenderMode;     ///< Render mode to use for rendering
        CRenderTarget m_TargetCam;      ///< Rendertarget for virtual camera
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // WIDGET_CAM_H
