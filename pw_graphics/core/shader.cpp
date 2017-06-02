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
/// \file       shader.cpp
/// \brief      Implementation of class "CShader"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-04-04
///
////////////////////////////////////////////////////////////////////////////////

#include "shader.h"

//--- Standard header --------------------------------------------------------//
#include <fstream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Loads shader from given file
///
/// \param _strFilename Filename of shader to be loaded
/// \param _Type Type of shader (vertex, fragment, geometry)
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CShader::load(const std::string& _strFilename, const GLenum _Type)
{
    METHOD_ENTRY("CShader::load")
    
    std::ifstream Filestream;

    //--- Open the given file ---//
    Filestream.open(_strFilename.c_str());
    if (!Filestream)
    {
        DOM_FIO(ERROR_MSG("Shader", "File " + _strFilename + " could not be opened."))
        Filestream.clear();
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("Shader", _strFilename + " succesfully opened."))
    }
    
    //--- Read shader from file ---//
    std::string strShaderCode;
    std::string strLine;
    while (!Filestream.eof())
    {
        std::getline(Filestream, strLine);
        strShaderCode.append(strLine+"\n");
    }
    Filestream.close();
    
    
    //--- Transfer to OpenGL specifications ---//
    const char* pchShaderCode = strShaderCode.c_str();
    GLint       nShaderCodeLength = strShaderCode.length();
    
    DEBUG_BLK(
        DOM_VAR(DEBUG_MSG("Shader", "Shadercode for shader " << _strFilename << ":"))
        std::cout << strShaderCode << std::endl;
    )
    
    //--- Create Shader ---//
    m_unID = glCreateShader(_Type);
    glShaderSource(m_unID, 1, (const GLchar **)&pchShaderCode, &nShaderCodeLength);
    glCompileShader(m_unID);
    
    //--- Check for errors ---//
    GLint nIsCompiled = 0;
    glGetShaderiv(m_unID, GL_COMPILE_STATUS, &nIsCompiled);
    if (nIsCompiled == GL_FALSE)
    {
        ERROR_MSG("Shader", "Failed to compile shader " << _strFilename)
        
        GLint nLengthMax = 0;
        glGetShaderiv(m_unID, GL_INFO_LOG_LENGTH, &nLengthMax);

        std::vector<GLchar> ErrorLog(nLengthMax);
        glGetShaderInfoLog(m_unID, nLengthMax, &nLengthMax, &ErrorLog[0]);

        ERROR_BLK(
        for (auto ci : ErrorLog)
        {
            std::cerr << ci;
        }
        std::cerr << std::endl;
        )
        glDeleteShader(m_unID);
        return false;
    }
    else
    {
        INFO_MSG("Shader", "Successfully compiled shader " << _strFilename)
    }
    
    m_Type = _Type;
    m_bIsLoaded = true;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deletes shader
///
////////////////////////////////////////////////////////////////////////////////
bool CShader::destroy()
{
    METHOD_ENTRY("CShader::destroy")
    if (m_bIsLoaded)
    {
        glDeleteShader(m_unID);
        m_bIsLoaded = false;
        return true;
    }
    else
    {
      DEBUG_MSG("Shader", "Shader already destroyed.")
      return false;
    }
}
