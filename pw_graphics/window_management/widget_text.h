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
/// \file       widget_text.h
/// \brief      Prototype of "CWidgetTexte"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-10
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WIDGET_TEXT_H
#define WIDGET_TEXT_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "widget.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a text widget
///
////////////////////////////////////////////////////////////////////////////////
class CWidgetText : public IWidget
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWidgetText();
        ~CWidgetText() override {}
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        void draw() override;
        void setText(const std::string&);
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        void mySetFont() override;
        void mySetFontColor() override;
        void mySetFontSize() override;
        
        //--- Variables [private] --------------------------------------------//
        sf::Text    m_Text; ///< Text displayed in this widget

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the text displayed in this widget
///
/// \param _strText Text to be displayed within widget
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetText::setText(const std::string& _strText)
{
    METHOD_ENTRY("CWidgetText::setText")
    m_Text.setString(_strText);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetText::mySetFont()
{
    METHOD_ENTRY("CWidgetText::mySetFont")
    m_Text.setFont(*m_pFont);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font colour for inherited widget
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetText::mySetFontColor()
{
    METHOD_ENTRY("CWidgetText::mySetFontColor")
    m_Text.setFillColor(sf::Color(m_FontColor[0]*255.0,
                                  m_FontColor[1]*255.0,
                                  m_FontColor[2]*255.0,
                                  m_FontColor[3]*255.0));
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the fonts size
///
////////////////////////////////////////////////////////////////////////////////
inline void CWidgetText::mySetFontSize()
{
    METHOD_ENTRY("CWidgetText::mySetFontSize")
    m_Text.setCharacterSize(m_nFontSize);
}

#endif // WIDGET_TEXT_H
