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
/// \file       visuals__data_storage.cpp
/// \brief      Implementation of class "CVisualsDataStorage"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-28
///
////////////////////////////////////////////////////////////////////////////////

#include "visuals_data_storage.h"

#include "widget_console.h"
#include "widget_text.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CVisualsDataStorage::CVisualsDataStorage() 
//                                         : m_pCamera(nullptr)
{
    METHOD_ENTRY("CVisualsDataStorage::CVisualsDataStorage")
    CTOR_CALL("CVisualsDataStorage::CVisualsDataStorage")
    
    m_pComConsole = new CComConsole();
    MEM_ALLOC("CComConsole")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CVisualsDataStorage::~CVisualsDataStorage()
{
    METHOD_ENTRY("CVisualsDataStorage::~CVisualsDataStorage")
    DTOR_CALL("CVisualsDataStorage::~CVisualsDataStorage")
    
    if (m_pComConsole != nullptr)
    {
        delete m_pComConsole;
        MEM_FREED("CComConsole")
        m_pComConsole = nullptr;
    }
    
    for (auto pCam : m_CamerasByName)
    {
        if (pCam.second != nullptr)
        {
            delete pCam.second;
            pCam.second = nullptr;
            MEM_FREED("CCamera")
        }
    }
    
    // Delete windows, this will delete widget, too
    for (auto Win : m_WindowsByValue)
    {
        if (Win.second != nullptr)
        {
            delete Win.second;
            Win.second = nullptr;
            MEM_FREED("CWindow")
        }
    }
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
IWidget* CVisualsDataStorage::getWidgetByValue(const UIDType _nUID) const
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
CWindow* CVisualsDataStorage::getWindowByValue(const UIDType _nUID) const
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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a camera
///
/// This method adds the given camera to the list of available cameras
///
/// \param _pCamera Camera to be added
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addCamera(CCamera* _pCamera)
{
    METHOD_ENTRY("CVisualsDataStorage::addCamera")
    
    m_CamerasByName.insert({_pCamera->getName(), _pCamera});
    m_CamerasByIndex.push_back(_pCamera);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a window to data storage
///
/// \note Use this method to add a window to the storage that has already been
///       created elsewhere. Otherwise, use
///       \ref CVisualsDataStorage::createWindow. 
///
/// \param _pWindow Window to be added
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addWindow(CWindow* _pWindow)
{
    METHOD_ENTRY("CVisualsDataStorage::addWindow")
    m_WindowsByValue.insert({_pWindow->getUID(), _pWindow});
    m_WinFrameUsersByValue.insert({_pWindow->getUID(), _pWindow});
    m_WindowsOrder.push_back(_pWindow->getUID());
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a widget to data storage
///
/// \note Use this method to add a widget to the storage that has already been
///       created elsewhere. Otherwise, use
///       \ref CVisualsDataStorage::createWidget.
///
/// \param _pWidget Widget to be added
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addWidget(IWidget* _pWidget)
{
    METHOD_ENTRY("CVisualsDataStorage::addWidget")
    m_WidgetsByValue.insert({_pWidget->getUID(), _pWidget});
    m_WinFrameUsersByValue.insert({_pWidget->getUID(), _pWidget});
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add widgets from queue to storage
///
/// This method will add the widgets from queue (see
/// \ref CVisualsDataStorage::addWidget) to storage.
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addWidgetsFromQueue()
{
    METHOD_ENTRY("CVisualsDataStorage::addWidgetsFromQueue")
    
    IWidget* pWidget = nullptr;
    while (m_WidgetsQueue.try_dequeue(pWidget))
    {
        this->addWidget(pWidget);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add windows from queue to storage
///
/// This method will add the windows from queue (see
/// \ref CVisualsDataStorage::addWindow) to storage.
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::addWindowsFromQueue()
{
    METHOD_ENTRY("CVisualsDataStorage::addWindowsFromQueue")
    
    CWindow* pWindow = nullptr;
    while (m_WindowsQueue.try_dequeue(pWindow))
    {
        this->addWindow(pWindow);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Close window with given UID
///
/// This method removes the given window from the internal list and deletes it.
///
/// \param _nUID UID of window to be removed
/// \return Success?
///
///////////////////////////////////////////////////////////////////////////////
bool CVisualsDataStorage::closeWindow(const UIDType _nUID)
{
    METHOD_ENTRY("CVisualsDataStorage::closeWindow")

    for (auto it2 = m_WindowsOrder.begin(); it2 != m_WindowsOrder.end(); ++it2)
    {
        if (*it2 == _nUID)
        {
            m_WindowsOrder.erase(it2);
            break;
        }
    }
    
    auto it = m_WindowsByValue.find(_nUID);
    if (it != m_WindowsByValue.end())
    {
        delete it->second;
        MEM_FREED("CWindow")
        m_WindowsByValue.erase(_nUID);
        return true;
    }
    else
    {
        WARNING_MSG("Visuals Data Storage", "Unknown window with UID <" << _nUID << ">")
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a widget of the given type
///
/// This method creates a widget and adds it to storage. In
/// \ref CreationModeType::QUEUED, it will be moved to a waiter queue and later
/// be added to storage. The reason for this lies in multithreading and return
/// value retrieval. When a widget is created, the UID has to be returned.
/// Hence, it is defined as a reader command (see \ref CComInterface). The
/// newly created widget is stored temporary in a concurrent queue and later
/// added to storage by the appropriate thread.
///
/// \param _WidgetType Type of widget to be created
/// \param _pMode Mode of creation (direct to storage or threading queue)
///
/// \return UID of newly created window
///
///////////////////////////////////////////////////////////////////////////////
UIDType CVisualsDataStorage::createWidget(const WidgetTypeType _WidgetType,
                                          const CreationModeType _pMode)
{
    METHOD_ENTRY("CVisualsDataStorage::createWidget")
    
    IWidget* pWidget = nullptr;
    
    switch (_WidgetType)
    {
        case WidgetTypeType::CONSOLE:
        {
            CWidgetConsole* pConsoleWidget = new CWidgetConsole();
            pConsoleWidget->setUIDVisuals(m_pUIDVisuals);
            pConsoleWidget->setComConsole(m_pComConsole);
            m_pComConsole->setComInterface(m_pComInterface);
            pWidget = pConsoleWidget;
            break;
        }
        case WidgetTypeType::TEXT:
        {
            CWidgetText* pTextWidget = new CWidgetText();
            pTextWidget->setUIDVisuals(m_pUIDVisuals);
            pWidget = pTextWidget;
            break;
        }
        default:
        {
            WARNING_MSG("Visuals Data Storage", "Unknown widget type.")
            break;
        }
    }
    MEM_ALLOC("IWidget")

    if (pWidget != nullptr)
    {
        pWidget->setFont(&m_Font);
        
        if (_pMode == CreationModeType::DIRECT)
        {
            this->addWidget(pWidget);
        }
        else
        {
            m_WidgetsQueue.try_enqueue(pWidget);
        }
    
        return pWidget->getUID();
    }
    else
    {
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Creates a generic window
///
/// This method creates a window and adds it to storage. In
/// \ref CreationModeType::QUEUED, it will be moved to a waiter queue and later
/// be added to storage. The reason for this lies in multithreading and return
/// value retrieval. When a window is created, the UID has to be returned.
/// Hence, it is defined as a reader command (see \ref CComInterface). The
/// newly created window is stored temporary in a concurrent queue and later
/// added to storage by the appropriate thread.
///
/// \param _pMode Mode of creation (direct to storage or threading queue)
///
/// \return UID of newly created window
///
///////////////////////////////////////////////////////////////////////////////
UIDType CVisualsDataStorage::createWindow(const CreationModeType _pMode)
{
    METHOD_ENTRY("CVisualsDataStorage::addWidget")
    
    CWindow* pWin = new CWindow();
    MEM_ALLOC("CWindow")
    
    pWin->setUIDVisuals(m_pUIDVisuals);
    pWin->setFont(&m_Font);
    if (_pMode == CreationModeType::DIRECT)
    {
        this->addWindow(pWin);
    }
    else
    {
        m_WindowsQueue.try_enqueue(pWin);
    }
    
    return pWin->getUID();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _WDS CVisualsDataStorage instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CVisualsDataStorage& _WDS)
{
    METHOD_ENTRY("CVisualsDataStorage::operator>>")
    
//     // Temporary reference to shape by UID to connect visuals with their shapes,
//     // after sequentually loading shapes and then visuals.
//     std::unordered_map<UIDType, IShape*> UIDShapeRef;
    
//     for (auto it : _WDS.m_DebrisVisuals)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CDebrisVisuals")
//         }
//     }
//     _WDS.m_DebrisVisuals.clear();
    
    // Thruster Visuals!!!
    
//     _WDS.m_UIDUserRef.clear();
    
//     //-------------------------------------------------------------------------
//     // Stream in camera
//     //-------------------------------------------------------------------------
//     {
//         if (_WDS.m_pCamera != nullptr)
//         {
//             delete _WDS.m_pCamera;
//             MEM_FREED("CCamera")
//             _WDS.m_pCamera = nullptr;
//         }
//         _WDS.m_pCamera = new CCamera;
//         MEM_ALLOC("CCamera")
//         _is >> _WDS.m_pCamera;
// 
//         /// \todo Clean up camera hooks via kinematics states and uids.
//         if (_WDS.m_pCamera->getKinematicsState().gotReference())
//         {
//             UIDType UID = _WDS.m_pCamera->getKinematicsState().getUIDRef();
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             _WDS.m_pCamera->hook(static_cast<IKinematicsStateUser*>(static_cast<CObject*>(pUIDUser)));
//         }
//     }
    

//     //-------------------------------------------------------------------------
//     // Stream in debris visuals
//     //-------------------------------------------------------------------------
//     {
//         DebrisVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of debris visuals to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CDebrisVisuals* pDebVis = new CDebrisVisuals;
//             MEM_ALLOC("CDebrisVisuals")
//             _is >> pDebVis;
//             
//             UIDType UID = pDebVis->getUIDRef();
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pDebVis->attachTo(static_cast<CDebris*>(pUIDUser));
//             _WDS.addDebrisVisuals(pDebVis);
//             
//             Log.progressBar("Loading debris visuals", i, nSize);
//         }
//     }

    /// \todo Implement DebrisEmitter serialisation to make debris work
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _WDS CVisualsDataStorage instance to stream
///
/// \return Stream with game state information of CVisualsDataStorage instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CVisualsDataStorage& _WDS)
{
    METHOD_ENTRY("CVisualsDataStorage::operator<<")

    /// \todo Implement DebrisEmitter serialisation to make debris work
    return _os;
}
