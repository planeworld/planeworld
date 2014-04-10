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
        CPolylineVisuals(CDoubleBufferedShape* const);
        ~CPolylineVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void                        attach(CDoubleBufferedShape* const);
        virtual const CBoundingBox& getBoundingBox();
        
    private:
        
        CPolylineVisuals();        ///< Constructor is private, copy constructor must be used
        
        CDoubleBufferedShape* m_pPolyline;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CPolylineVisuals::getBoundingBox()
{
    METHOD_ENTRY("CPolyLineVisuals::getBoundingBox")
    return m_pPolyline->getShapeCur()->getBoundingBox();
}

#endif
