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
#include "planet.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing collisions
///
////////////////////////////////////////////////////////////////////////////////
class CCollisionManager : virtual public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
//         CCollisionManager();
//         ~CCollisionManager();
        
        //--- Constant Methods -----------------------------------------------//
                
        //--- Methods --------------------------------------------------------//
        void detectCollisions();
        void setObjectList(const std::list<IObject*>&);
        
    private:
        
        void getSurfaceOfInterest();
        
        void test(CBody*, CBody*);
        void test(CCircle*, CCircle*, CCircle*, CCircle*, CBody*, CBody*);
        void test(CCircle*, CCircle*, CPolyLine*, CPolyLine*, CBody*, CBody*);
        void test(CPolyLine*, CPolyLine*, CPolyLine*, CPolyLine*, CBody*, CBody*);
        
        std::list<IObject*> m_ObjList;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of objects
///
/// \param _ObjList List of objects
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setObjectList(const std::list<IObject*>& _ObjList)
{
    METHOD_ENTRY("CCollisionManager::setObjectList")

    m_ObjList = _ObjList;

    METHOD_EXIT("CCollisionManager::setObjList")
}

#endif
