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
#include <eigen3/Eigen/Core>

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
template <class T>
class IIntegrator
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        virtual ~IIntegrator(){};
        
        //--- Constant Methods -----------------------------------------------//
        virtual IIntegrator<T>* clone() const = 0;
        virtual const T         getPrevValue() const = 0;
        virtual const T         getValue() const = 0;

        //--- Methods --------------------------------------------------------//
        virtual const T integrate(const T&, const double&) = 0;
        virtual const T integrateClip(const T&, const double&, const T&) = 0;
        virtual void    init(const T&) = 0;
        virtual void    reset() = 0;
        
        //--- friends --------------------------------------------------------//
        template <class U>
        friend std::istream& operator>>(std::istream&, IIntegrator<U>* const);
        template <class U>
        friend std::ostream& operator<<(std::ostream&, IIntegrator<U>* const);

    protected:
      
        //--- Protected Methods ----------------------------------------------//
        virtual std::istream& myStreamIn(std::istream&) = 0;
        virtual std::ostream& myStreamOut(std::ostream&) = 0;

        //--- Protected Variables --------------------------------------------//

};

#include "integrator.tpp"

#endif
