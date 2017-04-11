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
#include "font_user.h"
#include "widget.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a text widget
///
////////////////////////////////////////////////////////////////////////////////
class CWidgetText : public IFontUser,
                    public IWidget
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWidgetText();
        ~CWidgetText() override {}
        
        //--- Constant methods -----------------------------------------------//
        
        //--- Methods --------------------------------------------------------//
        void draw() override;
        void setText(const std::string&);
        
    protected:
        
        //--- Variables [private] --------------------------------------------//
        std::string     m_strText; ///< Text displayed in this widget

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
    m_strText = _strText;
}

#endif // WIDGET_TEXT_H
