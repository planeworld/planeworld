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
/// \file       kinematics_state_user.h
/// \brief      Prototype of interface "IKinematicsStateUser"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-19
///
////////////////////////////////////////////////////////////////////////////////

#ifndef KINEMATICS_STATE_USER_H
#define KINEMATICS_STATE_USER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "kinematics_state.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for classes that use a kinematic state
///
////////////////////////////////////////////////////////////////////////////////
class IKinematicsStateUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//

        //--- Constant methods -----------------------------------------------//
        bool isAttached() const {return m_KinematicsState.getRef()->isValid();}
        
        //--- Methods --------------------------------------------------------//
        CKinematicsState&   getKinematicsState();
        
        void attachTo(IKinematicsStateUser* const);
                
    protected:
        
        CKinematicsState    m_KinematicsState;  ///< Kinematics state data
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the kinematics state
///
/// \return Kinematics state
///
////////////////////////////////////////////////////////////////////////////////
inline CKinematicsState& IKinematicsStateUser::getKinematicsState()
{
    METHOD_ENTRY("IKinematicsStateUser::getKinematicsState")
    return m_KinematicsState;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches kinematics state user to another one
///
/// \param _pKinStateUser Kinematics state user to attach to
///
////////////////////////////////////////////////////////////////////////////////
inline void IKinematicsStateUser::attachTo(IKinematicsStateUser* const _pKinStateUser)
{
    METHOD_ENTRY("IKinematicsStateUser::attachTo")
    m_KinematicsState.setRef(&_pKinStateUser->getKinematicsState());
}

#endif // KINEMATICS_STATE_USER_H
