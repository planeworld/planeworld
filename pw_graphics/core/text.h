////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       text.h
/// \brief      Prototype of class "CText"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-06-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXT_H
#define TEXT_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "font_manager.h"
#include "graphics.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a graphical text
///
////////////////////////////////////////////////////////////////////////////////
class CText : virtual public CGraphicsBase
{

    public:

        //--- Constructor/Destructor -----------------------------------------//
        CText() = delete;
        CText(CFontManager* const _pFontManager) : CGraphicsBase(),
                  m_Color({{1.0, 1.0, 1.0, 1.0}}),
                  m_fSize(16.0f),
                  m_fPosX(0.0f),
                  m_fPosY(0.0f),
                  m_nWordWrap(FONT_MGR_NO_WORD_WRAP),
                  m_bCentered(false),
                  m_strFont(""),
                  m_strText("CText"),
                  m_pFontManager(_pFontManager) 
        {
            METHOD_ENTRY("CText::CText")
            CTOR_CALL("CText::CText")
        }

        //--- Constant Methods -----------------------------------------------//
        float getFontSize() const {return m_fSize;}
        
        //--- Methods --------------------------------------------------------//
        void display()
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
            m_pFontManager->setSize(m_fSize);
            
            m_pFontManager->drawText(m_strText, m_fPosX, m_fPosY, m_bCentered, m_nWordWrap);

            DOM_DEV(LabelDomDevNoFontManager:)
        }
        
        void setColor(const ColorTypeRGBA& _Color) {m_Color = _Color;}
        void setFont(const std::string& _strFont) {m_strFont = _strFont;}
        void setFontManager(CFontManager* _pFontManager) {m_pFontManager = _pFontManager;}
        void setSize(const float& _fSize) {m_fSize = _fSize;}
        void setPosition(const float& _fPosX, const float& _fPosY, const bool _bCentered = false)
        {
            m_fPosX = _fPosX;
            m_fPosY = _fPosY + m_fSize;
            m_bCentered = _bCentered;
        }
        void setText(const std::string _strText) {m_strText = _strText;}
        void setWordWrap(const int _nWordWrap) {m_nWordWrap = _nWordWrap;}
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Variables [private] --------------------------------------------//
        ColorTypeRGBA   m_Color;        ///< Text color
        float           m_fSize;        ///< Text size
        float           m_fPosX;        ///< X position
        float           m_fPosY;        ///< Y position
        int             m_nWordWrap;    ///< Word wrap (px)
        bool            m_bCentered;    ///< Center Text to position
        std::string     m_strFont;      ///< Name of font to use
        std::string     m_strText;      ///< Text to display
        
        CFontManager*   m_pFontManager; ///< Font manager to use for drawing
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // TEXT_H
