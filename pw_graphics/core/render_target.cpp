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
/// \file       render_target.cpp
/// \brief      Implementation of class "CRenderTarget"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-22
///
////////////////////////////////////////////////////////////////////////////////

#include "render_target.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise render target
///
/// \param _unResX Width  (resolution in pixels)
/// \param _unResY Height (resolution in pixels)
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CRenderTarget::init(const uint16_t _unResX, const uint16_t _unResY)
{
    METHOD_ENTRY("CRenderTarget::init")
    
    // Frame buffer
    glGenFramebuffers(1, &m_unIDFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_unIDFBO);
    
    // Texture
    glGenTextures(1, &m_unIDTex);
    glBindTexture(GL_TEXTURE_2D, m_unIDTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _unResX, _unResY, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Depth buffer
    glGenRenderbuffers(1, &m_unIDDeB);
    glBindRenderbuffer(GL_RENDERBUFFER, m_unIDDeB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _unResX, _unResY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_unIDDeB);
    
    // Configure frame buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_unIDTex, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    // Check for errors
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        ERROR_MSG("Render Target", "Failed to create frame buffer")
        return false;
    }
    
    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set coordinates for quad to map render target texture on
///
/// \param _fX0 X coordinate vertex 0
/// \param _fY0 Y coordinate vertex 0
/// \param _fX1 X coordinate vertex 1
/// \param _fY1 Y coordinate vertex 1
/// \param _fX2 X coordinate vertex 2
/// \param _fY2 Y coordinate vertex 2
/// \param _fX3 X coordinate vertex 3
/// \param _fY3 Y coordinate vertex 3
///
////////////////////////////////////////////////////////////////////////////////
void CRenderTarget::setTarget(const float _fX0, const float _fY0,
                              const float _fX1, const float _fY1,
                              const float _fX2, const float _fY2,
                              const float _fX3, const float _fY3)
{
    METHOD_ENTRY("CRenderTarget::setTarget")

    m_vecTarget[0]  = _fX0; m_vecTarget[1]  =_fY0; m_vecTarget[2]  = -10.0;
    m_vecTarget[3]  = _fX1; m_vecTarget[4]  =_fY1; m_vecTarget[5]  = -10.0;
    m_vecTarget[6]  = _fX3; m_vecTarget[7]  =_fY3; m_vecTarget[8]  = -10.0;
    m_vecTarget[9]  = _fX3; m_vecTarget[10] =_fY3; m_vecTarget[11] = -10.0;
    m_vecTarget[12] = _fX1; m_vecTarget[13] =_fY1; m_vecTarget[14] = -10.0;
    m_vecTarget[15] = _fX2; m_vecTarget[16] =_fY2; m_vecTarget[17] = -10.0;
}
