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
        UIDText.setText(std::to_string(_nUID));
        double fSizeX = UIDText.getLength()+5.0;
        m_Graphics.beginRenderBatch(1);
            m_Graphics.setColor(m_aBGColor);
            m_Graphics.filledRect(Vector2d(_nPosX, _nPosY),
                                  Vector2d(_nPosX + fSizeX, _nPosY+UIDText.getFontSize()));
        m_Graphics.endRenderBatch();
        m_Graphics.beginRenderBatch(2);
            UIDText.setPosition(_nPosX+fSizeX*0.5, _nPosY, TEXT_POSITION_CENTERED_X);
            UIDText.display();
        m_Graphics.endRenderBatch();
    }
}
