////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2016 Torsten Büschenfeld
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
/// \file       debris_visuals_thruster.h
/// \brief      Prototype of class "CDebrisVisualsThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-03
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_VISUALS_THRUSTER_H
#define DEBRIS_VISUALS_THRUSTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "debris.h"
#include "unique_id_referrer.h"
#include "world_data_storage_user.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of thruster output
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisVisualsThruster : virtual public CGraphicsBase,
                                       public IUniqueIDReferrer<CDebris>,
                                       public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisVisualsThruster(CDebris*);
        
        //--- Constant Methods -----------------------------------------------//
        void draw(CCamera* const) const;
                
        //--- Methods --------------------------------------------------------//
        
        
    private:
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // DEBRIS_VISUALS_THRUSTER_H
