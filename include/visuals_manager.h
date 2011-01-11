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
#include "bounding_box.h"
#include "engine_common.h"
#include "graphics.h"
#include "key_map.h"
#include "visuals.h"

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
        void            drawBoundingBox(const CBoundingBox&) const;
        void            drawWorld() const;
        const double    getFrequency() const;
        const bool      getVisualisation(const int&) const;
        const int       getVisualisations() const;
        bool            initGraphics() const;
                
        //--- Methods --------------------------------------------------------//
        VisualsIDType       addVisuals(IVisuals*);
        VisualsIDListType   addVisualsList(const VisualsListType&);
        void                setVisualisations(const int&);
        void                toggleVisualisations(const int&);
        void                unsetVisualisations(const int&);

    private:

        double              m_fFrequency;           ///< Frequency of visuals update
        int                 m_nVisualisations;      ///< Additional graphical output
        CKeyMap<IVisuals*>  m_VisualsMap;           ///< Map of visuals and keys
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of visuals update
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double CVisualsManager::getFrequency() const
{
    METHOD_ENTRY("CVisualsManager::getFrequency()")

    METHOD_EXIT("CVisualsManager::getFrequency()")
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

    METHOD_EXIT("CVisualsManager::getVisualisations")
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

    METHOD_EXIT("CVisualsManager::getVisualisations")
    return (m_Graphics.init());
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

    METHOD_EXIT("CVisualsManager::setVisualisations")
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

    METHOD_EXIT("CVisualsManager::toggleVisualisations")
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
    
    METHOD_EXIT("CVisualsManager::unsetVisualisations")
}

#endif
