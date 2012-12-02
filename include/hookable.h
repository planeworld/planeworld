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
/// \file       hookable.h
/// \brief      Prototype of interface "IHookable"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-02
///
////////////////////////////////////////////////////////////////////////////////

#ifndef HOOKABLE_H
#define HOOKABLE_H

//--- Program header ---------------------------------------------------------//

//--- Standard header --------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Interface for camera hookable objects.
/// 
////////////////////////////////////////////////////////////////////////////////
class IHookable
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//

        //--- Constant methods -----------------------------------------------//
        virtual const double   getHookAngle() const = 0;
        virtual const Vector2i getHookCell() const = 0;
        virtual const Vector2d getHookPosition() const = 0;

        //--- Methods --------------------------------------------------------//

    protected:

        //--- Abstract methods [protected] -----------------------------------//

        //-- Variables [protected] -------------------------------------------//
};

//--- Implementation is done here for inline optimisation --------------------//

#endif