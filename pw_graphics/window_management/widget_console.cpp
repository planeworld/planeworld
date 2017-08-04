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
/// \param _pFontManager Font manager to use for drawing text
///
////////////////////////////////////////////////////////////////////////////////
CWidgetConsole::CWidgetConsole(CFontManager* const _pFontManager) :
                               IWidget(_pFontManager),
                               ConsoleText(_pFontManager),
                               m_nComHistoryVisible(10)
{
    METHOD_ENTRY("CWidgetConsole::CWidgetConsole");
    CTOR_CALL("CWidgetConsole::CWidgetConsole");

    m_Type = WidgetTypeType::CONSOLE;
    
    m_UID.setName("Widget_Console_"+m_UID.getName());
    ConsoleText.setText(m_UID.getName());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw this widget
///
////////////////////////////////////////////////////////////////////////////////
void CWidgetConsole::draw()
{
    METHOD_ENTRY("CWidgetConsole::draw")

    m_Graphics.beginRenderBatch("world");
        this->drawFrame();
    m_Graphics.endRenderBatch();
    
    m_nComHistoryVisible = m_nFrameHeight / ConsoleText.getFontSize() - 1;

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
    
    ConsoleText.setText(oss.str());
    ConsoleText.setPosition(m_nFramePosX, m_nFramePosY);
    
    m_Graphics.beginRenderBatch("font");
        ConsoleText.display();
    m_Graphics.endRenderBatch();
    
    DOM_DEV(
        static bool bWarned = false;
        if (m_pUIDVisuals == nullptr)
        {
            if (!bWarned)
            {
                WARNING_MSG("Console Widget", "UID visuals not set.")
                bWarned = true;
            }
            goto DomDev;
        })
        m_pUIDVisuals->draw(m_nFramePosX, m_nFramePosY, m_UID.getValue());
    DOM_DEV(DomDev:)
        
    m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
}

