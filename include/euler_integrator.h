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
template <class T>
class CEulerIntegrator : public IIntegrator<T>
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        ~CEulerIntegrator();

        //--- Constant Methods -----------------------------------------------//
        const T getPrevValue() const;
        const T getValue() const;

        //--- Methods --------------------------------------------------------//
        const T integrate(const T&, const double&);
        const T integrateClip(const T&, const double&, const T&);

        void    init(const T&);
        void    reset();

    protected:

        //--- Protected Variables --------------------------------------------//
        T   m_PrevValue;     ///< Calculated value of previous timestep
        T   m_Value;         ///< Calculated value

};

//--- Implementation is done here for inline optimisation --------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the previous timestep
///
/// \return Value of previous timestep
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
inline const T CEulerIntegrator<T>::getPrevValue() const
{
    METHOD_ENTRY("CEulerIntegrator::getPrevValue()");
    METHOD_EXIT("CEulerIntegrator::getPrevValue()");

    return m_PrevValue;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the integral
///
/// \return Value of integral
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
inline const T CEulerIntegrator<T>::getValue() const
{
    METHOD_ENTRY("CEulerIntegrator::getValue()");
    METHOD_EXIT("CEulerIntegrator::getValue()");

    return m_Value;
}

//--- Implementation of template members -------------------------------------//
#include "euler_integrator.tpp"

#endif
