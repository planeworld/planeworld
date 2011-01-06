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
/// \file       euler_integrator.cpp
/// \brief      Implementation of class "CEulerIntegrator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "euler_integrator.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CEulerIntegrator::CEulerIntegrator()
{
    METHOD_ENTRY("CEulerIntegrator::CEulerIntegrator()");
    CTOR_CALL("CEulerIntegrator::CEulerIntegrator()");
    
    METHOD_EXIT("CEulerIntegrator::CEulerIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CEulerIntegrator::~CEulerIntegrator()
{
    METHOD_ENTRY("CEulerIntegrator::~CEulerIntegrator()");
    DTOR_CALL("CEulerIntegrator::~CEulerIntegrator()");
    
    METHOD_EXIT("CEulerIntegrator::~CEulerIntegrator()");
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
const Vector2d CEulerIntegrator::integrate(const Vector2d& _vecV, const double& _fStep)
{
    METHOD_ENTRY("CEulerIntegrator::integrate(const Vector2d&, const double&)");

    m_vecPrevValue = m_vecValue;
    m_vecValue += _vecV * _fStep;
    
    METHOD_EXIT("CEulerIntegrator::integrate(const const Vector2d&, const double&)");
    
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
const double CEulerIntegrator::integrate(const double& _fV, const double& _fStep)
{
    METHOD_ENTRY("CEulerIntegrator::integrate(const double&, const double&)");

    m_vecPrevValue[0] = m_vecValue[0];
    m_vecValue[0] += _fV * _fStep;

    METHOD_EXIT("CEulerIntegrator::integrate(const double&, const double&)");
    
    return m_vecValue[0];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _vecV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CEulerIntegrator::init(const Vector2d& _vecV)
{
    METHOD_ENTRY("init(const Vector2d&)");

    m_vecValue = _vecV;
    m_vecPrevValue = _vecV;

    METHOD_EXIT("init(const Vector2d&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initializes integrator with given value
///
/// \param _fV Initial value
///
///////////////////////////////////////////////////////////////////////////////
void CEulerIntegrator::init(const double& _fV)
{
    METHOD_ENTRY("init(const double&)");

    m_vecValue[0] = _fV;
    m_vecPrevValue[0] = _fV;

    METHOD_EXIT("init(const double&)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
void CEulerIntegrator::reset()
{
    METHOD_ENTRY("CEulerIntegrator::reset()");

    m_vecPrevValue.setZero();
    m_vecValue.setZero();

    METHOD_EXIT("CEulerIntegrator::reset()");
}
