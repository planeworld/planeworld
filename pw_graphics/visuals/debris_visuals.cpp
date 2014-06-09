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
/// \file       debris_visuals.cpp
/// \brief      Implementation of class "CDebrisVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-10
///
////////////////////////////////////////////////////////////////////////////////

#include "debris_visuals.h"

//--- Program header ---------------------------------------------------------//
#include "graphics.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pDebris Debris to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisuals::CDebrisVisuals(CDebris* _pDebris): m_pDebris(_pDebris)
{
    METHOD_ENTRY("CDebrisVisuals::CDebrisVisuals")
    CTOR_CALL("CDebrisVisuals::CDebrisVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisuals::~CDebrisVisuals()
{
    METHOD_ENTRY("CDebrisVisuals::~CDebrisVisuals()");
    DTOR_CALL("CDebrisVisuals::~CDebrisVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Debris
///
/// \param _pCamera Active camera for drawing visuals
///
/// \todo Filter by debris state. Don't draw inactive debris.
/// \todo Single calling is not very performant (see commented call of "dots"
///       for better reference).
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CDebrisVisuals::draw()");
    double fSizeR = 1.0 / m_pDebris->getPositions()->size();
    u_int32_t i = 0;
    for (boost::circular_buffer<Vector2d>::const_iterator ci = m_pDebris->getPositions()->begin();
                                                          ci != m_pDebris->getPositions()->end(); ++ci)
    {
        m_Graphics.setColor(std::sqrt(fSizeR * i), fSizeR * i, fSizeR * i * 0.2);
        m_Graphics.dot((*ci) - _pCamera->getCenter() +
                        IUniverseScaled::cellToDouble(m_pDebris->getCell() - _pCamera->getCell()));
        ++i;
    }
//     m_Graphics.dots((*m_pDebris->getPositions()),-_pCamera->getCenter()+
//                       IUniverseScaled::cellToDouble(m_pDebris->getCell() - _pCamera->getCell()));
}
