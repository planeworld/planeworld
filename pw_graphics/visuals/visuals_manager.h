////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2017 Torsten Büschenfeld
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
/// \file       visuals_manager.h
/// \brief      Prototype of class "CVisualsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-05
///
////////////////////////////////////////////////////////////////////////////////

#ifndef VISUALS_MANAGER_H
#define VISUALS_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "circle.h"
#include "com_console.h"
#include "com_interface_provider.h"
#include "font_manager.h"
#include "math_constants.h"
#include "planet.h"
#include "polygon.h"
#include "render_target.h"
#include "thread_module.h"
#include "universe.h"
#include "visuals_data_storage_user.h"
#include "widget_console.h"
#include "widget_text.h"
#include "window.h"
#include "world_data_storage_user.h"

const double CIRCLE_DEFAULT_RESOLUTION =  5.0;               ///< Default resolution for visual sampling, px/vertex.
const double CIRCLE_MINIMUM_SEGMENTS = 10.0;                 ///< Minimum number of circle segments
const double PLANET_VISUALS_DEFAULT_RESOLUTION=3.0;          ///< Default resolution for visual sampling px/vertex
const double PLANET_VISUALS_DEFAULT_MINIMUM_ANGLE=MATH_PI*0.01; ///< Default minium of 200 segments if above resolution limit

/// Specifies the type of creation mode. When directly created, entities are
/// are added to storage directly which should only be done within the same thread
enum class CreationModeType
{
    DIRECT,
    QUEUED
};

/// Specifies type of drawing operation. Visuals might be drawn independently
/// from text due to different draw spacen (i.e. screen space or world space).
enum class DrawModeType
{
    TEXT,
    VISUALS
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsManager : virtual public CGraphicsBase,
                                public IComInterfaceProvider,
                                public IThreadModule,
                                public IVisualsDataStorageUser,
                                public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CVisualsManager();
        ~CVisualsManager();
        
        //--- Constant Methods -----------------------------------------------//
        CCamera*        getCurrentCamera() const;
        bool            getVisualisation(const int&) const;
        int             getVisualisations() const;
        WindowHandleType*
                        getWindow() const;
                
        //--- Methods --------------------------------------------------------//
        bool            init();
        
        UIDType         createCamera(const CreationModeType = CreationModeType::DIRECT);
        UIDType         createWidget(const WidgetTypeType, const CreationModeType = CreationModeType::DIRECT);
        UIDType         createWindow(const CreationModeType = CreationModeType::DIRECT);
        
        void            cycleCamera();

        void            finishFrame();
        bool            processFrame();
        
        void            setDataPath(const std::string&);
        void            setUniverse(CUniverse* const);
        void            setVisualisations(const int&);
        void            setWindow(WindowHandleType* const);
        void            toggleVisualisations(const int&);
        void            unsetVisualisations(const int&);
        
    private:

        //--- Constant methods [private] -------------------------------------//
        void drawCircle(const Vector2i&, const Vector2d&, const double&, CCamera*) const;
        void drawCircle(CObject*, CCircle*, CCamera*) const;
        void drawPlanet(CObject*, CPlanet*, CCamera*) const;
        void drawPolygon(CObject*, CPolygon*, CCamera*) const;

        void drawParticles(CCamera* const) const;
        void drawObjects(CCamera* const) const;
        void drawObjectsPlanetsAtmospheres(CCamera* const) const;
        
        void            drawBoundingBoxes() const;
        void            drawCOM() const;
        void            drawTrajectories() const;
        
        #ifdef PW_MULTITHREADING
            void        preRun();
        #endif
        
        //--- Methods [private] ----------------------------------------------//
        void            addCamerasFromQueue();
        void            addWidgetsFromQueue();
        void            addWindowsFromQueue();
            
        void            drawDebugInfo();
        void            drawGrid(const DrawModeType);
        void            drawKinematicsState(const CKinematicsState* const, const double&, const DrawModeType);
        void            drawKinematicsStates(const DrawModeType);
        void            drawStars();
        void            drawTimers();
        void            drawWindows();
        void            drawWorld();
        void            updateUI();
        
        double          smoothFrameTime(CCircularBuffer<double>* const, const double&, const int);
                
        void            myInitComInterface();

        CShaderProgram      m_ShaderProgramCamWidget;       ///< Shader program for camera widget rendering
        CShaderProgram      m_ShaderProgramFont;            ///< Shader program for font rendering
        CShaderProgram      m_ShaderProgramMainScreen;      ///< Shader program to put image to screen buffer
        CShaderProgram      m_ShaderProgramStars;           ///< Shader program to render stars
        CShaderProgram      m_ShaderProgramWorld;           ///< Basic shader program
        CRenderMode         m_RenderModeFont;               ///< Mode for font rendering
        CRenderMode         m_RenderModeMainScreen;         ///< Mode for rendering texture to screen
        CRenderMode         m_RenderModeStars;              ///< Mode for rendering stars
        CRenderMode         m_RenderModeWorld;              ///< Default render mode
        CRenderTarget       m_RenderTargetScreen;           ///< Render target of screen (final image)
        
