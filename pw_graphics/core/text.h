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

// Constants
const bool TEXT_POSITION_CENTERED_X = true;

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
        CText(CFontManager* const _pFontManager);

        //--- Constant Methods -----------------------------------------------//
        int                 getFontSize() const {return m_nSize;}
        float               getLength() const {return m_fLength;}
        const std::string&  str() const {return m_strText;}
        
        //--- Methods --------------------------------------------------------//
        void display();
        
        void setColor(const ColorTypeRGBA& _Color) {m_Color = _Color;}
        void setFont(const std::string&);
        void setFontManager(CFontManager* _pFontManager) {m_pFontManager = _pFontManager;}
        void setSize(const int);
        void setPosition(const float& _fPosX, const float& _fPosY, const bool _bCentered = false);
        void setText(const std::string&);
        void setWordWrap(const int _nWordWrap) {m_nWordWrap = _nWordWrap;}
                
        //--- friends --------------------------------------------------------//

    private:
        
        //--- Variables [private] --------------------------------------------//
        ColorTypeRGBA   m_Color;        ///< Text color
        int             m_nSize;        ///< Text size
        float           m_fLength;      ///< Text length (px)
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
