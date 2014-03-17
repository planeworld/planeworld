/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
#include "rigidbody.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CObjectEmitter::CObjectEmitter()
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
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialises the emitter
///
///////////////////////////////////////////////////////////////////////////////
void CObjectEmitter::init()
{
    METHOD_ENTRY("CObjectEmitter::init")
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
    
//     double nNrOfObjects;
//     
//     if (_fF < 0.0)
//     {
//         nNrOfObjects=m_nNrMax;
//     }
//     else
//     {
//         double fNrOfObjects = m_fFrequency * _fF + m_fResidual;
//         nNrOfObjects = static_cast<int>(fNrOfObjects);
//         m_fResidual = fNrOfObjects - nNrOfObjects;
//     }
    
    if (_fF < 0.0)
    {
        std::cout << "Emitting all objects once." << std::endl;
    }
    else
    {
        double fNrOfObjects = m_fFrequency * _fF + m_fResidual;
        int    nNrOfObjects = static_cast<int>(fNrOfObjects);
        m_fResidual = fNrOfObjects - nNrOfObjects;
        
        for (int i=0; i<nNrOfObjects; ++i)
        {
            CRigidBody* pRigidBody = new CRigidBody;
            CCircle*    pCircle = new CCircle;
            MEM_ALLOC("pRigidBody")
            MEM_ALLOC("pCircle")
            
            pCircle->setDepths(SHAPE_DEPTH_ALL);
            pCircle->setCenter(0.0, 0.0);
            pCircle->setRadius(1.0);

            CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
            MEM_ALLOC("pShape")
            pShape->buffer(pCircle);
            pRigidBody->getGeometry()->addShape(pShape);
        
            CCircleVisuals* pCircleVisuals = new CCircleVisuals(pShape);
            MEM_ALLOC("pCircleVisuals")
            
            IObjectVisuals* pObjectVisuals = new IObjectVisuals(pRigidBody);
            MEM_ALLOC("pObjectVisuals")
            
            pObjectVisuals->addVisuals(pCircleVisuals);
            
            double fAngle = m_NormalDist(m_Generator)*m_fAngleVariance + m_fAngle;
            double fVelocity = m_NormalDist(m_Generator)*m_fVelocityVariance + m_fVelocity;
            pRigidBody->setVelocity((Vector2d(-2.0,0.0), fVelocity*Vector2d(std::cos(fAngle), sin(fAngle))));
            m_pDataStorage->addObject(pRigidBody);
            m_pDataStorage->addObjectVisuals(pObjectVisuals);
        }
    }
    
    
    
//     switch (m_EmitterDistribution)
//     {
//         case EMITTER_DISTRIBUTION_CIRCULAR_FIELD:
//             break;
//         case EMITTER_DISTRIBUTION_RECTANGULAR_FIELD:
//             for (int i=0; i<nNrOfDebris; ++i)
//             {
//                 double fX = m_UniformDist(m_Generator)*(m_fMaxX-m_fMinX) + m_fMinX;
//                 double fY = m_UniformDist(m_Generator)*(m_fMaxY-m_fMinY) + m_fMinY;
//                 m_pDebris->generate(Vector2d(fX, fY), Vector2d(0.0, 0.0));
//             }
//             break;
//         case EMITTER_DISTRIBUTION_POINT_SOURCE:
//             for (int i=0; i<nNrOfDebris; ++i)
//             {
//                 double fAngle = m_NormalDist(m_Generator)*m_fAngleVariance + m_fAngle;
//                 double fVelocity = m_NormalDist(m_Generator)*m_fVelocityVariance + m_fVelocity;
//                 m_pDebris->generate(Vector2d(2.0,0.0), fVelocity*Vector2d(std::cos(fAngle), sin(fAngle)));
//             }
//     }
}
