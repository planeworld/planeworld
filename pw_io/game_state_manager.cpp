////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \file       game_state_manager.cpp
/// \brief      Implementation of class "CGameStateManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-15
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
/// \bug Something is wrong with cell loading, e.g. commenting out cellUpdate
///      fixes one particular problem. Cell handling in visuals manager is 
///      concerned, too.
/// \todo Streaming of classes which inherit from other classes shouldn't call
///       (stream) members directly but calling superclass::operator<</>>.
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
