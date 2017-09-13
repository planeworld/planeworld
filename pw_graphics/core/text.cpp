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
/// \file       text.cpp
/// \brief      Implementation of class "CText"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-07-20
///
////////////////////////////////////////////////////////////////////////////////

#include "text.h"

//--- Standard header --------------------------------------------------------//

//--- Misc-Header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising font manager
///
////////////////////////////////////////////////////////////////////////////////
CText::CText(CFontManager* const _pFontManager) : CGraphicsBase(),
                  m_Color({{1.0, 1.0, 1.0, 1.0}}),
                  m_nSize(16),
                  m_fLength(0.0f),
                  m_fPosX(0.0f),
                  m_fPosY(0.0f),
                  m_nWordWrap(FONT_MGR_NO_WORD_WRAP),
                  m_bCentered(false),
                  m_bNewState(true),
                  m_strFont(FONT_MGR_FONT_DEFAULT),
                  m_strText("CText"),
                  m_pFontManager(_pFontManager) 
{
    METHOD_ENTRY("CText::CText")
    CTOR_CALL("CText::CText")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Display text on screen
///
////////////////////////////////////////////////////////////////////////////////
void CText::display()
{
    METHOD_ENTRY("CText::display")
    
    DOM_DEV
    (
        if (m_strFont == "")
        {
            WARNING_MSG("Text", "Font not set. Text would have been: " << m_strText)
            goto LabelDomDevNoFontManager;
        }
        if (m_pFontManager == nullptr)
        {
            WARNING_MSG("Text", "Font manager not set.")
            goto LabelDomDevNoFontManager;
        }
    )
    
    m_Graphics.setColor(m_Color);
    m_pFontManager->setFont(m_strFont);
    m_pFontManager->setSize(m_nSize);
    
    m_pFontManager->drawText(m_strText, m_fPosX, m_fPosY, m_bCentered, m_nWordWrap);

    DOM_DEV(LabelDomDevNoFontManager:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets font to be used for this text
///
/// \param _strFont Font to be used for this text
///
////////////////////////////////////////////////////////////////////////////////
void CText::setFont(const std::string& _strFont)
{
    METHOD_ENTRY("CText::setFont")
    m_strFont = _strFont;
    m_bNewState = true;
    
    // Set font in font manager. This is usually done before rendering, but 
    // here, it ensures that the font manager rasterises a new font if not
    // already existing. Hence, after this call all font/font size related
    // calls are valid, such as getTextLength.
    m_pFontManager->setFont(_strFont);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets font sizeto be used for this text
///
/// \param _nSize Font size to be used for this text
///
////////////////////////////////////////////////////////////////////////////////
void CText::setSize(const int _nSize)
{
    METHOD_ENTRY("CText::setSize")
    m_nSize = _nSize;
    m_bNewState = true;
    
    // Set font size in font manager. This is usually done before rendering, but 
    // here, it ensures that the font manager rasterises a new font size if not
    // already existing. Hence, after this call all font/font size related
    // calls are valid, such as getTextLength.
    m_pFontManager->setSize(_nSize);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets position of text
///
/// \param _fPosX X coordinate of text
/// \param _fPoxY Y coordinate of text
/// \param _bCentered Indicates, if text should be centered (true) or left
///                   aligned (false)
///
////////////////////////////////////////////////////////////////////////////////
void CText::setPosition(const float& _fPosX, const float& _fPosY, const bool _bCentered)
{
    METHOD_ENTRY("CText::setPosition")
    
    m_fPosX = _fPosX;
    m_fPosY = _fPosY + m_nSize;
    m_bCentered = _bCentered;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the actual text string to be stored
///
/// This class sets the text to be stored and calculates its length
///
/// \param _strText Text to be stored
///
////////////////////////////////////////////////////////////////////////////////
void CText::setText(const std::string& _strText)
{
    METHOD_ENTRY("CText::setText")
    
    m_strText = _strText;
    
    DOM_DEV(
        if (m_strFont == "")
        {
            WARNING_MSG("Text", "No font set, yet. Call <setFont> first. Text would have been: " << _strText)
            goto DomDevTextNoFont;
        }
    )
    m_bNewState = true;
    
    DOM_DEV(DomDevTextNoFont:)
}

