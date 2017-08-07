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
/// \file       render_mode.h
/// \brief      Prototype of class "CRenderMode"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-08-02
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODE_H
#define RENDER_MODE_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "shader_program.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

/// Specifies the type render mode
enum class RenderModeType
{
    VERT3COL4,
    VERT3COL4TEX2
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class to define the state of one specific render mode
///
/// A render mode is composed of information needed to render one batch of 
/// primitives with the same preconditions. This includes shader program, 
/// texture information, attributes such es vertices and colours etc.
///
////////////////////////////////////////////////////////////////////////////////
class CRenderMode
{

    public:
        
        //--- Constant methods -----------------------------------------------//
        RenderModeType              getRenderModeType() const {return m_RenderModeType;}
        const CShaderProgram*       getShaderProgram() const {return m_pShaderProgram;}
              
        //--- Methods --------------------------------------------------------//
        void setRenderModeType(const RenderModeType _RenderModeType) {m_RenderModeType = _RenderModeType;}
        void setShaderProgram(CShaderProgram* const _pShaderProgram) {m_pShaderProgram = _pShaderProgram;}
        void setTexture(const GLuint _nTexID) {m_nTexID = _nTexID;}
        void use() {m_pShaderProgram->use();}
        
    private:
        
        //--- Variables [private] --------------------------------------------//
        RenderModeType   m_RenderModeType = RenderModeType::VERT3COL4;  ///< Type of this render mode
        CShaderProgram*  m_pShaderProgram{nullptr};                     ///< Referred shader program, active for this mode
        GLuint           m_nTexID = 0;
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // RENDER_TARGET_H
