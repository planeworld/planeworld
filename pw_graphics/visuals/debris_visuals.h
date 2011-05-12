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

    METHOD_EXIT("CDebrisVisuals::attach")
}

#endif
