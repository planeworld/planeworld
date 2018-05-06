////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2018 Torsten Büschenfeld
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
/// \file       game_state_manager.h
/// \brief      Prototype of class "CGameStateManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

//--- Program header ---------------------------------------------------------//
#include "com_interface_provider.h"
#include "log.h"
#include "world_data_storage_user.h"

//--- Standard header --------------------------------------------------------//

//--- Misc. header -----------------------------------------------------------//

constexpr auto PW_FILENAME_DEFAULT = "pw_simstate";

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for loading and saving the game state
///
////////////////////////////////////////////////////////////////////////////////
class CGameStateManager : public IComInterfaceProvider,
                          public ISerializable,
                          public IWorldDataStorageUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CGameStateManager();

        //--- Constant methods -----------------------------------------------//
        bool load(const std::string& = "") const;
                
        //--- Methods --------------------------------------------------------//
        bool save(const std::string& = "");

    private:
        
        void myInitComInterface() override final
        {
            METHOD_ENTRY("CPhysicsManager::myInitComInterface")

            INFO_MSG("Game State Manager", "Initialising com interace.")
            
            m_pComInterface->registerFunction("load",
                                        CCommand<void,std::string>([&](const std::string& _strFile)
                                        {
                                            this->load(_strFile);
                                        }),
                                        "Loads simulation state from given file.",
                                        {{ParameterType::NONE, "No return value"},
                                        {ParameterType::STRING, "File to load simulation state from"}},
                                        "system", "gamestate"
                                        );
            m_pComInterface->registerFunction("save",
                                        CCommand<void,std::string>([&](const std::string& _strFile)
                                        {
                                            this->save(_strFile);
                                        }),
                                        "Saves simulation state to given file.",
                                        {{ParameterType::NONE, "No return value"},
                                        {ParameterType::STRING, "File to save simulation state to"}},
                                        "system", "gamestate"
                                        );
        }

        //--- Variables ------------------------------------------------------//
        std::string         m_strLastFilename; ///< Last filename used for saving
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // GAME_STATE_MANAGER_H
