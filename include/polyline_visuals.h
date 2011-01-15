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
#include "polyline.h"
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
        CPolylineVisuals(CPolyLine*);
        ~CPolylineVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void                draw() const;
                
        //--- Methods --------------------------------------------------------//
        void attach(CPolyLine*);
        virtual const CBoundingBox& getBoundingBox();
        
    private:
        
        CPolylineVisuals();        ///< Constructor is private, copy constructor must be used
        
        CPolyLine* m_pPolyline;

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

    METHOD_EXIT("CPolyLineVisuals::getBoundingBox")
    return m_pPolyline->getBoundingBox();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a polyline to polyline visuals
///
/// \param _pPolyline Polyline to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CPolylineVisuals::attach(CPolyLine* _pPolyline)
{
    METHOD_ENTRY("CPolylineVisuals::attach")

    m_pPolyline = _pPolyline;

    METHOD_EXIT("CPolylineVisuals::attach")
}

#endif
