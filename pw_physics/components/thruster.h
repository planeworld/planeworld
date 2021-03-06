////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2015-2018 Torsten Büschenfeld
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
/// \file       thruster.h
/// \brief      Prototype of class "CThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-07-09
///
////////////////////////////////////////////////////////////////////////////////

#ifndef THRUSTER_H
#define THRUSTER_H

//--- Program header ---------------------------------------------------------//
#include "emitter.h"
#include "handle.h"
#include "kinematics_state_user.h"
#include "world_data_storage_user.h"

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a thruster component
/// 
////////////////////////////////////////////////////////////////////////////////
class CThruster : public IKinematicsStateUser,
                  public IUIDUser,
                  public IWorldDataStorageUser
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CThruster();
        
        //--- Constant methods -----------------------------------------------//
        double    getAngle() const;
        Vector2d  getOrigin() const;
        double    getThrust() const;
        
        
        //--- Methods --------------------------------------------------------//
        const double& activate(const double&);
        void          deactivate();
        void          execute();

        void addEmitter(IEmitter* const);
        void setObject(CObject* const);
        
        void setAngle(const double&);
        void setOrigin(const Vector2d&);
        
        void setThrustMax(const double&);
         
    protected:
                
        //--- Variables ------------------------------------------------------//
        std::vector<CHandle<IEmitter>> m_hEmitters; ///< Emitters for thrust particles
        CHandle<CObject> m_hObject;          ///< Physical object representing thruster
        
        bool        m_bActive;              ///< Flags if thruster is activated
      
        double      m_fThrust;              ///< Thrust applied to hooked object
        double      m_fThrustMax;           ///< Maximum thrust of this thruster
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Deactivate thruster
///
/// This method stops the thruster and thus, no force is applied within the
/// execute() method.
///
///////////////////////////////////////////////////////////////////////////////
inline void CThruster::deactivate()
{
    METHOD_ENTRY("CThruster::deactivate")
    m_fThrust = 0.0;
    
    m_bActive = false;
    for (const auto Emitter : m_hEmitters)
    {
        if (Emitter.isValid())
        {
            Emitter->deactivate();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the angle of thrust vector.
///
/// \param _fAngle Angle of thrust vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setAngle(const double& _fAngle)
{
    METHOD_ENTRY("CThruster::setAngle")
    m_KinematicsState.setAngle(_fAngle);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the origin of thruster
///
/// \param _vecOrigin Origin of thruster
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setOrigin(const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CThruster::setOrigin")
    m_KinematicsState.setOrigin(_vecOrigin);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the maximum thrust for this thruster.
///
/// \param _fThrustMax Maximum thrust for this thruster
///
////////////////////////////////////////////////////////////////////////////////
inline void CThruster::setThrustMax(const double& _fThrustMax)
{
    METHOD_ENTRY("CThruster::setThrustMax")
    m_fThrust = _fThrustMax;
    m_fThrustMax = _fThrustMax;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns angle of thrust vector
///
/// \return Angle of thrust vector
///
////////////////////////////////////////////////////////////////////////////////
inline double CThruster::getAngle() const
{
    METHOD_ENTRY("CThruster::getAngle")
    return m_KinematicsState.getAngle();    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns origin of thruster
///
/// \return Origin of thruster
///
////////////////////////////////////////////////////////////////////////////////
inline Vector2d CThruster::getOrigin() const
{
    METHOD_ENTRY("CThruster::getOrigin")
    return m_KinematicsState.getOrigin();    
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns current thrust
///
/// \return Current thrust
///
////////////////////////////////////////////////////////////////////////////////
inline double CThruster::getThrust() const
{
    METHOD_ENTRY("CThruster::getThrust")
    return m_fThrust;    
}

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Attaches thruster to UID user
// ///
// ////////////////////////////////////////////////////////////////////////////////
// inline void CThruster::mySetRef()
// {
//     METHOD_ENTRY("CThruster::mySetRef")
//     if (IEmitterReferrer::m_pRef != nullptr)
//     {
//         m_KinematicsState.setRef(&(IEmitterReferrer::m_pRef->getKinematicsState()));
//     }
//     if (IObjectReferrer::m_pRef != nullptr)
//         m_KinematicsState.setRef(&(IObjectReferrer::m_pRef->getKinematicsState()));
// }

#endif // THRUSTER_H
