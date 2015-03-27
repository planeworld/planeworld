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
/// \file       hooker.cpp
/// \brief      Implementation of class "IHooker"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-10-31
///
////////////////////////////////////////////////////////////////////////////////

#include "hooker.h"

//--- Program header ---------------------------------------------------------//
#include "hookable.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
IHooker::~IHooker()
{
    METHOD_ENTRY("IHooker::~IHooker")
    DTOR_CALL("IHooker::~IHooker")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Hooks to a hookable
///
/// \param _pHookable Hookable to hook to
///
///////////////////////////////////////////////////////////////////////////////
bool IHooker::hook(IHookable* const _pHookable)
{
    METHOD_ENTRY("IHooker::hook")

    if (_pHookable->getHookableType() != HOOKABLE_OBJECT)
    {
        ERROR_MSG("Hooker", "Unable to hook to this entity. Hookable should be an object.")
        return false;
    }
    else
    {
      m_pHookable = _pHookable;
      m_pHookable->addHooker(this);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Update the hooker
///
///////////////////////////////////////////////////////////////////////////////
void IHooker::updateFromHookable()
{
    METHOD_ENTRY("IHooker::updateFromHookable")
}
