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
/// \file       win_frame_user.cpp
/// \brief      Implementation of interface "IWinFrameUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////

#include "win_frame_user.h"

//--- Program header ---------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
IWinFrameUser::IWinFrameUser() : CGraphicsBase(),
                                 IUIDVisualsUser(),
                                 IUIDUser(),
                                 m_WinColorBG({{0.1, 0.1, 0.1, 0.75}}),
                                 m_WinColorFG({{0.3, 0.3, 0.3, 0.75}}),
                                 m_nFrameBorderX(5),
                                 m_nFrameBorderY(5),
                                 m_nFramePosX(0),
                                 m_nFramePosY(0),
                                 m_nFrameWidth(400),
                                 m_nFrameHeight(200)
{
    METHOD_ENTRY("IWinFrameUser::IWinFrameUser");
    CTOR_CALL("IWinFrameUser::IWinFrameUser");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw frame for this window/widget 
///
/// This is a helper method that might be called by inherited classes
///
////////////////////////////////////////////////////////////////////////////////
void IWinFrameUser::drawFrame()
{
    METHOD_ENTRY("IWinFrameUser::drawFrame")
    
    // Draw frame
    m_Graphics.setColor(m_WinColorBG);
    m_Graphics.filledRect(Vector2d(m_nFramePosX, m_nFramePosY+m_nFrameHeight),
                          Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY));
    m_Graphics.setColor(m_WinColorFG);
    m_Graphics.rect(Vector2d(m_nFramePosX, m_nFramePosY+m_nFrameHeight),
                    Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY));
}
