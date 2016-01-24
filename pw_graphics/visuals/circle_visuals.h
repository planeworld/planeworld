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
        CCircleVisuals(CDoubleBufferedShape* const);
        ~CCircleVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        virtual void attach(CDoubleBufferedShape* const);
        virtual const CBoundingBox& getBoundingBox();
        
    private:
        
        //--- Constructor [private] ------------------------------------------//
        CCircleVisuals();                   ///< Constructor is private
        
        //--- Methods [private] ----------------------------------------------//
        std::istream&       myStreamIn(std::istream&);
        std::ostream&       myStreamOut(std::ostream&);
        
        //--- Variables [private] --------------------------------------------//
        CDoubleBufferedShape* m_pCircle;    ///< Pointer to buffered shape

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CCircleVisuals::getBoundingBox()
{
    METHOD_ENTRY("CCircleVisuals::getBoundingBox")
    return m_pCircle->getShapeCur()->getBoundingBox();
}

#endif
