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
/// \file       adams_bashforth_integrator.cpp
/// \brief      Implementation of class "CAdamsBashforthIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "adams_bashforth_integrator.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CAdamsBashforthIntegrator::CAdamsBashforthIntegrator()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator()");
    CTOR_CALL("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator()");
    
    METHOD_EXIT("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
    DTOR_CALL("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
    
    METHOD_EXIT("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep
///
/// \param _vecV Integration vector
/// \param _fStep Timestep
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
const Vector2d CAdamsBashforthIntegrator::integrate(const Vector2d& _vecV, const double& _fStep)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate(const Vector2d&, const double&)");

    m_vecDeriv[3] = m_vecDeriv[2];
    m_vecDeriv[2] = m_vecDeriv[1];
    m_vecDeriv[1] = m_vecDeriv[0];
    m_vecDeriv[0] = _vecV;

    m_vecPrevValue = m_vecValue;
    m_vecValue +=   (m_vecDeriv[0] * 55.0/24.0 - 
                     m_vecDeriv[1] * 59.0/24.0 +
                     m_vecDeriv[2] * 37.0/24.0 -
                     m_vecDeriv[3] *  3.0/ 8.0) *
                    _fStep;

    METHOD_EXIT("CAdamsBashforthIntegrator::integrate(const const Vector2d&, const double&)");
    return m_vecValue;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep
///
/// \param _fValue Integration value
/// \param _fStep Timestep
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
const double CAdamsBashforthIntegrator::integrate(const double& _fV, const double& _fStep)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate(const double&, const double&)");

    m_vecDeriv[3][0] = m_vecDeriv[2][0];
    m_vecDeriv[2][0] = m_vecDeriv[1][0];
    m_vecDeriv[1][0] = m_vecDeriv[0][0];
    m_vecDeriv[0][0] = _fV;

    m_vecPrevValue[0] = m_vecValue[0];
    m_vecValue[0] += _fStep *
                    (55.0/24.0 * m_vecDeriv[0][0] -
                     59.0/24.0 * m_vecDeriv[1][0] +
                     37.0/24.0 * m_vecDeriv[2][0] -
                      3.0/ 8.0 * m_vecDeriv[3][0]);

    METHOD_EXIT("CAdamsBashforthIntegrator::integrate(const double&, const double&)");
    return m_vecValue[0];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _vecV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsBashforthIntegrator::init(const Vector2d& _vecV)
{
    METHOD_ENTRY("init(const Vector2d&)");

    m_vecValue = _vecV;
    m_vecPrevValue = _vecV;
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();

    METHOD_EXIT("init(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _fV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsBashforthIntegrator::init(const double& _fV)
{
    METHOD_ENTRY("init(const double&)");

    m_vecValue[0] = _fV;
    m_vecPrevValue[0] = _fV;
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();

    METHOD_EXIT("init(const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsBashforthIntegrator::reset()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::reset()");

    m_vecPrevValue.setZero();
    m_vecValue.setZero();
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();

    METHOD_EXIT("CAdamsBashforthIntegrator::reset()");
}
