////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
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
#include "physics_manager.h"
#include "universe.h"
#include "visuals_data_storage_user.h"
#include "world_data_storage_user.h"

const double CIRCLE_DEFAULT_RESOLUTION =  5.0;               ///< Default resolution for visual sampling, px/vertex.
const double CIRCLE_MINIMUM_SEGMENTS = 10.0;                 ///< Minimum number of circle segments
const double PLANET_VISUALS_DEFAULT_RESOLUTION=3.0;          ///< Default resolution for visual sampling px/vertex
const double PLANET_VISUALS_DEFAULT_MINIMUM_ANGLE=M_PI*0.01; ///< Default minium of 200 segments if above resolution limit
const double VISUALS_DEFAULT_FREQUENCY = 60.0;               ///< Default frequency for graphics update

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsManager : virtual public CGraphicsBase,
                                public IVisualsDataStorageUser,
                                public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CVisualsManager();
        ~CVisualsManager();
        
        //--- Constant Methods -----------------------------------------------//
        CCamera*        getCurrentCamera() const;
        const double&   getFrequency() const;
        bool            getVisualisation(const int&) const;
        int             getVisualisations() const;
        WindowHandleType*
                        getWindowHandle() const;
        bool            initGraphics() const;
                
        //--- Methods --------------------------------------------------------//
        void            cycleCamera();
        
        void            finishFrame();
        void            processFrame();
        
        void            setConsoleText(const std::string&);
        void            setFont(const std::string&);
        void            setFrequency(const double&);
        void            setUniverse(CUniverse* const);
        void            setPhysicsManager(CPhysicsManager* const);
        void            setVisualisations(const int&);
        void            setWindow(WindowHandleType* const);
        void            toggleConsoleMode();
        void            toggleVisualisations(const int&);
        void            unsetVisualisations(const int&);

    private:

        //--- Constant methods [private] -------------------------------------//
        void drawCircle(CObject*, CCircle*, CCamera*) const;
        void drawPlanet(CObject*, CPlanet*, CCamera*) const;
        void drawPolygon(CObject*, CPolygon*, CCamera*) const;
        void drawObjects(CCamera* const) const;
        
        void            drawBoundingBoxes() const;
        void            drawConsole() const;
        void            drawGrid() const;
        void            drawGridHUD() const;
        void            drawKinematicsState(const CKinematicsState&, const double&) const;
        void            drawKinematicsStates() const;
        void            drawTimers() const;
        void            drawTrajectories() const;
        void            drawWorld() const;

        CUniverse*                      m_pUniverse;        ///< Procedurally generated universe
        CPhysicsManager*                m_pPhysicsManager;  ///< Reference to physics
        double                          m_fFrequency;       ///< Frequency of visuals update
        int                             m_nVisualisations;  ///< Additional graphical output
        std::uint32_t                   m_nStarIndex;       ///< Indicates procedurally generated star
        std::uint32_t                   m_unCameraIndex;    ///< Index of currently used camera
        CCamera*                        m_pCamera;          ///< Camera for player view
        
        bool                            m_bConsoleMode;     ///< Indicates if console mode is active
        std::string                     m_strConsoleText;   ///< Console text to be displayed
        std::string                     m_strFont;          ///< Font name and location
        sf::Font                        m_Font;             ///< Font for displayed output
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
/// \brief Returns frequency of visuals update
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double& CVisualsManager::getFrequency() const
{
    METHOD_ENTRY("CVisualsManager::getFrequency")
    return (m_fFrequency);
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
inline WindowHandleType* CVisualsManager::getWindowHandle() const
{
    METHOD_ENTRY("CVisualsManager::getWindowHandle")
    return (m_Graphics.getWindow());
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the Graphics
///
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
inline bool CVisualsManager::initGraphics() const
{
    METHOD_ENTRY("CVisualsManager::getVisualisations")
    return (m_Graphics.init());
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the console text for this frame if console mode is active
///
/// \param _strText Console text for this frame
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setConsoleText(const std::string& _strText)
{
    METHOD_ENTRY("CVisualsManager::setConsoleText")
    if (m_bConsoleMode)
    {
        m_strConsoleText = _strText;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets font
///
/// \param _strFont
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setFont(const std::string& _strFont)
{
    METHOD_ENTRY("CVisualsManager::setFont")
    m_strFont = _strFont;
    
    if (!m_Font.loadFromFile(m_strFont))
    {
        WARNING_MSG("Visuals Manager", "Couldn't find font file " << m_strFont <<
                                       ".")
    }
    else
    {
        DOM_FIO(INFO_MSG("Visuals Manager", "Font " << m_strFont << 
                                            " successfully loaded."))
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the frequency for visual update
///
/// \param _fFrequency Frequency for visual update
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setFrequency(const double& _fFrequency)
{
    METHOD_ENTRY("CVisualsManager::setFrequency")
    m_fFrequency = _fFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets procedurally generated universe for visualisation
///
/// \param _pUniverse Procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setUniverse(CUniverse* const _pUniverse)
{
    METHOD_ENTRY("CVisualsManager::setUniverse")
    m_pUniverse = _pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets reference to physics to access its parameters
///
/// \param _pPhysMan Reference to physics manager
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setPhysicsManager(CPhysicsManager* const _pPhysMan)
{
    METHOD_ENTRY("CVisualsManager::setPhysicsManager")
    m_pPhysicsManager = _pPhysMan;
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
/// \brief Toggles a state of console mode
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::toggleConsoleMode()
{
    METHOD_ENTRY("CVisualsManager::toggleConsoleMode")
    m_bConsoleMode ^= true;
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

#endif
