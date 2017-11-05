////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2014-2017 Torsten Büschenfeld
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
/// \file       particle_emitter.cpp
/// \brief      Implementation of class "CParticleEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-18
///
////////////////////////////////////////////////////////////////////////////////

#include "particle_emitter.h"

uint32_t CParticleEmitter::m_unNrOfEmitters = 0;

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CParticleEmitter::CParticleEmitter() : m_ParticleType(ParticleTypeType::DOT)
{
    METHOD_ENTRY("CParticleEmitter::CParticleEmitter")
    CTOR_CALL("CParticleEmitter::CParticleEmitter")
    
    m_hParticles.set(new CParticle);
    MEM_ALLOC("CParticle")
    m_hParticles.get()->setNumber(10);
        
    m_Generator.seed(m_unNrOfEmitters++);
//     IHooker::m_strName += ": Particle_"+ std::to_string(m_unNrOfEmitters++);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CParticleEmitter::~CParticleEmitter()
{
    METHOD_ENTRY("CParticleEmitter::~CParticleEmitter")
    DTOR_CALL("CParticleEmitter::~CParticleEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the emitter
///
///////////////////////////////////////////////////////////////////////////////
void CParticleEmitter::init()
{
    METHOD_ENTRY("CParticleEmitter::init")
    
//     if (m_pRef)
//     // Add particle
//     if (m_ParticleType == PARTICLE_TYPE_DOT)
//     {
//         m_pRef->setParticleType(PARTICLE_TYPE_DOT);
//     }
//     else
//     {
//         m_pRef->setParticleType(PARTICLE_TYPE_THRUST);
//     }
//     
//     m_pDataStorage->addParticle(m_pRef);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Starts emitting of world objects
///
/// This calls one emitation. If a timed emitation is aimed at, the method is
/// called with a positive frequency defining the engines frequency of
/// emitation. Since the actual frequency set for this emitter might be higher
/// than the frequency the method is called at, the number of particle per emit
/// is chosen accordingly.
///
/// \param _fF Engine emitation frequency
///
///////////////////////////////////////////////////////////////////////////////
void CParticleEmitter::emit(const double& _fF)
{
    METHOD_ENTRY("CParticleEmitter::emit")
    
    /// \todo Activation could be checked in interface class IEmitter, calling
    ///       a myEmit method if activated
    if (m_bActive)
    {
        uint32_t nNrOfParticle;
        
        if (_fF < 0.0)
        {
            nNrOfParticle=m_nNr;
        }
        else
        {
            double fNrOfParticle = m_fFrequency * _fF + m_fResidual;
            nNrOfParticle = static_cast<int>(fNrOfParticle);
            m_fResidual = fNrOfParticle - nNrOfParticle;
        }
        
        switch (m_EmitterDistribution)
        {
            case EmitterDistributionType::CIRCULAR_FIELD:
                break;
            case EmitterDistributionType::RECTANGULAR_FIELD:
                for (auto i=0u; i<nNrOfParticle; ++i)
                {
                    double fX = m_UniformDist(m_Generator)*(m_fMaxX-m_fMinX) + m_fMinX;
                    double fY = m_UniformDist(m_Generator)*(m_fMaxY-m_fMinY) + m_fMinY;
                    m_hParticles.get()->generate(Vector2d(fX, fY)+ m_KinematicsState.getOrigin(), Vector2d(0.0, 0.0));
                }
                break;
            case EmitterDistributionType::POINT_SOURCE:
                for (auto i=0u; i<nNrOfParticle; ++i)
                {
                    double      fAngle = m_NormalDist(m_Generator)*m_fAngleStd + m_KinematicsState.getAngle();
                    double      fVelocity = (m_NormalDist(m_Generator)*m_fVelocityStd + m_fVelocity)*m_fIntensity;
                    Rotation2Dd Rotation(fAngle);

                    (m_pDataStorage->getParticleByValueBack(m_hParticles.getUID())->generate(m_KinematicsState.getOrigin(),
                                                   fVelocity*(Rotation*Vector2d(1.0, 0.0)) + m_fVelocityInheritance * m_KinematicsState.getVelocity()));
                }
                break;
            default:
                break;
        }
    }
}
