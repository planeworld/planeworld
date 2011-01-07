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
/// \file       adams_bashforth_integrator.tpp
/// \brief      Implementation of template class "CAdamsBashforthIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
CAdamsBashforthIntegrator<T>::~CAdamsBashforthIntegrator()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
    DTOR_CALL("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
    
    METHOD_EXIT("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep
///
/// \param _V Integration vector
/// \param _fStep Timestep
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
const T CAdamsBashforthIntegrator<T>::integrate(const T& _V, const double& _fStep)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate(const Vector2d&, const double&)");

    m_Deriv[3] = m_Deriv[2];
    m_Deriv[2] = m_Deriv[1];
    m_Deriv[1] = m_Deriv[0];
    m_Deriv[0] = _V;

    m_PrevValue = m_Value;
    m_Value +=   (m_Deriv[0] * 55.0/24.0 - 
                     m_Deriv[1] * 59.0/24.0 +
                     m_Deriv[2] * 37.0/24.0 -
                     m_Deriv[3] *  3.0/ 8.0) *
                    _fStep;

    METHOD_EXIT("CAdamsBashforthIntegrator::integrate(const const Vector2d&, const double&)");
    return m_Value;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _V Initial value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
void CAdamsBashforthIntegrator<T>::init(const T& _V)
{
    METHOD_ENTRY("init(const Vector2d&)");

    m_Value = _V;
    m_PrevValue = _V;
    m_Deriv[0]=0;
    m_Deriv[1]=0;
    m_Deriv[2]=0;
    m_Deriv[3]=0;

    METHOD_EXIT("init(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _V Initial value
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline void CAdamsBashforthIntegrator<Vector2d>::init(const Vector2d& _V)
{
    METHOD_ENTRY("init(const Vector2d&)");

    m_Value = _V;
    m_PrevValue = _V;
    m_Deriv[0].setZero();
    m_Deriv[1].setZero();
    m_Deriv[2].setZero();
    m_Deriv[3].setZero();

    METHOD_EXIT("init(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
void CAdamsBashforthIntegrator<T>::reset()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::reset()");

    m_PrevValue=0;
    m_Value=0;
    m_Deriv[0]=0;
    m_Deriv[1]=0;
    m_Deriv[2]=0;
    m_Deriv[3]=0;

    METHOD_EXIT("CAdamsBashforthIntegrator::reset()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline void CAdamsBashforthIntegrator<Vector2d>::reset()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::reset()");

    m_PrevValue.setZero();
    m_Value.setZero();
    m_Deriv[0].setZero();
    m_Deriv[1].setZero();
    m_Deriv[2].setZero();
    m_Deriv[3].setZero();

    METHOD_EXIT("CAdamsBashforthIntegrator::reset()");
}
