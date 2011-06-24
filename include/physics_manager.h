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
// #include <list>

//--- Program header ---------------------------------------------------------//
#include "collision_manager.h"
#include "joint.h"

const double PHYSICS_DEFAULT_FREQUENCY = 60.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the physics
///
/// This class manages the lists for displaying objects etc.
///
/// \todo Create own list for gravity enabled object. This makes computation
///       faster, especially for debris.
/// \todo Implement methods to move dynamic/static objects between lists to
///       allow online state change.
/// 
////////////////////////////////////////////////////////////////////////////////
class CPhysicsManager
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPhysicsManager();
        ~CPhysicsManager();
        
        //--- Constant Methods -----------------------------------------------//
        const double getFrequency() const;

        //--- Methods --------------------------------------------------------//
        void setConstantGravitation(const Vector2d&);
        void addGlobalForces();
        void addDebris(CDebris*);
        void addJoint(IJoint*);
        void addObject(IObject*);
        void addObjects(std::list<IObject*>);
        void collisionDetection();
        void initObjects();
        void moveMasses(int);
        
        void accelerateTime();
        void decelerateTime();
        void resetTime();
        

    private:
        
        CCollisionManager   m_CollisionManager;     ///< Instance for collision handling

        CTimer m_Timer;                             ///< Timer for physics
        double m_fFrequency;                        ///< Frequency of physics calculation
        double m_fTimeAccel;                        ///< Factor for global acceleration

        Vector2d m_vecConstantGravitation;          ///< Vector for constant gravitation

        std::list<CDebris*> m_Debris;               ///< List of debris
        std::list<IJoint*>  m_JointList;            ///< List of joints
        std::list<IObject*> m_DynamicObjects;       ///< List of dynamic objects
        std::list<IObject*> m_StaticObjects;        ///< List of static objects
        
        ContactList         m_ContactList;          ///< List of contacts
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns frequency of physics calculations
///
/// \return Frequency in Hertz
///
////////////////////////////////////////////////////////////////////////////////
inline const double CPhysicsManager::getFrequency() const
{
    METHOD_ENTRY("CPhysicsManager::getFrequency()")

    METHOD_EXIT("CPhysicsManager::getFrequency()")
    return (m_fFrequency);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set vector for a constant gravitation
///
/// \param _vecG Gravity vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::setConstantGravitation(const Vector2d& _vecG)
{
    METHOD_ENTRY("CPhysicsManager::setConstantGravitation")

    m_vecConstantGravitation = _vecG;

    METHOD_EXIT("CPhysicsManager::setConstantGravitation")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Accelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::accelerateTime()
{
    METHOD_ENTRY("CPhysicsManager::accelerateTime")
    
    if (m_fTimeAccel < 16.0)
        m_fTimeAccel *= 2.0;

    METHOD_EXIT("CPhysicsManager::accelerateTime")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Decelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::decelerateTime()
{
    METHOD_ENTRY("CPhysicsManager::decelerateTime")

    m_fTimeAccel /= 2.0;

    METHOD_EXIT("CPhysicsManager::decelerateTime")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets time acceleration factor
///
////////////////////////////////////////////////////////////////////////////////
inline void CPhysicsManager::resetTime()
{
    METHOD_ENTRY("CPhysicsManager::resetTime")

    m_fTimeAccel = 1.0;

    METHOD_EXIT("CPhysicsManager::resetTime")
}

#endif
