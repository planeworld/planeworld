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
