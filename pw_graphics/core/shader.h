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
/// \file       shader.h
/// \brief      Prototype of class "CShader"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-04
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_H
#define SHADER_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <string>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a vertex or fragment shader
///
////////////////////////////////////////////////////////////////////////////////
class CShader
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        ~CShader();

        //--- Constant Methods -----------------------------------------------//
        const GLuint getID() const;
        
        //--- Methods --------------------------------------------------------//
        bool load(const std::string&, const GLenum);
        bool destroy();
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Variables [private] --------------------------------------------//
        bool    m_bIsLoaded = false; // Indicates, if Shader is loaded and not deleted
      
        GLenum m_Type = 0; // Type of shader
        GLuint m_unID = 0; // ID of shader
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes shader if still loaded
///
////////////////////////////////////////////////////////////////////////////////
inline CShader::~CShader()
{
    METHOD_ENTRY("CShader::getID")
    DTOR_CALL("CShader::~CShader")
    this->destroy();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns ID of shader
///
/// \return ID of shader
///
////////////////////////////////////////////////////////////////////////////////
inline const GLuint CShader::getID() const
{
    METHOD_ENTRY("CShader::getID")
    return m_unID;
}

#endif // SHADER_H
