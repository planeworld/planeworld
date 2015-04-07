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
/// \file       debris_visuals_thruster.h
/// \brief      Prototype of class "CDebrisVisualsThruster"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2015-04-03
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEBRIS_VISUALS_THRUSTER_H
#define DEBRIS_VISUALS_THRUSTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "camera.h"
#include "debris.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of thruster output
///
////////////////////////////////////////////////////////////////////////////////
class CDebrisVisualsThruster : public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CDebrisVisualsThruster(CDebris*);
        ~CDebrisVisualsThruster();
        
        //--- Constant Methods -----------------------------------------------//
        void draw(const CCamera* const) const;
                
        //--- Methods --------------------------------------------------------//
        void attach(CDebris*);
        
    private:
        
        CDebrisVisualsThruster();       ///< Constructor is private
        
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
inline void CDebrisVisualsThruster::attach(CDebris* _pDebris)
{
    METHOD_ENTRY("CDebrisVisualsThruster::attach")
    m_pDebris = _pDebris;
}

#endif
