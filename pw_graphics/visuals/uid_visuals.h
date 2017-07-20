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
/// \file       uid_visuals.h
/// \brief      Prototype of class "CUIDVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-05-27
///
////////////////////////////////////////////////////////////////////////////////
#ifndef UID_VISUALS_H
#define UID_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "font_user.h"
#include "graphics.h"
#include "text.h"
#include "unique_id.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Visuals of entity UID's
///
////////////////////////////////////////////////////////////////////////////////
class CUIDVisuals : public CGraphicsBase,
                    public IFontUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CUIDVisuals() = delete;
        CUIDVisuals(CFontManager* const _pFontManager) : CGraphicsBase(),
                        IFontUser(_pFontManager),
                        UIDText(_pFontManager),
                        m_aBGColor({{1.0, 1.0, 1.0, 0.9}}),
                        m_bShowUID(false) {}
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        void draw(const int, const int, const UIDType);
        void setBGColor(const ColorTypeRGBA& _aRGBA) {m_aBGColor = _aRGBA;}
        
        void show() {m_bShowUID = true;}
        void hide() {m_bShowUID = false;}
        void toggle() {m_bShowUID ^= true;}
        
        //--- Variables ------------------------------------------------------//
        CText           UIDText;        ///< UID as drawable text
        
    private:
        
        //--- Methods [private] ----------------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        ColorTypeRGBA   m_aBGColor;   ///< Background color
        
        bool            m_bShowUID;   ///< Indicates, if UIDs are shown
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // UID_VISUALS_H
