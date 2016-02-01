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
#include "object.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for visualisation of world objects
///
/// \todo Implement getVisualsType for istream creation
///
////////////////////////////////////////////////////////////////////////////////
class IVisuals : virtual public CGraphicsBase
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IVisuals(){}
        
        //--- Constant Methods -----------------------------------------------//
        virtual IVisuals* clone(CDoubleBufferedShape* const) const = 0;
        virtual void      draw(CCamera* const,
                               const IObject* const) const = 0;
                        
        //--- Methods --------------------------------------------------------//
        virtual void                attach(CDoubleBufferedShape* const) = 0;
        virtual const CBoundingBox& getBoundingBox() = 0;
        
        //--- Friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, IVisuals* const);
        friend std::ostream& operator<<(std::ostream&, IVisuals* const);
        
    protected:

        //--- Protected methods ----------------------------------------------//
        virtual std::istream& myStreamIn (std::istream&) = 0;
        virtual std::ostream& myStreamOut(std::ostream&) = 0;
        
};


#endif
