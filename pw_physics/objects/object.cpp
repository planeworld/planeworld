////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       object.cpp
/// \brief      Implementation of interface "CObject"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "object.h"

uint32_t CObject::m_unNrOfObjects = 0;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////
CObject::CObject(): m_bGravitation(true),
                m_bDynamics(true),
                m_fTimeFac(1.0),
                m_nDepthlayers(SHAPE_DEPTH_ALL)
{
    METHOD_ENTRY("CObject::CObject")
    CTOR_CALL("CObject::CObject")

    // Default name for any object:
    m_UID.setName("Object_" + m_UID.getName());
    
    m_pIntAng = new CEulerIntegrator<double>;
    MEM_ALLOC("CEulerIntegrator");
    m_pIntAngVel = new CEulerIntegrator<double>;
    MEM_ALLOC("CEulerIntegrator");
    m_pIntPos = new CEulerIntegrator<Vector2d>;
    MEM_ALLOC("CEulerIntegrator")
    m_pIntVel = new CEulerIntegrator<Vector2d>;
    MEM_ALLOC("CEulerIntegrator")

    m_vecForce.setZero();
    m_vecCell.setZero();
    
    m_Lifetime.start();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////
CObject::~CObject()
{
    METHOD_ENTRY("CObject::~CObject")
    DTOR_CALL("CObject::~CObject")

    if (m_pIntAng != nullptr)
    {
        delete m_pIntAng;
        m_pIntAng = nullptr;
        MEM_FREED("IIntegrator")
    }

    if (m_pIntAngVel != nullptr)
    {
        delete m_pIntAngVel;
        m_pIntAngVel = nullptr;
        MEM_FREED("IIntegrator")
    }
    if (m_pIntPos != nullptr)
    {
        delete m_pIntPos;
        m_pIntPos = nullptr;
        MEM_FREED("IIntegrator")
    }

    if (m_pIntVel != nullptr)
    {
        delete m_pIntVel;
        m_pIntVel = nullptr;
        MEM_FREED("IIntegrator")
    }

    m_Lifetime.stop();
    DOM_STATS(
        DEBUG_MSG("Object", "Lifetime (" << m_UID.getName() << "): "
                << m_Lifetime.getTime())
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones an object
///
/// \return Pointer to cloned object
///
////////////////////////////////////////////////////////////////////////////////
CObject* CObject::clone() const
{
    METHOD_ENTRY("CObject::clone")
    
    CObject* pClone = new CObject;
    MEM_ALLOC("CObject")

    //--- Variables of CObject -----------------------------------------------//

    pClone->m_KinematicsState = m_KinematicsState;
    pClone->m_bGravitation = m_bGravitation;
    pClone->m_bDynamics    = m_bDynamics;
    // m_Lifetime: New individual object
    pClone->m_fTimeFac     = m_fTimeFac;
    pClone->m_Geometry     = (*m_Geometry.clone());
    // m_vecForce: No Forces on newly created object;
    pClone->m_nDepthlayers = m_nDepthlayers;
    
    if (pClone->m_pIntPos != nullptr)
    {
        delete pClone->m_pIntPos;
        MEM_FREED("IIntegrator")
        pClone->m_pIntPos = nullptr;
    }
    pClone->m_pIntPos      = m_pIntPos->clone();
    
    if (pClone->m_pIntVel != nullptr)
    {
        delete pClone->m_pIntVel;
        MEM_FREED("IIntegrator")
        pClone->m_pIntVel = nullptr;
    }
    pClone->m_pIntVel      = m_pIntVel->clone();     
    
    // m_strName: Don't clone the name, this is an individual object
    pClone->m_Anchors      = m_Anchors;
    
    //--- Variables of CBody -------------------------------------------------//
    pClone->m_fTorque = m_fTorque;
    
    if (pClone->m_pIntAng != nullptr)
    {
        delete pClone->m_pIntAng;
        MEM_FREED("IIntegrator")
        pClone->m_pIntAng = nullptr;
    }
    pClone->m_pIntAng      = m_pIntAng->clone();
    
    if (pClone->m_pIntAngVel != nullptr)
    {
        delete pClone->m_pIntAngVel;
        MEM_FREED("IIntegrator")
        pClone->m_pIntAngVel = nullptr;
    }
    pClone->m_pIntAngVel      = m_pIntAngVel->clone();
    
    //--- Variables of IUniverseScaled ---------------------------------------//
    pClone->m_vecCell = m_vecCell;
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns an anchor
///
/// \param _nID ID of anchor that should be returned
///
/// \return Demanded anchor
///
///////////////////////////////////////////////////////////////////////////////
Vector2d CObject::getAnchor(const int& _nID) const
{
    METHOD_ENTRY("CObject::getAnchor")

    /// \todo Use getLocalPosition here:
    
    Vector2d vecResult;
    Rotation2Dd Rotation(m_KinematicsState.getLocalAngle());

    vecResult = Rotation * m_Anchors[_nID] + m_pIntPos->getValue();

    return vecResult;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a force to object
///
/// \param _vecF Force to be applied
/// \param _vecPOC Point of contact
///
///////////////////////////////////////////////////////////////////////////////
void CObject::addForce(const Vector2d& _vecF, const Vector2d& _vecPOC)
{
    METHOD_ENTRY("CObject::addForce")

    m_vecForce  +=  _vecF;
    m_fTorque   +=  (_vecPOC - m_pIntPos->getValue())[0] * _vecF[1] -
                    (_vecPOC - m_pIntPos->getValue())[1] * _vecF[0];
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a force to object (local coordinates given)
///
/// \param _vecF Force to be applied
/// \param _vecPOC Point of contact
///
///////////////////////////////////////////////////////////////////////////////
void CObject::addForceLC(const Vector2d& _vecF, const Vector2d& _vecPOC)
{
    METHOD_ENTRY("CObject::addForceLC")

    Rotation2Dd Rotation(m_KinematicsState.getAngle());
    
    m_vecForce  +=  Rotation * _vecF;
    
    Vector2d vecTmp = Rotation * (_vecPOC-m_Geometry.getCOM());
    m_fTorque   +=  vecTmp[0] * m_vecForce[1] - vecTmp[1] * m_vecForce[0];
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clear all forces acting on object
///
////////////////////////////////////////////////////////////////////////////////
void CObject::clearForces()
{
    METHOD_ENTRY("CObject::clearForces")

    m_fTorque = 0.0;
    m_vecForce.setZero();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Update the cell of this object
///
/// \todo Shouldn't this be a method of IUniverseScaled?!
///
////////////////////////////////////////////////////////////////////////////////
void CObject::updateCell()
{
    METHOD_ENTRY("CObject::updateCell")

    Vector2d vecUpdate;
    Vector2i vecUpdateCell;
    vecUpdate.setZero();
    vecUpdateCell.setZero();
    
    if      (m_pIntPos->getValue()[0] >  DEFAULT_CELL_SIZE)
    {
        vecUpdate[0]     -= DEFAULT_CELL_SIZE_2;
        vecUpdateCell[0] += 1;
    }
    else if (m_pIntPos->getValue()[0] < -DEFAULT_CELL_SIZE)
    {
        vecUpdate[0] += DEFAULT_CELL_SIZE_2;
        vecUpdateCell[0] -= 1;
    }
    else if (m_pIntPos->getValue()[1] >  DEFAULT_CELL_SIZE)
    {
        vecUpdate[1] -= DEFAULT_CELL_SIZE_2;
        vecUpdateCell[1] += 1;
    }
    else if (m_pIntPos->getValue()[1] < -DEFAULT_CELL_SIZE)
    {
        vecUpdate[1] += DEFAULT_CELL_SIZE_2;
        vecUpdateCell[1] -= 1;
    }
    m_pIntPos->init(m_pIntPos->getValue()+vecUpdate);
    /// \bug Bounding box must be updated with new relative position
    
    m_vecCell += vecUpdateCell;
    this->setCell(m_vecCell);

    DEBUG(
        if (std::abs(vecUpdateCell[0]) > 0 || std::abs(vecUpdateCell[1] > 0))
        {
            DOM_VAR(DEBUG_MSG("Object Interface","Cell update for " << m_UID.getName() << " is " <<
                            vecUpdateCell[0] << ", " << vecUpdateCell[1]))
        }
    )
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculate dynamics of object
///
/// \param _fTimeStep Time between two frames
///
///////////////////////////////////////////////////////////////////////////////
void CObject::dynamics(const double& _fTimeStep)
{
    METHOD_ENTRY("CObject::dynamics")

    // Call object specific dynamics if enabled
    if (m_bDynamics)
    {
        Vector2d vecAccel = m_vecForce;
    
        if (m_Geometry.getMass() > 0.0) vecAccel /= m_Geometry.getMass();
        
        m_pIntVel->integrate(vecAccel, _fTimeStep*m_fTimeFac);
        m_pIntPos->integrate(m_pIntVel->getValue(),_fTimeStep*m_fTimeFac);
        
        double fAngleAccel = m_fTorque;
        
        if (m_Geometry.getInertia() > 0.0) fAngleAccel /= m_Geometry.getInertia();
        
        double fAngleVel = m_pIntAngVel->integrate(fAngleAccel, _fTimeStep*m_fTimeFac);
        m_KinematicsState.setAngleVelocity(fAngleVel);
        m_KinematicsState.setAngle(m_pIntAng->integrateClip(fAngleVel, _fTimeStep*m_fTimeFac, 2.0*M_PI));
        
        m_Trajectory.update(m_pIntPos->getValue(), m_vecCell);
    }
    
    // Update kinematics state from integrator
    m_KinematicsState.setOrigin(m_pIntPos->getValue()-m_Geometry.getCOM());
    m_KinematicsState.setVelocity(m_pIntVel->getValue());
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise object
///
/// When the object is initialised, it is set back to the state of simulation
/// begin. Specific init methods are called as well as transformation. Hence,
/// fixed objects don't need to be transformed more than once.
///
///////////////////////////////////////////////////////////////////////////////
void CObject::init()
{
    METHOD_ENTRY("CObject::init")

    // First, calculate geometry, i.e. center of mass, inertia, etc.
    m_Geometry.update();
    
    // Initialise object position and velocity (global coordinates)
    m_pIntAng->init(m_KinematicsState.getAngle());
    m_pIntAngVel->init(m_KinematicsState.getAngleVelocity());
    m_pIntPos->init(m_KinematicsState.getLocalPosition(m_Geometry.getCOM()));
    m_pIntVel->init(m_KinematicsState.getVelocity());
    
    this->setCell(m_vecCell);
    
    // Call transform twice to correctly set bounding boxes via front and backbuffer
    this->transform();
    this->transform();
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Method to set cell, overloaded to hand cell to bounding box.
///
/// \param _vecCell Grid cell of the object
///
////////////////////////////////////////////////////////////////////////////////
void CObject::setCell(const Vector2i& _vecCell)
{
    METHOD_ENTRY("CObject::setCell")

    m_vecCell = _vecCell;
    m_Geometry.getBoundingBox().setCell(_vecCell);
    std::list<CDoubleBufferedShape*>::const_iterator ci = m_Geometry.getShapes()->begin();
    while (ci != m_Geometry.getShapes()->end())
    {
        (*ci)->getShapeCur()->getBoundingBox().setCell(m_vecCell);
        ++ci;
    }
    ci = m_Geometry.getShapes()->begin();
    while (ci != m_Geometry.getShapes()->end())
    {
        (*ci)->getShapeBuf()->getBoundingBox().setCell(m_vecCell);
        ++ci;
    }    
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets a new integrator for this instance
///
/// \param _IntType Which integrator to use
///
///////////////////////////////////////////////////////////////////////////////
void CObject::setNewIntegrator(const IntegratorType& _IntType)
{
    METHOD_ENTRY("CObject::setNewIntegrator")

    if (m_pIntAng != nullptr)
    {
        delete m_pIntAng;
        MEM_FREED("IIntegrator");
        m_pIntAng = nullptr;
    }
    if (m_pIntAngVel != nullptr)
    {
        delete m_pIntAngVel;
        MEM_FREED("IIntegrator");
        m_pIntAngVel = nullptr;
    }
    if (m_pIntPos != nullptr)
    {
        delete m_pIntPos;
        MEM_FREED("IIntegrator")
        m_pIntPos = nullptr;
    }
    if (m_pIntVel != nullptr)
    {
        delete m_pIntVel;
        MEM_FREED("IIntegrator")
        m_pIntVel = nullptr;
    }

    switch (_IntType)
    {
        case INTEGRATOR_EULER:
            m_pIntAng = new CEulerIntegrator<double>;
            m_pIntAngVel = new CEulerIntegrator<double>;
            m_pIntPos = new CEulerIntegrator<Vector2d>;
            m_pIntVel = new CEulerIntegrator<Vector2d>;
            MEM_ALLOC("CEulerIntegrator")
            MEM_ALLOC("CEulerIntegrator")
            MEM_ALLOC("CEulerIntegrator")
            MEM_ALLOC("CEulerIntegrator")
            break;
        case INTEGRATOR_ADAMS_BASHFORTH:
            m_pIntAng = new CAdamsBashforthIntegrator<double>;
            m_pIntAngVel = new CAdamsBashforthIntegrator<double>;
            m_pIntPos = new CAdamsBashforthIntegrator<Vector2d>;
            m_pIntVel = new CAdamsBashforthIntegrator<Vector2d>;
            MEM_ALLOC("CAdamsBashforthIntegrator")
            MEM_ALLOC("CAdamsBashforthIntegrator")
            MEM_ALLOC("CAdamsBashforthIntegrator")
            MEM_ALLOC("CAdamsBashforthIntegrator")
            break;
        case INTEGRATOR_ADAMS_MOULTON:
            m_pIntAng = new CAdamsMoultonIntegrator<double>;
            m_pIntAngVel = new CAdamsMoultonIntegrator<double>;
            m_pIntPos = new CAdamsMoultonIntegrator<Vector2d>;
            m_pIntVel = new CAdamsMoultonIntegrator<Vector2d>;
            MEM_ALLOC("CAdamsMoultonIntegrator")
            MEM_ALLOC("CAdamsMoultonIntegrator")
            MEM_ALLOC("CAdamsMoultonIntegrator")
            MEM_ALLOC("CAdamsMoultonIntegrator")
            break;
    }

}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transform Object
///
///////////////////////////////////////////////////////////////////////////////
void CObject::transform()
{
    METHOD_ENTRY("CObject::transform")

    // Don't touch fixed objects
    // Otherwise:
    if (m_bDynamics)
    {
        m_Geometry.transform(m_KinematicsState.getAngle(), m_KinematicsState.getOrigin());
        m_KinematicsState.transform(m_pIntPos->getValue(), m_Geometry.getCOM());
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pObj CObject instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CObject* const _pObj)
{
    METHOD_ENTRY("CObject::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    // From IKinematicsStateUser
    _is >> _pObj->m_KinematicsState;
    
    // From IUniqueIDUser
    _is >> _pObj->m_UID;
    
    _is >> _pObj->m_bGravitation;
    _is >> _pObj->m_bDynamics;
    _is >> _pObj->m_Lifetime;
    _is >> _pObj->m_fTimeFac;
    _is >> _pObj->m_Geometry;
    _is >> _pObj->m_vecForce[0];
    _is >> _pObj->m_vecForce[1];
    _is >> _pObj->m_fTorque;
    _is >> _pObj->m_nDepthlayers;
    _is >> _pObj->m_pIntAng;
    _is >> _pObj->m_pIntAngVel;
    _is >> _pObj->m_pIntPos;
    _is >> _pObj->m_pIntVel;
//     _is >> _pObj->m_Anchors.size();
//     for (const auto ci : _pObj->m_Anchors)
//         _is >> ci;
//     _is << _pObj->m_Trajectory;

    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pObj CObject instance to stream
///
/// \return Stream with game state information of CObject instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CObject* const _pObj)
{
    METHOD_ENTRY("CObject::operator<<")
    
    _os << "Object:" << std::endl;
    
    // From IKinematicsStateUser
    _os << _pObj->m_KinematicsState << std::endl;
    
    // From IUniqueIDUser
    _os << _pObj->m_UID << std::endl;
    
    _os << _pObj->m_bGravitation << std::endl;
    _os << _pObj->m_bDynamics << std::endl;
    _os << _pObj->m_Lifetime << std::endl;
    _os << _pObj->m_fTimeFac << std::endl;
    _os << _pObj->m_Geometry << std::endl;
    _os << _pObj->m_vecForce[0] << " " <<
           _pObj->m_vecForce[1] << std::endl;
    _os << _pObj->m_fTorque << std::endl;
    _os << _pObj->m_nDepthlayers << std::endl;
    _os << _pObj->m_pIntAng << std::endl;
    _os << _pObj->m_pIntAngVel << std::endl;
    _os << _pObj->m_pIntPos << std::endl;
    _os << _pObj->m_pIntVel << std::endl;
//     _os << _pObj->m_Anchors.size() << std::endl;
//     for (const auto ci : _pObj->m_Anchors)
//         _os << ci;
//     _os << _pObj->m_Trajectory;
    
    return _os;
}
