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
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(CCamera* const, const IObject* const) const;
                
    private:
        
        //--- Constructor [private] ------------------------------------------//
        CPlanetVisuals();       ///< Constructor is private
        
        //--- Methods [private] ----------------------------------------------//
        std::istream&       myStreamIn(std::istream&);
        std::ostream&       myStreamOut(std::ostream&);
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
    
    this->attachTo(_pPlanet);
}

#endif // PLANET_VISUALS_H
