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
/// \file       game_state_manager.h
/// \brief      Implementation of class "CGameStateManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-06
///
////////////////////////////////////////////////////////////////////////////////

#include "game_state_manager.h"

#include <fstream>

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CGameStateManager::CGameStateManager() 
{
    METHOD_ENTRY("CGameStateManager::CGameStateManager")
    CTOR_CALL("CGameStateManager::CGameStateManager")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief 
///
////////////////////////////////////////////////////////////////////////////////
bool CGameStateManager::load()
{
    METHOD_ENTRY("CGameStateManager::load")

    for (auto it : m_pDataStorage->getDynamicObjects())
    {
        std::cout << *(it.second) << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief 
///
////////////////////////////////////////////////////////////////////////////////
bool CGameStateManager::save() const
{
    METHOD_ENTRY("CGameStateManager::save")
    
    std::string strFilename = "testfile";
    std::fstream Filestream;

    // Close an already open stream
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(
        WARNING_MSG("Gamestate Manager", "Warning, there's already an open filestream for this object... closing."))
    }
    
    Filestream.open(strFilename.c_str(),std::ios::out);
    if (!Filestream)
    {
        ERROR_MSG("Gamestate Manager", "File " + strFilename + " could not be created.")
        Filestream.clear();
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " succesfully created."))
    }

    Filestream << std::setprecision(100) << *m_pDataStorage << std::endl;
    
    // Only close an open stream
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " closed."))
    }
    
    return true;
}
