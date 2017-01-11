////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2017 Torsten Büschenfeld
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
/// \file       physics_manager.h
/// \brief      Prototype of class "CPhysicsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-07
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "collision_manager.h"
#include "com_interface_provider.h"
#include "conf_pw.h"
#include "emitter.h"
#include "sim_timer.h"
#include "thruster.h"
#include "universe.h"
#include "world_data_storage_user.h"

//--- Misc header ------------------------------------------------------------//

const bool        PHYSICS_ALLOW_STEP_SIZE_INC   = true;     ///< Increasing step size when accelerating is allowed
const bool        PHYSICS_FORBID_STEP_SIZE_INC  = false;    ///< Increasing step size when accelerating is forbidden
const double      PHYSICS_DEFAULT_FREQUENCY     = 200.0;    ///< Default physics frequency
const double      PHYSICS_DEBRIS_DEFAULT_FREQUENCY = 30.0;  ///< Default physics frequency for debris

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the physics
///
/// This class computes everything that is physics related.
///
////////////////////////////////////////////////////////////////////////////////
class CPhysicsManager : public IComInterfaceProvider,
                        public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPhysicsManager();
        ~CPhysicsManager();
        
        //--- Constant Methods -----------------------------------------------//
        double      getFrequency() const;
        CUniverse*  getUniverse() const;

        //--- Methods --------------------------------------------------------//
        CSimTimer&                        getSimTimerGlobal();
        std::array<CSimTimer,3>&          getSimTimerLocal();

        void setConstantGravity(const Vector2d&);
        void setFrequency(const double&);
        void setFrequencyDebris(const double&);
        void setUniverse(CUniverse* const);
        
        void addComponent(CThruster* const);
        void addComponents(const ComponentsType&);
        void addEmitter(IEmitter* const);
        void addEmitters(const EmittersType&);
        void initComponents();
        void initEmitters();
        void initObjects();
        
        void pause();
        void processFrame();
        void processOneFrame();
        void togglePause();
        
        void accelerateTime(const bool = false);
        void decelerateTime();
        void resetTime();
        
        #ifdef PW_MULTITHREADING
          void run();
          void terminate();
        #else
          const double& getTimeAccel() const;
          void setTimeSlept(const double&);
        #endif

    private:
        
        //--- Constant methods [private] -------------------------------------//
        void moveMasses(int) const;
        
        //--- Methods [private] ----------------------------------------------//
        void addGlobalForces();
        void collisionDetection();
        void myInitComInterface();
        void updateCells();
      
        CUniverse*          m_pUniverse;            ///< The procedurally generated universe
        CCollisionManager   m_CollisionManager;     ///< Instance for collision handling

        double              m_fG;                   ///< Gravitational constant
        double              m_fFrequency;           ///< Frequency of physics processing
        double              m_fFrequencyDebris;     ///< Frequency of debris physics processing
        double              m_fTimeAccel;           ///< Time acceleration of simulation
        double              m_fTimeSlept;           ///< Sleep time of thread
        
        Vector2d                    m_vecConstantGravitation;   ///< Vector for constant gravitation

        ComponentsType              m_Components;               ///< List of components
        EmittersType                m_Emitters;                 ///< List of emitters
        
        double                      m_fCellUpdateResidual;      ///< Residual for calculation of cell update
        bool                        m_bCellUpdateFirst;         ///< Indicates the first cell update (to initialise access)
        bool                        m_bPaused;                  ///< Indicates if physics caluculations are paused
        bool                        m_bProcessOneFrame;         ///< Indicates if physics should be run stepwise
        #ifdef PW_MULTITHREADING
          bool                      m_bRunning = false;         ///< Indicates if physics thread is running
        #endif
        
        CSimTimer                   m_SimTimerGlobal;           ///< Simulation time since beginning of simulation
        std::array<CSimTimer,3>     m_SimTimerLocal;            ///< Local timer / stop watch in simulation time
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of physics calculations
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline double CPhysicsManager::getFrequency() const
{
    METHOD_ENTRY("CPhysicsManager::getFrequency()")
    return (m_fFrequency);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the procedurally generated universe
///
/// \return Procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline CUniverse* CPhysicsManager::getUniverse() const
{
    METHOD_ENTRY("CPhysicsManager::getUniverse()")
    return m_pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the timer in simulation time that counts from the beginning of
///        the simulation.
///
/// \return Simulation timer
///
////////////////////////////////////////////////////////////////////////////////
inline CSimTimer& CPhysicsManager::getSimTimerGlobal()
{
    METHOD_ENTRY("CPhysicsManager::getSimTimerGlobal")
    return m_SimTimerGlobal;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns local timers / stop watches counting in simulation time
///
/// \return Local simulation timers
///
////////////////////////////////////////////////////////////////////////////////
inline std::array<CSimTimer,3>& CPhysicsManager::getSimTimerLocal()
{
    METHOD_ENTRY("CPhysicsManager::getSimTimerLocal")
    return m_SimTimerLocal;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set vector for a constant gravity
///
/// \param _vecG Gravity vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setConstantGravity(const Vector2d& _vecG)
{
    METHOD_ENTRY("CPhysicsManager::setConstantGravity")
    m_vecConstantGravitation = _vecG;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set frequency for physics processing
///
/// \param _fFrequency Frequency for physics processing
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setFrequency(const double& _fFrequency)
{
    METHOD_ENTRY("CPhysicsManager::setFrequency")
    m_fFrequency = _fFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set frequency for debris physics processing
///
/// \param _fFrequency Frequency for debris physics processing
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setFrequencyDebris(const double& _fFrequency)
{
    METHOD_ENTRY("CPhysicsManager::setFrequencyDebris")
    m_fFrequencyDebris = _fFrequency;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the procedurally generated universe
///
/// \param _pUniverse The procedurally generated universe
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setUniverse(CUniverse* const _pUniverse)
{
    METHOD_ENTRY("CPhysicsManager::setUniverse")
    m_pUniverse = _pUniverse;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pauses physics processing independend from current state
/// 
/// Pausing instead of toggling pause is needed, to definitely pause the physics
/// in cases like loading or saving.
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::pause()
{
    METHOD_ENTRY("CPhysicsManager::pause")
    m_bPaused = true;
    INFO_MSG("Physics Manager", "Physics processing paused.")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Stepwise processing when paused
/// 
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::processOneFrame()
{
    METHOD_ENTRY("CPhysicsManager::processOneFrame")
    m_bProcessOneFrame = true;
}

#ifdef PW_MULTITHREADING
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Stops physics thread
  ///
  ////////////////////////////////////////////////////////////////////////////////
  inline void CPhysicsManager::terminate()
  {
      METHOD_ENTRY("CPhysicsManager::terminate")
      m_bRunning = false;
  }
#else
  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Returns time acceleration factor
  ///
  /// \return Time acceleration factor
  ///
  ////////////////////////////////////////////////////////////////////////////////
  inline const double& CPhysicsManager::getTimeAccel() const
  {
      METHOD_ENTRY("CPhysicsManager::getTimeAccel")
      return m_fTimeAccel;
  }

  ////////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Sets the time slept in main thread to cope with time acceleration
  ///
  /// \param _fTimeSlept Time the main thread slept
  ///
  ////////////////////////////////////////////////////////////////////////////////
  inline void CPhysicsManager::setTimeSlept(const double& _fTimeSlept)
  {
      METHOD_ENTRY("CPhysicsManager::setTimeSlept")
      m_fTimeSlept = _fTimeSlept;
  }
#endif // PW_MULTITHREADING

#endif // PHYSICS_MANAGER_H
