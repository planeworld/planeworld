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
/// \file       window.h
/// \brief      Prototype of class "CWindow"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-08
///
////////////////////////////////////////////////////////////////////////////////
#ifndef WINDOW_H
#define WINDOW_H

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "font_user.h"
#include "text.h"
#include "widget.h"
#include "win_frame_user.h"

//--- Misc header ------------------------------------------------------------//

/// Defines areas of action, such as close, resize etc.
enum class WinAreaType
{
    CLOSE,
    RESIZE,
    TITLE,
    WIN
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Definition of a graphical window
///
////////////////////////////////////////////////////////////////////////////////
class CWindow : public IFontUser,
                public IWinFrameUser
{

    public:
        
        //--- Constructor/Destructor------------------------------------------//
        CWindow() = delete;
        CWindow(CFontManager* const);
        virtual ~CWindow();
        
        //--- Constant methods -----------------------------------------------//
        IWidget* getWidget() const {return m_pWidget;}
        
        bool isClosable() const {return m_bClosable;}
        bool isInside(const int, const int, const WinAreaType = WinAreaType::WIN) const;
        bool isVisible() const;
        
        //--- Methods --------------------------------------------------------//
        void center();
        void draw();
        void setClosability(const bool _bClosable) {m_bClosable = _bClosable;}
        void setVisibilty(const bool);
        void setWidget(IWidget* const);
        
        //--- Variables ------------------------------------------------------//
        CText   Title;              ///< Window title
        
    private:
        
        void myResize(const int, const int) override;
        void mySetColorBG(const ColorTypeRGBA&) override;
        void mySetColorFG(const ColorTypeRGBA&) override;
        void mySetPosition(const int ,const int) override;
        
        //--- Variables [private] --------------------------------------------//
        IWidget*    m_pWidget;      ///< Widget of this \ref CWindow
        bool        m_bVisible;     ///< Indicates, if this window is visible
        bool        m_bClosable;    ///< Indicates, if this window may be closed
        int         m_nSizeClose;   ///< Size (both dimensions) of close area
        int         m_nSizeResize;  ///< Size (both dimensions) of resize area
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns visibility of window
///
/// \return Visible?
///
////////////////////////////////////////////////////////////////////////////////
inline bool CWindow::isVisible() const
{
    METHOD_ENTRY("CWindow::isVisible")
    return m_bVisible;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the visibility of this window
///
/// \param _bVisible Set window invisible (false) or visible (true)
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::setVisibilty(const bool _bVisible)
{
    METHOD_ENTRY("CWindow::setVisibilty")
    m_bVisible = _bVisible;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets (new) widget for this window
///
/// \param _pWidget Widget to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::setWidget(IWidget* const _pWidget)
{
    METHOD_ENTRY("CWindow::setWidget")
    if (m_pWidget != nullptr)
    {
        NOTICE_MSG("Window", "Widget already existing, overwriting...")
        delete m_pWidget;
        MEM_FREED("IWidget")
    }
    m_pWidget = _pWidget;
    this->mySetPosition(m_nFramePosX, m_nFramePosY);
    this->myResize(m_nFrameWidth, m_nFrameHeight);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resizes inherited widget
///
/// \param _nX Size X, width
/// \param _nY Size Y, height
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::myResize(const int _nX, const int _nY)
{
    METHOD_ENTRY("CWindow::myResize")
    if (m_pWidget != nullptr)
        m_pWidget->resize(_nX-m_nFrameBorderX*2, _nY-m_nFrameBorderY*2-Title.getFontSize());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the backgroud colour for inherited widget
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::mySetColorBG(const ColorTypeRGBA& _RGBA)
{
    METHOD_ENTRY("CWindow::mySetColorBG")
    if (m_pWidget != nullptr)
        m_pWidget->setColorBG(_RGBA, WIN_INHERIT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the foreground colour for inherited widget
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::mySetColorFG(const ColorTypeRGBA& _RGBA)
{
    METHOD_ENTRY("CWindow::mySetColorFG")
    if (m_pWidget != nullptr)
        m_pWidget->setColorFG(_RGBA, WIN_INHERIT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the position of the window
///
/// \param _nX Position X
/// \param _nY Position Y
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::mySetPosition(const int _nX, const int _nY)
{
    METHOD_ENTRY("CWindow::mySetPosition")
    if (m_pWidget != nullptr)
        m_pWidget->setPosition(_nX+m_nFrameBorderX, _nY+Title.getFontSize()+m_nFrameBorderY);
}

#endif // WINDOW_H
