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
/// \file       engine_manager.cpp
/// \brief      Implementation of class "CEngineManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "engine_manager.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::CEngineManager()                                   
{
    METHOD_ENTRY("CEngineManager::CEngineManager")
    CTOR_CALL("CEngineManager::CEngineManager")

    METHOD_EXIT("CEngineManager::CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::~CEngineManager()
{
    METHOD_ENTRY("CEngineManager::~CEngineManager")
    DTOR_CALL("CEngineManager::~CEngineManager")

    METHOD_EXIT("CEngineManager::~CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the physics engine
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::runPhysics() const
{
    METHOD_ENTRY("CEngineManager::runPhysics")

    

    METHOD_EXIT("CEngineManager::runPhysics")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Start the graphics engine
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::runGraphics() const
{
    METHOD_ENTRY("CEngineManager::runGraphics")

    

    METHOD_EXIT("CEngineManager::runGraphics")
}