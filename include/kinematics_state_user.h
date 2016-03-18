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
/// \todo Get rid of hooking by using a proper implementation of uid user and
///       its virtual myAttachTo method if applicable
///
////////////////////////////////////////////////////////////////////////////////
class IKinematicsStateUser
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//

        //--- Constant methods -----------------------------------------------//
        CKinematicsState&   getKinematicsState();
        const std::string&  getName() {return m_strName;}
        
        //--- Methods --------------------------------------------------------//
        bool hook(IKinematicsStateUser* const);
        void setName(const std::string& _strName){m_strName=_strName;}
                
    protected:
        
        CKinematicsState    m_KinematicsState;  ///< Kinematics state data
        
        std::string m_strName = {"TESTNAME"};
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

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Hooks to another user of kinematics state
///
/// \param _KinematicsStateUser User of kinematics state to hook to
///
///////////////////////////////////////////////////////////////////////////////
inline bool IKinematicsStateUser::hook(IKinematicsStateUser* const _pKinematicsStateUser)
{
    METHOD_ENTRY("IKinematicsStateUser::hook")

    m_KinematicsState.attachTo(&(_pKinematicsStateUser->getKinematicsState()));
    return true;
}

#endif // KINEMATICS_STATE_USER_H
