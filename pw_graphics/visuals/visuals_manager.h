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
#include "universe.h"
#include "world_data_storage_user.h"

const double VISUALS_DEFAULT_FREQUENCY = 60.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation
///
////////////////////////////////////////////////////////////////////////////////
class CVisualsManager : virtual public CGraphicsBase, public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CVisualsManager();
        ~CVisualsManager();
        
        //--- Constant Methods -----------------------------------------------//
        void            drawBoundingBoxes() const;
        void            drawGrid() const;
        void            drawGridHUD() const;
        void            drawTrajectories() const;
        void            drawWorld();
        void            finishFrame() const;
        const double    getFrequency() const;
        const bool      getVisualisation(const int&) const;
        const int       getVisualisations() const;
        WindowHandleType* const
                        getWindowHandle() const;
        bool            initGraphics() const;
                
        //--- Methods --------------------------------------------------------//
        void            setCamera(CCamera*);
        void            setDataPath(const std::string&);
        void            setFrequency(const double&);
        void            setUniverse(CUniverse* const);
        void            setVisualisations(const int&);
        void            setWindow(WindowHandleType* const);
        void            toggleVisualisations(const int&);
        void            unsetVisualisations(const int&);

    private:

        CUniverse*                      m_pUniverse;        ///< Procedurally generated universe
        double                          m_fFrequency;       ///< Frequency of visuals update
        int                             m_nVisualisations;  ///< Additional graphical output
        int                             m_nStarIndex;       ///< Indicates procedurally generated star
        CCamera*                        m_pCamera;          ///< Camera for player view
        
        std::string                     m_strDataPath;      ///< Data path for resources
        sf::Font                        m_Font;             ///< Font for displayed output
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
/// \brief Returns Window handle
///
/// \return Window handle
///
////////////////////////////////////////////////////////////////////////////////
inline WindowHandleType* const CVisualsManager::getWindowHandle() const
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
/// \brief Sets data path, to correctly load font
///
/// \param _strDataPath
///
////////////////////////////////////////////////////////////////////////////////
inline void CVisualsManager::setDataPath(const std::string& _strDataPath)
{
    METHOD_ENTRY("CVisualsManager::setDataPath")
    m_strDataPath = _strDataPath;
    
    if (!m_Font.loadFromFile(m_strDataPath+"/fonts/consola.ttf"))
    {
        WARNING_MSG("Visuals Manager", "Couldn't find font file " << m_strDataPath <<
                                       "/fonts/consola.ttf.")
    }
    else
    {
        DOM_FIO(INFO_MSG("Visuals Manager", "Font " << m_strDataPath << "/fonts/consola.ttf"
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

#endif
