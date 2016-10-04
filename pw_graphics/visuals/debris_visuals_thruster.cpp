////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2016 Torsten Büschenfeld
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
CDebrisVisualsThruster::CDebrisVisualsThruster(CDebris* _pDebris)
{
    METHOD_ENTRY("CDebrisVisualsThruster::CDebrisVisualsThruster")
    CTOR_CALL("CDebrisVisualsThruster::CDebrisVisualsThruster")
    this->attachTo(_pDebris);
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
void CDebrisVisualsThruster::draw(CCamera* const _pCamera) const
{
    METHOD_ENTRY("CDebrisVisualsThruster::draw()");
    CDebris* pDebris = static_cast<CDebris*>(m_pDataStorage->getUIDUsersByValueFront()->at(m_UIDRef));
    double fSizeR = 1.0 / pDebris->getPositions()->size();
    
    if (m_Graphics.getResPMX() > 0.02)
    {
        for (auto i=0u; i<pDebris->getPositions()->size(); ++i)
        {
            if (_pCamera->getBoundingBox().isInside(pDebris->getPositions()->at(i)))
            {
                m_Graphics.setColor(std::sqrt(fSizeR * i), fSizeR * i, fSizeR * i * 0.2, 0.05);
                m_Graphics.filledCircle(pDebris->getPositions()->at(i) - _pCamera->getCenter(),
        //                         IUniverseScaled::cellToDouble(pDebris->getCell() - _pCamera->getCell()),
                                (double(pDebris->getPositions()->size()-i) * 0.01 + 3.0)
                );
            }
        }
    }
}
