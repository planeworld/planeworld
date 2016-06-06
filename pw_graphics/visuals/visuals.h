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
#include "object.h"
#include "unique_id_referrer.h"

/// specifies the type of shape visuals
enum class ShapeVisualsType
{
    NONE,
    CIRCLE,
    PLANET,
    POLYGON,
    TERRAIN
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
/// \todo Implement getVisualsType for istream creation
///
////////////////////////////////////////////////////////////////////////////////
class IVisuals : virtual public CGraphicsBase,
                         public IUniqueIDReferrer<CDoubleBufferedShape>
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IVisuals(){}
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals* clone(CDoubleBufferedShape* const) const = 0;
        virtual void      draw(CCamera* const,
                               CObject* const) const = 0;
                               
        virtual const ShapeVisualsType  getShapeVisualsType() const;
                        
        //--- Methods --------------------------------------------------------//
        virtual const CBoundingBox& getBoundingBox();
        
        //--- Friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, IVisuals* const);
        friend std::ostream& operator<<(std::ostream&, IVisuals* const);
        
    protected:
      
        //--- Protected methods ----------------------------------------------//
        virtual std::istream& myStreamIn (std::istream&) = 0;
        virtual std::ostream& myStreamOut(std::ostream&) = 0;
        
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the shape visuals type
///
/// \return Type of shape visuals
///
////////////////////////////////////////////////////////////////////////////////
inline const ShapeVisualsType IVisuals::getShapeVisualsType() const
{
    METHOD_ENTRY("IVisuals::getShapeVisualsType")
    return ShapeVisualsType::NONE;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns the bounding box of given shape of visual
///
/// \return Bounding box
///
////////////////////////////////////////////////////////////////////////////////
inline const CBoundingBox& IVisuals::getBoundingBox()
{
    METHOD_ENTRY("IVisuals::getBoundingBox")
    return m_pRef->getShapeCur()->getBoundingBox();
}

#endif // VISUALS_H
