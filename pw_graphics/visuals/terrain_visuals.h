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
/// \file       terrain_visuals.h
/// \brief      Prototype of class "CTerrainVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-12
///
////////////////////////////////////////////////////////////////////////////////

#ifndef TERRAIN_VISUALS_H
#define TERRAIN_VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "visuals.h"

//--- Misc header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the visualisation of a terrain
///
////////////////////////////////////////////////////////////////////////////////
class CTerrainVisuals : public IVisuals
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CTerrainVisuals(CDoubleBufferedShape* const);
        ~CTerrainVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals*   clone(CDoubleBufferedShape* const) const;
        virtual void        draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void                attach(CDoubleBufferedShape* const);
        const CBoundingBox& getBoundingBox();
                
    private:
        
        CTerrainVisuals();       ///< Constructor is private
        
        //--- Variables [private] --------------------------------------------//
        CDoubleBufferedShape* m_pTerrain; ///< Pointer to corresponding shape
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& CTerrainVisuals::getBoundingBox()
{
    METHOD_ENTRY("CTerrainVisuals::getBoundingBox")
    return m_pTerrain->getShapeCur()->getBoundingBox();
}

#endif
