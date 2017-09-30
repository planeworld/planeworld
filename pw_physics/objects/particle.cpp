////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2011-2017 Torsten Büschenfeld
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
/// \file       particle.cpp
/// \brief      Implementation of class "CParticle"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-05-05
///
////////////////////////////////////////////////////////////////////////////////

#include "particle.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, initialising members
///
///////////////////////////////////////////////////////////////////////////////
CParticle::CParticle() : IGridUser(),
                     IUniqueIDUser(),
                     m_ParticleType(ParticleTypeType::DOT),
                     m_fTimeFac(1.0)
{
    METHOD_ENTRY("CParticle::CParticle")
    CTOR_CALL("CParticle::CParticle")
    
    m_PosList.reserve(PARTICLE_DEFAULT_NUMBER);
    m_VelList.reserve(PARTICLE_DEFAULT_NUMBER);
    m_PosListPrev.reserve(PARTICLE_DEFAULT_NUMBER);
    m_StateList.reserve(PARTICLE_DEFAULT_NUMBER);
    
    m_vecForce.setZero();
    
    m_UID.setName("Particle_"+m_UID.getName());
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor
///
/// \param _Particle Particle to be copied
///
///////////////////////////////////////////////////////////////////////////////
CParticle::CParticle(const CParticle& _Particle) : IGridUser(_Particle),
                                                   IUniqueIDUser(_Particle)
                                           
{
    METHOD_ENTRY("CParticle::CParticle")
    CTOR_CALL("CParticle::CParticle")
    
    this->copy(_Particle);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones particle
///
/// \return Pointer to cloned particle
///
////////////////////////////////////////////////////////////////////////////////
CParticle* CParticle::clone() const
{
    METHOD_ENTRY("CParticle::clone")
    
    CParticle* pClone = new CParticle(*this);
    MEM_ALLOC("CParticle")

    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _Particle Particle to be copied and assigned
///
/// \return Copied particle to be assigned
///
////////////////////////////////////////////////////////////////////////////////
CParticle& CParticle::operator=(const CParticle& _Particle)
{
    METHOD_ENTRY("CParticle::operator=")
    IUniqueIDUser::operator=(_Particle);
    IGridUser::operator=(_Particle);
    this->copy(_Particle);
    
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculates dynamics of the particle
///
/// This method calculates the dynamics -- acceleration, velocity, position --
/// of all particle in the list
///
///////////////////////////////////////////////////////////////////////////////
void CParticle::dynamics(const double& _fStep)
{
    METHOD_ENTRY("CParticle::dynamics")
    
    if (m_PosList.size() > 0)
    {
        m_BBox.setLowerLeft(m_PosList[0]);
        m_BBox.setUpperRight(m_PosList[0]);
        
        Vector2d vecStep = m_vecForce * _fStep * m_fTimeFac;
        for (auto i=0u; i<m_PosList.size(); ++i)
        {
            // Only if state is active
            if (m_StateList[i] == PARTICLE_STATE_ACTIVE)
            {
                m_BBox.update(m_PosList[i]);
                m_PosListPrev[i] = m_PosList[i];
                m_VelList[i] += vecStep;
                m_PosList[i] += m_VelList[i] * _fStep;
                m_BBox.update(m_PosList[i]);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Generate a new particle
///
/// This method generates a new particle. Because of using a circular buffer, a
/// new particle might overwrite the oldest one if the maximum number of particle
/// is reached.
///
/// \param _vecP Position of the new particle
/// \param _vecV Velocity of the new particle
///
///////////////////////////////////////////////////////////////////////////////
void CParticle::generate(const Vector2d& _vecP, const Vector2d& _vecV)
{
    METHOD_ENTRY("CParticle::generate")
    
    m_PosList.push_back(_vecP);
    m_PosListPrev.push_back(_vecP);
    m_VelList.push_back(_vecV);
    m_StateList.push_back(PARTICLE_STATE_ACTIVE);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set number of particle
///
/// \param _nN Number of particle
///
////////////////////////////////////////////////////////////////////////////////
void CParticle::setNumber(const int& _nN)
{
    METHOD_ENTRY("CParticle::setNumber")

    m_PosList.reserve(_nN);
    m_StateList.reserve(_nN);
    m_PosListPrev.reserve(_nN);
    for (auto i=0u; i<m_PosList.capacity();++i)
    {
        m_StateList.push_back(PARTICLE_STATE_ACTIVE);
    }
    m_VelList.reserve(_nN);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pParticle CParticle instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CParticle* const _pParticle)
{
    METHOD_ENTRY("CParticle::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    /// \todo Stream data from IGridUser
    
    // From IUniqueIDUser
    _is >> _pParticle->m_UID;
    
    strTmp="";
    _is >> strTmp; _pParticle->m_ParticleType = mapStringToParticleType(strTmp);
    
    _is >> _pParticle->m_Lifetime;
    _is >> _pParticle->m_fTimeFac;
    
    _is >> _pParticle->m_PosList;
    _is >> _pParticle->m_PosListPrev;
    _is >> _pParticle->m_VelList;
    _is >> _pParticle->m_StateList;
    
    _is >> _pParticle->m_BBox;
    
    _is >> _pParticle->m_fDamping;
    _is >> _pParticle->m_nDepthlayers;
    _is >> _pParticle->m_vecForce[0] >> _pParticle->m_vecForce[1];
    
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _pParticle CParticle instance to stream
///
/// \return Stream with game state information of CParticle instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CParticle* const _pParticle)
{
    METHOD_ENTRY("CParticle::operator<<")
    
    _os << "Particle:" << std::endl;
    
    /// \todo Stream data from IGridUser
    
    // From IUniqueIDUser
    _os << _pParticle->m_UID << std::endl;
    
    _os << s_ParticleTypeToStringMap.at(_pParticle->getParticleType()) << std::endl;
    
    _os << _pParticle->m_Lifetime << std::endl;
    _os << _pParticle->m_fTimeFac << std::endl;
    
    _os << _pParticle->m_PosList << std::endl;
    _os << _pParticle->m_PosListPrev << std::endl;
    _os << _pParticle->m_VelList << std::endl;
    _os << _pParticle->m_StateList << std::endl;
    
    _os << _pParticle->m_StateList << std::endl;
    
    _os << _pParticle->m_fDamping << std::endl;
    _os << _pParticle->m_nDepthlayers << std::endl;
    _os << _pParticle->m_vecForce[0] << " " << _pParticle->m_vecForce[1] << std::endl;
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies particle
///
/// \param _Particle Particle to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CParticle::copy(const CParticle& _Particle) 
{
    METHOD_ENTRY("CParticle::copy")
    
    //--- Variables of CParticle -----------------------------------------------//
    m_PosList = _Particle.m_PosList;
    m_PosListPrev = _Particle.m_PosListPrev;
    m_VelList = _Particle.m_VelList;
    m_StateList = _Particle.m_StateList;
    m_BBox = _Particle.m_BBox;
    
    // m_Lifetime: New individual object
    m_ParticleType = _Particle.m_ParticleType;
    m_fTimeFac = _Particle.m_fTimeFac;
    m_fDamping = _Particle.m_fDamping;
    m_nDepthlayers = _Particle.m_nDepthlayers;
    m_vecForce = _Particle.m_vecForce;
}
