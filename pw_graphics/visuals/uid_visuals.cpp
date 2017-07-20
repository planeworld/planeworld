////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2017 Torsten Büschenfeld
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
/// \file       visuals_manager.h
/// \brief      Implementation of class "CVisualsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-06
///
////////////////////////////////////////////////////////////////////////////////

#include "uid_visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draws a circle using graphics base
///
/// \param _nPosX Position X in screen space
/// \param _nPosY Position Y in screen space
/// \param _nUID  UID to be displayed
///
///
////////////////////////////////////////////////////////////////////////////////
void CUIDVisuals::draw(const int _nPosX, const int _nPosY, const UIDType _nUID)
{
    METHOD_ENTRY("CUIDVisuals::draw")
    if (m_bShowUID)
    {
        m_Graphics.beginRenderBatch();
            m_Graphics.setColor(m_aBGColor);
            m_Graphics.filledRect(Vector2d(_nPosX, _nPosY),
                                  Vector2d(_nPosX+30, _nPosY+15));
        m_Graphics.endRenderBatch();
        m_Graphics.beginRenderBatch(GRAPHICS_SHADER_MODE_FONT);
            UIDText.setText(std::to_string(_nUID));
            UIDText.setPosition(_nPosX, _nPosY);
            UIDText.display();
        m_Graphics.endRenderBatch();
    }
}
