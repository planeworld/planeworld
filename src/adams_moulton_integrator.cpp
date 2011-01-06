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
/// \file       adams_moulton_integrator.cpp
/// \brief      Implementation of class "CAdamsMoultonIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-04
///
////////////////////////////////////////////////////////////////////////////////

#include "adams_moulton_integrator.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CAdamsMoultonIntegrator::CAdamsMoultonIntegrator()
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::CAdamsMoultonIntegrator()");
    CTOR_CALL("CAdamsMoultonIntegrator::CAdamsMoultonIntegrator()");
    
    METHOD_EXIT("CAdamsMoultonIntegrator::CAdamsMoultonIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CAdamsMoultonIntegrator::~CAdamsMoultonIntegrator()
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::~CAdamsMoultonIntegrator()");
    DTOR_CALL("CAdamsMoultonIntegrator::~CAdamsMoultonIntegrator()");
    
    METHOD_EXIT("CAdamsMoultonIntegrator::~CAdamsMoultonIntegrator()");
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
const Vector2d CAdamsMoultonIntegrator::integrate(const Vector2d& _vecV, const double& _fStep)
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::integrate(const Vector2d&, const double&)");

    m_vecDeriv[4] = m_vecDeriv[3];
    m_vecDeriv[3] = m_vecDeriv[2];
    m_vecDeriv[2] = m_vecDeriv[1];
    m_vecDeriv[1] = m_vecDeriv[0];
    m_vecDeriv[0] = _vecV;

    m_vecPrevValue = m_vecValue;
    m_vecValue +=   (m_vecDeriv[0] * 251.0/720.0 + 
                     m_vecDeriv[1] * 646.0/720.0 -
                     m_vecDeriv[2] * 264.0/720.0 +
                     m_vecDeriv[3] * 106.0/720.0 -
                     m_vecDeriv[4] * 19.0 /720.0) *
                    _fStep;

    METHOD_EXIT("CAdamsMoultonIntegrator::integrate(const const Vector2d&, const double&)");
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
const double CAdamsMoultonIntegrator::integrate(const double& _fV, const double& _fStep)
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::integrate(const double&, const double&)");

    m_vecDeriv[4][0] = m_vecDeriv[3][0];
    m_vecDeriv[3][0] = m_vecDeriv[2][0];
    m_vecDeriv[2][0] = m_vecDeriv[1][0];
    m_vecDeriv[1][0] = m_vecDeriv[0][0];
    m_vecDeriv[0][0] = _fV;

    m_vecPrevValue[0] = m_vecValue[0];
    m_vecValue[0] += (m_vecDeriv[0][0] * 251.0/720.0 + 
                     m_vecDeriv[1][0] * 646.0/720.0 -
                     m_vecDeriv[2][0] * 264.0/720.0 +
                     m_vecDeriv[3][0] * 106.0/720.0 -
                     m_vecDeriv[4][0] * 19.0 /720.0) *
                    _fStep;

    METHOD_EXIT("CAdamsMoultonIntegrator::integrate(const double&, const double&)");
    return m_vecValue[0];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _vecV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsMoultonIntegrator::init(const Vector2d& _vecV)
{
    METHOD_ENTRY("init(const Vector2d&)");

    m_vecValue = _vecV;
    m_vecPrevValue = _vecV;
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();
    m_vecDeriv[4].setZero();

    METHOD_EXIT("init(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _fV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsMoultonIntegrator::init(const double& _fV)
{
    METHOD_ENTRY("init(const double&)");

    m_vecValue[0] = _fV;
    m_vecPrevValue[0] = _fV;
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();
    m_vecDeriv[4].setZero();

    METHOD_EXIT("init(const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
void CAdamsMoultonIntegrator::reset()
{
    METHOD_ENTRY("CAdamsMoultonIntegrator::reset()");

    m_vecPrevValue.setZero();
    m_vecValue.setZero();
    m_vecDeriv[0].setZero();
    m_vecDeriv[1].setZero();
    m_vecDeriv[2].setZero();
    m_vecDeriv[3].setZero();
    m_vecDeriv[4].setZero();

    METHOD_EXIT("CAdamsMoultonIntegrator::reset()");
}
