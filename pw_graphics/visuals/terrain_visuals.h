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
#include "terrain.h"
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
        CTerrainVisuals(CTerrain*);
        ~CTerrainVisuals();
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const, const IObject* const) const;
                
        //--- Methods --------------------------------------------------------//
        void                attach(CTerrain*);
        const CBoundingBox& getBoundingBox();
                
    private:
        
        CTerrainVisuals();       ///< Constructor is private
        
        //--- Variables [private] --------------------------------------------//
        CTerrain*            m_pTerrain;          ///< Pointer to corresponding shape
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Attaches a terrain to terrain visuals
///
/// \param _pTerrain Terrain to attach
///
////////////////////////////////////////////////////////////////////////////////
inline void CTerrainVisuals::attach(CTerrain* _pTerrain)
{
    METHOD_ENTRY("CTerrainVisuals::attach")

    m_pTerrain = _pTerrain;

    METHOD_EXIT("CTerrainVisuals::attach")
}

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

    METHOD_EXIT("CTerrainVisuals::getBoundingBox")
    return m_pTerrain->getBoundingBox();
}

#endif
