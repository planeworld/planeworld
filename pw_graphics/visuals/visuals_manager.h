/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
#include "engine_common.h"
#include "debris_visuals.h"
#include "object_visuals.h"
#include "universe.h"

const double VISUALS_DEFAULT_FREQUENCY = 30.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsManager : virtual public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CVisualsManager();
        ~CVisualsManager();
        
        //--- Constant Methods -----------------------------------------------//
        void            drawBoundingBoxes() const;
        void            drawGrid() const;
        void            drawGridHUD() const;
        void            drawWorld() const;
        void            finishFrame() const;
        const double    getFrequency() const;
        const bool      getVisualisation(const int&) const;
        const int       getVisualisations() const;
        bool            initGraphics() const;
                
        //--- Methods --------------------------------------------------------//
        void            addVisuals(CDebrisVisuals*);
        void            addVisuals(IObjectVisuals* const);
        void            addVisualsList(const std::vector<IObjectVisuals*>&);
        void            setCamera(CCamera*);
        void            setUniverse(CUniverse* const);
        void            setVisualisations(const int&);
        void            setWindow(sf::Window* const);
        void            toggleVisualisations(const int&);
        void            unsetVisualisations(const int&);

    private:

        CUniverse*                      m_pUniverse;        ///< Procedurally generated universe
        double                          m_fFrequency;       ///< Frequency of visuals update
        int                             m_nVisualisations;  ///< Additional graphical output
        std::vector<IObjectVisuals*>    m_ObjectVisuals;    ///< List of object visuals
        std::list<CDebrisVisuals*>      m_DebrisVisuals;    ///< List of debris visuals
        CCamera*                        m_pCamera;          ///< Camera for player view
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Drawing finished, now swap buffers
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::finishFrame() const
{
    METHOD_ENTRY("CVisualsManager::finishFrame")
    m_Graphics.swapBuffers();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of visuals update
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double CVisualsManager::getFrequency() const
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
inline const int CVisualsManager::getVisualisations() const
{
    METHOD_ENTRY("CVisualsManager::getVisualisations")
    return (m_nVisualisations);
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
/// \brief Add debris visuals to list
///
/// \param _pV Debris visuals
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::addVisuals(CDebrisVisuals* _pDebris)
{
    METHOD_ENTRY("CVisualsManager::addVisuals")
    m_DebrisVisuals.push_back(_pDebris);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets camera for visualisation
///
/// \param _pCamera Camera
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setCamera(CCamera* _pCamera)
{
    METHOD_ENTRY("CVisualsManager::setCamera")
    m_pCamera = _pCamera;
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
inline void CVisualsManager::setWindow(sf::Window* const _pWindow)
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

#endif
