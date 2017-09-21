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

#include "widget_cam.h"
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
/// \brief Return camera, accessed by given UID value
///
/// \param _nUID UID of camera to return
///
/// \return Camera with given UID value
///
////////////////////////////////////////////////////////////////////////////////
CCamera* CVisualsDataStorage::getCameraByValue(const UIDType _nUID) const
{
    METHOD_ENTRY("CVisualsDataStorage::getCameraByValue")

    const auto ci = m_CamerasByValue.find(_nUID);
    if (ci != m_CamerasByValue.end())
    {
        return ci->second;
    }
    else
    {
        WARNING_MSG("Visuals Data Storage", "Unknown camera with UID <" << _nUID << ">")
        return nullptr;
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
    m_CamerasByValue.insert({_pCamera->getUID(), _pCamera});
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
/// \brief Add a window to list of centered windows
///
/// \param _pWindow Window to be added to list of centered windows
/// \param _bKeep Keep window centered if main screen is resized
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::centerWindow(CWindow* _pWindow, const bool _bKeep)
{
    METHOD_ENTRY("CVisualsDataStorage::centerWindow")
    _pWindow->center();
    if (_bKeep)
    {
        m_WindowsCenteredByValue.insert({_pWindow->getUID(), _pWindow});
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Remove a window from list of centered windows
///
/// \param _nUID UID of window to be removed from list of centered windows
///
///////////////////////////////////////////////////////////////////////////////
void CVisualsDataStorage::releaseCenteredWindow(const UIDType _nUID)
{
    METHOD_ENTRY("CVisualsDataStorage::releaseCenteredWindow")
    auto it = m_WindowsCenteredByValue.find(_nUID);
    if (it != m_WindowsCenteredByValue.end())
    {
        m_WindowsCenteredByValue.erase(it);
    }
    else
    {
        WARNING_MSG("Visuals Data Storage", "Unknown window with UID <" << _nUID << ">")
    }
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
    
    if (_pWidget->getType() == WidgetTypeType::CAMERA)
    {
        m_CameraWidgetsByValue.insert({_pWidget->getUID(), static_cast<CWidgetCam*>(_pWidget)});
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
        IWidget* pWidget = it->second->getWidget();
        if (pWidget != nullptr)
        {
            if (pWidget->getType() == WidgetTypeType::CAMERA)
            {
                DOM_DEV(
                    if (m_CameraWidgetsByValue.find(pWidget->getUID()) ==
                        m_CameraWidgetsByValue.end())
                    {
                        WARNING_MSG("Visuals Data Storage", "Unknown camera widget with UID <" << pWidget->getUID() << ">")
                        goto DomDevCameraWidgets;
                    }
                    m_CameraWidgetsByValue.erase(pWidget->getUID());
                )
                DOM_DEV(DomDevCameraWidgets:)
            }
            DOM_DEV(
                if (m_WidgetsByValue.find(pWidget->getUID()) ==
                    m_WidgetsByValue.end())
                {
                    WARNING_MSG("Visuals Data Storage", "Unknown widget with UID <" << pWidget->getUID() << ">")
                    goto DomDevWidgets;
                }
                m_WidgetsByValue.erase(pWidget->getUID());
            )
            DOM_DEV(DomDevWidgets:)

            // Remove widgets pointers from list. Widget deletion is handeled by
            // window/widget destructors
            
            if (m_WindowsCenteredByValue.find(_nUID) !=
                m_WindowsCenteredByValue.end())
            {
                m_WindowsCenteredByValue.erase(_nUID);
            }
        }
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
    
//     for (auto it : _WDS.m_ParticleVisuals)
//     {
//         if (it != nullptr)
//         {
//             delete it;
//             it = nullptr;
//             MEM_FREED("CParticleVisuals")
//         }
//     }
//     _WDS.m_ParticleVisuals.clear();
    
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
//     // Stream in particle visuals
//     //-------------------------------------------------------------------------
//     {
//         ParticleVisualsType::size_type nSize;
//         _is >> nSize;    
//         DOM_VAR(INFO_MSG("World data storage", "Number of particle visuals to load: " << nSize))
//         for (auto i=0u; i<nSize; ++i)
//         {
//             CParticleVisuals* pDebVis = new CParticleVisuals;
//             MEM_ALLOC("CParticleVisuals")
//             _is >> pDebVis;
//             
//             UIDType UID = pDebVis->getUIDRef();
//             IUniqueIDUser* pUIDUser = _WDS.m_UIDUserRef[UID];
//             pDebVis->attachTo(static_cast<CParticle*>(pUIDUser));
//             _WDS.addParticleVisuals(pDebVis);
//             
//             Log.progressBar("Loading particle visuals", i, nSize);
//         }
//     }

    /// \todo Implement ParticleEmitter serialisation to make particle work
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

    /// \todo Implement ParticleEmitter serialisation to make particle work
    return _os;
}
