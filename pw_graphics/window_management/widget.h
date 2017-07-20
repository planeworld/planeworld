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
/// \file       widget.h
/// \brief      Prototype of interface "IWidget"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-08
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WIDGET_H
#define WIDGET_H

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "font_user.h"
#include "graphics.h"
#include "win_frame_user.h"

//--- Misc header ------------------------------------------------------------//

// Forward declaration
class IWidget;

/// List type of Widgets
typedef std::vector<IWidget*> WidgetsType;

/// Specifies the type of the widget
enum class WidgetTypeType
{
    INVALID,
    CONSOLE,
    TEXT
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a generic widget interface
///
////////////////////////////////////////////////////////////////////////////////
class IWidget : public IFontUser,
                public IWinFrameUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        IWidget() = delete;
        IWidget(CFontManager* const _pFontManager) : IFontUser(_pFontManager), IWinFrameUser() {}
        virtual ~IWidget() {}
        
        //--- Constant methods -----------------------------------------------//
        virtual void draw() = 0;
        
        WidgetTypeType getType() const {return m_Type;}

        //--- Methods --------------------------------------------------------//
        
    protected:
        
        //--- Variables [private] --------------------------------------------//
        WidgetTypeType m_Type{WidgetTypeType::INVALID};

};

//--- Enum parser ------------------------------------------------------------//
static std::unordered_map<WidgetTypeType, std::string> s_WidgetTypeToStringMap = {
    {WidgetTypeType::CONSOLE, "console"},
    {WidgetTypeType::TEXT, "text"}
}; ///< Map from WidgetTypeType to string

const std::unordered_map<std::string, WidgetTypeType> STRING_TO_WIDGET_TYPE_MAP = {
    {"console", WidgetTypeType::CONSOLE},
    {"text", WidgetTypeType::TEXT}
}; ///< Map from string to WidgetTypeType

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Maps given string to console mode
///
/// \return Console mode
///
////////////////////////////////////////////////////////////////////////////////
static WidgetTypeType mapStringToWidgetType(const std::string& _strS)
{
    METHOD_ENTRY("mapStringToWidgetType")
    
    const auto ci = STRING_TO_WIDGET_TYPE_MAP.find(_strS);
    if (ci != STRING_TO_WIDGET_TYPE_MAP.end())
        return ci->second;
    else
        return WidgetTypeType::INVALID;
}

//--- Implementation is done here for inline optimisation --------------------//

#endif // WIDGET_H
