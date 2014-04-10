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
/// \file       adams_bashforth_integrator.h
/// \brief      Prototype of interface "CAdamsBashforthIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ADAMS_BASHFORTH_INTEGRATOR_H
#define ADAMS_BASHFORTH_INTEGRATOR_H

//--- Program header ---------------------------------------------------------//
#include "integrator.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing an Adams-Bashforth integrator
///
/// This integrator is a multistep integrator, using multiple steps from the
/// past. In this case, the 4th order Adams-Bashforth integrator is used.
/// 
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CAdamsBashforthIntegrator : public IIntegrator<T>
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CAdamsBashforthIntegrator();
        ~CAdamsBashforthIntegrator();

        //--- Constant Methods -----------------------------------------------//
        IIntegrator<T>* clone() const;
        const T         getPrevValue() const;
        const T         getValue() const;

        //--- Methods --------------------------------------------------------//
        const T  integrate(const T&, const double&);
        const T  integrateClip(const T&, const double&, const T&);

        void     init(const T&);
        void     reset();

    protected:

        //--- Protected Variables --------------------------------------------//
        T    m_Deriv[4];      ///< Derivatives of previous timesteps
        T    m_PrevValue;     ///< Calculated value of previous timestep
        T    m_Value;         ///< Calculated value

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the previous timestep
///
/// \return Value of previous timestep
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline const T CAdamsBashforthIntegrator<T>::getPrevValue() const
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::getPrevValue")
    return m_PrevValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the integral
///
/// \return Value of integral
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
inline const T CAdamsBashforthIntegrator<T>::getValue() const
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::getValue")
    return m_Value;
}

//--- Implementation of template members -------------------------------------//
#include "adams_bashforth_integrator.tpp"

#endif
