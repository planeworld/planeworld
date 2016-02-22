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
/// \file       game_state_manager.cpp
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
/// \brief Load game state information from disk
///
////////////////////////////////////////////////////////////////////////////////
bool CGameStateManager::load() const
{
    METHOD_ENTRY("CGameStateManager::load")
    
    std::string strFilename = "testfile.sav";
    std::ifstream Filestream;

    // Close an already open stream
    /// This becomes relevant as soon as the filestream is a member variable
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(
        WARNING_MSG("Gamestate Manager", "Warning, there's already an open filestream for this object... closing."))
    }
    
    Filestream.open(strFilename.c_str());
    if (!Filestream)
    {
        DOM_FIO(ERROR_MSG("Gamestate Manager", "File " + strFilename + " could not be opened."))
        Filestream.clear();
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " succesfully opened."))
    }
    
    /// \todo Use std::numeric_limits to set precision
    Filestream >> std::setprecision(17) >> std::setw(25) >> *m_pDataStorage;
    
    // Only close an open stream
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " closed."))
    }
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Save game state information on disk.
///
////////////////////////////////////////////////////////////////////////////////
bool CGameStateManager::save() const
{
    METHOD_ENTRY("CGameStateManager::save")
    
    std::string strFilename = "testfile.sav";
    std::ofstream Filestream;

    // Close an already open stream 
    /// This becomes relevant as soon as the filestream is a member variable
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(
        WARNING_MSG("Gamestate Manager", "Warning, there's already an open filestream for this object... closing."))
    }
    
    Filestream.open(strFilename.c_str());
    if (!Filestream)
    {
        DOM_FIO(ERROR_MSG("Gamestate Manager", "File " + strFilename + " could not be created."))
        Filestream.clear();
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " succesfully created."))
    }

    /// \todo Use std::numeric_limits to set precision
    Filestream << std::setprecision(17) << std::setw(25) << *m_pDataStorage << std::endl;
    
    // Only close an open stream
    if (Filestream.is_open() == true)
    {
        Filestream.close();
        DOM_FIO(DEBUG_MSG("Gamestate Manager", strFilename + " closed."))
    }
    
    return true;
}
