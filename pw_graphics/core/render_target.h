////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       render_target.h
/// \brief      Prototype of class "CRenderTarget"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-22
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

const bool RENDER_TARGET_CLEAR = true;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class to define a render target, using a frame buffer object
///        (render to texture).
///
////////////////////////////////////////////////////////////////////////////////
class CRenderTarget : virtual public CGraphicsBase
{

    public:
      
        //--- Constructor/Destructor -----------------------------------------//
        ~CRenderTarget();
        
        //--- Methods --------------------------------------------------------//
        bool init(const std::uint16_t, const std::uint16_t,
                  const std::uint16_t = 1u);

        //--- Constant Methods -----------------------------------------------//
              GLuint                getIDTex() const;
        const std::vector<GLfloat>& getTexUV() const;
        void bind(const bool _bClear = false) const;
        void unbind() const;
        
    private:
        
        //--- Variables [protected] ------------------------------------------//
        GLuint m_unIDFBO = 0;   ///< ID of framebuffer object
        GLuint m_unIDTex = 0;   ///< ID of texture
        
        std::uint16_t m_unResX = 100u; ///< Resolution x
        std::uint16_t m_unResY = 100u; ///< Resolution y
        std::uint16_t m_unSub  =   1u; ///< Subsampling factor

        std::vector<GLfloat> m_vecTexUV = {
                                    0.0f, 0.0f,
                                    1.0f, 0.0f,
                                    0.0f, 1.0f,
                                    1.0f, 1.0f
                                  }; ///< Texture coordinates
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return id of texture to use as render target
///
/// \return ID of texture to use as render target
///
////////////////////////////////////////////////////////////////////////////////
inline GLuint CRenderTarget::getIDTex() const
{
    METHOD_ENTRY("CRenderTarget::getIDTex")
    return m_unIDTex;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return the texture coordinates referred to render target
///
/// \return Texture coordinates referred to render target
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<GLfloat>& CRenderTarget::getTexUV() const
{
    METHOD_ENTRY("CRenderTarget::getTexUV")
    return m_vecTexUV;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Bind the framebuffer object
///
/// \param _bClear Clear frame buffer?
///
////////////////////////////////////////////////////////////////////////////////
inline void CRenderTarget::bind(const bool _bClear) const
{
    METHOD_ENTRY("CRenderTarget::bind")
    
    std::cout << "RT   [" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_unIDFBO);
    if (_bClear) glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, m_unResX / m_unSub, m_unResY / m_unSub);
    m_Graphics.setViewPort((-m_unResX >> 1) / GRAPHICS_PX_PER_METER, (m_unResX >> 1) / GRAPHICS_PX_PER_METER,
                           (-m_unResY >> 1) / GRAPHICS_PX_PER_METER, (m_unResY >> 1) / GRAPHICS_PX_PER_METER);
    m_Graphics.restartRenderBatch();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Unbind the framebuffer object
///
////////////////////////////////////////////////////////////////////////////////
inline void CRenderTarget::unbind() const
{
    METHOD_ENTRY("CRenderTarget::unbind")
    
    std::cout << "RT   ]" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif // RENDER_TARGET_H
