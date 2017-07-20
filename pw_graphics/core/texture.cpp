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
/// \file       texture.cpp
/// \brief      Implementation of class "CTexture"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-05
///
////////////////////////////////////////////////////////////////////////////////

#include "texture.h"

//--- Standard header --------------------------------------------------------//
#include <fstream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise texture
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
bool CTexture::init()
{
    METHOD_ENTRY("CTexture::init")
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Deletes shader
///
////////////////////////////////////////////////////////////////////////////////
bool CTexture::destroy()
{
    METHOD_ENTRY("CTexture::destroy")
    glDeleteShader(m_unID);
    return true;
}
