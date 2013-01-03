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
/// \file       universe.h
/// \brief      Prototype of class "CUniverse"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-11
///
////////////////////////////////////////////////////////////////////////////////

#ifndef UNIVERSE_H
#define UNIVERSE_H

//--- Standard header --------------------------------------------------------//
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "circle.h"
#include "circle_visuals.h"
#include "object_visuals.h"
#include "rigidbody.h"
#include "star_system.h"

//--- Misc header ------------------------------------------------------------//

const double UNIVERSE_CELL_SIZE=1.0e12;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing a universe. Mainly needed for procedural generation.
///
/// Some notes: A globular cluster, which is represented at the moment, consists
/// of 10000 to several million stars. Their density is 0.4 stars/parsec on
/// average and 100-1000 stars/parsec in the inner core.
/// One cell of the space grid in planeworld that avoids numerical problems is
/// 1.0e12m. One parsec is 30.857e15m. This means that the star density is
/// 0.4/30.857e15m which is 0.4e12/30.857e15=1.3e-5/cell. Moreover, given
/// m_nNrOfStars and using the 3-sigma value to cut the distribution, the
/// density is 0.4/30.857e15m = m_nNrOfStars / 3-Sigma.That is
/// Sigma = m_nNrOfStars*30.857e15/(3*0.4)
///
////////////////////////////////////////////////////////////////////////////////
class CUniverse
{
    
    public:
        CRigidBody*                 m_pStar;
        CCircle*                    m_pStarShape;
        CCircleVisuals*             m_pStarVisuals;
        IObjectVisuals*             m_pStarObjectVisuals;
    
        //--- Constructor/Destructor -----------------------------------------//
        CUniverse();
        virtual ~CUniverse();
        
        //--- Constant Methods -----------------------------------------------//
        const std::vector<IObject*>&          getObjects() const;
        const std::vector<CStarSystem*>&      getStarSystems() const;
        const std::vector<IObjectVisuals*>&   getVisuals() const;

        //--- Methods --------------------------------------------------------//
        void generate(const int&, const int&);
        void clone(const CUniverse&);
        
    private:
        
        CUniverse(const CUniverse&);                ///< Private, prevent copy-construction, use clone instead
        CUniverse& operator=(const CUniverse&);     ///< Private, prevent assignment, use clone instead
        
        //--- Constant Methods [private] -------------------------------------//
        const std::string starClassToString(const int&) const;
        
        std::vector<IObject*>           m_Objects;          ///< Objects of the universe
        std::vector<CStarSystem*>       m_StarSystems;      ///< Star system in this universe
        std::vector<IObjectVisuals*>    m_Visuals;          ///< Object visuals of the universe
        int                             m_nNrOfPlanetsMax;  ///< Maximum number of planets

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return generated objects
///
/// \return List of generated objects
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<IObject*>& CUniverse::getObjects() const
{
    METHOD_ENTRY("CUniverse::getObjects")
    return m_Objects;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the list of all star systems.
///
/// \return Vector (list), which holds the star systems
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector< CStarSystem* >& CUniverse::getStarSystems() const
{
    METHOD_ENTRY("CUniverse::getStarSystems")
    return m_StarSystems;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Return generated object visuals
///
/// \return List of generated object visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const std::vector<IObjectVisuals*>& CUniverse::getVisuals() const
{
    METHOD_ENTRY("CUniverse::getVisuals")
    return m_Visuals;
}

#endif
