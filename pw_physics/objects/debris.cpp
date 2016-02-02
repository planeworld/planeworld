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
    
    m_PosList.reserve(DEBRIS_DEFAULT_NUMBER);
    m_VelList.reserve(DEBRIS_DEFAULT_NUMBER);
    m_PosListPrev.reserve(DEBRIS_DEFAULT_NUMBER);
    m_VelListPrev.reserve(DEBRIS_DEFAULT_NUMBER);
    m_StateList.reserve(DEBRIS_DEFAULT_NUMBER);
    
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
    
    Vector2d vecStep = m_vecForce * _fStep * m_fTimeFac;
    for (auto i=0u; i<m_PosList.size(); ++i)
    {
        // Only if state is active
        ///< \bug This doesn't work due to some vector internal bitset conversion
//         if (m_StateList[i])
        {
            m_PosListPrev[i] = m_PosList[i];
            m_VelList[i] = m_VelList[i];
            m_VelList[i] += vecStep;
            m_PosList[i] += m_VelList[i] * _fStep;
        }
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

    m_PosList.reserve(_nN);
    m_StateList.reserve(_nN);
    m_PosListPrev.reserve(_nN);
    for (auto i=0u; i<m_PosList.capacity();++i)
    {
        m_StateList.push_back(true);
    }
    m_VelList.reserve(_nN);
    m_VelListPrev.reserve(_nN);
}
