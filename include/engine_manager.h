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
/// \file       engine_manager.h
/// \brief      Prototype of class "CEngineManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_MANAGER_H
#define ENGINE_MANAGER_H

//--- Standard header --------------------------------------------------------//
// #include <list>

//--- Program header ---------------------------------------------------------//
#include "collision_manager.h"
#include "joint.h"
#include "visuals_manager.h"

const double ENGINE_DEFAULT_FREQUENCY = 80.0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the engine(s)
///
/// This class manages the lists for displaying objects etc.
/// 
////////////////////////////////////////////////////////////////////////////////
class CEngineManager
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CEngineManager();
        ~CEngineManager();
        
        //--- Constant Methods -----------------------------------------------//
        const double getFrequency() const;

        void drawWorld() const;
        
        //--- Methods --------------------------------------------------------//
        void setConstantGravitation(const Vector2d&);
        void addGlobalForces();
        void addJoint(IJoint*);
        void addObject(IObject*);
        void collisionDetection();
        void initObjects();
        void moveMasses();
        void setVisualsManager(CVisualsManager*);
        
        void accelerateTime();
        void decelerateTime();
        void resetTime();
        

    private:
        
        CCollisionManager   m_CollisionManager;     ///< Instance for collision handling
        CVisualsManager*    m_pVisualsManager;      ///< Instance for graphical representation

        CTimer m_Timer;                             ///< Timer for physics
        double m_fFrequency;                        ///< Frequency of physics calculation
        double m_fTimeAccel;                        ///< Factor for global acceleration

        Vector2d m_vecConstantGravitation;          ///< Vector for constant gravitation

        std::list<IJoint*>  m_JointList;            ///< List of joints
        std::list<IObject*> m_ObjList;              ///< List of objects
        
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
inline const double CEngineManager::getFrequency() const
{
    METHOD_ENTRY("CEngineManager::getFrequency()")

    METHOD_EXIT("CEngineManager::getFrequency()")
    return (m_fFrequency);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set vector for a constant gravitation
///
/// \param _vecG Gravity vector
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::setConstantGravitation(const Vector2d& _vecG)
{
    METHOD_ENTRY("CEngineManager::setConstantGravitation")

    m_vecConstantGravitation = _vecG;

    METHOD_EXIT("CEngineManager::setConstantGravitation")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets up instance for visualisation
///
/// \param _pVis Pointer to visualisation instance
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::setVisualsManager(CVisualsManager* _pVis)
{
    METHOD_ENTRY("CEngineManager::setVisualsManager")

    m_pVisualsManager = _pVis;

    METHOD_EXIT("CEngineManager::setVisualsManager")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Accelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::accelerateTime()
{
    METHOD_ENTRY("CEngineManager::accelerateTime")
    
    if (m_fTimeAccel < 16.0)
        m_fTimeAccel *= 2.0;

    METHOD_EXIT("CEngineManager::accelerateTime")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Decelerates time by factor 2
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::decelerateTime()
{
    METHOD_ENTRY("CEngineManager::decelerateTime")

    m_fTimeAccel /= 2.0;

    METHOD_EXIT("CEngineManager::decelerateTime")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Resets time acceleration factor
///
////////////////////////////////////////////////////////////////////////////////
inline void CEngineManager::resetTime()
{
    METHOD_ENTRY("CEngineManager::resetTime")

    m_fTimeAccel = 1.0;

    METHOD_EXIT("CEngineManager::resetTime")
}

#endif
