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
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
CAdamsBashforthIntegrator<T>::CAdamsBashforthIntegrator() : m_PrevValue(0.0), m_Value(0.0)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator")
    CTOR_CALL("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator")
    
    m_Deriv[0] = 0.0;
    m_Deriv[1] = 0.0;
    m_Deriv[2] = 0.0;
    m_Deriv[3] = 0.0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline CAdamsBashforthIntegrator<Vector2d>::CAdamsBashforthIntegrator()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator")
    CTOR_CALL("CAdamsBashforthIntegrator::CAdamsBashforthIntegrator")
    
    m_Deriv[0].setZero();
    m_Deriv[1].setZero();
    m_Deriv[2].setZero();
    m_Deriv[3].setZero();
    m_PrevValue.setZero();
    m_Value.setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
CAdamsBashforthIntegrator<T>::~CAdamsBashforthIntegrator()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator")
    DTOR_CALL("CAdamsBashforthIntegrator::~CAdamsBashforthIntegrator")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones integrator
///
/// \return Pointer to cloned integrator
///
////////////////////////////////////////////////////////////////////////////////
template <class T>
IIntegrator<T>* CAdamsBashforthIntegrator<T>::clone() const
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::clone")
    
    CAdamsBashforthIntegrator<T>* pClone = new CAdamsBashforthIntegrator;
    MEM_ALLOC("CAdamsBashforthIntegrator")
        
    (*pClone) = (*this);
    
    return pClone;
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
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate")

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
const T CAdamsBashforthIntegrator<T>::integrateClip(const T& _V,
                                                    const double& _fStep,
                                                    const T& _Clip)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate")

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
               
    int nF = floor(m_Value / _Clip);
    if (nF >= 1)
        m_Value -= nF*_Clip;
    else if (nF <= -2)
        m_Value -= (nF+1)*_Clip;

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
inline const Vector2d CAdamsBashforthIntegrator<Vector2d>::integrateClip(const Vector2d& _V,
                                                                         const double& _fStep,
                                                                         const Vector2d& _Clip)
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::integrate")

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
    METHOD_ENTRY("init")

    m_Value = _V;
    m_PrevValue = _V;
    m_Deriv[0]=0;
    m_Deriv[1]=0;
    m_Deriv[2]=0;
    m_Deriv[3]=0;
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
    METHOD_ENTRY("init")

    m_Value = _V;
    m_PrevValue = _V;
    m_Deriv[0].setZero();
    m_Deriv[1].setZero();
    m_Deriv[2].setZero();
    m_Deriv[3].setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <class T>
void CAdamsBashforthIntegrator<T>::reset()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::reset")

    m_PrevValue=0;
    m_Value=0;
    m_Deriv[0]=0;
    m_Deriv[1]=0;
    m_Deriv[2]=0;
    m_Deriv[3]=0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Reset the integrator, i.e. clear it's last value
///
///////////////////////////////////////////////////////////////////////////////
template <>
inline void CAdamsBashforthIntegrator<Vector2d>::reset()
{
    METHOD_ENTRY("CAdamsBashforthIntegrator::reset")

    m_PrevValue.setZero();
    m_Value.setZero();
    m_Deriv[0].setZero();
    m_Deriv[1].setZero();
    m_Deriv[2].setZero();
    m_Deriv[3].setZero();
}
