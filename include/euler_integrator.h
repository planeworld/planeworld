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
/// \file       euler_integrator.h
/// \brief      Prototype of interface "CEulerIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef EULER_INTEGRATOR_H
#define EULER_INTEGRATOR_H

// Program header
#include "integrator.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing an integrator using Euler.
/// 
////////////////////////////////////////////////////////////////////////////////
class CEulerIntegrator : public IIntegrator
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CEulerIntegrator();
        ~CEulerIntegrator();

        //--- Constant Methods -----------------------------------------------//
        const Vector2d  getPrevValue() const;
        const Vector2d  getValue() const;

        //--- Methods --------------------------------------------------------//
        const Vector2d  integrate(const Vector2d&, const double&);
        const double    integrate(const double&, const double&);

        void            init(const Vector2d&);
        void            init(const double&);
        void            reset();

    protected:

        //--- Protected Variables --------------------------------------------//
        Vector2d    m_vecPrevValue;     ///< Calculated value of previous timestep
        Vector2d    m_vecValue;         ///< Calculated value

};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the previous timestep
///
/// \return Value of previous timestep
///
///////////////////////////////////////////////////////////////////////////////
inline const Vector2d CEulerIntegrator::getPrevValue() const
{
    METHOD_ENTRY("CEulerIntegrator::getPrevValue()");
    METHOD_EXIT("CEulerIntegrator::getPrevValue()");

    return m_vecPrevValue;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the integral
///
/// \return Value of integral
///
///////////////////////////////////////////////////////////////////////////////
inline const Vector2d CEulerIntegrator::getValue() const
{
    METHOD_ENTRY("CEulerIntegrator::getValue()");
    METHOD_EXIT("CEulerIntegrator::getValue()");

    return m_vecValue;
}

#endif
