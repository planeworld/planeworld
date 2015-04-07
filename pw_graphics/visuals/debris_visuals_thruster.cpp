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
/// \file       debris_visuals_thruster.cpp
/// \brief      Implementation of class "CDebrisVisualsThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-03
///
////////////////////////////////////////////////////////////////////////////////

#include "debris_visuals_thruster.h"

//--- Program header ---------------------------------------------------------//
#include "graphics.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pDebris Debris to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisualsThruster::CDebrisVisualsThruster(CDebris* _pDebris): m_pDebris(_pDebris)
{
    METHOD_ENTRY("CDebrisVisualsThruster::CDebrisVisualsThruster")
    CTOR_CALL("CDebrisVisualsThruster::CDebrisVisualsThruster")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisVisualsThruster::~CDebrisVisualsThruster()
{
    METHOD_ENTRY("CDebrisVisualsThruster::~CDebrisVisualsThruster()");
    DTOR_CALL("CDebrisVisualsThruster::~CDebrisVisualsThruster()");
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
void CDebrisVisualsThruster::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CDebrisVisualsThruster::draw()");
    double fSizeR = 1.0 / m_pDebris->getPositions()->size();
    u_int32_t i = 0;
    
    for (boost::circular_buffer<Vector2d>::const_iterator ci = m_pDebris->getPositions()->begin();
                                                          ci != m_pDebris->getPositions()->end(); ++ci)
    {
        m_Graphics.setPointSize((double(m_pDebris->getPositions()->size()-i) * 0.05 + 3.0) * _pCamera->getZoom());
        m_Graphics.setColor(std::sqrt(fSizeR * i), fSizeR * i, fSizeR * i * 0.2, 0.05);
        m_Graphics.dot((*ci) - _pCamera->getCenter() +
                        IUniverseScaled::cellToDouble(m_pDebris->getCell() - _pCamera->getCell()));
        ++i;
    }
    m_Graphics.setPointSize(1.0);
}
