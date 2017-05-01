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
/// \file       win_frame_user.h
/// \brief      Prototype of interface "IWinFrameUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-08
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WIN_FRAME_USER_H
#define WIN_FRAME_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "graphics.h"
#include "log.h"

//--- Misc header ------------------------------------------------------------//

const bool WIN_INHERIT = true;
const bool WIN_NO_INHERIT = false;


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Defines a window frame and its properties
///
////////////////////////////////////////////////////////////////////////////////
class IWinFrameUser : virtual public CGraphicsBase
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        IWinFrameUser();
        
        //--- Constant methods -----------------------------------------------//
        int getPositionX() const {return m_nFramePosX;}
        int getPositionY() const {return m_nFramePosY;}
        int getWidth() const {return m_nFrameWidth;}
        int getHeight() const {return m_nFrameHeight;}

        //--- Methods --------------------------------------------------------//
        void resize(const int, const int);
        void setColorBG(const ColorTypeRGBA&, const bool);
        void setColorFG(const ColorTypeRGBA&, const bool);
        void setPosition(const int, const int);
        
    protected:
        
                void drawFrame();
        virtual void myResize(const int, const int) {}
        virtual void mySetColorBG(const ColorTypeRGBA&) {}
        virtual void mySetColorFG(const ColorTypeRGBA&) {}
        virtual void mySetPosition(const int, const int) {}
        
        //--- Variables [protected] ------------------------------------------//
        ColorTypeRGBA   m_WinColorBG;    ///< Background colour for this window frame
        ColorTypeRGBA   m_WinColorFG;    ///< Foreground colour for this window frame
        int             m_nFrameBorderX; ///< Inner frame, border left/right
        int             m_nFrameBorderY; ///< Inner frame, border top/bottom
        int             m_nFramePosX;    ///< X position of top left corner
        int             m_nFramePosY;    ///< Y position of top left corner
        int             m_nFrameWidth;   ///< Width of window
        int             m_nFrameHeight;  ///< Height of window

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resizes this window and its inherited widgets
///
/// \param _nX Size X, width
/// \param _nY Size Y, height
///
////////////////////////////////////////////////////////////////////////////////
inline void IWinFrameUser::resize(const int _nX, const int _nY)
{
    METHOD_ENTRY("IWinFrameUser::resize")
    m_nFrameWidth = _nX;
    m_nFrameHeight = _nY;
    
    this->myResize(_nX, _nY);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the background colour for this and optionally inherited windows
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
/// \param _bInherit Indicates, if colour setting should be inherited to childs
///
////////////////////////////////////////////////////////////////////////////////
inline void IWinFrameUser::setColorBG(const ColorTypeRGBA& _RGBA, const bool _bInherit)
{
    METHOD_ENTRY("IWinFrameUser::setColorBG")
    m_WinColorBG = _RGBA;
    if (_bInherit)
    {
        this->mySetColorBG(_RGBA);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the foreground colour for this and optionally inherited windows
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
/// \param _bInherit Indicates, if colour setting should be inherited to childs
///
////////////////////////////////////////////////////////////////////////////////
inline void IWinFrameUser::setColorFG(const ColorTypeRGBA& _RGBA, const bool _bInherit)
{
    METHOD_ENTRY("IWinFrameUser::setColorFG")
    m_WinColorFG = _RGBA;
    if (_bInherit)
    {
        this->mySetColorFG(_RGBA);
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the position for this and optionally inherited windows
///
/// \param _nPosX Position X
/// \param _nPosY Position Y
///
////////////////////////////////////////////////////////////////////////////////
inline void IWinFrameUser::setPosition(const int _nPosX, const int _nPosY)
{
    METHOD_ENTRY("IWinFrameUser::setPosition")
    m_nFramePosX = _nPosX;
    m_nFramePosY = _nPosY;
    
    this->mySetPosition(_nPosX, _nPosY);
}

#endif // WIN_FRAME_USER_H
