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
/// \file       collision_manager.h
/// \brief      Prototype of class "CCollisionManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "body.h"
#include "debris.h"
#include "planet.h"
#include "terrain.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing collisions
///
/// \todo Pass pointers to lists instead of references, now at least the list
///       structure is copied.
///
////////////////////////////////////////////////////////////////////////////////
class CCollisionManager/* : virtual public CGraphicsBase*/
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
//         CCollisionManager();
//         ~CCollisionManager();
        
        //--- Constant Methods -----------------------------------------------//
                
        //--- Methods --------------------------------------------------------//
        void detectCollisions();
        void setDebris(const std::list<CDebris*>&);
        void setDynamicObjects(const std::list<IObject*>&);
        void setStaticObjects(const std::list<IObject*>&);
        
    private:
        
        void getSurfaceOfInterest();
        
        void test(CBody*, CBody*);
        void test(CBody*, CDebris*);
        void test(CCircle*, CCircle*, CBody*, CDebris*);
        void test(CTerrain*, CDebris*);
        void test(CCircle*, CCircle*, CCircle*, CCircle*, CBody*, CBody*);
        void test(CCircle*, CCircle*, CPolyLine*, CPolyLine*, CBody*, CBody*);
        void test(CPolyLine*, CPolyLine*, CPolyLine*, CPolyLine*, CBody*, CBody*);
        
        std::list<CDebris*> m_Debris;
        std::list<IObject*> m_DynamicObjects;
        std::list<IObject*> m_StaticObjects;
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of debris, which are always dynamic
///
/// \param _DebrisList List of debris
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setDebris(const std::list<CDebris*>& _DebrisList)
{
    METHOD_ENTRY("CCollisionManager::setDebris")

    m_Debris = _DebrisList;

    METHOD_EXIT("CCollisionManager::setDebris")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of dynamic objects
///
/// \param _ObjList List of dynamic objects
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setDynamicObjects(const std::list<IObject*>& _ObjList)
{
    METHOD_ENTRY("CCollisionManager::setDynamicObjects")

    m_DynamicObjects = _ObjList;

    METHOD_EXIT("CCollisionManager::setDynamicObjects")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of static objects
///
/// \param _ObjList List of static objects
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setStaticObjects(const std::list<IObject*>& _ObjList)
{
    METHOD_ENTRY("CCollisionManager::setStaticObjects")

    m_StaticObjects = _ObjList;

    METHOD_EXIT("CCollisionManager::setStaticObjects")
}

#endif
