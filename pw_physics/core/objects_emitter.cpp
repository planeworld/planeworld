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
/// \file       objects_emitter.cpp
/// \brief      Implementation of class "CObjectEmitter"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2014-03-02
///
////////////////////////////////////////////////////////////////////////////////

#include "objects_emitter.h"

#include "circle.h"
#include "circle_visuals.h"
#include "object_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CObjectEmitter::CObjectEmitter() : m_pTemplate(nullptr),
                                   m_pTemplateVisuals(nullptr) 
{
    METHOD_ENTRY("CObjectEmitter::CObjectEmitter")
    CTOR_CALL("CObjectEmitter::CObjectEmitter")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, deletes objects
///
///////////////////////////////////////////////////////////////////////////////
CObjectEmitter::~CObjectEmitter()
{
    METHOD_ENTRY("CObjectEmitter::~CObjectEmitter")
    DTOR_CALL("CObjectEmitter::~CObjectEmitter")
    
    if (m_pTemplate != nullptr)
    {
        delete m_pTemplate;
        MEM_FREED("CObject")
        m_pTemplate = nullptr;
    }
    if (m_pTemplateVisuals != nullptr)
    {
        delete m_pTemplateVisuals;
        MEM_FREED("IObjectVisuals")
        m_pTemplateVisuals = nullptr;
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
void CObjectEmitter::emit(const double& _fF)
{
    METHOD_ENTRY("CObjectEmitter::emit")
    
    /// \todo Activation could be checked in interface class IEmitter, calling
    ///       a myEmit method if activated
    if (m_bActive)
    {
        double nNrOfObjects;
        
        if (_fF < 0.0)
        {
            nNrOfObjects=m_nNr;
        }
        else
        {
            double fNrOfObjects = m_fFrequency * _fF + m_fResidual;
            nNrOfObjects = static_cast<int>(fNrOfObjects);
            m_fResidual = fNrOfObjects - nNrOfObjects;
        }
        
        switch (m_EmitterDistribution)
        {
            case EMITTER_DISTRIBUTION_CIRCULAR_FIELD:
                break;
            case EMITTER_DISTRIBUTION_RECTANGULAR_FIELD:
                for (int i=0; i<nNrOfObjects; ++i)
                {
                    double fX = m_UniformDist(m_Generator)*(m_fMaxX-m_fMinX) + m_fMinX;
                    double fY = m_UniformDist(m_Generator)*(m_fMaxY-m_fMinY) + m_fMinY;
                    
                    CObject* pObject = m_pTemplate->clone();
                    IObjectVisuals* pObjectVisuals = m_pTemplateVisuals->clone(pObject);
                    
                    pObject->setOrigin(Vector2d(fX, fY) + m_KinematicsState.getOrigin());
                    m_pDataStorage->addObject(pObject);
                    m_pDataStorage->addObjectVisuals(pObjectVisuals);
                    
                    pObject->init();
                }
                break;
            case EMITTER_DISTRIBUTION_POINT_SOURCE:
                for (int i=0; i<nNrOfObjects; ++i)
                {
                    double fAngle = m_NormalDist(m_Generator)*m_fAngleStd + m_KinematicsState.getAngle();;
                    double fVelocity = m_NormalDist(m_Generator)*m_fVelocityStd + m_fVelocity;
                                    
                    CObject* pObject = m_pTemplate->clone();
                    IObjectVisuals* pObjectVisuals = m_pTemplateVisuals->clone(pObject);
                    
                    pObject->setOrigin(m_KinematicsState.getOrigin());
                    pObject->setVelocity(fVelocity*Vector2d(std::cos(fAngle), sin(fAngle)));
                    m_pDataStorage->addObject(pObject);
                    m_pDataStorage->addObjectVisuals(pObjectVisuals);
                    
                    pObject->init();
                }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the emitter
///
///////////////////////////////////////////////////////////////////////////////
void CObjectEmitter::init()
{
    METHOD_ENTRY("CObjectEmitter::init")
    
    // Create default object if needed (circlular mass)
    if (m_pTemplate == nullptr)
    {
        m_pTemplate = new CObject;
        CCircle*    pCircle = new CCircle;
        MEM_ALLOC("CObject")
        MEM_ALLOC("IShape")
        
        pCircle->setMass(1.0e12);
        pCircle->setDepths(SHAPE_DEPTH_ALL);
        pCircle->setCenter(0.0, 0.0);
        pCircle->setRadius(1.0);

        CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
        MEM_ALLOC("CDoubleBufferedShape")
        pShape->buffer(pCircle);
        m_pTemplate->getGeometry()->addShape(pShape);
    
        CCircleVisuals* pCircleVisuals = new CCircleVisuals(pShape);
        MEM_ALLOC("CCircleVisuals")
        
        m_pTemplateVisuals = new IObjectVisuals(m_pTemplate);
        MEM_ALLOC("IObjectVisuals")
        
        m_pTemplateVisuals->addVisuals(pCircleVisuals);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Set the template for object emitation
///
/// \param _pObj  Object to be cloned and emitted
/// \param _pObjV Object visuals to be cloned and emitted
///
///////////////////////////////////////////////////////////////////////////////
void CObjectEmitter::setTemplate(CObject* const _pObj, IObjectVisuals* const _pObjV)
{
    METHOD_ENTRY("CObjectEmitter::setTemplate")
    
    if (m_pTemplate != nullptr)
    {
        delete m_pTemplate;
        MEM_FREED("CObject")
        m_pTemplate = nullptr;
        NOTICE_MSG("Objects Emitter", "Template object already existing, replacing.");
    }
    if (m_pTemplateVisuals != nullptr)
    {
        delete m_pTemplateVisuals;
        MEM_FREED("IObjectVisuals")
        m_pTemplateVisuals = nullptr;
        NOTICE_MSG("Objects Emitter", "Template object visuals already existing, replacing.");
    }
    
    m_pTemplate = _pObj;
    m_pTemplateVisuals = _pObjV;
    
}
