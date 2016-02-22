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
/// \file       terrain_visuals.h
/// \brief      Prototype of class "CTerrainVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TERRAIN_VISUALS_H
#define TERRAIN_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a terrain
///
////////////////////////////////////////////////////////////////////////////////
class CTerrainVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CTerrainVisuals(){}
        CTerrainVisuals(CDoubleBufferedShape* const);
        ~CTerrainVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(CCamera* const, const IObject* const) const;
        
        const ShapeVisualsType getShapeVisualsType() const;
                
        //--- Methods --------------------------------------------------------//
                
    private:
      
        //--- Methods [private] ----------------------------------------------//
        std::istream&       myStreamIn(std::istream&);
        std::ostream&       myStreamOut(std::ostream&);
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape visuals type
///
/// \return Type of shape visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeVisualsType CTerrainVisuals::getShapeVisualsType() const
{
    METHOD_ENTRY("CTerrainVisuals::getShapeVisualsType")
    return ShapeVisualsType::TERRAIN;
}


#endif // TERRAIN_VISUALS_H
