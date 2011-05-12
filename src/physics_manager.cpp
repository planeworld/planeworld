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
/// \file       physics_manager.cpp
/// \brief      Implementation of class "CPhysicsManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-07-07
///
////////////////////////////////////////////////////////////////////////////////

#include "physics_manager.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::CPhysicsManager() : m_fFrequency(PHYSICS_DEFAULT_FREQUENCY),
                                     m_fTimeAccel(1.0)
                                   
{
    METHOD_ENTRY("CPhysicsManager::CPhysicsManager")
    CTOR_CALL("CPhysicsManager::CPhysicsManager")
    
    m_vecConstantGravitation.setZero();

    METHOD_EXIT("CPhysicsManager::CPhysicsManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CPhysicsManager::~CPhysicsManager()
{
    METHOD_ENTRY("CPhysicsManager::~CPhysicsManager")
    DTOR_CALL("CPhysicsManager::~CPhysicsManager")

    for (std::list< IObject* >::iterator it = m_ObjList.begin();
        it != m_ObjList.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IObject*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IObject*", "Memory already freed."))
        }
    };
    
    for (std::list< CDebris* >::iterator it = m_DebrisList.begin();
        it != m_DebrisList.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("CDebris*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("CDebris*", "Memory already freed."))
        }
    };

    for (std::list< IJoint* >::iterator it = m_JointList.begin();
        it != m_JointList.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IJoint*")
        }
        else
        {
            DOM_MEMF(DEBUG_MSG("IJoint*", "Memory already freed."))
        }
    };

    METHOD_EXIT("CPhysicsManager::~CPhysicsManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add global forces to all objects
///
/// This method adds global forces to all objects, for example gravitation.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addGlobalForces()
{
    METHOD_ENTRY("CPhysicsManager::addGlobalForces")

    std::list< IObject* >::const_iterator cj;
    double fCCSqr;
    Vector2d vecCC;
    Vector2d vecG;

    for (std::list< IJoint* >::const_iterator ci = m_JointList.begin();
        ci != m_JointList.end(); ++ci)
    {
        (*ci)->react();
    }

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        cj = ci;
        ++cj;
        
        if ((*ci)->getGravitationState() == true)
        {
            while (cj != m_ObjList.end())
            {
                vecCC = ((*ci)->getCOM() - (*cj)->getCOM());
                fCCSqr = vecCC.squaredNorm();

                if (fCCSqr > 400.0)
                {
                    vecG = vecCC.normalized() * ((*ci)->getMass() * (*cj)->getMass()) / fCCSqr
//                             * 6.6742e+0;
                        * 6.6742e-11;
                    if ((*cj)->getGravitationState() == true)
                        (*ci)->addForce(-vecG, (*ci)->getCOM());
                    (*cj)->addForce(vecG, (*cj)->getCOM());
                }
                ++cj;
            }
        }

        (*ci)->addAcceleration(m_vecConstantGravitation);

    };

    METHOD_EXIT("CPhysicsManager::addGlobalForces")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a debris to list
///
/// This method adds the given debris object to the list of debris.
///
/// \param _pDebris Debris that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addDebris(CDebris* _pDebris)
{
    METHOD_ENTRY("CPhysicsManager::addDebris")

    m_DebrisList.push_back(_pDebris);

    METHOD_EXIT("CPhysicsManager::addDebris")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a joint to list
///
/// This method adds the given joint to the list of joints.
///
/// \param _pJoint Joint that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addJoint(IJoint* _pJoint)
{
    METHOD_ENTRY("CPhysicsManager::addJoint")

    m_JointList.push_back(_pJoint);

    METHOD_EXIT("CPhysicsManager::addJoint")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a object to list
///
/// This method adds the given object to the list of masses. 
///
/// \param _pObject Mass that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addObject(IObject* _pObject)
{
    METHOD_ENTRY("CPhysicsManager::addObject")

    m_ObjList.push_back(_pObject);

    METHOD_EXIT("CPhysicsManager::addObject")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add a list of objects to internal object list
///
/// \param _Objects Objects that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::addObjects(std::list<IObject*> _Objects)
{
    METHOD_ENTRY("CPhysicsManager::addObjects")

    std::list<IObject*>::iterator it = m_ObjList.end();
    
    m_ObjList.splice(it,_Objects);

    METHOD_EXIT("CPhysicsManager::addObjects")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests all objects for collision
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::collisionDetection()
{
    METHOD_ENTRY("CPhysicsManager::collisionDetection")

//     m_ContactList.clear();
    m_CollisionManager.setObjectList(m_ObjList);
    m_CollisionManager.detectCollisions();
    
//  for (ContactList::const_iterator ci = m_ContactList.begin();
//      ci != m_ContactList.end(); ++ci)
//  {
//      (*ci).getObjectA()->disableDynamics();
//      (*ci).getObjectB()->disableDynamics();
//  }

    METHOD_EXIT("CPhysicsManager::collisionDetection")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Moves and rotates the masses
///
/// At the moment, this is just method for testing purposes.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::moveMasses(int nTest)
{
    METHOD_ENTRY("CPhysicsManager::moveMasses")

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        (*ci)->dynamics(1.0/m_fFrequency*m_fTimeAccel);
        (*ci)->transform();
        (*ci)->clearForces();
    }
    if (nTest % 30 == 0)
    {
        for (std::list< CDebris* >::const_iterator ci = m_DebrisList.begin();
            ci != m_DebrisList.end(); ++ci)
        {
            (*ci)->dynamics(1.0/m_fFrequency*m_fTimeAccel*30);
        }
    }

    METHOD_EXIT("CPhysicsManager::moveMasses")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise all objects
///
/// Initialisiation of all objects resets objects position, speed etc. to
/// their state at the beginning of the simulation. It has to be called at
/// least once at start to ensure a proper state of fixed objects.
///
///////////////////////////////////////////////////////////////////////////////
void CPhysicsManager::initObjects()
{
    METHOD_ENTRY("CPhysicsManager::resetStates")

    m_Timer.stop();
    m_Timer.start();

    INFO_MSG("Physics Manager", "Initialising objects.")

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        (*ci)->init();
    };

    METHOD_EXIT("CPhysicsManager::resetStates")
}
