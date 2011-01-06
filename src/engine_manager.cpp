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
/// \file       engine_manager.cpp
/// \brief      Implementation of class "CEngineManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "engine_manager.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::CEngineManager() : m_pVisualsManager(0),
                                   m_fFrequency(ENGINE_DEFAULT_FREQUENCY),
                                   m_fTimeAccel(1.0)
                                   
{
    METHOD_ENTRY("CEngineManager::CEngineManager")
    CTOR_CALL("CEngineManager::CEngineManager")

    METHOD_EXIT("CEngineManager::CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CEngineManager::~CEngineManager()
{
    METHOD_ENTRY("CEngineManager::~CEngineManager")
    DTOR_CALL("CEngineManager::~CEngineManager")

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

    METHOD_EXIT("CEngineManager::~CEngineManager")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Displays all visible objects
///
/// This method calls the draw method of every object.
///
/// \todo Pass new structure bbox instead of object to visuals
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::drawWorld() const
{
    METHOD_ENTRY("CEngineManager::drawWorld")

    m_pVisualsManager->drawWorld();

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        m_pVisualsManager->drawBoundingBox((*ci)->getGeometry().getBoundingBox());
    };
    
//     if (m_nVisualisations & VISUALS_OBJECT_INTERSECTIONS)
//     {
//         for (ContactList::const_iterator ci = m_ContactList.begin();
//             ci != m_ContactList.end(); ++ci)
//         {
//             m_Graphics.setColor(0.0, 1.0, 0.0);
//             std::list<Vector2d> Intersections = (*ci).getPointsOfContact();
//             for (std::list<Vector2d>::const_iterator cj = Intersections.begin();
//                     cj != Intersections.end(); ++cj)
//                 m_Graphics.dot((*cj));
//             m_Graphics.setColor(1.0, 1.0, 1.0, 1.0);
//         }
//     }

    METHOD_EXIT("CEngineManager::drawWorld")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add global forces to all objects
///
/// This method adds global forces to all objects, for example gravitation.
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::addGlobalForces()
{
    METHOD_ENTRY("CEngineManager::addGlobalForces")

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
        while (cj != m_ObjList.end())
        {
            if (((*ci)->getGravitationState() == true) ||
                ((*cj)->getGravitationState() == true))
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
                    if ((*ci)->getGravitationState() == true)
                        (*cj)->addForce(vecG, (*cj)->getCOM());
                }
            }
            ++cj;
        }

        (*ci)->addAcceleration(m_vecConstantGravitation);

    };

    METHOD_EXIT("CEngineManager::addGlobalForces")
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
void CEngineManager::addJoint(IJoint* _pJoint)
{
    METHOD_ENTRY("CEngineManager::addJoint")

    m_JointList.push_back(_pJoint);

    METHOD_EXIT("CEngineManager::addJoint")
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
void CEngineManager::addObject(IObject* _pObject)
{
    METHOD_ENTRY("CEngineManager::addObject")

    m_ObjList.push_back(_pObject);

    METHOD_EXIT("CEngineManager::addObject")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests all objects for collision
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::collisionDetection()
{
    METHOD_ENTRY("CEngineManager::collisionDetection")

//     m_ContactList.clear();
    m_CollisionManager.setObjectList(m_ObjList);
    m_CollisionManager.detectCollisions();
    
//  for (ContactList::const_iterator ci = m_ContactList.begin();
//      ci != m_ContactList.end(); ++ci)
//  {
//      (*ci).getObjectA()->disableDynamics();
//      (*ci).getObjectB()->disableDynamics();
//  }

    METHOD_EXIT("CEngineManager::collisionDetection")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Moves and rotates the masses
///
/// At the moment, this is just method for testing purposes.
///
///////////////////////////////////////////////////////////////////////////////
void CEngineManager::moveMasses()
{
    METHOD_ENTRY("CEngineManager::moveMasses")

//     double fDiffTime;

//     m_Timer.stop();
//     m_Timer.start();

//     fDiffTime = m_Timer.getTime();

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        (*ci)->dynamics(1.0/m_fFrequency*m_fTimeAccel);
        (*ci)->transform();
        (*ci)->clearForces();
    }

    METHOD_EXIT("CEngineManager::moveMasses")
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
void CEngineManager::initObjects()
{
    METHOD_ENTRY("CEngineManager::resetStates")

    m_Timer.stop();
    m_Timer.start();

    INFO_MSG("Engine Manager", "Initialising objects.")

    for (std::list< IObject* >::const_iterator ci = m_ObjList.begin();
        ci != m_ObjList.end(); ++ci)
    {
        (*ci)->init();
    };

    METHOD_EXIT("CEngineManager::resetStates")
}
