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
CDebris::CDebris() : IGridUser(),
                     IUniqueIDUser(),
                     m_DebrisType(DEBRIS_TYPE_DOT),
                     m_fTimeFac(1.0)
{
    METHOD_ENTRY("CDebris::CDebris")
    CTOR_CALL("CDebris::CDebris")
    
    m_PosList.reserve(DEBRIS_DEFAULT_NUMBER);
    m_VelList.reserve(DEBRIS_DEFAULT_NUMBER);
    m_PosListPrev.reserve(DEBRIS_DEFAULT_NUMBER);
    m_StateList.reserve(DEBRIS_DEFAULT_NUMBER);
    
    m_vecForce.setZero();
    
    m_UID.setName("Debris_"+m_UID.getName());
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy constructor
///
/// \param _Debris Debris to be copied
///
///////////////////////////////////////////////////////////////////////////////
CDebris::CDebris(const CDebris& _Debris) : IGridUser(_Debris),
                                           IUniqueIDUser(_Debris)
                                           
{
    METHOD_ENTRY("CDebris::CDebris")
    CTOR_CALL("CDebris::CDebris")
    
    this->copy(_Debris);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones debris
///
/// \return Pointer to cloned debris
///
////////////////////////////////////////////////////////////////////////////////
CDebris* CDebris::clone() const
{
    METHOD_ENTRY("CDebris::clone")
    
    CDebris* pClone = new CDebris(*this);
    MEM_ALLOC("CDebris")

    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _Debris Debris to be copied and assigned
///
/// \return Copied debris to be assigned
///
////////////////////////////////////////////////////////////////////////////////
CDebris& CDebris::operator=(const CDebris& _Debris)
{
    METHOD_ENTRY("CDebris::operator=")
    IUniqueIDUser::operator=(_Debris);
    IGridUser::operator=(_Debris);
    this->copy(_Debris);
    
    return *this;
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
    
    m_BBox.setLowerLeft(m_PosList[0]);
    m_BBox.setUpperRight(m_PosList[0]);
    
    Vector2d vecStep = m_vecForce * _fStep * m_fTimeFac;
    for (auto i=0u; i<m_PosList.size(); ++i)
    {
        // Only if state is active
        if (m_StateList[i] == DEBRIS_STATE_ACTIVE)
        {
            m_BBox.update(m_PosList[i]);
            m_PosListPrev[i] = m_PosList[i];
            m_VelList[i] += vecStep;
            m_PosList[i] += m_VelList[i] * _fStep;
            m_BBox.update(m_PosList[i]);
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
    m_StateList.push_back(DEBRIS_STATE_ACTIVE);
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
        m_StateList.push_back(DEBRIS_STATE_ACTIVE);
    }
    m_VelList.reserve(_nN);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pDebris CDebris instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CDebris* const _pDebris)
{
    METHOD_ENTRY("CDebris::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    /// \todo Stream data from IGridUser
    
    // From IUniqueIDUser
    _is >> _pDebris->m_UID;
    
    strTmp="";
    _is >> strTmp; _pDebris->m_DebrisType = mapStringToDebrisType.at(strTmp);
    
    _is >> _pDebris->m_Lifetime;
    _is >> _pDebris->m_fTimeFac;
    
    _is >> _pDebris->m_PosList;
    _is >> _pDebris->m_PosListPrev;
    _is >> _pDebris->m_VelList;
    _is >> _pDebris->m_StateList;
    
    _is >> _pDebris->m_BBox;
    
    _is >> _pDebris->m_fDamping;
    _is >> _pDebris->m_nDepthlayers;
    _is >> _pDebris->m_vecForce[0] >> _pDebris->m_vecForce[1];
    
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os Source stream
/// \param _pDebris CDebris instance to stream
///
/// \return Stream with game state information of CDebris instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CDebris* const _pDebris)
{
    METHOD_ENTRY("CDebris::operator<<")
    
    _os << "Debris:" << std::endl;
    
    /// \todo Stream data from IGridUser
    
    // From IUniqueIDUser
    _os << _pDebris->m_UID << std::endl;
    
    _os << mapDebrisTypeToString.at(_pDebris->getDebrisType()) << std::endl;
    
    _os << _pDebris->m_Lifetime << std::endl;
    _os << _pDebris->m_fTimeFac << std::endl;
    
    _os << _pDebris->m_PosList << std::endl;
    _os << _pDebris->m_PosListPrev << std::endl;
    _os << _pDebris->m_VelList << std::endl;
    _os << _pDebris->m_StateList << std::endl;
    
    _os << _pDebris->m_StateList << std::endl;
    
    _os << _pDebris->m_fDamping << std::endl;
    _os << _pDebris->m_nDepthlayers << std::endl;
    _os << _pDebris->m_vecForce[0] << " " << _pDebris->m_vecForce[1] << std::endl;
    
    return _os;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies debris
///
/// \param _Debris Debris to be copied
///
////////////////////////////////////////////////////////////////////////////////
void CDebris::copy(const CDebris& _Debris) 
{
    METHOD_ENTRY("CDebris::copy")
    
    //--- Variables of CDebris -----------------------------------------------//
    m_PosList = _Debris.m_PosList;
    m_PosListPrev = _Debris.m_PosListPrev;
    m_VelList = _Debris.m_VelList;
    m_StateList = _Debris.m_StateList;
    m_BBox = _Debris.m_BBox;
    
    // m_Lifetime: New individual object
    m_DebrisType = _Debris.m_DebrisType;
    m_fTimeFac = _Debris.m_fTimeFac;
    m_fDamping = _Debris.m_fDamping;
    m_nDepthlayers = _Debris.m_nDepthlayers;
    m_vecForce = _Debris.m_vecForce;
}
