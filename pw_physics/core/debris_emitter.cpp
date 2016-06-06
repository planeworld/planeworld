////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2016 Torsten Büschenfeld
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

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
#include "debris_visuals_thruster.h"

uint32_t CDebrisEmitter::m_unNrOfEmitters = 0;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CDebrisEmitter::CDebrisEmitter() : m_DebrisType(DEBRIS_TYPE_DOT)
{
    METHOD_ENTRY("CDebrisEmitter::CDebrisEmitter")
    CTOR_CALL("CDebrisEmitter::CDebrisEmitter")
    
    m_pDebris = new CDebris;
    MEM_ALLOC("CDebris")
    m_pDebris->setNumber(1);
    
    m_Generator.seed(m_unNrOfEmitters);
//     IHooker::m_strName += ": Debris_"+ std::to_string(m_unNrOfEmitters++);
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
    if (m_DebrisType == DEBRIS_TYPE_DOT)
    {
        CDebrisVisuals* pDebrisVisuals = new CDebrisVisuals(m_pDebris);
        MEM_ALLOC("CDebrisVisuals")
        
        // Add visuals
        m_pDataStorage->addDebrisVisuals(pDebrisVisuals);
    }
    else
    {
        CDebrisVisualsThruster* pDebrisVisuals = new CDebrisVisualsThruster(m_pDebris);
        MEM_ALLOC("CDebrisVisualsThruster")
        
        // Add visuals
        m_pDataStorage->addDebrisVisualsThruster(pDebrisVisuals);
    }
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
    
    /// \todo Activation could be checked in interface class IEmitter, calling
    ///       a myEmit method if activated
    if (m_bActive)
    {
        uint32_t nNrOfDebris;
        
        if (_fF < 0.0)
        {
            nNrOfDebris=m_nNr;
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
                for (auto i=0u; i<nNrOfDebris; ++i)
                {
                    double fX = m_UniformDist(m_Generator)*(m_fMaxX-m_fMinX) + m_fMinX;
                    double fY = m_UniformDist(m_Generator)*(m_fMaxY-m_fMinY) + m_fMinY;
                    m_pDebris->generate(Vector2d(fX, fY)+ m_KinematicsState.getOrigin(), Vector2d(0.0, 0.0));
                }
                break;
            case EMITTER_DISTRIBUTION_POINT_SOURCE:
                for (auto i=0u; i<nNrOfDebris; ++i)
                {
                    double      fAngle = m_NormalDist(m_Generator)*m_fAngleStd + m_KinematicsState.getAngle();
                    double      fVelocity = m_NormalDist(m_Generator)*m_fVelocityStd + m_fVelocity;
                    Rotation2Dd Rotation(fAngle);

//                     if (m_bIsHooked)
//                     {
//                         m_pDebris->generate(m_KinematicsState.getOrigin(),
//                                             fVelocity*(Rotation*Vector2d(0.0, 1.0)) +
//                                             0.75*static_cast<CObject*>(m_pHookable)->getVelocity());
//                     }
//                     else
                    {
                        m_pDebris->generate(m_KinematicsState.getOrigin(),
                                            fVelocity*(Rotation*Vector2d(1.0, 0.0)) + m_KinematicsState.getVelocity());
                    }
                }
        }
    }
}
