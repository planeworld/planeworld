////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
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
#include "circle.h"
#include "debris.h"
#include "planet.h"
#include "polygon.h"
#include "terrain.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing collisions
///
////////////////////////////////////////////////////////////////////////////////
class CCollisionManager
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
//         CCollisionManager();
//         ~CCollisionManager();
        
        //--- Constant Methods -----------------------------------------------//
                
        //--- Methods --------------------------------------------------------//
        void detectCollisions();
        void setDebris(const DebrisType&);
        void setDynamicObjects(const ObjectsType&);
        void setStaticObjects(const ObjectsType&);
        
    private:
        
        struct PointLineContact
        {
            double fT;
            double fAlpha;
        };
        
        void getSurfaceOfInterest();
        
        void    test(CBody*, CBody*);
        void    test(CBody*, CDebris*);
        void    test(CCircle*, CCircle*, CBody*, CDebris*);
        void    test(CPlanet*, CPlanet*, CBody*, CDebris*);
        void    test(CPolygon*, CPolygon*, CBody*, CDebris*);
        void    test(CTerrain*, CDebris*);
        void    test(CCircle*, CCircle*, CCircle*, CCircle*, CBody*, CBody*);
        void    test(CCircle*, CCircle*, CPolygon*, CPolygon*, CBody*, CBody*);
        void    test(CPolygon*, CPolygon*, CPolygon*, CPolygon*, CBody*, CBody*);
        double              testLineCircle(const Vector2d&, const Vector2d&,
                                           const Vector2d&, const Vector2d&,
                                           const CCircle* const,  const CCircle* const);
        double              testPointCircle(const Vector2d&, const Vector2d&,
                                            const CCircle* const,  const CCircle* const);
        PointLineContact    testPointLine(const Vector2d&, const Vector2d&, const Vector2d&,
                                          const Vector2d&, const Vector2d&, const Vector2d&);
        
        DebrisType              m_Debris;           ///< List of debris
        ObjectsType             m_DynamicObjects;   ///< List of dynamic objects
        ObjectsType             m_StaticObjects;    ///< List of static objects
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of debris, which are always dynamic
///
/// \param _DebrisList List of debris
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setDebris(const DebrisType& _DebrisList)
{
    METHOD_ENTRY("CCollisionManager::setDebris")

    m_Debris = _DebrisList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of dynamic objects
///
/// \param _ObjList List of dynamic objects
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setDynamicObjects(const ObjectsType& _ObjList)
{
    METHOD_ENTRY("CCollisionManager::setDynamicObjects")

    m_DynamicObjects = _ObjList;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Pass list of static objects
///
/// \param _ObjList List of static objects
///
////////////////////////////////////////////////////////////////////////////////
inline void CCollisionManager::setStaticObjects(const ObjectsType& _ObjList)
{
    METHOD_ENTRY("CCollisionManager::setStaticObjects")

    m_StaticObjects = _ObjList;
}

#endif
