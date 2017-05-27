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
/// \file       widget_console.cpp
/// \brief      Implementation of class "CWidgetConsole"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////

#include "widget_console.h"

//--- Program header ---------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
////////////////////////////////////////////////////////////////////////////////
CWidgetConsole::CWidgetConsole() : IWidget(),
                                   m_nComHistoryVisible(10)
{
    METHOD_ENTRY("CWidgetConsole::CWidgetConsole");
    CTOR_CALL("CWidgetConsole::CWidgetConsole");

    m_Type = WidgetTypeType::CONSOLE;
    
    m_UID.setName("Widget_Console_"+m_UID.getName());
    m_ConsoleText.setString(m_UID.getName());
    m_ConsoleText.setFont(*m_pFont);
    m_ConsoleText.setCharacterSize(m_nFontSize);
    m_ConsoleText.setFillColor(sf::Color(m_FontColor[0]*255.0,
                                         m_FontColor[1]*255.0,
                                         m_FontColor[2]*255.0,
                                         m_FontColor[3]*255.0));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw this widget
///
////////////////////////////////////////////////////////////////////////////////
void CWidgetConsole::draw()
{
    METHOD_ENTRY("CWidgetConsole::draw")

    this->drawFrame();
    
    m_nComHistoryVisible = m_nFrameHeight / m_pFont->getLineSpacing(m_nFontSize) - 1;

    std::stringstream oss;
    auto i = m_pComConsole->getCommands().size() - m_nComHistoryVisible;
    if (i > m_pComConsole->getCommands().size()) i = 0;
    while (i < m_pComConsole->getCommands().size())
    {
        oss << "> " << m_pComConsole->getCommands().at(i);
        if (m_pComConsole->getReturnValues().at(i) != "")
        {
            oss << " => " << m_pComConsole->getReturnValues().at(i);
        }
        oss << "\n";
        
        ++i;
    }
    oss << s_ConsoleModeTypeToStringMap[m_pComConsole->getMode()] << " > " << m_pComConsole->getCurrentCommand() << "_";
    
    m_Graphics.getWindow()->pushGLStates();

    m_ConsoleText.setString(oss.str());
    m_ConsoleText.setPosition(m_nFramePosX, m_nFramePosY);
    m_Graphics.getWindow()->draw(m_ConsoleText);
    m_Graphics.getWindow()->popGLStates();        
        
    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
}

