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
#include "font_manager.h"
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
        IFontUser() = delete;
        IFontUser(CFontManager* const);
        
        //--- Constant methods -----------------------------------------------//

        //--- Methods --------------------------------------------------------//
        
    protected:
        
        //--- Variables [private] --------------------------------------------//
        CFontManager*   m_pFontManager; ///< Font manager instance to use
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
inline IFontUser::IFontUser(CFontManager* const _pFontManager) : m_pFontManager(_pFontManager)
{
    METHOD_ENTRY("IFontUser::IFontUser")
    CTOR_CALL("IFontUser")
}

#endif // FONT_USER_H
