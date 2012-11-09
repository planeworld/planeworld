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
    
    METHOD_EXIT("CDebris::CDebris")
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
    double fStep = _fStep*m_fTimeFac;
    while (it != m_PosList.end())
    {
        (*itPrev) = (*it);
        (*jtPrev) = (*jt);
        (*jt) += Vector2d(0.0,1.81) * fStep;
        (*it) += (*jt) * _fStep;
        ++it; ++jt; ++itPrev; ++jtPrev;
    }

    METHOD_EXIT("CDebris::dynamics")
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

    METHOD_EXIT("CDebris::init")
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
    m_PosListPrev.resize(_nN);
    for (int i=0; i<m_PosList.capacity();++i)
        m_PosList.push_back(Vector2d((rand()%20000000) - 10000000,(rand()%20000000) - 10000000)/100000);
    m_VelList.resize(_nN);
    m_VelListPrev.resize(_nN);

    METHOD_EXIT("CDebris::setNumber")
}
