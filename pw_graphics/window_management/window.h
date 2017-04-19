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
#include "log.h"
#include "unique_id_user.h"
#include "widget.h"
#include "win_frame_user.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Definition of a graphical window
///
////////////////////////////////////////////////////////////////////////////////
class CWindow : public IFontUser,
                public IUniqueIDUser,
                public IWinFrameUser
{

    public:
    
        //--- Constructor/Destructor------------------------------------------//
        CWindow();
        virtual ~CWindow();
        
        //--- Constant methods -----------------------------------------------//
        void draw() const; 
        bool isInside(const int, const int) const;
        bool isVisible() const;
        
        //--- Methods --------------------------------------------------------//
        void center();
        void setTitle(const std::string&);
        void setVisibilty(const bool);
        void setWidget(IWidget* const);
        void toggleVisibility();
        
    private:
        
        void myResize(const int, const int);
        void mySetColorBG(const ColorTypeRGBA&);
        void mySetColorFG(const ColorTypeRGBA&);
        void mySetFontColor(const ColorTypeRGBA&);
        void mySetPosition(const int, const int);
        
        //--- Variables [private] --------------------------------------------//
        std::string m_strTitle; ///< Window title
        
        IWidget*    m_pWidget;  ///< Widget of this \ref CWindow
        bool        m_bCenter;  ///< Indicates, if this window is centered
        bool        m_bVisible; ///< Indicates, if this window is visible
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns, if given coordinates are inside of window
///
/// \param _nX X coordinate (screen space)
/// \param _nY Y coordinate (screen space)
///
/// \return Given coordinates inside of window?
///
////////////////////////////////////////////////////////////////////////////////
inline bool CWindow::isInside(const int _nX, const int _nY) const
{
    METHOD_ENTRY("CWindow::isInside")
    return ((_nX >= m_nFramePosX) & (_nX < m_nFramePosX+m_nFrameWidth) &
            (_nY >= m_nFramePosY) & (_nY < m_nFramePosY+m_nFrameHeight));
}

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
/// \brief Sets the title of this window
///
/// \param _strTitle Title to be set
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::setTitle(const std::string& _strTitle)
{
    METHOD_ENTRY("CWindow::setTitle")
    m_strTitle = _strTitle;
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
/// \brief Toggles the visibility of this window
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::toggleVisibility()
{
    METHOD_ENTRY("CWindow::toggleVisibility")
    m_bVisible ^= true;
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
        m_pWidget->resize(_nX-m_nFrameBorderX*2, _nY-m_nFrameBorderY*2-m_pFont->getLineSpacing(m_nFontSize));
    if (m_bCenter) this->center();
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
/// \brief Sets the font colour for inherited widget
///
/// \param _RGBA Colour as RGBA array (0.0 - 1.0)
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::mySetFontColor(const ColorTypeRGBA& _RGBA)
{
    METHOD_ENTRY("CWindow::mySetFontColor")
    if (m_pWidget != nullptr)
        m_pWidget->setFontColor(_RGBA, WIN_INHERIT);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the position for inherited widget
///
/// \param _nPosX Position X
/// \param _nPosY Position Y
///
////////////////////////////////////////////////////////////////////////////////
inline void CWindow::mySetPosition(const int _nPosX, const int _nPosY)
{
    METHOD_ENTRY("CWindow::mySetPosition")
    m_bCenter = false;
    if (m_pWidget != nullptr)
        m_pWidget->setPosition(_nPosX + m_nFrameBorderX, _nPosY+m_nFrameBorderY+m_pFont->getLineSpacing(m_nFontSize));
}

#endif // WINDOW_H
