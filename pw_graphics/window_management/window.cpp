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
/// \file       window.cpp
/// \brief      Implementation of class "CWindow"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-04-09
///
////////////////////////////////////////////////////////////////////////////////

#include "window.h"

//--- Program header ---------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
/// \param _pFontManager Font Manager instance used for drawing text
///
///////////////////////////////////////////////////////////////////////////////
CWindow::CWindow(CFontManager* const _pFontManager) : 
                     IFontUser(_pFontManager),
                     IWinFrameUser(),
                     Title(_pFontManager),
                     m_pWidget(nullptr),
                     m_bCenter(false),
                     m_bVisible(true),
                     m_bClosable(true),
                     m_nSizeClose(10),
                     m_nSizeResize(10)
{
    METHOD_ENTRY("CWindow::CWindow");
    CTOR_CALL("CWindow::CWindow");
    
    m_UID.setName("Win_"+m_UID.getName());
    Title.setText(m_UID.getName());
    Title.setSize(20);
//     m_Title.setFillColor(sf::Color(m_FontColor[0]*255.0,
//                                    m_FontColor[1]*255.0,
//                                    m_FontColor[2]*255.0,
//                                    m_FontColor[3]*255.0));
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CWindow::~CWindow()
{
    METHOD_ENTRY("CWindow::~CWindow")
    DTOR_CALL("CWindow::~CWindow")
    
    if (m_pWidget != nullptr)
    {
        delete m_pWidget;
        MEM_FREED("IWidget")
        m_pWidget = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns, if given coordinates are inside of given window area
///
/// \param _nX X coordinate (screen space)
/// \param _nY Y coordinate (screen space)
/// \param _Area Area to be tested (this distinguishes the different areas for
///              action such as close, resize or title)
///
/// \return Given coordinates inside of window?
///
////////////////////////////////////////////////////////////////////////////////
bool CWindow::isInside(const int _nX,
                       const int _nY,
                       const WinAreaType _Area) const
{
    METHOD_ENTRY("CWindow::isInside")
    
    bool bInside = false;
    switch (_Area)
    {
        case WinAreaType::CLOSE:
        {
            if (m_bClosable)
            {
                bInside = ((_nX >= m_nFramePosX+m_nFrameWidth-m_nSizeClose) & (_nX < m_nFramePosX+m_nFrameWidth) &
                           (_nY >= m_nFramePosY) & (_nY < m_nFramePosY+m_nSizeClose));
            }
            break;
        }
        case WinAreaType::RESIZE:
        {
            bInside = ((_nX >= m_nFramePosX+m_nFrameWidth-m_nSizeResize)  & (_nX < m_nFramePosX+m_nFrameWidth) &
                       (_nY >= m_nFramePosY+m_nFrameHeight-m_nSizeResize) & (_nY < m_nFramePosY+m_nFrameHeight));
            break;
        }
        case WinAreaType::TITLE:
        {
            bInside = ((_nX >= m_nFramePosX) & (_nX < m_nFramePosX+m_nFrameWidth) &
                       (_nY >= m_nFramePosY) & (_nY < m_nFramePosY+Title.getFontSize()));
            break;
        }
        case WinAreaType::WIN:
        {
            bInside = ((_nX >= m_nFramePosX) & (_nX < m_nFramePosX+m_nFrameWidth) &
                       (_nY >= m_nFramePosY) & (_nY < m_nFramePosY+m_nFrameHeight));
            break;
        }
    }
    
    return bInside;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Center window referring to main application window
///
///////////////////////////////////////////////////////////////////////////////
void CWindow::center() 
{
    METHOD_ENTRY("CWindow::center")
    this->setPosition((m_Graphics.getWidthScr()-m_nFrameWidth) / 2,
                      (m_Graphics.getHeightScr()-m_nFrameHeight) / 2);
    
    m_bCenter = true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw window and its contents
///
///////////////////////////////////////////////////////////////////////////////
void CWindow::draw() 
{
    METHOD_ENTRY("CWindow::draw")
    
    if (m_bVisible)
    {
        m_Graphics.beginRenderBatch();
            // Draw background area
            this->drawFrame();
            
            // Draw title frame
            m_Graphics.setColor(m_WinColorFG);
            int nSpacing = Title.getFontSize();
            m_Graphics.rect(Vector2d(m_nFramePosX, m_nFramePosY+nSpacing),
                            Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY));
            
            DOM_DEV(
                static bool bWarned = false;
                if (m_pUIDVisuals == nullptr)
                {
                    if (!bWarned)
                    {
                        WARNING_MSG("Window", "UID visuals not set.")
                        bWarned = true;
                    }
                    goto DomDev;
                })
                
            m_pUIDVisuals->draw(m_nFramePosX, m_nFramePosY, m_UID.getValue());
            
            DOM_DEV(DomDev:)
            
            m_Graphics.setColor(0.7, 0.3, 0.3, 1.0);
            // Draw close button area
            if (m_bClosable)
            {
                m_Graphics.filledRect(Vector2d(m_nFramePosX+m_nFrameWidth-m_nSizeClose, m_nFramePosY),
                                      Vector2d(m_nFramePosX+m_nFrameWidth,m_nFramePosY+m_nSizeClose));
            }
            
            // Draw resize button area
            m_Graphics.filledRect(Vector2d(m_nFramePosX+m_nFrameWidth-m_nSizeResize,
                                           m_nFramePosY+m_nFrameHeight-m_nSizeResize),
                                  Vector2d(m_nFramePosX+m_nFrameWidth, m_nFramePosY+m_nFrameHeight));
            
            m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
        m_Graphics.endRenderBatch();
        
        m_Graphics.beginRenderBatch(GRAPHICS_SHADER_MODE_FONT);
            Title.setPosition(m_nFramePosX + m_nFrameWidth/2, m_nFramePosY, true);
            Title.display();
        m_Graphics.endRenderBatch();
        
        if (m_pWidget != nullptr) m_pWidget->draw();
    }
}
