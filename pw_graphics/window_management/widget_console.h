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
/// \file       widget_console.h
/// \brief      Prototype of "CWidgetConsole"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WIDGET_CONSOLE_H
#define WIDGET_CONSOLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "com_console.h"
#include "widget.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a command console widget
///
////////////////////////////////////////////////////////////////////////////////
class CWidgetConsole : public IWidget
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWidgetConsole();
        ~CWidgetConsole() override {}
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        void draw() override;
        void setComConsole(CComConsole* const);
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        void mySetFont() override;
        void mySetFontColor() override;
        void mySetFontSize() override;
        
        //--- Variables [private] --------------------------------------------//
        CComConsole*    m_pComConsole;          ///< Command console used in this widget
        int             m_nComHistoryVisible;   ///< Visible part of command history

        sf::Text        m_ConsoleText;          ///< Text output in com console
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the command console used in this widget
///
/// \param _pComConsole Command console to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetConsole::setComConsole(CComConsole* const _pComConsole)
{
    METHOD_ENTRY("CWidgetConsole::setComConsole")
    m_pComConsole = _pComConsole;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetConsole::mySetFont()
{
    METHOD_ENTRY("CWidgetConsole::mySetFont")
    m_ConsoleText.setFont(*m_pFont);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font colour for inherited widget
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetConsole::mySetFontColor()
{
    METHOD_ENTRY("CWidgetConsole::mySetFontColor")
    m_ConsoleText.setFillColor(sf::Color(m_FontColor[0]*255.0,
                                         m_FontColor[1]*255.0,
                                         m_FontColor[2]*255.0,
                                         m_FontColor[3]*255.0));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the fonts size
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetConsole::mySetFontSize()
{
    METHOD_ENTRY("CWidgetConsole::mySetFontSize")
    m_ConsoleText.setCharacterSize(m_nFontSize);
}

#endif // WIDGET_CONSOLE_H
