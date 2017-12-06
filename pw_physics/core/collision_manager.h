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
#include "circle.h"
#include "particle.h"
#include "object.h"
#include "planet.h"
#include "polygon.h"
#include "terrain.h"
#include "world_data_storage_user.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing collisions
///
////////////////////////////////////////////////////////////////////////////////
class CCollisionManager : public IWorldDataStorageUser
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
//         CCollisionManager();
//         ~CCollisionManager();
        
        //--- Constant Methods -----------------------------------------------//
                
        //--- Methods --------------------------------------------------------//
        void detectCollisions();
        void setParticle(const ParticleType&);
        
    private:
        
        struct PointLineContact
        {
            double fT;
            double fAlpha;
        };
        
        void getSurfaceOfInterest();
        
        void    test(CObject*, CObject*);
        void    test(CObject*, CParticle*);
        void    test(CCircle*, CCircle*, CObject*, CParticle*);
        void    test(CPlanet*, CPlanet*, CObject*, CParticle*);
        void    test(CPolygon*, CPolygon*, CObject*, CParticle*);
        void    test(CTerrain*, CParticle*);
        void    test(CCircle*, CCircle*, CCircle*, CCircle*, CObject*, CObject*);
        void    test(CCircle*, CCircle*, CPolygon*, CPolygon*, CObject*, CObject*);
        void    test(CPolygon*, CPolygon*, CPolygon*, CPolygon*, CObject*, CObject*);
        double              testLineCircle(const Vector2d&, const Vector2d&,
                                           const Vector2d&, const Vector2d&,
                                           const CCircle* const,  const CCircle* const);
        double              testPointCircle(const Vector2d&, const Vector2d&,
                                            const CCircle* const,  const CCircle* const);
        PointLineContact    testPointLine(const Vector2d&, const Vector2d&, const Vector2d&,
                                          const Vector2d&, const Vector2d&, const Vector2d&);
        
        ParticleType              m_Particle;           ///< List of particle
};

//--- Implementation is done here for inline optimisation --------------------//

#endif
