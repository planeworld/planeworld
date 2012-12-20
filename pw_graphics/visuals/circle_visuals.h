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
#include "circle.h"
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
        CCircleVisuals(CCircle*);
        ~CCircleVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void attach(CCircle*);
        virtual const CBoundingBox& getBoundingBox();
        
    private:
        
        CCircleVisuals();       ///< Constructor is private
        
        CCircle* m_pCircle;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a circle to circle visuals
///
/// \param _pCirc Circle to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CCircleVisuals::attach(CCircle* _pCirc)
{
    METHOD_ENTRY("CCircleVisuals::attach")

    m_pCircle = _pCirc;

    METHOD_EXIT("CCircleVisuals::attach")
}

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

    METHOD_EXIT("CCircleVisuals::getBoundingBox")
    return m_pCircle->getBoundingBox();
}

#endif
