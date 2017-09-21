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
/// \file       particle_emitter.h
/// \brief      Prototype of class "CParticleEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-02-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "particle.h"
#include "emitter.h"

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for a source that emits particle.
///
////////////////////////////////////////////////////////////////////////////////
class CParticleEmitter : public IEmitter,
                         public IUniqueIDReferrer<CParticle>
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CParticleEmitter();
        ~CParticleEmitter();
        
        //--- Constant Methods -----------------------------------------------//
        void emit(const double&);
        
        const ParticleTypeType& getParticleType()  const;
              EmitterType getEmitterType() const;
        
        //--- Methods --------------------------------------------------------//
        void        init();
        void        setParticleType(const ParticleTypeType&);
        void        setNumber(const std::uint32_t&);

    private:
        
        //--- Variables [private] --------------------------------------------//
        ParticleTypeType      m_ParticleType;       ///< Type of particle
        static uint32_t     m_unNrOfEmitters;   ///< Static counter for name initialisation and tracking
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the type of particle
///
/// \return Type of particle
///
////////////////////////////////////////////////////////////////////////////////
inline const ParticleTypeType& CParticleEmitter::getParticleType() const
{
    METHOD_ENTRY("CParticleEmitter::getParticleTypeType")
    return m_ParticleType;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the emitters type - RTTI
///
/// \return Type: particle emitter
///
////////////////////////////////////////////////////////////////////////////////
inline EmitterType CParticleEmitter::getEmitterType() const
{
    METHOD_ENTRY("CParticleEmitter::getEmitterType")
    return EMITTER_PARTICLE;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the type of particle
///
/// \param _ParticleType Type of particle
///
///////////////////////////////////////////////////////////////////////////////
inline void CParticleEmitter::setParticleType(const ParticleTypeType& _ParticleType)
{
    METHOD_ENTRY("CParticleEmitter::setParticleTypeType")
    m_ParticleType = _ParticleType;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the maximum number of particle for this emitter
///
/// \param _nNrMax Maximum number of particle.
///
///////////////////////////////////////////////////////////////////////////////
inline void CParticleEmitter::setNumber(const std::uint32_t& _nNrMax)
{
    METHOD_ENTRY("CParticleEmitter::setMode")
    m_nNr = _nNrMax;
    m_pRef->setNumber(_nNrMax);
}

#endif
