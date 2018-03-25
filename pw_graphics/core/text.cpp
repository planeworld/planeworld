////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 - 2018 Torsten Büschenfeld
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
                  m_strText(""),
                  m_pFontManager(_pFontManager) 
{
    METHOD_ENTRY("CText::CText")
    CTOR_CALL("CText::CText")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add another part of text. Current state of parameters are used
///
/// \param _strText Text part to add to text
///
////////////////////////////////////////////////////////////////////////////////
void CText::addTextPart(const std::string& _strText)
{
    METHOD_ENTRY("CText::addTextPart")
    
    DOM_DEV(
        if (m_strFont == "")
        {
            WARNING_MSG("Text", "No font set, yet. Call <setFont> first. Text would have been: " << _strText)
            goto DomDevTextNoFont;
        }
    )
    
    m_PartsColors.push_back(m_Color);
    m_PartsSizes.push_back(m_nSize);
    m_PartsFonts.push_back(m_strFont);
    m_PartsTexts.push_back(_strText);
    
    m_strText += _strText;
    
    DOM_DEV(DomDevTextNoFont:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clear text (all parts)
///
////////////////////////////////////////////////////////////////////////////////
void CText::clearText()
{
    METHOD_ENTRY("CText::clearText")
    
    m_PartsColors.clear();
    m_PartsFonts.clear();
    m_PartsSizes.clear();
    m_PartsTexts.clear();
    m_strText.clear();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Display text on screen
///
////////////////////////////////////////////////////////////////////////////////
void CText::display()
{
    METHOD_ENTRY("CText::display")

    auto i = 0u;
    
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
    
    for (auto Text : m_PartsTexts)
    {
        m_Graphics.setColor(m_PartsColors[i]);
        m_pFontManager->setFont(m_PartsFonts[i]);
        m_pFontManager->setSize(m_PartsSizes[i]);
    
        if (i==0u)
            m_pFontManager->drawText(Text, m_fPosX, m_fPosY, m_bCentered, m_nWordWrap);
        else
            m_pFontManager->drawText(Text, m_bCentered, m_nWordWrap);
        ++i;
    }

    DOM_DEV(LabelDomDevNoFontManager:)
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return length of text (all parts composed) in px
///
/// \return Length of text in px
///
////////////////////////////////////////////////////////////////////////////////
float CText::getLength()
{
    METHOD_ENTRY("CText::getLength")
    // Calculate length of text
    if (m_bNewState)
    {
        m_fLength = 0.0;
        double fLengthMax = 0.0;
        char chLast = 0;
        for (auto Text : m_PartsTexts)
        {
            if (chLast != 10)
                m_fLength += m_pFontManager->getTextLength(Text, m_strFont, m_nSize);
            else
                m_fLength = m_pFontManager->getTextLength(Text, m_strFont, m_nSize);
            
            if (m_fLength > fLengthMax) fLengthMax = m_fLength;
            
            chLast = Text.back();
            m_bNewState = false;
        }
        m_fLength = fLengthMax;
    }
    return m_fLength;
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
/// This method can be used as for better readability, if only one text part
/// exists. In this case, all existing parts will be deleted
///
/// \param _strText Text to be stored
///
////////////////////////////////////////////////////////////////////////////////
void CText::setText(const std::string& _strText)
{
    METHOD_ENTRY("CText::setText")
    
    this->clearText();
    this->addTextPart(_strText);
    
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

