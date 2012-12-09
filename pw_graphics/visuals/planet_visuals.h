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
#include "planet.h"
#include "visuals.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a planet
///
////////////////////////////////////////////////////////////////////////////////
class CPlanetVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPlanetVisuals(CPlanet*);
        ~CPlanetVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void                attach(CPlanet*);
        const CBoundingBox& getBoundingBox();
                
    private:
        
        CPlanetVisuals();       ///< Constructor is private
        
        //--- Constant Methods [private] -------------------------------------//
        
        //--- Variables [private] --------------------------------------------//
        CPlanet*            m_pPlanet;          ///< Pointer to corresponding shape
};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPlanet Planet to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
inline CPlanetVisuals::CPlanetVisuals(CPlanet* _pPlanet): m_pPlanet(_pPlanet)
                                                                      
{
    METHOD_ENTRY("CPlanetVisuals::CPlanetVisuals")
    CTOR_CALL("CPlanetVisuals::CPlanetVisuals")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a planet to planet visuals
///
/// \param _pPlanet Planet to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CPlanetVisuals::attach(CPlanet* _pPlanet)
{
    METHOD_ENTRY("CPlanetVisuals::attach")

    m_pPlanet = _pPlanet;

    METHOD_EXIT("CPlanetVisuals::attach")
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

    METHOD_EXIT("CPlanetVisuals::getBoundingBox")
    return m_pPlanet->getBoundingBox();
}

#endif
