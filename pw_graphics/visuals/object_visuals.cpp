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
/// \file       object_visuals.cpp
/// \brief      Implementation of class "IObjectVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-09
///
////////////////////////////////////////////////////////////////////////////////

#include "object_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
IObjectVisuals::~IObjectVisuals()
{
    METHOD_ENTRY("IObjectVisuals::~IObjectVisuals")
    DTOR_CALL("IObjectVisuals::~IObjectVisuals")
    
    for (std::vector<IVisuals*>::iterator it = m_Visuals.begin();
        it != m_Visuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IVisuals*");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw all shape visuals of this object
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void IObjectVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("IObjectVisuals::draw")
    
    for (std::vector<IVisuals*>::const_iterator ci  = m_Visuals.begin();
                                                ci != m_Visuals.end(); ++ci)
    {
        if ((*ci)->getBoundingBox().overlaps(_pCamera->getBoundingBox()))
        {
            if ((((*ci)->getBoundingBox().getWidth() * _pCamera->getZoom()) <  0.3) && 
                (((*ci)->getBoundingBox().getHeight() * _pCamera->getZoom()) < 0.3))
            {
                m_Graphics.dot((*ci)->getBoundingBox().getLowerLeft() -
                                _pCamera->getCenter()-_pCamera->getCellToPos());
            }
            else
            {
                (*ci)->draw(_pCamera, m_pObject);
            }
        }
    }
}
