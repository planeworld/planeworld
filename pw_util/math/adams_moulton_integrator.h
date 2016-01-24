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
/// \file       adams_moulton_integrator.h
/// \brief      Prototype of interface "CAdamsMoultonIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-04
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ADAMS_MOULTON_INTEGRATOR_H
#define ADAMS_MOULTON_INTEGRATOR_H

//--- Program header ---------------------------------------------------------//
#include "integrator.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class representing an Adams-Moulton integrator
///
/// This integrator is a multistep integrator, using multiple steps from the
/// past. In this case, the 4th order Adams-Moulton integrator is used.
/// 
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CAdamsMoultonIntegrator : public IIntegrator<T>
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CAdamsMoultonIntegrator();
        ~CAdamsMoultonIntegrator();

        //--- Constant Methods -----------------------------------------------//
        IIntegrator<T>* clone() const;
        const T         getPrevValue() const;
        const T         getValue() const;

        //--- Methods --------------------------------------------------------//
        const T integrate(const T&, const double&);
        const T integrateClip(const T&, const double&, const T&);

        void    init(const T&);
        void    reset();

    protected:
      
        //--- Protected methods ----------------------------------------------//
        std::istream& myStreamIn(std::istream&);
        std::ostream& myStreamOut(std::ostream&);

        //--- Protected Variables --------------------------------------------//
        T   m_Deriv[5];      ///< Derivatives of previous timesteps
        T   m_PrevValue;     ///< Calculated value of previous timestep
        T   m_Value;         ///< Calculated value

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
inline const T CAdamsMoultonIntegrator<T>::getPrevValue() const
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::getPrevValue")
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
inline const T CAdamsMoultonIntegrator<T>::getValue() const
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::getValue")
    return m_Value;
}

//--- Implementation of template members -------------------------------------//

#include "adams_moulton_integrator.tpp"

#endif
