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
#include "emitter.h"
#include "sim_timer.h"
#include "thread_module.h"
#include "thruster.h"
#include "universe.h"
#include "world_data_storage_user.h"

//--- Misc header ------------------------------------------------------------//

//--- Constants --------------------------------------------------------------//
const bool        PHYSICS_ALLOW_STEP_SIZE_INC   = true;     ///< Increasing step size when accelerating is allowed
const bool        PHYSICS_FORBID_STEP_SIZE_INC  = false;    ///< Increasing step size when accelerating is forbidden
const double      PHYSICS_DEFAULT_FREQUENCY     = 200.0;    ///< Default physics frequency
const double      PHYSICS_PARTICLE_DEFAULT_FREQUENCY = 30.0;  ///< Default physics frequency for particle

//--- Type definitions -------------------------------------------------------//

/// Type for concurrent object queue 
typedef moodycamel::ConcurrentQueue<IEmitter*> EmittersQueueType;
/// Type for concurrent object queue 
typedef moodycamel::ConcurrentQueue<CObject*> ObjectsQueueType;
/// Type for concurrent particles queue 
typedef moodycamel::ConcurrentQueue<CParticle*> ParticlesQueueType;
/// Type for concurrent shape queue
typedef moodycamel::ConcurrentQueue<IShape*>  ShapesQueueType;
/// Type for concurrent shape queue
typedef moodycamel::ConcurrentQueue<CThruster*>  ThrustersQueueType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the physics
///
/// This class computes everything that is physics related.
///
////////////////////////////////////////////////////////////////////////////////
class CPhysicsManager : public IComInterfaceProvider,
                        public IThreadModule,
                        public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPhysicsManager();
        ~CPhysicsManager();
        
        //--- Constant Methods -----------------------------------------------//
        const std::array<CSimTimer,4>&    getSimTimer() const;
        CUniverse*                        getUniverse() const;

        //--- Methods --------------------------------------------------------//
        UIDType createEmitter(const EmitterType);
        UIDType createEmitter(const std::string&);
        UIDType createObject();
        UIDType createParticles(const ParticleTypeType);
        UIDType createParticles(const std::string&);
        UIDType createShape(const ShapeType);
        UIDType createShape(const std::string&);
        UIDType createThruster();
        
        void setConstantGravity(const Vector2d&);
        void setFrequencyParticle(const double&);
        
        void pause();
        void processFrame();
        void processOneFrame();
        void togglePause();
        
        void accelerateTime(const bool = false);
        void decelerateTime();
        void resetTime();
        
        #ifndef PW_MULTITHREADING
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
        void processQueues();
        void updateCells();
        
        EmittersQueueType   m_EmittersToBeAddedToWorld;         ///< Emitters already created to be added to world
        ObjectsQueueType    m_ObjectsToBeAddedToWorld;          ///< Objects already created to be added to world
        ParticlesQueueType  m_ParticlesToBeAddedToWorld;        ///< Particles already created to be added to world
        ShapesQueueType     m_ShapesToBeAddedToWorld;           ///< Shapes already created to be added to world
        ThrustersQueueType  m_ThrustersToBeAddedToWorld;        ///< Thrusters already created to be added to world
        
      
        CUniverse*          m_pUniverse;                        ///< The procedurally generated universe
        CCollisionManager   m_CollisionManager;                 ///< Instance for collision handling

        double              m_fG;                               ///< Gravitational constant
        double              m_fFrequencyParticle;                 ///< Frequency of particle physics processing
        
        Vector2d                    m_vecConstantGravitation;   ///< Vector for constant gravitation

        std::string                 m_strCellUpdateLast;        ///< Last updated object concerning grid cells
        double                      m_fCellUpdateResidual;      ///< Residual for calculation of cell update
        bool                        m_bCellUpdateFirst;         ///< Indicates the first cell update (to initialise access)
        bool                        m_bPaused;                  ///< Indicates if physics caluculations are paused
        bool                        m_bProcessOneFrame;         ///< Indicates if physics should be run stepwise
        
        std::array<CSimTimer,4>     m_SimTimer;                 ///< Timer / stop watch in simulation time
        
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns timers / stop watches counting in simulation time
///
/// \return Simulation timers
///
////////////////////////////////////////////////////////////////////////////////
inline const std::array<CSimTimer,4>& CPhysicsManager::getSimTimer() const
{
    METHOD_ENTRY("CPhysicsManager::getSimTimerLocal")
    return m_SimTimer;
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
/// \brief Set frequency for particle physics processing
///
/// \param _fFrequency Frequency for particle physics processing
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setFrequencyParticle(const double& _fFrequency)
{
    METHOD_ENTRY("CPhysicsManager::setFrequencyParticle")
    m_fFrequencyParticle = _fFrequency;
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

#ifndef PW_MULTITHREADING
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
