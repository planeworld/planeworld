////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2016 Torsten Büschenfeld
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
/// \file       debris_visuals.h
/// \brief      Prototype of class "CDebrisVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-10
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_VISUALS_H
#define DEBRIS_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "debris.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a debris
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisVisuals : public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisVisuals(CDebris*);
        ~CDebrisVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        void draw(const CCamera* const) const;
                
        //--- Methods --------------------------------------------------------//
        void attach(CDebris*);
        
    private:
        
        CDebrisVisuals();       ///< Constructor is private
        
        CDebris* m_pDebris;

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a debris to debris visuals
///
/// \param _pDebris Debris to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CDebrisVisuals::attach(CDebris* _pDebris)
{
    METHOD_ENTRY("CDebrisVisuals::attach")
    m_pDebris = _pDebris;
}

#endif
