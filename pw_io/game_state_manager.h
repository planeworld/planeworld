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
/// \brief      Prototype of class "CGameStateManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-01-15
///
////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "world_data_storage_user.h"

//--- Standard header --------------------------------------------------------//

//--- Misc. header -----------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for loading and saving the game state
///
////////////////////////////////////////////////////////////////////////////////
class CGameStateManager : public IWorldDataStorageUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CGameStateManager();
        CGameStateManager(const CGameStateManager());

        //--- Constant methods -----------------------------------------------//
        bool save() const;
        
        //--- Methods --------------------------------------------------------//
        bool load();

    private:

        //--- Variables ------------------------------------------------------//
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // GAME_STATE_MANAGER_H
