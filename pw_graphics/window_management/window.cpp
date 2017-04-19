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
/// \file       window.cpp
/// \brief      Implementation of class "CWindow"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////

#include "window.h"

//--- Program header ---------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CWindow::CWindow() : IUniqueIDUser(),
                     IWinFrameUser(),
                     m_pWidget(nullptr),
                     m_bCenter(false),
                     m_bVisible(false)
{
    METHOD_ENTRY("CWindow::CWindow");
    CTOR_CALL("CWindow::CWindow");
    
    m_UID.setName("Win_"+m_UID.getName());
    m_strTitle = m_UID.getName();
    m_nFontSize = 20; // Fontsize only affects the title at the moment
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CWindow::~CWindow()
{
    METHOD_ENTRY("CWindow::~CWindow")
    DTOR_CALL("CWindow::~CWindow")
    
    if (m_pWidget != nullptr)
    {
        delete m_pWidget;
        MEM_FREED("IWidget")
        m_pWidget = nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw window and its contents
///
///////////////////////////////////////////////////////////////////////////////
void CWindow::draw() const 
{
    METHOD_ENTRY("CWindow::draw")
    
    if (m_bVisible)
    {
        m_Graphics.setColor(m_WinColorBG);
        m_Graphics.filledRect(Vector2d(m_nFramePosX, m_nFramePosY+m_nFrameHeight),
                              Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY));
        m_Graphics.setColor(m_WinColorFG);
        int nSpacing = m_pFont->getLineSpacing(m_nFontSize);
        m_Graphics.rect(Vector2d(m_nFramePosX, m_nFramePosY+nSpacing),
                        Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY));
        m_Graphics.rect(Vector2d(m_nFramePosX, m_nFramePosY+m_nFrameHeight),
                        Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY+nSpacing));
        m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);

        m_Graphics.getWindow()->pushGLStates();
        sf::Text Text;

        Text.setString(m_strTitle);
        Text.setFont(*m_pFont);
        Text.setCharacterSize(m_nFontSize);
        Text.setPosition(m_nFramePosX + m_nFrameWidth/2 - Text.getGlobalBounds().width/2, m_nFramePosY);
        Text.setFillColor(sf::Color(m_FontColor[0]*255.0,
                                    m_FontColor[1]*255.0,
                                    m_FontColor[2]*255.0,
                                    m_FontColor[3]*255.0));
        m_Graphics.getWindow()->draw(Text);
        m_Graphics.getWindow()->popGLStates();        
        
        if (m_pWidget != nullptr) m_pWidget->draw();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Center window referring to main application window
///
///////////////////////////////////////////////////////////////////////////////
void CWindow::center() 
{
    METHOD_ENTRY("CWindow::center")
    this->setPosition((m_Graphics.getWidthScr()-m_nFrameWidth) / 2,
                      (m_Graphics.getHeightScr()-m_nFrameHeight) / 2);
    
    m_bCenter = true;
}
