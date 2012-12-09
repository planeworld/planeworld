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
/// \file       spring_visuals.h
/// \brief      Prototype of class "CSpringVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-11
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SPRING_VISUALS_H
#define SPRING_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "spring.h"
#include "visuals.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a spring
///
////////////////////////////////////////////////////////////////////////////////
class CSpringVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CSpringVisuals(CSpring*);
        ~CSpringVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void                draw(const CCamera* const) const;
                
        //--- Methods --------------------------------------------------------//
                void                attach(CSpring*);
        virtual const CBoundingBox& getBoundingBox();
        
    private:
        
        CSpringVisuals(); ///< Constructor is private, copy constructor must be used
        
        CBoundingBox    m_BoundingBox; ///< Bounding box of spring
        CSpring*        m_pSpring;     ///< Pointer to physical object

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a spring to spring visuals
///
/// \param _pCirc Spring to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CSpringVisuals::attach(CSpring* _pSpring)
{
    METHOD_ENTRY("CSpringVisuals::attach")

    m_pSpring = _pSpring;
}

#endif
