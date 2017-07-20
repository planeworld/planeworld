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
/// \file       texture.h
/// \brief      Prototype of class "CTexture"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-05
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_H
#define TEXTURE_H

#define GL_GLEXT_PROTOTYPES

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//
#include "GL/gl.h"
#include "GL/glext.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a texture
///
////////////////////////////////////////////////////////////////////////////////
class CTexture
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        ~CTexture();

        //--- Constant Methods -----------------------------------------------//
        GLuint getID() const;
        
        //--- Methods --------------------------------------------------------//
        bool destroy();
        bool init();
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Variables [private] --------------------------------------------//
        GLuint m_unID = 0; // ID of texture
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes texture
///
////////////////////////////////////////////////////////////////////////////////
inline CTexture::~CTexture()
{
    METHOD_ENTRY("CTexture::~CTexture")
    DTOR_CALL("CTexture::~CTexture")
    this->destroy();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns ID of texture
///
/// \return ID of texture
///
////////////////////////////////////////////////////////////////////////////////
inline GLuint GLuint CTexture::getID() const
{
    METHOD_ENTRY("CTexture::getID")
    return m_unID;
}

#endif // TEXTURE_H
