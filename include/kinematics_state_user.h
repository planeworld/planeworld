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
        CKinematicsState&   getKinematicsState();
        const std::string&  getName() {return m_strName;}
        
        //--- Methods --------------------------------------------------------//
        void addHook(IKinematicsStateUser* const);
        bool hook(IKinematicsStateUser* const);
        void setName(const std::string& _strName){m_strName=_strName;}
                
    protected:
        
        CKinematicsState  m_KinematicsState; ///< Kinematics state data
        
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a hook to this kinematics state user.
///
/// \param _pKinematicsStateUser Kinematics state user to be hooked
///
////////////////////////////////////////////////////////////////////////////////
inline void IKinematicsStateUser::addHook(IKinematicsStateUser* const _pKinematicsStateUser)
{
    METHOD_ENTRY("IKinematicsStateUser::addHook")
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

    m_KinematicsState.setReference(&(_pKinematicsStateUser->getKinematicsState()));
    
    _pKinematicsStateUser->addHook(this);
    return true;
}

#endif // KINEMATICS_STATE_USER_H
