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
/// \file       font_user.h
/// \brief      Prototype of interface "IFontUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////
#ifndef FONT_USER_H
#define FONT_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"

//--- Misc header ------------------------------------------------------------//


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Definition of font properties that can be used by the ingeriting class
///
////////////////////////////////////////////////////////////////////////////////
class IFontUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        IFontUser();
        
        //--- Constant methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        void setFont(sf::Font* const);
        void setFontColor(const ColorTypeRGBA&, const bool);
        void setFontSize(const int);
        
    protected:
        
        virtual void mySetFontColor(const ColorTypeRGBA&) {}
        
        //--- Variables [private] --------------------------------------------//
        sf::Font*       m_pFont;        ///< Font instance to be used
        ColorTypeRGBA   m_FontColor;    ///< Colour of font
        int             m_nFontSize;    ///< Size of font

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font used for the inheriting class
///
/// \param _pFont Font to be set
///
////////////////////////////////////////////////////////////////////////////////
inline IFontUser::IFontUser() : m_pFont(nullptr),
                                m_FontColor({1.0, 1.0, 1.0, 1.0}),
                                m_nFontSize(16)
{
    METHOD_ENTRY("IFontUser::IFontUser")
    CTOR_CALL("IFontUser")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the font used for the inheriting class
///
/// \param _pFont Font to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void IFontUser::setFont(sf::Font* const _pFont)
{
    METHOD_ENTRY("IFontUser::setFont")
    m_pFont = _pFont;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the colour of the font
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
/// \param _bInherit Indicates, if colour setting should be inherited to childs
///
////////////////////////////////////////////////////////////////////////////////
inline void IFontUser::setFontColor(const ColorTypeRGBA& _RGBA, const bool _bInherit)
{
    METHOD_ENTRY("IFontUser::setFontColor")
    m_FontColor = _RGBA;
    if (_bInherit)
    {
        this->mySetFontColor(_RGBA);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the size of the font
///
/// \param _nFontSize Font size to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void IFontUser::setFontSize(const int _nFontSize)
{
    METHOD_ENTRY("IFontUser::setFontSize")
    m_nFontSize = _nFontSize;
}

#endif // FONT_USER_H
