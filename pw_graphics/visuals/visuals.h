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
/// \file       visuals.h
/// \brief      Prototype of class "IVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#ifndef VISUALS_H
#define VISUALS_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "camera.h"
// #include "key_map.h"
// #include "object.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
////////////////////////////////////////////////////////////////////////////////
class IVisuals : virtual public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IVisuals(){}
        
        //--- Constant Methods -----------------------------------------------//
        virtual void draw(const CCamera* const) const = 0;
        
//         const Vector2i getCell() const;
                
        //--- Methods --------------------------------------------------------//
        virtual const CBoundingBox& getBoundingBox() = 0;
        
    protected:
        
//         IObject* m_pObject;     ///< Pointer to corresponding object

};

// typedef KeyType VisualsIDType;
// typedef std::list<IVisuals*> VisualsListType;
// typedef std::list<VisualsIDType> VisualsIDListType;

//--- Implementation is done here for inline optimisation --------------------//

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Returns the cell that the visual is located at
// ///
// /// \return Cell the visual is located
// ///
// ////////////////////////////////////////////////////////////////////////////////
// inline const Vector2i IVisuals::getCell() const
// {
//     METHOD_ENTRY("IVisuals::getCell")
//     return m_pObject->getCell();
// }

#endif
