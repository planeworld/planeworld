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
/// \file       integrator.h
/// \brief      Prototype of interface "IIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "eigen2/Eigen/Core"

using namespace Eigen;

/// specifies the type of line to be drawn
typedef enum 
{
    INTEGRATOR_EULER,
    INTEGRATOR_ADAMS_BASHFORTH,
    INTEGRATOR_ADAMS_MOULTON
} IntegratorType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Abstract class representing an integrator
/// 
////////////////////////////////////////////////////////////////////////////////
class IIntegrator
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        IIntegrator() {};                               ///< Constructor
        virtual ~IIntegrator(){};                       ///< Destructor
        
        //--- Constant Methods -----------------------------------------------//
        virtual const Vector2d  getPrevValue() const = 0;
        virtual const Vector2d  getValue() const = 0;

        //--- Methods --------------------------------------------------------//
        virtual const Vector2d  integrate(const Vector2d&, const double&) = 0;
        virtual const double    integrate(const double&, const double&) = 0;
        virtual void            init(const Vector2d&) = 0;
        virtual void            reset() = 0;

    protected:

        //--- Protected Variables --------------------------------------------//

};

#endif
