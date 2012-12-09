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
/// \file       spring.cpp
/// \brief      Implementation of class "CSpring"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "spring.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CSpring::CSpring(): m_fC(1.0),
                m_fLength(1.0)
{
    METHOD_ENTRY("CSpring::CSpring")
    CTOR_CALL("CSpring::CSpring")
    
//  m_strName = "Spring";
//  m_Lifetime.start();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CSpring::~CSpring()
{
    METHOD_ENTRY("CSpring::~CSpring")
    DTOR_CALL("CSpring::~CSpring")

//  m_Lifetime.stop();
//  DEBUG_MSG("Object", "Lifetime (" << m_strName << "): "
//              << m_Lifetime.getDiffTime(), LOG_DOMAIN_VAR);
//  DEBUG_MSG("Object", "Gametime (" << m_strName << "): "
//              << m_Lifetime.getDiffTime(), LOG_DOMAIN_VAR);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate reaction forces
///
///////////////////////////////////////////////////////////////////////////////
void CSpring::react() const
{
    METHOD_ENTRY("CSpring::react")

    Vector2d vecTmp =   m_pObjectB->getAnchor(m_AnchorIDB) -
                        m_pObjectA->getAnchor(m_AnchorIDA);
    Vector2d vecF = vecTmp.normalized()*(vecTmp.norm()-m_fLength)*m_fC;

    m_pObjectA->addForce( vecF, m_pObjectA->getAnchor(m_AnchorIDA));
    m_pObjectB->addForce(-vecF, m_pObjectB->getAnchor(m_AnchorIDB));
}
