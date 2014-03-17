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
/// \file       debris_emitter.cpp
/// \brief      Implementation of class "CDebrisEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-18
///
////////////////////////////////////////////////////////////////////////////////

#include "debris_emitter.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisEmitter::CDebrisEmitter() : m_nNrMax(1)
{
    METHOD_ENTRY("CDebrisEmitter::CDebrisEmitter")
    CTOR_CALL("CDebrisEmitter::CDebrisEmitter")
    
    m_pDebris = new CDebris;
    MEM_ALLOC("m_pDebris")
    m_pDebris->setNumber(1);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CDebrisEmitter::~CDebrisEmitter()
{
    METHOD_ENTRY("CDebrisEmitter::~CDebrisEmitter")
    DTOR_CALL("CDebrisEmitter::~CDebrisEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the emitter
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisEmitter::init()
{
    METHOD_ENTRY("CDebrisEmitter::init")
    
    // Add debris
    m_pDataStorage->addDebris(m_pDebris);
    
    // Create visuals
    CDebrisVisuals* pDebrisVisuals = new CDebrisVisuals(m_pDebris);
    MEM_ALLOC("pDebrisVisuals")
    
    // Add visuals
    m_pDataStorage->addDebrisVisuals(pDebrisVisuals);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Starts emitting of world objects
///
/// This calls one emitation. If a timed emitation is aimed at, the method is
/// called with a positive frequency defining the engines frequency of
/// emitation. Since the actual frequency set for this emitter might be higher
/// than the frequency the method is called at, the number of debris per emit
/// is chosen accordingly.
///
/// \param _fF Engine emitation frequency.
///
///////////////////////////////////////////////////////////////////////////////
void CDebrisEmitter::emit(const double& _fF)
{
    METHOD_ENTRY("CDebrisEmitter::emit")
    
    double nNrOfDebris;
    
    if (_fF < 0.0)
    {
        nNrOfDebris=m_nNrMax;
    }
    else
    {
        double fNrOfDebris = m_fFrequency * _fF + m_fResidual;
        nNrOfDebris = static_cast<int>(fNrOfDebris);
        m_fResidual = fNrOfDebris - nNrOfDebris;
    }
    
    switch (m_EmitterDistribution)
    {
        case EMITTER_DISTRIBUTION_CIRCULAR_FIELD:
            break;
        case EMITTER_DISTRIBUTION_RECTANGULAR_FIELD:
            for (int i=0; i<nNrOfDebris; ++i)
            {
                double fX = m_UniformDist(m_Generator)*(m_fMaxX-m_fMinX) + m_fMinX;
                double fY = m_UniformDist(m_Generator)*(m_fMaxY-m_fMinY) + m_fMinY;
                m_pDebris->generate(Vector2d(fX, fY), Vector2d(0.0, 0.0));
            }
            break;
        case EMITTER_DISTRIBUTION_POINT_SOURCE:
            for (int i=0; i<nNrOfDebris; ++i)
            {
                double fAngle = m_NormalDist(m_Generator)*m_fAngleVariance + m_fAngle;
                double fVelocity = m_NormalDist(m_Generator)*m_fVelocityVariance + m_fVelocity;
                m_pDebris->generate(Vector2d(2.0,0.0), fVelocity*Vector2d(std::cos(fAngle), sin(fAngle)));
            }
    }
}
