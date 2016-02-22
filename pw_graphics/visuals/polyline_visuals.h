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
/// \file       circle_visuals.h
/// \brief      Prototype of class "CPolylineVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef POLYLINE_VISUALS_H
#define POLYLINE_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a polyline
///
////////////////////////////////////////////////////////////////////////////////
class CPolylineVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CPolylineVisuals(){}
        CPolylineVisuals(CDoubleBufferedShape* const);
        ~CPolylineVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(CCamera* const, const IObject* const) const;
        
        const ShapeVisualsType getShapeVisualsType() const;
                
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
inline const ShapeVisualsType CPolylineVisuals::getShapeVisualsType() const
{
    METHOD_ENTRY("CPolylineVisuals::getShapeVisualsType")
    return ShapeVisualsType::POLYLINE;
}


#endif // POLYLINE_VISUALS_H
