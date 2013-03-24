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
/// \file       planet_visuals.h
/// \brief      Prototype of class "CPlanetVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-17
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PLANET_VISUALS_H
#define PLANET_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals.h"

//--- Misc header ------------------------------------------------------------//

const double PLANET_VISUALS_DEFAULT_RESOLUTION=3.0;          ///< Default resolution for visual sampling px/vertex
const double PLANET_VISUALS_DEFAULT_MINIMUM_ANGLE=M_PI*0.01; ///< Default minium of 200 segments if above resolution limit

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a planet
///
////////////////////////////////////////////////////////////////////////////////
class CPlanetVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPlanetVisuals(CDoubleBufferedShape* const);
        ~CPlanetVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void                attach(CDoubleBufferedShape* const);
        const CBoundingBox& getBoundingBox();
                
    private:
        
        CPlanetVisuals();       ///< Constructor is private
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        CDoubleBufferedShape* m_pPlanet;          ///< Pointer to corresponding shape
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPlanet Planet to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
inline CPlanetVisuals::CPlanetVisuals(CDoubleBufferedShape* const _pPlanet)
                                                                      
{
    METHOD_ENTRY("CPlanetVisuals::CPlanetVisuals")
    CTOR_CALL("CPlanetVisuals::CPlanetVisuals")
    
    this->attach(_pPlanet);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CPlanetVisuals::getBoundingBox()
{
    METHOD_ENTRY("CPlanetVisuals::getBoundingBox")
    return m_pPlanet->getShapeCur()->getBoundingBox();
}

#endif
