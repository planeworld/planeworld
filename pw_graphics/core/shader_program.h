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
/// \file       shader_program.h
/// \brief      Prototype of class "CShaderProgram"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-09
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <string>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "shader.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a vertex or fragment shader
///
////////////////////////////////////////////////////////////////////////////////
class CShaderProgram
{

    public:

        //--- Constructor/Destructor -----------------------------------------//

        //--- Constant Methods -----------------------------------------------//
        const GLuint getID() const;
        void         use() const;
        
        //--- Methods --------------------------------------------------------//
        void addShader(const CShader&);
        void create();
        bool link();
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Variables [protected] ------------------------------------------//
        GLuint m_unID = 0;           // ID of shader program
        std::vector<GLuint> Shaders; // Shaders attached to program
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns ID of shader
///
/// \return ID of shader
///
////////////////////////////////////////////////////////////////////////////////
inline const GLuint CShaderProgram::getID() const
{
    METHOD_ENTRY("CShaderProgram::getID")
    return m_unID;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Use this GL shader program 
///
////////////////////////////////////////////////////////////////////////////////
inline void CShaderProgram::use() const
{
    METHOD_ENTRY("CShaderProgram::use")
    glUseProgram(m_unID);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds given shader to shader program
///
/// \param _Shader Shader to be added to program
///
////////////////////////////////////////////////////////////////////////////////
inline void CShaderProgram::addShader(const CShader& _Shader)
{
    METHOD_ENTRY("CShaderProgram::addShader")
    glAttachShader(m_unID, _Shader.getID());
    Shaders.push_back(_Shader.getID());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates GL shader program
///
////////////////////////////////////////////////////////////////////////////////
inline void CShaderProgram::create()
{
    METHOD_ENTRY("CShaderProgram::create")
    m_unID = glCreateProgram();
}

#endif // SHADER_PROGRAM_H
