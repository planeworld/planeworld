////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2017 Torsten Büschenfeld
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
/// \file       visuals_data_storage.h
/// \brief      Prototype of class "CVisualsDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-28
///
////////////////////////////////////////////////////////////////////////////////

#ifndef VISUALS_DATA_STORAGE_H
#define VISUALS_DATA_STORAGE_H

//--- Standard header --------------------------------------------------------//
#include <list>
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "com_console.h"
#include "com_interface.h"
#include "spinlock.h"
#include "uid_visuals_user.h"
#include "widget.h"
#include "window.h"

class CWidgetCam;

/// Map of cameras, accessed by name
typedef std::unordered_map<std::string, CCamera*> CamerasByNameType;
/// Map of cameras, accessed by UID value
typedef std::unordered_map<UIDType, CCamera*> CamerasByValueType;
/// Vector of cameras, accessed by index to cycle through cameras
typedef std::vector<CCamera*> CamerasByIndexType;

/// Map of window frame users, accessed by UID value
typedef std::unordered_map<UIDType, IWinFrameUser*> WinFrameUsersByValueType;
/// Map of widgets, accessed by UID value
typedef std::unordered_map<UIDType, IWidget*> WidgetsByValueType;
/// Map of windows, accessed by UID value
typedef std::unordered_map<UIDType, CWindow*> WindowsByValueType;
/// Vector of camera widgets, accessed by index to cycle through active cameras
typedef std::unordered_map<UIDType, CWidgetCam*> CameraWidgetsByValueType;
/// Queue for display order of windows
typedef std::list<UIDType> WindowOrderType;

/// Concurrent queue of widgets
typedef moodycamel::ConcurrentQueue<CCamera*> CamerasQueueType;
/// Concurrent queue of widgets
typedef moodycamel::ConcurrentQueue<IWidget*> WidgetsQueueType;
/// Concurrent queue of windows
typedef moodycamel::ConcurrentQueue<CWindow*> WindowsQueueType;

const bool WIN_KEEP_CENTERED = true; ///< Indicates that a window should be kept centered when resizing the window

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that stores all visuals data
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsDataStorage : public IComInterfaceUser,
                            public IUIDVisualsUser
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CVisualsDataStorage();
        ~CVisualsDataStorage();
        
        //--- Constant Methods -----------------------------------------------//
        const CamerasByIndexType&       getCamerasByIndex() const;
        const CamerasByNameType&        getCamerasByName() const;
        const CamerasByValueType&       getCamerasByValue() const;
        const CameraWidgetsByValueType& getCameraWidgets() const {return m_CameraWidgetsByValue;}
        CComConsole*                    getComConsole() const;
        IWidget*                        getWidgetByValue(const UIDType) const;
        CWindow*                        getWindowByValue(const UIDType) const;
        const WidgetsByValueType*       getWidgetsByValue() const;
        const WindowsByValueType*       getWindowsByValue() const;
        const WindowsByValueType*       getWindowsCenteredByValue() const;
        
        //--- Methods --------------------------------------------------------//
        void addCamera(CCamera*);
        void addWidget(IWidget*);
        void addWindow(CWindow*);
        
        void centerWindow(CWindow*, const bool = false);
        void releaseCenteredWindow(const UIDType);
        
        bool closeWindow(const UIDType);

        CCamera*         getCameraByValue(const UIDType);
        WindowOrderType* getWindowUIDsInOrder() {return &m_WindowsOrder;}

        void setFontManager(CFontManager* const _pFontManager) {m_pFontManager = _pFontManager;}

        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CVisualsDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CVisualsDataStorage&);
        
        //--- Variables ------------------------------------------------------//
        CSpinlock AccessCameras;
        
    private:
      
        CamerasByIndexType          m_CamerasByIndex;           ///< Cameras, accessed by index
        CamerasByNameType           m_CamerasByName;            ///< Cameras, accessed by name
        CamerasByValueType          m_CamerasByValue;           ///< Cameras, accessed by value
        CameraWidgetsByValueType    m_CameraWidgetsByValue;     ///< Camera widgets, accessed by value
        WidgetsByValueType          m_WidgetsByValue;           ///< Widgets, accessed by value
        WindowsByValueType          m_WindowsByValue;           ///< Windows, accessed by value
        WindowsByValueType          m_WindowsCenteredByValue;   ///< Windows, that are centered on screen
        WinFrameUsersByValueType    m_WinFrameUsersByValue;     ///< Entities using a window frame, accessed by value
        WindowOrderType             m_WindowsOrder;             ///< Display order of windows
        
        CComConsole*                m_pComConsole;              ///< Com console for command input
        CFontManager*               m_pFontManager;             ///< Font handling instance
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras, accessed by linear index
///
/// \return Cameras
///
////////////////////////////////////////////////////////////////////////////////
inline const CamerasByIndexType& CVisualsDataStorage::getCamerasByIndex() const
{
    METHOD_ENTRY("CVisualsDataStorage::getCamerasByIndex")
    return m_CamerasByIndex;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras, accessed by name
///
/// \return Cameras
///
////////////////////////////////////////////////////////////////////////////////
inline const CamerasByNameType& CVisualsDataStorage::getCamerasByName() const
{
    METHOD_ENTRY("CVisualsDataStorage::getCamerasByName")
    return m_CamerasByName;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras, accessed by UID value
///
/// \return Cameras
///
////////////////////////////////////////////////////////////////////////////////
inline const CamerasByValueType& CVisualsDataStorage::getCamerasByValue() const
{
    METHOD_ENTRY("CVisualsDataStorage::getCamerasByValue")
    return m_CamerasByValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns com console.
///
/// \return Com console
///
////////////////////////////////////////////////////////////////////////////////
inline CComConsole* CVisualsDataStorage::getComConsole() const
{
    METHOD_ENTRY("CVisualsDataStorage::getComConsole")
    return m_pComConsole;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns widgets, accessable by value
///
/// \return Widgets
///
////////////////////////////////////////////////////////////////////////////////
inline const WidgetsByValueType* CVisualsDataStorage::getWidgetsByValue() const
{
    METHOD_ENTRY("CVisualsDataStorage::getWidgetsByValue")
    return &m_WidgetsByValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns windows, accessable by value
///
/// \return Windows
///
////////////////////////////////////////////////////////////////////////////////
inline const WindowsByValueType* CVisualsDataStorage::getWindowsByValue() const
{
    METHOD_ENTRY("CVisualsDataStorage::getWindowsByValue")
    return &m_WindowsByValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns windows that are centered on screen, accessable by value
///
/// \return Windows centered on screen
///
////////////////////////////////////////////////////////////////////////////////
inline const WindowsByValueType* CVisualsDataStorage::getWindowsCenteredByValue() const
{
    METHOD_ENTRY("CVisualsDataStorage::getWindowsCenteredByValue")
    return &m_WindowsCenteredByValue;
}

#endif // VISUALS_DATA_STORAGE_H
