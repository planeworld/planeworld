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
class CAdamsMoultonIntegrator : public IIntegrator
{
    
    public:
    
        //--- Constructor/Destructor -----------------------------------------//
        CAdamsMoultonIntegrator();
        ~CAdamsMoultonIntegrator();

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
        Vector2d    m_vecDeriv[5];      ///< Derivatives of previous timesteps
        Vector2d    m_vecPrevValue;     ///< Calculated value of previous timestep
        Vector2d    m_vecValue;         ///< Calculated value

};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the previous timestep
///
/// \return Value of previous timestep
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d CAdamsMoultonIntegrator::getPrevValue() const
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::getPrevValue")
    METHOD_EXIT("CAdamsMoultonIntegrator::getPrevValue")
    return m_vecPrevValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns value of the integral
///
/// \return Value of integral
///
////////////////////////////////////////////////////////////////////////////////
inline const Vector2d CAdamsMoultonIntegrator::getValue() const
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::getValue")
    METHOD_EXIT("CAdamsMoultonIntegrator::getValue")
    return m_vecValue;
}

#endif
