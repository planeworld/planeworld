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
#include <unordered_map>

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "com_console.h"
#include "com_interface.h"
#include "widget.h"
#include "window.h"

/// Map of cameras, accessed by name
typedef std::unordered_map<std::string, CCamera*> CamerasByNameType;
/// Vector of cameras, accessed by index to cycle through cameras
typedef std::vector<CCamera*> CamerasByIndexType;

/// Map of window frame users, accessed by UID value
typedef std::unordered_map<UIDType, IWinFrameUser*> WinFrameUsersByValueType;
/// Map of widgets, accessed by UID value
typedef std::unordered_map<UIDType, IWidget*> WidgetsByValueType;
/// Map of windows, accessed by UID value
typedef std::unordered_map<UIDType, CWindow*> WindowsByValueType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class that stores all visuals data
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsDataStorage : public IComInterfaceUser
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CVisualsDataStorage();
        ~CVisualsDataStorage();
        
        //--- Constant Methods -----------------------------------------------//
        const CamerasByIndexType&   getCamerasByIndex() const;
        const CamerasByNameType&    getCamerasByName() const;
        CComConsole* const          getComConsole() const;
        IWidget* const              getWidgetByValue(const UIDType) const;
        CWindow* const              getWindowByValue(const UIDType) const;
        const WidgetsByValueType*   getWidgetsByValue() const;
        const WindowsByValueType*   getWindowsByValue() const;
        
        //--- Methods --------------------------------------------------------//
        void addCamera(CCamera*);
        void addWidget(IWidget*);
        void addWindow(CWindow*);
        
        bool closeWindow(const UIDType);
        
        UIDType createWidget(const WidgetTypeType);
        UIDType createWindow();
        
        void setFont(const sf::Font& _Font) {m_Font = _Font;}

        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CVisualsDataStorage&);
        friend std::ostream& operator<<(std::ostream&, CVisualsDataStorage&);
        
    private:
      
        CamerasByIndexType          m_CamerasByIndex;           ///< Cameras, accessed by index
        CamerasByNameType           m_CamerasByName;            ///< Cameras, accessed by name
        WidgetsByValueType          m_WidgetsByValue;           ///< Widgets, accessed by value
        WindowsByValueType          m_WindowsByValue;           ///< Windows, accessed by value
        WinFrameUsersByValueType    m_WinFrameUsersByValue;     ///< Entities using a window frame, accessed by value
        
        CComConsole*                m_pComConsole;              ///< Com console for command input
        sf::Font                    m_Font;                     ///< Currently used font for visuals
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns cameras.
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
/// \brief Returns cameras.
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
/// \brief Returns com console.
///
/// \return Com console
///
////////////////////////////////////////////////////////////////////////////////
inline CComConsole* const CVisualsDataStorage::getComConsole() const
{
    METHOD_ENTRY("CVisualsDataStorage::getComConsole")
    return m_pComConsole;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return widget, accessed by given UID value
///
/// \param _nUID UID of widget to return
///
/// \return Widget with given UID value
///
////////////////////////////////////////////////////////////////////////////////
inline IWidget* const CVisualsDataStorage::getWidgetByValue(const UIDType _nUID) const
{
    METHOD_ENTRY("CVisualsDataStorage::getWidgetByValue")
    
    const auto ci = m_WidgetsByValue.find(_nUID);
    if (ci != m_WidgetsByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("Visuals Data Storage", "Unknown widget with UID <" << _nUID << ">")
        return nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return window, accessed by given UID value
///
/// \param _nUID UID of window to return
///
/// \return Window with given UID value
///
////////////////////////////////////////////////////////////////////////////////
inline CWindow* const CVisualsDataStorage::getWindowByValue(const UIDType _nUID) const
{
    METHOD_ENTRY("CVisualsDataStorage::getWindowByValue")
    
    const auto ci = m_WindowsByValue.find(_nUID);
    if (ci != m_WindowsByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("Visuals Data Storage", "Unknown window with UID <" << _nUID << ">")
        return nullptr;
    }
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


#endif // VISUALS_DATA_STORAGE_H
