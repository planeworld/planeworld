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
/// \file       debris.cpp
/// \brief      Implementation of class "CDebris"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-05
///
////////////////////////////////////////////////////////////////////////////////

#include "debris.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CDebris::CDebris() : m_fTimeFac(1.0)
{
    METHOD_ENTRY("CDebris::CDebris")
    CTOR_CALL("CDebris::CDebris")
    
    m_PosList.set_capacity(DEBRIS_DEFAULT_NUMBER);
    m_VelList.set_capacity(DEBRIS_DEFAULT_NUMBER);
    m_PosListPrev.set_capacity(DEBRIS_DEFAULT_NUMBER);
    m_VelListPrev.set_capacity(DEBRIS_DEFAULT_NUMBER);
    m_StateList.set_capacity(DEBRIS_DEFAULT_NUMBER);
    
    m_vecForce.setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculates dynamics of the debris
///
/// This method calculates the dynamics -- acceleration, velocity, position --
/// of all debris in the list
///
///////////////////////////////////////////////////////////////////////////////
void CDebris::dynamics(const double& _fStep)
{
    METHOD_ENTRY("CDebris::dynamics")
    
    boost::circular_buffer<Vector2d>::iterator it=m_PosList.begin();
    boost::circular_buffer<Vector2d>::iterator jt=m_VelList.begin();
    boost::circular_buffer<Vector2d>::iterator itPrev=m_PosListPrev.begin();
    boost::circular_buffer<Vector2d>::iterator jtPrev=m_VelListPrev.begin();
    boost::circular_buffer<bool>::const_iterator ci=m_StateList.begin();
    Vector2d vecStep = m_vecForce * _fStep * m_fTimeFac;
    while (it != m_PosList.end())
    {
        // Only if state is active
        if ((*ci))
        {
            (*itPrev) = (*it);
            (*jtPrev) = (*jt);
            (*jt) += vecStep;
            (*it) += (*jt) * _fStep;
        }
        ++it; ++jt; ++itPrev; ++jtPrev; ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Generate a new debris
///
/// This method generates a new debris. Because of using a circular buffer, a
/// new debris might overwrite the oldest one if the maximum number of debris
/// is reached.
///
/// \param _vecP Position of the new debris
/// \param _vecV Velocity of the new debris
///
///////////////////////////////////////////////////////////////////////////////
void CDebris::generate(const Vector2d& _vecP, const Vector2d& _vecV)
{
    METHOD_ENTRY("CDebris::generate")
    
    m_PosList.push_back(_vecP);
    m_PosListPrev.push_back(_vecP);
    m_VelList.push_back(_vecV);
    m_VelListPrev.push_back(_vecV);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Specific initialisation
///
/// This method initialises it's specific members.
///
///////////////////////////////////////////////////////////////////////////////
void CDebris::init()
{
    METHOD_ENTRY("CDebris::init")

//     m_fAngle = 0.0;
//     m_pIntAng->reset();
//     m_pIntAngVel->reset();
//     
//     for (std::list< IShape* >::const_iterator ci = m_Geometry.getShapes().begin();
//         ci != m_Geometry.getShapes().end(); ++ci)
//     {
//         (*ci)->transform(m_fAngle, m_pIntPos->getValue());
// 
//         // Update depthlayers
//         m_nDepthlayers |= (*ci)->getDepths();
// 
//         // Update bounding box of current time step
//         m_Geometry.updateBoundingBox((*ci)->getBoundingBox());
//     }
//     // Copy geometry to previous timestep
//     m_Geometry.update();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set number of debris
///
/// \param _nN Number of debris
///
////////////////////////////////////////////////////////////////////////////////
void CDebris::setNumber(const int& _nN)
{
    METHOD_ENTRY("CDebris::setNumber")

    m_PosList.set_capacity(_nN);
    m_StateList.set_capacity(_nN);
    m_PosListPrev.set_capacity(_nN);
    for (auto i=0u; i<m_PosList.capacity();++i)
    {
        m_StateList.push_back(true);
    }
    m_VelList.set_capacity(_nN);
    m_VelListPrev.set_capacity(_nN);
}
