////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2012-2016 Torsten Büschenfeld
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
/// \file       object_visuals.cpp
/// \brief      Implementation of class "IObjectVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-12-09
///
////////////////////////////////////////////////////////////////////////////////

#include "object_visuals.h"

#include "circle_visuals.h"
#include "planet_visuals.h"
#include "polygon_visuals.h"
#include "terrain_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
IObjectVisuals::~IObjectVisuals()
{
    METHOD_ENTRY("IObjectVisuals::~IObjectVisuals")
    DTOR_CALL("IObjectVisuals::~IObjectVisuals")
    
    for (std::vector<IVisuals*>::iterator it = m_Visuals.begin();
        it != m_Visuals.end(); ++it)
    {
        // Free memory if pointer is still existent
        if ((*it) != 0)
        {
            delete (*it);
            (*it) = 0;
            MEM_FREED("IVisuals");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Clones object visuals
///
/// \param _pObj Object for these visuals
///
/// \return Pointer to cloned object visuals
///
////////////////////////////////////////////////////////////////////////////////
IObjectVisuals* IObjectVisuals::clone(IObject* const _pObj) const
{
    METHOD_ENTRY("IObjectVisuals::clone")
    
    IObjectVisuals* pClone = new IObjectVisuals(_pObj);
    MEM_ALLOC("IObjectVisuals")
    
    std::vector<IVisuals*>::const_iterator  ci = m_Visuals.begin();
    DBShapesType::const_iterator            cj=_pObj->getGeometry()->getShapes()->begin();
    while(ci != m_Visuals.end())
    {
        pClone->m_Visuals.push_back((*ci)->clone((*cj)));
        ++ci; ++cj;
    }
    
    return pClone;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw all shape visuals of this object
///
/// \todo At the moment, an object is only drawn if in the same cell as the
///       camera, otherwise, when zoomed out, a dot is drawn. Bounding Boxes are
///       not fully adjusted to cells yet.
///
/// \param _pCamera Draw visuals with respect to this camera
///
////////////////////////////////////////////////////////////////////////////////
void IObjectVisuals::draw(CCamera* const _pCamera) const
{
    METHOD_ENTRY("IObjectVisuals::draw")
    
    if (m_pRef->getGeometry()->getBoundingBox().overlaps(_pCamera->getBoundingBox()))
    {
        for (std::vector<IVisuals*>::const_iterator ci  = m_Visuals.begin();
                                                    ci != m_Visuals.end(); ++ci)
        {
            if ((*ci)->getBoundingBox().overlaps(_pCamera->getBoundingBox()))
            {
                if ((((*ci)->getBoundingBox().getWidth() * m_Graphics.getResPMX()) <  1.0) && 
                    (((*ci)->getBoundingBox().getHeight() * m_Graphics.getResPMY()) < 1.0))
                {
                    m_Graphics.dot((*ci)->getBoundingBox().getLowerLeft() - _pCamera->getCenter() + 
                                    IUniverseScaled::cellToDouble(m_pRef->getCell()-_pCamera->getCell()));
                }
                else
                {
                    (*ci)->draw(_pCamera, m_pRef);
                }
            }
        }
        m_Graphics.circle(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM()) - _pCamera->getCenter(), 0.6);
        m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
        m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(-0.6, 0.0)) -_pCamera->getCenter());
        m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(+0.6, 0.0)) -_pCamera->getCenter());
        m_Graphics.endLine();
        m_Graphics.beginLine(PolygonType::LINE_SINGLE, -10.0);
        m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(0.0, -0.6)) -_pCamera->getCenter());
        m_Graphics.addVertex(m_pRef->getKinematicsState().getLocalPosition(m_pRef->getGeometry()->getCOM() - Vector2d(0.0, +0.6)) -_pCamera->getCenter());
        m_Graphics.endLine();
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
/// \param _pObjVis IObjectVisuals instance to stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& _is, IObjectVisuals* const _pObjVis)
{
    METHOD_ENTRY("IObjectVisuals::operator>>")
    
    std::string strTmp;
    _is >> strTmp;
    
    for (auto it : _pObjVis->m_Visuals)
    {
        if (it != nullptr)
        {
            delete it;
            MEM_FREED("IVisuals")
            it = nullptr;
        }
    }
    _pObjVis->m_Visuals.clear();
    
    // From IUniqueIDReferrer
    _is >> _pObjVis->m_UIDRef;
    
    std::vector<IVisuals*>::size_type nSize;
    _is >> nSize;

    for (auto i=0u; i<nSize; ++i)
    {
        // Cast streamable basetype to strongly typed enum ObjectType
        std::underlying_type<ShapeVisualsType>::type nShpVisType;
        _is >> nShpVisType;
        
        switch (static_cast<ShapeVisualsType>(nShpVisType))
        {
            case ShapeVisualsType::CIRCLE:
            {
                CCircleVisuals* pShpVis = new CCircleVisuals;
                MEM_ALLOC("CCircleVisuals")
                _is >> pShpVis;
                _pObjVis->addVisuals(pShpVis);
                break;
            }
            case ShapeVisualsType::PLANET:
            {
                CPlanetVisuals* pShpVis = new CPlanetVisuals;
                MEM_ALLOC("CPlanetVisuals")
                _is >> pShpVis;
                _pObjVis->addVisuals(pShpVis);
                break;
            }
            case ShapeVisualsType::POLYGON:
            {
                CPolygonVisuals* pShpVis = new CPolygonVisuals;
                MEM_ALLOC("CPolygonVisuals")
                _is >> pShpVis;
                _pObjVis->addVisuals(pShpVis);
                break;
            }
            case ShapeVisualsType::TERRAIN:
            {
                CTerrainVisuals* pShpVis = new CTerrainVisuals;
                MEM_ALLOC("CTerrainVisuals")
                _is >> pShpVis;
                _pObjVis->addVisuals(pShpVis);
                break;
            }
            case ShapeVisualsType::NONE:
                break;
        }
    }
    
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
/// \param _pObjVis IObjectVisuals instance to stream
///
/// \return Stream with game state information of IObjectVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& _os, IObjectVisuals* const _pObjVis)
{
    METHOD_ENTRY("IObjectVisuals::operator<<")
    
    _os << "ObjectVisuals:" << std::endl;
    
    // From IUniqueIDReferrer
    _os << _pObjVis->m_UIDRef << std::endl;
    
    _os << _pObjVis->m_Visuals.size() << std::endl;
    for (const auto ci : _pObjVis->m_Visuals)
    {
        _os << ci << std::endl;
    }

    return _os;
}

