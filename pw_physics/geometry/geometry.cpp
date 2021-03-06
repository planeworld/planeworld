////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2010-2018 Torsten Büschenfeld
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
    m_fMass(1.0),
    m_bAutoCOM(true),
    m_bAutoInertia(true),
    m_bShapesValid(false)
{
    METHOD_ENTRY("CGeometry::CGeometry")
    CTOR_CALL("CGeometry::CGeometry")

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

    this->copy(_Geom);
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

    // Free memory if pointer is still existent
    for (auto hShp : m_Shapes)
    {
        if (hShp.ptr() != nullptr)
        {
            delete hShp.ptr();
            hShp.setPtr(nullptr);
            MEM_FREED("IShape")
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy assignment operator
///
/// \param _Geom Geometry that should be copied and assigned
///
/// \return Pointer to copied and assigned geometry
///
////////////////////////////////////////////////////////////////////////////////
CGeometry& CGeometry::operator=(const CGeometry& _Geom)
{
    METHOD_ENTRY("CGeometry::operator=")

    if (this != &_Geom)
    {
        this->copy(_Geom);
    }

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

    CGeometry* pClone = new CGeometry(*this);
    MEM_ALLOC("CGeometry")

    return pClone;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a shape to the current list of shapes
///
/// \param _pShape Shape, that should be added to list
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::addShape(IShape* const _pShape)
{
    METHOD_ENTRY("CGeometry::addShape")
    m_Shapes.push_back(_pShape);
    m_bShapesValid = false;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy the given shapelist
///
/// \param _ShapeList Shapelist to be set
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::setShapes(const ShapesType& _ShapeList)
{
    METHOD_ENTRY("CGeometry::setShapes")

    // Free memory if pointer is still existent
    for (auto hShp : m_Shapes)
    {
        if (hShp.ptr() != nullptr)
        {
            delete hShp.ptr();
            hShp.set(nullptr);
            MEM_FREED("IShape")
        }
    }
    m_Shapes.clear();
    m_Shapes = _ShapeList;
    m_bShapesValid = false;
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

    // Initialise multi frame bounding box with previous time step
    ShapesType::const_iterator ci = m_Shapes.cbegin();

    // If there are any shapes, go on
    if (ci != m_Shapes.cend())
    {
        m_AABB = (*ci)->getBoundingBox();

        while ((++ci) != m_Shapes.cend())
        {
            m_AABB.update((*ci)->getBoundingBox());
        }

        this->update();

        // Initialise single frame bounding box
        m_AABBS.setLowerLeft(m_vecCOM+_vecOrigin);
        m_AABBS.setUpperRight(m_vecCOM+_vecOrigin);

        // Update Bounding boxes with current time step
        for (ShapesType::const_iterator ci = m_Shapes.cbegin();
                ci != m_Shapes.cend(); ++ci)
        {
            (*ci)->transform(_fAngle, m_vecCOM, _vecOrigin);

            // Update depthlayers
            //         m_nDepthlayers |= (*ci)->getShapeCur()->getDepths();

            m_AABB.update((*ci)->getBoundingBox());
            m_AABBS.update((*ci)->getBoundingBox());
        }
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

    for (const auto& hShp : m_Shapes)
    {
        m_bShapesValid &= hShp->isValid();
    }
    if (!m_bShapesValid)
    {
        if (m_bAutoCOM)
        {
            m_vecCOM.setZero();
            m_fMass = 0.0;
            for (const auto& hShp : m_Shapes)
            {
                m_vecCOM += hShp->getMass() *
                            hShp->getCentroid();
                m_fMass  += hShp->getMass();
                hShp->isValid() = true;
            }
            if (m_fMass > 0.0)
            {
                m_vecCOM /= m_fMass;
            }
        }
        else
        {
            m_fMass = 0.0;
            for (const auto& hShp : m_Shapes)
            {
                m_fMass  += hShp->getMass();
                hShp->isValid() = true;
            }
        }

        if (m_bAutoInertia)
        {
            m_fInertia = 0.0;
            for (const auto& hShp : m_Shapes)
            {
                m_fInertia +=  hShp->getInertia() +
                               hShp->getMass() *
                               (hShp->getCentroid() -
                                m_vecCOM).squaredNorm();
            }
        }

        m_bShapesValid = true;

        DOM_VAR(DEBUG_MSG("Geometry", "Center of mass calculated: " << m_vecCOM[0] << ", " << m_vecCOM[1]))
        DOM_VAR(DEBUG_MSG("Geometry", "Inertia calculated: " << m_fInertia))
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
//     _is >> _Geo.m_AABB;
//     _is >> _Geo.m_AABBS;
    _is >> _Geo.m_vecCOM[0] >> _Geo.m_vecCOM[1];
    _is >> _Geo.m_fInertia;
    _is >> _Geo.m_fMass;
    _is >> _Geo.m_bAutoCOM;
    _is >> _Geo.m_bAutoInertia;
    _is >> _Geo.m_bShapesValid;

    for (auto it : _Geo.m_Shapes)
    {
        if (it.isValid())
        {
            delete it.ptr();
            MEM_FREED("IShape")
            it.set(nullptr);
        }
    }

//     ShapesType::size_type nSize;
//     _is >> nSize;
//
//     for (auto i=0u; i<nSize; ++i)
//     {
//         IShape* pShape = new CDoubleBufferedShape;
//         MEM_ALLOC("CDoubleBufferedShape")
//         _is >> (*pDBShape);
//         _Geo.m_pShapes->push_back(pDBShape);
//     }

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
//     _os << _Geo.m_AABB << std::endl;
//     _os << _Geo.m_AABBS << std::endl;
    _os << _Geo.m_vecCOM << " " << _Geo.m_vecCOM << std::endl;
    _os << _Geo.m_fInertia << std::endl;
    _os << _Geo.m_fMass << std::endl;
    _os << _Geo.m_bAutoCOM << std::endl;
    _os << _Geo.m_bAutoInertia << std::endl;
    _os << _Geo.m_bShapesValid << std::endl;
    _os << _Geo.getShapes().size() << std::endl;
//     for (const auto ci : (*_Geo.m_pShapes))
//     {
//         _os << (*ci) << std::endl;
//     }

    return _os;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy data from given geometry
///
/// \param _Geom Geometry to copy data from
///
///////////////////////////////////////////////////////////////////////////////
void CGeometry::copy(const CGeometry& _Geom)
{
    METHOD_ENTRY("CGeometry::copy")

    m_AABB = _Geom.m_AABB;
    m_AABBS = _Geom.m_AABBS;

    // Free memory if pointer is still existent
    for (auto& hShp : m_Shapes)
    {
        if (hShp.isValid())
        {
            delete hShp.ptr();
            hShp.setPtr(nullptr);
            MEM_FREED("IShape")
        }
    }
    m_Shapes.clear();

    for (const auto& hShp : _Geom.m_Shapes)
    {
        m_Shapes.push_back(hShp->clone());
    }

    m_vecCOM    = _Geom.m_vecCOM;
    m_fInertia  = _Geom.m_fInertia;
    m_fMass     = _Geom.m_fMass;
    m_bAutoCOM  = _Geom.m_bAutoCOM;
    m_bAutoInertia = _Geom.m_bAutoInertia;
    m_bShapesValid = _Geom.m_bShapesValid;
}
