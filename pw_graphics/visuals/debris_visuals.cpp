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
    
    METHOD_EXIT("CDebrisVisuals::CDebrisVisuals")
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

    METHOD_EXIT("CDebrisVisuals::~CDebrisVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Debris
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CDebrisVisuals::draw()");

    m_Graphics.dots(m_pDebris->getPositions(),-_pCamera->getCenter());
    
    METHOD_EXIT("CDebrisVisuals::draw()");
}
