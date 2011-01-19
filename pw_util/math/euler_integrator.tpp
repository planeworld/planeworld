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
/// \file       euler_integrator.tpp
/// \brief      Implementation of template class "CEulerIntegrator"
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
CEulerIntegrator<T>::~CEulerIntegrator()
{
    METHOD_ENTRY("CEulerIntegrator::~CEulerIntegrator()");
    DTOR_CALL("CEulerIntegrator::~CEulerIntegrator()");
    
    METHOD_EXIT("CEulerIntegrator::~CEulerIntegrator()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep 
///
/// \param _V Integration value
/// \param _fStep Timestep
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
const T CEulerIntegrator<T>::integrate(const T& _V,
                                       const double& _fStep)
{
    METHOD_ENTRY("CEulerIntegrator::integrate")

    m_PrevValue = m_Value;
    m_Value += _V * _fStep;

    METHOD_EXIT("CEulerIntegrator::integrate")
    return m_Value;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep with clipping
///
/// This method integrates but also clips the result with respect to support
/// point. Thus, values like angles may be integrated without overflow
/// problems.
///
/// \param _V Integration value
/// \param _fStep Timestep
/// \param _Clip Min/Max Value to clip
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
const T CEulerIntegrator<T>::integrateClip(const T& _V,
                                           const double& _fStep,
                                           const T& _Clip)
{
    METHOD_ENTRY("CEulerIntegrator::integrate")

    m_PrevValue = m_Value;
    m_Value += _V * _fStep;

    int nF = floor(m_Value / _Clip);
    if (nF >= 1)
        m_Value -= nF*_Clip;
    else if (nF <= -2)
        m_Value -= (nF+1)*_Clip;
    
    METHOD_EXIT("CEulerIntegrator::integrate")
    return m_Value;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Integrates the next timestep with clipping
///
/// This method integrates but also clips the result with respect to support
/// point. Thus, values like angles may be integrated without overflow
/// problems.
///
/// \param _V Integration vector
/// \param _fStep Timestep
/// \param _Clip Min/Max Value to clip
///
/// \return New value
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline const Vector2d CEulerIntegrator<Vector2d>::integrateClip(const Vector2d& _V,
                                                                const double& _fStep,
                                                                const Vector2d& _Clip)
{
    METHOD_ENTRY("CEulerIntegrator::integrate")

    m_Value = m_Value;
    m_Value += _V * _fStep;

    int nF = floor(m_Value[0] / _Clip[0]);
    if (nF >= 1)
        m_Value[0] -= nF*_Clip[0];
    else if (nF <= -2)
        m_Value[0] -= (nF+1)*_Clip[0];
    nF = floor(m_Value[1] / _Clip[1]);
    if (nF >= 1)
        m_Value[1] -= nF*_Clip[1];
    else if (nF <= -2)
        m_Value[1] -= (nF+1)*_Clip[1];
    
    METHOD_EXIT("CEulerIntegrator::integrate")
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
void CEulerIntegrator<T>::init(const T& _V)
{
    METHOD_ENTRY("init")

    m_Value = _V;
    m_PrevValue = _V;

    METHOD_EXIT("init")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
void CEulerIntegrator<T>::reset()
{
    METHOD_ENTRY("CEulerIntegrator::reset")

    m_PrevValue=0;
    m_Value=0;

    METHOD_EXIT("CEulerIntegrator::reset")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline void CEulerIntegrator<Vector2d>::reset()
{
    METHOD_ENTRY("CEulerIntegrator::reset")

    m_PrevValue.setZero();
    m_Value.setZero();

    METHOD_EXIT("CEulerIntegrator::reset")
}