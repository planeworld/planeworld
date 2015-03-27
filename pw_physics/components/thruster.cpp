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
/// \file       thruster.cpp
/// \brief      Implementation of class "CThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-03-04
///
////////////////////////////////////////////////////////////////////////////////

#include "thruster.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the thruster
///
/// This method initialises a thruster. This includes hooking an emitter for
/// thruster output to the physical object.
///
/// \param _pHookable The entity the thruster is hooked on
/// \param _pEmitter The emitter for this thruster
///
/// \return Returns if thruster initialisation was successful
///
///////////////////////////////////////////////////////////////////////////////
bool CThruster::init(IHookable* const _pHookable, IEmitter* const _pEmitter)
{
    if (_pHookable->getHookableType() != HOOKABLE_OBJECT)
    {
        ERROR_MSG("Thruster", "Unable to init to this entity. Hookable should be an object.")
        return false;
    }
    else
    {
      m_pEmitter = _pEmitter;
      _pHookable->addHooker(_pEmitter);
      _pHookable->addHooker(this);
      return true;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Apply thruster
///
/// This method applies a force depending on the orientation and position of
/// the object the thruster is hooked on.
///
///////////////////////////////////////////////////////////////////////////////
void CThruster::fire()
{
  static_cast<IObject*>(m_pHookable)->addForce(Vector2d(0.0, 200.0),
                                              Vector2d(0.0, 0.0));
}
