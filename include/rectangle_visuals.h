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
/// \file       rectangle_visuals.h
/// \brief      Prototype of class "CRectangleVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef RECTANGLE_VISUALS_H
#define RECTANGLE_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "rectangle.h"
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a rectangle
///
////////////////////////////////////////////////////////////////////////////////
class CRectangleVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CRectangleVisuals(CRectangle*);
        ~CRectangleVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void                draw() const;
        virtual const CBoundingBox& getBoundingBox() const;
        
        //--- Methods --------------------------------------------------------//
        void attach(CRectangle*);
        
    private:
        
        CRectangleVisuals();        ///< Constructor is private, copy constructor must be used
        
        CRectangle* m_pRectangle;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CRectangleVisuals::getBoundingBox() const
{
    METHOD_ENTRY("CRectangleVisuals::getBoundingBox")

    METHOD_EXIT("CRectangleVisuals::getBoundingBox")
    return m_pRectangle->getBoundingBox();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a rectangle to rectangle visuals
///
/// \param _pRect Rectangle to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CRectangleVisuals::attach(CRectangle* _pRect)
{
    METHOD_ENTRY("CRectangleVisuals::attach")

    m_pRectangle = _pRect;

    METHOD_EXIT("CRectangleVisuals::attach")
}

#endif
