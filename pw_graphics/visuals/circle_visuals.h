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
/// \file       circle_visuals.h
/// \brief      Prototype of class "CCircleVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CIRCLE_VISUALS_H
#define CIRCLE_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals.h"

const double CIRCLE_DEFAULT_RESOLUTION = 5.0; ///< Default resolution for visual sampling, px/vertex.
const double CIRCLE_MINIMUM_SEGMENTS  = 10.0; ///< Minimum number of circle segments

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a circle
///
////////////////////////////////////////////////////////////////////////////////
class CCircleVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CCircleVisuals(){}
        CCircleVisuals(CDoubleBufferedShape* const);
        ~CCircleVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(CCamera* const, IObject* const) const;
        
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
inline const ShapeVisualsType CCircleVisuals::getShapeVisualsType() const
{
    METHOD_ENTRY("CCircleVisuals::getShapeVisualsType")
    return ShapeVisualsType::CIRCLE;
}

#endif // CIRCLE_VISUALS_H
