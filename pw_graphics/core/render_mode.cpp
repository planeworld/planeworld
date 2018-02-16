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
/// \date       2017-12-01
///
////////////////////////////////////////////////////////////////////////////////

#include "render_mode.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add an integer uniform to this rendermode
///
/// \param _strName Name of uniform
/// \param _pnInt Integer uniform
///
////////////////////////////////////////////////////////////////////////////////
void CRenderMode::addUniform(const std::string& _strName, GLint* const _pnInt)
{
    METHOD_ENTRY("CRenderMode::addUniform")
    DOM_DEV(
        if (m_pShaderProgram == nullptr)
        {
            WARNING_MSG("Render Mode", "Shader program not set.")
            goto DomDev;
        }
    )
    
    m_UniformsInt.insert({{glGetUniformLocation(m_pShaderProgram->getID(), _strName.c_str()), _pnInt}});
    
    DOM_DEV(DomDev:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add float uniform to this rendermode
///
/// \param _strName Name of uniform
/// \param _pf Float uniform
///
////////////////////////////////////////////////////////////////////////////////
void CRenderMode::addUniform(const std::string& _strName, GLfloat* const _pf)
{
    METHOD_ENTRY("CRenderMode::addUniform")
    GLint nUniformLocation = 0;
    DOM_DEV(
        if (m_pShaderProgram == nullptr)
        {
            WARNING_MSG("Render Mode", "Shader program not set.")
            goto DomDev;
        }
    )
    
    nUniformLocation = glGetUniformLocation(m_pShaderProgram->getID(), _strName.c_str());
    if (nUniformLocation != -1)
    {
        m_UniformsFloat.insert({{nUniformLocation, _pf}});
    }
    else
    {
        WARNING_MSG("Render Mode", "Unknown uniform, maybe shader program is not yet linked.")
    }
    
    DOM_DEV(DomDev:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set texture for texture unit 0
///
/// \param _strName Name of sampler uniform
/// \param _unTexID Texture ID
///
////////////////////////////////////////////////////////////////////////////////
void CRenderMode::setTexture0(const std::string& _strName, const GLuint _unTexID)
{
    METHOD_ENTRY("CRenderMode::setTexture0")
    GLint nUniformLocation = 0;
    DOM_DEV(
        if (m_pShaderProgram == nullptr)
        {
            WARNING_MSG("Render Mode", "Shader program not set.")
            goto DomDev;
        }
    )
    
    m_unTexID0 = _unTexID;
    nUniformLocation = glGetUniformLocation(m_pShaderProgram->getID(), _strName.c_str());
    m_UniformsTextures[0] = nUniformLocation;
    
    DOM_DEV(DomDev:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set texture for texture unit 1
///
/// \param _strName Name of sampler uniform
/// \param _unTexID Texture ID
///
////////////////////////////////////////////////////////////////////////////////
void CRenderMode::setTexture1(const std::string& _strName, const GLuint _unTexID)
{
    METHOD_ENTRY("CRenderMode::setTexture1")
    GLint nUniformLocation = 0;
    DOM_DEV(
        if (m_pShaderProgram == nullptr)
        {
            WARNING_MSG("Render Mode", "Shader program not set.")
            goto DomDev;
        }
    )
    
    m_unTexID1 = _unTexID;
    nUniformLocation = glGetUniformLocation(m_pShaderProgram->getID(), _strName.c_str());
    m_UniformsTextures[1] = nUniformLocation;
    
    DOM_DEV(DomDev:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set up internals to use this render mode
///
////////////////////////////////////////////////////////////////////////////////
void CRenderMode::use()
{
    METHOD_ENTRY("CRenderMode::use")
    
    m_pShaderProgram->use();
    
    if (m_RenderModeType == RenderModeType::VERT3COL4TEX2)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_unTexID0);
    }
    if (m_RenderModeType == RenderModeType::VERT3COL4TEX2X2)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_unTexID0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_unTexID1);
    }
    
    for (auto UniformInt : m_UniformsInt)
    {
        glUniform1i(UniformInt.first, *UniformInt.second);
    }
    for (auto UniformFloat : m_UniformsFloat)
    {
        glUniform1f(UniformFloat.first, *UniformFloat.second);
    }
    if (m_RenderModeType == RenderModeType::VERT3COL4TEX2)
    {
        glUniform1i(m_UniformsTextures[0], 0);
    }
    if (m_RenderModeType == RenderModeType::VERT3COL4TEX2X2)
    {
        glUniform1i(m_UniformsTextures[0], 0);
        glUniform1i(m_UniformsTextures[1], 1);
    }
}