        bool                            m_bFullscreen;      ///< Indicates fullscreen mode
        int                             m_nMainWinHeight;   ///< Height of window if not in fullscreen mode
        int                             m_nMainWinWidth;    ///< Width of window if not in fullscreen mode
        
        int                             m_nVisualisations;  ///< Additional graphical output
        std::uint32_t                   m_nStarIndex;       ///< Indicates procedurally generated star
        std::uint32_t                   m_unCameraIndex;    ///< Index of currently used camera
        CCamera*                        m_pCamera;          ///< Camera for player view
        
        int                             m_nCursorX;         ///< Mouse cursor position x
        int                             m_nCursorY;         ///< Mouse cursor position y
        int                             m_nCursorX0;        ///< Mouse cursor position x, previous frame
        int                             m_nCursorY0;        ///< Mouse cursor position y, previous frame
        int                             m_nCursorOffsetX;   ///< Position x of cursor when MB left was first pressed
        int                             m_nCursorOffsetY;   ///< Position y of cursor when MB left was first pressed
        bool                            m_bCursor;          ///< Indicates if mouse cursor is enabled
        bool                            m_bMBLeft;          ///< Left mouse button
        
        CSpinlock                       m_CamCreationLock;
        
        CamerasQueueType                m_CamerasQueue;     ///< Queue of new cameras to be added to storage
        WidgetsQueueType                m_WidgetsQueue;     ///< Queue of new widgets to be added to storage
        WindowsQueueType                m_WindowsQueue;     ///< Queue of new windows to be added to storage
        
        UIDType                         m_ConsoleWidgetID;  ///< ID of console widget for later access
        UIDType                         m_ConsoleWindowID;  ///< ID of console window for later access
        
        std::string                     m_strConsoleText;   ///< Console text to be displayed
        std::string                     m_strFont;          ///< Font name

        std::string                     m_strDataPath;      ///< Path for data such as fonts and shaders
        
        CFontManager                    m_FontManager;      ///< Font manager for displaying UI text
        CUIDVisuals                     m_UIDVisuals;       ///< Graphical display of UIDs
        CText                           m_TextParticle;       ///< Text object for particle names
        CText                           m_TextDebugInfo;    ///< Text object for debug information
        CText                           m_TextObjects;      ///< Text object for object names
        CText                           m_TextScale;        ///< Text object for universe scale
        CText                           m_TextTimers;       ///< Text object for timers
        CText                           m_TextStarSystems;  ///< Text object for Star Systems
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns currently used camera
///
/// \return Camera currently used
///
////////////////////////////////////////////////////////////////////////////////
inline CCamera* CVisualsManager::getCurrentCamera() const
{
    METHOD_ENTRY("CVisualsManager::getCurrentCamera")
    return (m_pVisualsDataStorage->getCamerasByIndex())[m_unCameraIndex];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns visualisations that are set
///
/// \return Visualisations as bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline int CVisualsManager::getVisualisations() const
{
    METHOD_ENTRY("CVisualsManager::getVisualisations")
    return (m_nVisualisations);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns Window handle
///
/// \return Window handle
///
////////////////////////////////////////////////////////////////////////////////
inline WindowHandleType* CVisualsManager::getWindow() const
{
    METHOD_ENTRY("CVisualsManager::getWindow")
    return (m_Graphics.getWindow());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets data path for graphical data such as fonts and shaders
///
/// \param _strDataPath Base path for graphical data such as fonts and shaders
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setDataPath(const std::string& _strDataPath)
{
    METHOD_ENTRY("CVisualsManager::setDataPath")
    m_strDataPath = _strDataPath;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set visualisations that shall be displayed
///
/// \param _nVis Bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setVisualisations(const int& _nVis)
{
    METHOD_ENTRY("CVisualsManager::setVisualisations")
    m_nVisualisations |= _nVis;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set window to draw on
///
/// \param _pWindow Window to draw on
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setWindow(WindowHandleType* const _pWindow)
{
    METHOD_ENTRY("CVisualsManager::setWindow")
    m_Graphics.setWindow(_pWindow);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Toggles a state of a visualisation
///
/// \param _nVis Bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::toggleVisualisations(const int& _nVis)
{
    METHOD_ENTRY("CVisualsManager::toggleVisualisations")
    m_nVisualisations ^= _nVis;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set visualisations that shall be displayed
///
/// \param _nVis Bit pattern
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::unsetVisualisations(const int& _nVis)
{
    METHOD_ENTRY("CVisualsManager::unsetVisualisations")
    m_nVisualisations &= (~_nVis);
}

#ifdef PW_MULTITHREADING
    inline void CVisualsManager::preRun()
    {
        METHOD_ENTRY("CVisualsManager::preRun")
        m_Graphics.getWindow()->setActive(true);
    }
#endif

#endif
