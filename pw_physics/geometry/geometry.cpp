////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2016 Torsten Büschenfeld
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
/// \file       geometry.cpp
/// \brief      Implementation of class "CGeometry"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-18
///
////////////////////////////////////////////////////////////////////////////////

#include "geometry.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CGeometry::CGeometry() : m_fInertia(0.0),
                         m_fMass(1.0)
{
    METHOD_ENTRY("CGeometry::CGeometry")
    CTOR_CALL("CGeometry::CGeometry")
    
    m_pShapes = new std::list<CDoubleBufferedShape*>();
    MEM_ALLOC("ShapeList")
    
    m_vecCOM.setZero();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _Geom Geometry that should be copied
///
///////////////////////////////////////////////////////////////////////////////
CGeometry::CGeometry(const CGeometry& _Geom)
{
    METHOD_ENTRY("CGeometry::CGeometry")
    CTOR_CALL("CGeometry::CGeometry")
    
    m_AABB = _Geom.m_AABB;

    m_pShapes = new std::list<CDoubleBufferedShape*>();
    MEM_ALLOC("ShapeList")
    
    DBShapesType::const_iterator ci=_Geom.m_pShapes->begin();
    while (ci != _Geom.m_pShapes->end())
    {
        m_pShapes->push_back((*ci)->clone());
        ++ci;
    }
    
    m_vecCOM    = _Geom.m_vecCOM;
    m_fInertia  = _Geom.m_fInertia;
    m_fMass     = _Geom.m_fMass;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
CGeometry::~CGeometry()
{
    METHOD_ENTRY("CGeometry::~CGeometry")
    DTOR_CALL("CGeometry::~CGeometry")

    for (std::list< CDoubleBufferedShape* >::iterator it = m_pShapes->begin();
        it != m_pShapes->end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != nullptr)
        {
            delete (*it);
            (*it) = nullptr;
            MEM_FREED("CDoubleBufferedShape")
        }
    }
    
    // Free memory if pointer is still existent
    if (m_pShapes != nullptr)
    {
        delete m_pShapes;
        m_pShapes = nullptr;
        MEM_FREED("ShapeList")
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones geometry
///
/// \param _Geom Geometry that should be copied
///
/// \return Pointer to cloned geometry
///
////////////////////////////////////////////////////////////////////////////////
CGeometry& CGeometry::operator=(const CGeometry& _Geom)
{
    METHOD_ENTRY("CGeometry::operator=")
    
    m_AABB = _Geom.m_AABB;

    // Free memory if pointer is still existent
    if (m_pShapes != nullptr)
    {
        delete m_pShapes;
        m_pShapes = nullptr;
        MEM_FREED("ShapeList")
    }
    m_pShapes = new std::list<CDoubleBufferedShape*>();
    MEM_ALLOC("ShapeList")
    
    DBShapesType::const_iterator ci=_Geom.m_pShapes->begin();
    while (ci != _Geom.m_pShapes->end())
    {
        m_pShapes->push_back((*ci)->clone());
        ++ci;
    }
    
    m_vecCOM    = _Geom.m_vecCOM;
    m_fInertia  = _Geom.m_fInertia;
    m_fMass     = _Geom.m_fMass;
    
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones geometry
///
/// \return Pointer to cloned geometry
///
////////////////////////////////////////////////////////////////////////////////
CGeometry* CGeometry::clone() const
{
    METHOD_ENTRY("CGeometry::clone")
    
    CGeometry* pClone = new CGeometry;
    MEM_ALLOC("CGeometry")
    
    DBShapesType::const_iterator ci=m_pShapes->begin();
    while (ci != m_pShapes->end())
    {
        pClone->m_pShapes->push_back((*ci)->clone());
        ++ci;
    }
    
    pClone->m_AABB = m_AABB;
    pClone->m_vecCOM = m_vecCOM;
    pClone->m_fInertia = m_fInertia;
    pClone->m_fMass = m_fMass;
    
    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a shape to the current list of shapes
///
/// \param _pShape Shape, that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::addShape(CDoubleBufferedShape* const _pShape)
{
    METHOD_ENTRY("CGeometry::addShape")
    m_pShapes->push_back(_pShape);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy the given shapelist
///
/// \param _pShapeList Shapelist to be set
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::setShapes(std::list<CDoubleBufferedShape*>* _pShapeList)
{
    METHOD_ENTRY("CGeometry::setShapes")

    m_pShapes->clear();
    m_pShapes = _pShapeList;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Transform geometry with given parameters from local to global
///        coordinates
///
/// \param _fAngle Angle of local coordinate system
/// \param _vecOrigin Origin of local coordinate system
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::transform(const double& _fAngle, const Vector2d& _vecOrigin)
{
    METHOD_ENTRY("CGeometry::transform")
    
    std::list< CDoubleBufferedShape* >::const_iterator ci = m_pShapes->begin();
    m_AABB = (*ci)->getShapeCur()->getBoundingBox();
    while ((++ci) != m_pShapes->end())
    {
        // Update bounding box of previous time step for continuous collision dection
        m_AABB.update((*ci)->getShapeCur()->getBoundingBox());
    }
    this->update();
    for (std::list< CDoubleBufferedShape* >::const_iterator ci = m_pShapes->begin();
         ci != m_pShapes->end(); ++ci)
    {
        (*ci)->getShapeCur()->transform(_fAngle, m_vecCOM, _vecOrigin);

        // Update depthlayers
//         m_nDepthlayers |= (*ci)->getShapeCur()->getDepths();

        // Update bounding box of current time step
        m_AABB.update((*ci)->getShapeCur()->getBoundingBox());
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Swap current shapelist with shapes of previous time step
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::update()
{
    METHOD_ENTRY("CGeometry::update")
    
    // Should be global to catch additions or setting of whole shapes
    bool bShapesValid = true;
    for (const auto ci : *m_pShapes)
    {
        bShapesValid &= ci->getShapeCur()->isValid();
    }
    if (!bShapesValid)
    {
        m_vecCOM.setZero();
        m_fMass = 0.0;
        for (const auto ci : *m_pShapes)
        {
            m_vecCOM += ci->getShapeCur()->getMass() *
                        ci->getShapeCur()->getCentroid();
            m_fMass  += ci->getShapeCur()->getMass();
            ci->getShapeCur()->isValid() = true;
        }
        if (m_fMass > 0.0)
        {
            m_vecCOM /= m_fMass;
        }
        
        m_fInertia = 0.0;
        for (const auto ci : *m_pShapes)
        {
            m_fInertia +=  ci->getShapeCur()->getInertia() + 
                           ci->getShapeCur()->getMass() *
                          (ci->getShapeCur()->getCentroid() -
                           m_vecCOM).squaredNorm();
        }
        DOM_VAR(DEBUG_MSG("Geometry", "Center of mass calculated: " << m_vecCOM[0] << ", " << m_vecCOM[1]))
        DOM_VAR(DEBUG_MSG("Geometry", "Inertia calculated: " << m_fInertia))
    }
    for (auto it : *m_pShapes)
    {
        it->swapBuffer();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _Geo CGeometry instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, CGeometry& _Geo)
{
    METHOD_ENTRY("CGeometry::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    _is >> _Geo.m_AABB;
    _is >> _Geo.m_vecCOM[0] >> _Geo.m_vecCOM[1];
    _is >> _Geo.m_fInertia;
    _is >> _Geo.m_fMass;
    
    for (auto it : (*_Geo.m_pShapes))
    {
        if (it != nullptr)
        {
            delete it;
            MEM_FREED("CDoubleBufferedShape")
            it = nullptr;
        }
    }
    
    DBShapesType::size_type nSize;
    _is >> nSize;
    
    for (auto i=0u; i<nSize; ++i)
    {
        CDoubleBufferedShape* pDBShape = new CDoubleBufferedShape;
        MEM_ALLOC("CDoubleBufferedShape")
        _is >> (*pDBShape);
        _Geo.m_pShapes->push_back(pDBShape);
    }
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _Geo CGeometry instance to stream
///
/// \return Stream with game state information of CGeometry instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, CGeometry& _Geo)
{
    METHOD_ENTRY("CGeometry::operator<<")
    
    _os << "Geometry:" << std::endl;
    _os << _Geo.m_AABB << std::endl;
    _os << _Geo.m_vecCOM << " " << _Geo.m_vecCOM << std::endl;
    _os << _Geo.m_fInertia << std::endl;
    _os << _Geo.m_fMass << std::endl;
    _os << _Geo.getShapes()->size() << std::endl;
    for (const auto ci : (*_Geo.m_pShapes))
    {
        _os << (*ci) << std::endl;
    }
    
    return _os;
}
