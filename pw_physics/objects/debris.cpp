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
    
    m_PosList.resize(DEBRIS_DEFAULT_NUMBER);
    m_VelList.resize(DEBRIS_DEFAULT_NUMBER);
    
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
    
    std::vector<Vector2d >::iterator it=m_PosList.begin();
    std::vector<Vector2d >::iterator jt=m_VelList.begin();
    double fStep = _fStep*m_fTimeFac;
    while (it != m_PosList.end())
    {
        (*jt) += Vector2d(0.0,-9.81)/*Vector2d(double((rand()%2000) - 1000)/100,double((rand()%2000) - 1000)/100)*/ * fStep;
        (*it) += (*jt) * _fStep;
        ++it; ++jt;
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

