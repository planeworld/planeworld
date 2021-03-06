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
/// \file       collision_manager.cpp
/// \brief      Implementation of class "CCollisionManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-26
///
////////////////////////////////////////////////////////////////////////////////

#include "collision_manager.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests objects for collisions, depending on broad phase
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::detectCollisions()
{
    METHOD_ENTRY("CCollisionManager::detectCollisions")

    //--------------------------------------------------------------------------
    // Test dynamic objects
    //--------------------------------------------------------------------------
//     for (ObjectsByNameType::const_iterator ci = m_DynamicObjects.begin();
//         ci != m_DynamicObjects.end(); ++ci)
//     {
//         cj = ci;
//         ++cj;
//         while (cj != m_DynamicObjects.end())
//         {
//             // Test for overlapping depthlayers
//             if ((ci->second->getDepths() & cj->second->getDepths()) > 0)
//             {
// 
//                 // Test for overlapping bounding boxes
//                 if (ci->second->getGeometry()->getBoundingBox().overlaps(cj->second->getGeometry()->getBoundingBox(),1))
//                 {
//                     this->test(ci->second, cj->second);
//                     
// //                     CContact Contact;
// //                     Contact = (*ci)->collidesWith((*cj));
// //                     m_ContactList.push_back(Contact);
// //                     m_ContactList.insert(m_ContactList.end(), Contacts.begin(),
// //                                         Contacts.end());
//                 }
//             }
//             ++cj;
//         }
// //         for (std::list< CParticle* >::const_iterator cj = m_Particle.begin();
// //             cj != m_Particle.end(); ++cj)
// //         {
// //             switch((*ci)->getObjectType())
// //             {
// //                 case ObjectType::OBJECT_BODY:
// //                     this->test(static_cast<CObject*>((*ci)), (*cj));
// //             }
// //         }
// //         m_Graphics.setColor(0.0, 0.8, 0.0);
// //         m_Graphics.showVec((*ci)->getVelocity(), (*ci)->getCOM());
// //         m_Graphics.setColor(1.0, 1.0, 1.0);
//     }
    
//     //--------------------------------------------------------------------------
//     // Test static objects against particle
//     //--------------------------------------------------------------------------
//     for (ObjectsByNameType::const_iterator ci = m_StaticObjects.begin();
//         ci != m_StaticObjects.end(); ++ci)
//     {
//         for (std::list< CParticle* >::const_iterator cj = m_Particle.begin();
//             cj != m_Particle.end(); ++cj)
//         {
//             switch(ci->second->getObjectType())
//             {
//                 case ObjectType::OBJECT_BODY:
//                     this->test(static_cast<CObject*>(ci->second), (*cj));
//                     break;
//             }
//         }
//     }
//     
//     //--------------------------------------------------------------------------
//     // Test dynamic objects against particle
//     //--------------------------------------------------------------------------
//     for (ObjectsByNameType::const_iterator ci = m_DynamicObjects.begin();
//         ci != m_DynamicObjects.end(); ++ci)
//     {
//         for (std::list< CParticle* >::const_iterator cj = m_Particle.begin();
//             cj != m_Particle.end(); ++cj)
//         {
//             switch(ci->second->getObjectType())
//             {
//                 case ObjectType::OBJECT_BODY:
//                     this->test(static_cast<CObject*>(ci->second), (*cj));
//                     break;
//             }
//         }
//     }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests object against particle
///
/// \param _p1 Object
/// \param _p2 Particle
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CObject* _p1, CParticle* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
//     ShapesType::const_iterator ci  = _p1->getGeometry()->getShapes()->begin();
//     
//     while (ci != _p1->getGeometry()->getShapes()->end())
//     {
//         switch((*ci)->getShapeType())
//         {
//             case ShapeType::SHAPE_TERRAIN:
//                 this->test(static_cast<CTerrain*>((*ci,_p2));
//                 break;
//             case ShapeType::SHAPE_CIRCLE:
//                 this->test(static_cast<CCircle*>((*ci)), static_cast<CCircle*>((*ci)->getShapeBuf()), _p1, _p2);
//                 break;
//             case ShapeType::SHAPE_PLANET:
//                 this->test(static_cast<CPlanet*>((*ci)), static_cast<CPlanet*>((*ci)->getShapeBuf()), _p1, _p2);
//                 break;
//             case ShapeType::SHAPE_POLYGON:
//                 this->test(static_cast<CPolygon*>((*ci)->getShapeCur()), static_cast<CPolygon*>((*ci)->getShapeBuf()), _p1, _p2);
//                 break;
//             case ShapeType::SHAPE_NONE:
//                 break;
//         }
//         ++ci;
//     }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests circle shape against particle
///
/// \param _pC1 Circle shape, time t1
/// \param _pC0 Circle shape, time t0
/// \param _p1 Object 1
/// \param _p2 Particle
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCircle* _pC1, CCircle* _pC0, CObject* _p1, CParticle* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    CCircularBuffer<Vector2d>* pPos (_p2->getPositions());
    CCircularBuffer<Vector2d>* pVel(_p2->getVelocities());
    CCircularBuffer<Vector2d>* pPosP (_p2->getPreviousPositions());
    
    Vector2d vecPOC;
    Vector2d vecC0 = _pC0->getCenter();
    Vector2d vecC1 = _pC1->getCenter();
    double   fR0   = _pC0->getRadius();

    for (auto i=0u; i<pPos->size(); ++i)
    {
        CBoundingBox BBox;
        BBox.setLowerLeft(pPos->at(i));
        BBox.setUpperRight(pPos->at(i));
        BBox.update(pPos->at(i));
        if (_p1->getGeometry()->getBoundingBox().overlaps(BBox))
        {
            double   fT   = 2.0;
            Vector2d vecA = pPosP->at(i) - vecC0;
            Vector2d vecB = pPos->at(i) - pPosP->at(i) - vecC1 + vecC0;
            
            double fA = vecB.dot(vecB);
            double fB = 2.0 * vecA.dot(vecB);
            double fC = vecA.dot(vecA) - fR0*fR0;
            
            if (!isApproxZero(fA))
            {
                double fR = fB*fB - 4.0*fA*fC;
                if (fR >= 0.0)
                {
                    double fT1 = (-fB + sqrt(fR)) / (2.0*fA);
                    double fT2 = (-fB - sqrt(fR)) / (2.0*fA);
                
                    if ((fT1 >= 0.0) && ( fT1 < fT))
                    {
                        fT = fT1;
                    }
                    if ((fT2 >= 0.0) && ( fT2 < fT))
                    {
                        fT = fT2;
                    }
                }
                if (fT<=1.0)
                {
                    vecPOC = pPosP->at(i) + fT * (pPos->at(i) - pPosP->at(i));
                    double fParticleAngle = std::atan2(pPos->at(i)[1]-pPosP->at(i)[1], pPos->at(i)[0]-pPosP->at(i)[0]);
                    
                    Vector2d vecC = vecC0 + fT * ((vecC1 - vecC0));
                    
                    double fCircleAngle = std::atan2((vecC[0]-vecPOC[0]), -(vecC[1]-vecPOC[1]));
                                        
                    Vector2d vecNewVelOrth;
                    Vector2d vecNewVelTang;
                    
                    vecNewVelTang = Vector2d(std::cos(fCircleAngle),
                                             std::sin(fCircleAngle));
                    vecNewVelOrth = Vector2d(std::cos(fCircleAngle+M_PI*0.5),
                                             std::sin(fCircleAngle+M_PI*0.5));
                    
                    double fTang = std::cos(fCircleAngle-fParticleAngle)*1.0;
                    double fOrth = std::sin(fCircleAngle-fParticleAngle)*0.5;
                    double fDamping = sqrt(fTang*fTang+fOrth*fOrth)*0.7071;

                    
                    double fNorm = pVel->at(i).norm();
                    pVel->at(i)[0] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[0];
                    pVel->at(i)[1] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[1];
                    
                    // Add the velocity of the object because particle' are virtually weightless.
                    // Otherwise, they would be passed in the next step
                    pVel->at(i) += _p1->getVelocity();
                    
                    // pPos->at(i)= vecPOC+(vecNewVelOrth)/(vecNewVelOrth).norm()*0.001;
                    // Cannot use POC here, because particle' are virtually weightless. Thus, the
                    // object moves on and does not care about POC position.
                    pPos->at(i)= vecC1+(vecNewVelOrth)/(vecNewVelOrth).norm()*(fR0+0.001);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests planet shape against particle
///
/// \param _pP1 Planet shape, time t1
/// \param _pP0 Planet shape, time t0
/// \param _p1 Object 1
/// \param _p2 Particle
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CPlanet* _pP1, CPlanet* _pP0, CObject* _p1, CParticle* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
//     CCircularBuffer<Vector2d>* pPos (_p2->getPositions());
//     CCircularBuffer<Vector2d>* pVel(_p2->getVelocities());
//     CCircularBuffer<Vector2d>* pPosP (_p2->getPreviousPositions());
    
    Vector2d vecPOC;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests polygon shape against particle
///
/// \param _pP1 Polygon shape, time t1
/// \param _pP0 Polygon shape, time t0
/// \param _p1 Object 1
/// \param _p2 Particle
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CPolygon* _pP1, CPolygon* _pP0, CObject* _p1, CParticle* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    CCircularBuffer<Vector2d>* pPos (_p2->getPositions());
    CCircularBuffer<Vector2d>* pVel(_p2->getVelocities());
    CCircularBuffer<Vector2d>* pPosP (_p2->getPreviousPositions());
    
    double fDampOrth = 0.5;
    double fDampTang = 1.0;
    
    VertexListType::const_iterator ciP01;
    VertexListType::const_iterator ciP00;
    VertexListType::const_iterator ciP11;
    VertexListType::const_iterator ciP10;
    
    // Iterators storing the segment vertices of collision
    VertexListType::const_iterator ciPS1;
    VertexListType::const_iterator ciPS0;
    
    for (auto i=0u; i<pPos->size(); ++i)
    {
        CBoundingBox BBox;
        BBox.setLowerLeft(pPos->at(i));
        BBox.setUpperRight(pPos->at(i));
        BBox.update(pPosP->at(i));
        if (_p1->getGeometry()->getBoundingBox().overlaps(BBox))
        {
            ciP01 = _pP1->getVertices().begin();
            ciP00 = _pP0->getVertices().begin();
            ciP11 = ciP01;
            ciP10 = ciP00;
            ++ciP11;
            ++ciP10;
            ciPS1 = ciP11;
            ciPS0 = ciP01;
            
            Vector2d vecPOC;
            
            double fT = 2.0;
            double fAlpha = -1.0;
            
            while (ciP11 != _pP1->getVertices().end())
            {
                CCollisionManager::PointLineContact Contact;
                Contact = this->testPointLine(pPos->at(i), pPosP->at(i), (*ciP01), (*ciP11), (*ciP00), (*ciP10));
                if ((Contact.fT >= 0.0) && (Contact.fT < fT))
                {
                    fT     = Contact.fT;
                    fAlpha = Contact.fAlpha;
                    ciPS0  = ciP01;
                    ciPS1  = ciP11;
                }
                    
                ++ciP01; ++ciP00; ++ciP11; ++ciP10;
            }
            if (_pP1->getPolygonType() == PolygonType::LINE_LOOP)
            {
                ciP11 = _pP1->getVertices().begin();
                ciP10 = _pP1->getVertices().begin();

                CCollisionManager::PointLineContact Contact;
                Contact = this->testPointLine(pPos->at(i), pPosP->at(i), (*ciP01), (*ciP11), (*ciP00), (*ciP10));
                if ((Contact.fT >= 0.0) && (Contact.fT < fT))
                {
                    fT     = Contact.fT;
                    fAlpha = Contact.fAlpha;
                    ciPS0  = ciP01;
                    ciPS1  = ciP11;
                }
            }
            
            if (fT<=1.0)
            {
                // Calculate point of contact
                vecPOC = pPosP->at(i) + fT * (pPos->at(i) - pPosP->at(i));
                
                // Projection onto segment and separation of orthogonal an tangential component
                Vector2d vecNewVelTang = (pVel->at(i).dot(((*ciPS0)-(*ciPS1)).normalized())) * ((*ciPS0)-(*ciPS1)).normalized();
                Vector2d vecNewVelOrth = vecNewVelTang - pVel->at(i);
                
                pVel->at(i) = (vecNewVelOrth * fDampOrth + vecNewVelTang * fDampTang) * 0.7071;

                // Add the velocity of the object because particle' are virtually weightless.
                // Otherwise, they would be passed in the next step
                pVel->at(i) += _p1->getVelocity();
                
                // pPos->at(i)= vecPOC+(vecNewVelOrth)/(vecNewVelOrth).norm()*0.001;
                // Cannot use POC here, because particle' are virtually weightless. Thus, the
                // object moves on and does not care about POC position.
                pPos->at(i)  = (*ciPS0) + fAlpha * ((*ciPS1)-(*ciPS0)) + vecNewVelOrth.normalized()*0.001;
            } 
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests terrain shape against particle
///
/// The test of current particle position with the object's bounding box does not
/// prevent tunneling. Particle is meant to be a less accurate but fast and
/// physically plausible particle class.
///
/// \todo Build temporary bounding box for moving particle to have a correct
///       broad phase detection.
///
/// \param _p1 Terrain shape
/// \param _p2 Particle
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CTerrain* _p1, CParticle* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    CCircularBuffer<Vector2d>* pPos (_p2->getPositions());
    CCircularBuffer<Vector2d>* pVel(_p2->getVelocities());
    CCircularBuffer<Vector2d>* pPosP (_p2->getPreviousPositions());
    
    double fInc = _p1->getGroundResolution();

    for (auto i=0u; i<pPos->size(); ++i)
    {
        // Simple broad phase collision detection, does _not_ prevent tunneling.
//         if (_p1->getBoundingBox().isInside(pPos->at(i)))
        {
            Vector2d vecPOC;
            Vector2d vecTmp;
            double fT = 2.0;
            
            double fTerrainLeft  =  _p1->getCenter()[0]-_p1->getWidth()*0.5;
            double fTerrainRight =  _p1->getCenter()[0]+_p1->getWidth()*0.5;
            
            double fParticleLeft;
            double fParticleRight;
            if (pPos->at(i)[0] < (pPosP->at(i)[0]))
            {
                fParticleLeft  = pPos->at(i)[0];
                fParticleRight = pPosP->at(i)[0];
            }
            else
            {
                fParticleLeft  = pPosP->at(i)[0];
                fParticleRight = pPos->at(i)[0];
            }
            
            if (fParticleLeft > fTerrainLeft)
                fTerrainLeft = fParticleLeft;
            if (fParticleRight < fTerrainRight)
                fTerrainRight = fParticleRight;
                
            double fX0 = _p1->snapToTerrainGrid(fTerrainLeft); 
            double fX1 = fX0+fInc;
            
            double fY0 = _p1->getSurface(fX0);
            double fY1 = _p1->getSurface(fX1);
                         
            double fX0Seg = fX0;
            double fX1Seg = fX1;
            double fY0Seg = fY0;
            double fY1Seg = fY1;

            while ( fX0 < fTerrainRight )
            {
                double fAx = fX1 - fX0;
                double fAy = fY1 - fY0;
                double fCx = pPosP->at(i)[0]-fX0;
                double fCy = pPosP->at(i)[1]-fY0;
                double fDx = (pPos->at(i) - pPosP->at(i))[0];
                double fDy = (pPos->at(i) - pPosP->at(i))[1];

                double fTmpA = fAx*fCy-fAy*fCx;
                double fTmpB = fAx*fDy-fAy*fDx;
                
                double fTmpT = -(fTmpA / fTmpB);
                
                if (fTmpB != 0)
                {
                    if ((fTmpT >= 0.0) && (fTmpT <= 1.0))
                    {
                        if (fTmpT < fT)
                        {
                            vecTmp = pPosP->at(i) + fTmpT*(pPos->at(i)-pPosP->at(i));
                            if ((vecTmp-Vector2d(fX0,fY0)).norm() < Vector2d(fX1-fX0,fY1-fY0).norm())
                            {
                                fT = fTmpT;
                                vecPOC = vecTmp;
                                fX0Seg = fX0;
                                fX1Seg = fX1;
                                fY0Seg = fY0;
                                fY1Seg = fY1;
                            }
                        }
                    }
                }
                
                fX0 = fX1;
                fX1 = fX1+fInc;
                fY0 = fY1;
                fY1 = _p1->getSurface(fX1);
            }
            if (fT<=1.0)
            {
                double fSegAngle = std::atan2(fY1Seg-fY0Seg, fX1Seg-fX0Seg);
                double fPosAngle = std::atan2(pPos->at(i)[1]-pPosP->at(i)[1], pPos->at(i)[0]-pPosP->at(i)[0]);
                
                Vector2d vecNewVelOrth;
                Vector2d vecNewVelTang;
                
                vecNewVelTang = Vector2d(std::cos(fSegAngle),
                                         std::sin(fSegAngle));
                vecNewVelOrth = Vector2d(std::cos(fSegAngle+M_PI*0.5),
                                         std::sin(fSegAngle+M_PI*0.5));
                
                double fTang =    std::cos(fSegAngle-fPosAngle)*0.5;
                double fOrth =    std::sin(fSegAngle-fPosAngle)*0.5;
                double fDamping = sqrt(fTang*fTang+fOrth*fOrth)*0.7071;
                
                double fNorm = pVel->at(i).norm();
                pVel->at(i)[0] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[0];
                pVel->at(i)[1] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[1];
                pPos->at(i)=vecPOC+(pPosP->at(i)-vecPOC)/(pPosP->at(i)-vecPOC).norm()*0.001;
             
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two objects for collision
///
/// \param _p1 Object 1
/// \param _p2 Object 2
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CObject* _p1, CObject* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")

    ShapesType::const_iterator ci  = _p1->getGeometry()->getShapes().cbegin();
    ShapesType::const_iterator cj;
    
    while (ci != _p1->getGeometry()->getShapes().cend())
    {
        cj  = _p2->getGeometry()->getShapes().cbegin();
        while (cj != _p2->getGeometry()->getShapes().cend())
        {
            switch((*ci)->getShapeType())
            {
                case ShapeType::CIRCLE:
                    switch((*cj)->getShapeType())
                    {
                        case ShapeType::CIRCLE:
                            CCircle* pCircA1;
                            CCircle* pCircB1;
                            CCircle* pCircA0;
                            CCircle* pCircB0;
//                             pCircA1 = static_cast<CCircle*>((*ci)->getShapeCur());
//                             pCircB1 = static_cast<CCircle*>((*cj)->getShapeCur());
//                             pCircA0 = static_cast<CCircle*>((*ci)->getShapeBuf());
//                             pCircB0 = static_cast<CCircle*>((*cj)->getShapeBuf());
                            this->test(pCircA1, pCircA0, pCircB1, pCircB0, _p1, _p2);
                            break;
                        case ShapeType::PLANET:
//                             this->getSurfaceOfInterest();
                            break;
                        case ShapeType::POLYGON:
                            CCircle* pCirc1;
                            CCircle* pCirc0;
                            CPolygon* pPoly1;
                            CPolygon* pPoly0;
//                             pCirc1 = static_cast<CCircle*>((*ci)->getShapeCur());
//                             pCirc0 = static_cast<CCircle*>((*ci)->getShapeBuf());
//                             pPoly1 = static_cast<CPolygon*>((*cj)->getShapeCur());
//                             pPoly0 = static_cast<CPolygon*>((*cj)->getShapeBuf());
//                             this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        case ShapeType::TERRAIN:
                            break;
                        case ShapeType::NONE:
                            break;
                    }
                    break;
                case ShapeType::PLANET:
                    switch((*cj)->getShapeType())
                    {
                        case ShapeType::CIRCLE:
//                             CCircle* pCirc1;
//                             CCircle* pCirc0;
//                             CPolygon* pPoly1;
//                             CPolygon* pPoly0;
//                             pCirc1 = static_cast<CCircle*>((*cj));
//                             pCirc0 = static_cast<CCircle*>((*cj0));
//                             pPoly1 = static_cast<CPolygon*>((*ci));
//                             pPoly0 = static_cast<CPolygon*>((*ci0));
//                             this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        case ShapeType::PLANET:
//                             this->getSurfaceOfInterest();
                            break;
                        case ShapeType::POLYGON:
//                             CPolygon* pPolyA1;
//                             CPolygon* pPolyB1;
//                             CPolygon* pPolyA0;
//                             CPolygon* pPolyB0;
//                             pPolyA1 = static_cast<CPolygon*>((*ci));
//                             pPolyB1 = static_cast<CPolygon*>((*cj));
//                             pPolyA0 = static_cast<CPolygon*>((*ci0));
//                             pPolyB0 = static_cast<CPolygon*>((*cj0));
//                             this->test(pPolyA1, pPolyA0, pPolyB1, pPolyB0, _p1, _p2);
//                             this->test(pPolyB1, pPolyB0, pPolyA1, pPolyA0, _p2, _p1);
                            break;
                        case ShapeType::TERRAIN:
                            break;
                        case ShapeType::NONE:
                            break;
                    }
                    break;
                case ShapeType::POLYGON:
                    switch((*cj)->getShapeType())
                    {
                        case ShapeType::CIRCLE:
                        {
                            CCircle* pCirc1;
                            CCircle* pCirc0;
                            CPolygon* pPoly1;
                            CPolygon* pPoly0;
//                             pCirc1 = static_cast<CCircle*>((*cj)->getShapeCur());
//                             pCirc0 = static_cast<CCircle*>((*cj)->getShapeBuf());
//                             pPoly1 = static_cast<CPolygon*>((*ci)->getShapeCur());
//                             pPoly0 = static_cast<CPolygon*>((*ci)->getShapeBuf());
//                             this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        }
                        case ShapeType::PLANET:
                        {
//                             CPlanet* pPlanet1;
//                             CPlanet* pPlanet0;
//                             CPolygon* pPoly1;
//                             CPolygon* pPoly0;
//                             pPlanet1 = static_cast<CPlanet*>((*cj));
//                             pPlanet0 = static_cast<CPlanet*>((*cj0));
//                             pPoly1 = static_cast<CPolygon*>((*ci));
//                             pPoly0 = static_cast<CPolygon*>((*ci0));
//                             this->getSurfaceOfInterest();
                            break;
                        }
                        case ShapeType::POLYGON:
                        {
                            CPolygon* pPolyA1;
                            CPolygon* pPolyB1;
                            CPolygon* pPolyA0;
                            CPolygon* pPolyB0;
//                             pPolyA1 = static_cast<CPolygon*>((*ci)->getShapeCur());
//                             pPolyB1 = static_cast<CPolygon*>((*cj)->getShapeCur());
//                             pPolyA0 = static_cast<CPolygon*>((*ci)->getShapeBuf());
//                             pPolyB0 = static_cast<CPolygon*>((*cj)->getShapeBuf());
//                             this->test(pPolyA1, pPolyA0, pPolyB1, pPolyB0, _p1, _p2);
//                             this->test(pPolyB1, pPolyB0, pPolyA1, pPolyA0, _p2, _p1);
                            break;
                        }
                        case ShapeType::TERRAIN:
                            break;
                        case ShapeType::NONE:
                            break;
                    }
                    break;
                case ShapeType::TERRAIN:
                    break;
                case ShapeType::NONE:
                    break;
                          
            }
            ++cj;
        }
        ++ci;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Calculates the possible collision angle on planet's surface
///
/// Since the surface of the planet is a function, there is exactly one height
/// value for each angle. Thus, mapping the object (or its bounding box) on the
/// planets surface, one can determine the possible collision area.
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::getSurfaceOfInterest()
{
    METHOD_ENTRY("CCollisionManager::getSurfaceOfInterest")
    
//     double   fRad      = m_pPlanet->getRadius();
//     double   fHeight   = m_pPlanet->getHeight();
//     double   fPAng     = m_pPlanet->getAngle();
//     int      nSeed     = m_pPlanet->getSeed();
//     double   fSmooth   = m_pPlanet->getSmoothness();
//     Vector2d vecDist   = m_pPlanet->getCenter()-_pCamera->getCenter();
//     Vector2d vecCenter = m_pPlanet->getCenter()-_pCamera->getCenter();
//     
//     if (vecDist.norm() > fRad-_pCamera->getBoundingCircleRadius())
//     {
//         Vector2d    vecEx(1.0, 0.0);
//         double      fAng;    
//         double      fAngEnd;
//         PolygonType    LineT;
// 
//         double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecDist.norm()));
//         if (isnan(fAlpha))
//         {
//             fAng = 0.0;
//             fAngEnd = 2.0*M_PI;
//             LineT = LINE_LOOP;
//         }
//         else
//         {
//             double fAng0 = std::acos((- vecDist.dot(vecEx)) / vecDist.norm());
//             
//             if (vecDist[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
//             
//             fAng = fAng0-fAlpha;
//             fAngEnd = fAng0+fAlpha;
//             LineT = LINE_STRIP;
//         }
//         
//         if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
//         
//         double fInc = m_pPlanet->getGroundResolution() / m_pPlanet->getRadius();
//         
//         m_Graphics.beginLine(LineT, ShapeType::SHAPE_DEFAULT_DEPTH);
// 
//             while ( fAng <= fAngEnd)
//             {
//                 double fHght = m_pPlanet->getSurface().GetValue(std::cos(fAng-fPAng)*fRad,
//                                                                 std::sin(fAng-fPAng)*fRad,0.0);
//                 
//                 m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                               vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));
// //                 m_Graphics.dot(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
// //                                               vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));
// 
//                 if (_pCamera->getZoom() <= 1.0)
//                     fAng += fInc / _pCamera->getZoom();
//                 else
//                     fAng += fInc;
//             }
// 
//         m_Graphics.endLine();
//     }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two shapes (circles) for collision
///
/// \param _pCA1 Circle 1, time t1
/// \param _pCA0 Circle 1, time t0
/// \param _pCB1 Circle 2, time t1
/// \param _pCB0 Circle 2, time t0
/// \param _p1 Object 1
/// \param _p2 Object 2
///
/// \return collision?
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCircle* _pCA1, CCircle* _pCA0,
                             CCircle* _pCB1, CCircle* _pCB0,
                             CObject* _p1, CObject* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    Vector2d vecPA1 = _pCA1->getCenter();
    Vector2d vecPA0 = _pCA0->getCenter();
    Vector2d vecPB1 = _pCB1->getCenter();
    Vector2d vecPB0 = _pCB0->getCenter();
    double fRA = _pCA0->getRadius();
    double fRB = _pCB0->getRadius();
    double fR;
    
    if ((vecPA0-vecPB0).norm() >= (fRA + fRB))
    {
        fR = fRA+fRB;
    }
    else
    {
        fR = fRA-fRB; // No need for abs() since it will be squared
    }
    
    Vector2d vecA = vecPA0-vecPB0;
    Vector2d vecB = (vecPA1 - vecPA0) - (vecPB1 - vecPB0);
    
    double fT = 2.0;
    
    if (!isApproxZero(vecB.squaredNorm()))
    {
        double fP = (vecA.dot(vecB)) / (vecB.squaredNorm());
        double fQ = (vecA.squaredNorm() - fR*fR) / (vecB.squaredNorm());
        
        if ((fP*fP - fQ) >= 0.0)
        {
            double fT1 = -fP + sqrt(fP*fP - fQ);
            double fT2 = -fP - sqrt(fP*fP - fQ);
            
            if ((fT1 >= 0.0) && ( fT1 < fT))
                fT = fT1;
            if ((fT2 >= 0.0) && ( fT2 < fT))
                fT = fT2;
        }
    }
    if (fT <= 1.0)
    {
//         _p1->disableDynamics();
//         _p1->transform();
//         _p2->disableDynamics();
//         _p2->transform();
//         _p1->setVelocity(Vector2d(0.0,0.0));
//         _p2->setVelocity(Vector2d(0.0,0.0));
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two shapes (circles and polygons) for collision
///
/// The vertices of polygons must be tested separately to ensure collsion
/// detection at polygon angles < 90 degree.
///
/// \param _pA1 Circle, time t1
/// \param _pA0 Circle, time t0
/// \param _pB1 Polygon, time t1
/// \param _pB0 Polygon, time t0
/// \param _p1 Object 1
/// \param _p2 Object 2
///
/// \return collision?
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCircle* _pA1, CCircle* _pA0,
                             CPolygon* _pB1, CPolygon* _pB0,
                             CObject* _p1, CObject* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    Vector2d vecA1 = _pA1->getCenter();
    Vector2d vecA0 = _pA0->getCenter();
    
    VertexListType::const_iterator ciB01 = _pB1->getVertices().begin();
    VertexListType::const_iterator ciB00 = _pB0->getVertices().begin();
    
    VertexListType::const_iterator ciB11 = ciB01;
    VertexListType::const_iterator ciB10 = ciB00;
    ++ciB11;
    ++ciB10;
    
    Vector2d vecPOC;
    
    double fT = 2.0;
    
    while (ciB10 != _pB0->getVertices().end())
    {
        //-------------------------------------------
        // Test every point for collsion with circle
        //-------------------------------------------
        double fTmpT = this->testPointCircle((*ciB01),(*ciB00), _pA1, _pA0);
        if ((fTmpT >= 0.0) && ( fTmpT < fT))
        {
            fT = fTmpT;
            vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
        }
        
        //------------------------------------------
        // Test every line for collsion with circle
        //------------------------------------------
        fTmpT = this->testLineCircle((*ciB01),(*ciB11),(*ciB00),(*ciB10),_pA1,_pA0);
        if ((fTmpT >= 0.0) && ( fTmpT < fT))
        {
            Vector2d vecSeg0 = (*ciB00) + fTmpT*((*ciB01) - (*ciB00));
            Vector2d vecSeg1 = (*ciB10) + fTmpT*((*ciB11) - (*ciB10));
            double fLength = ((*ciB10) - (*ciB00)).norm();
            double fPOC = (vecSeg1 - vecSeg0).dot(
                        (vecA0 + fTmpT*(vecA1-vecA0)) - vecSeg0
                        ) / fLength;
            if ((fPOC >= 0.0) && (fPOC <= fLength))
            {
                fT = fTmpT;
                Vector2d vecSeg = vecSeg1 - vecSeg0;
                vecPOC = vecSeg0 + fPOC * vecSeg / vecSeg.norm();                        
            }
        }
        ++ciB00;
        ++ciB10;
        ++ciB01;
        ++ciB11;
    }
    if (_pB0->getPolygonType() == PolygonType::LINE_LOOP)
    {
        ciB11 = _pB1->getVertices().begin();
        ciB10 = _pB0->getVertices().begin();
        
        //-------------------------------------------
        // Test every point for collsion with circle
        //-------------------------------------------
        double fTmpT = this->testPointCircle((*ciB01),(*ciB00), _pA1, _pA0);
        if ((fTmpT >= 0.0) && ( fTmpT < fT))
        {
            fT = fTmpT;
            vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
        }
        
        //------------------------------------------
        // Test every line for collsion with circle
        //------------------------------------------
        fTmpT = this->testLineCircle((*ciB01),(*ciB11),(*ciB00),(*ciB10),_pA1,_pA0);
        if ((fTmpT >= 0.0) && ( fTmpT < fT))
        {
            Vector2d vecSeg0 = (*ciB00) + fTmpT*((*ciB01) - (*ciB00));
            Vector2d vecSeg1 = (*ciB10) + fTmpT*((*ciB11) - (*ciB10));
            double fLength = ((*ciB10) - (*ciB00)).norm();
            double fPOC = (vecSeg1 - vecSeg0).dot(
                        (vecA0 + fTmpT*(vecA1-vecA0)) - vecSeg0
                        ) / fLength;
            if ((fPOC >= 0.0) && (fPOC <= fLength))
            {
                fT = fTmpT;
                Vector2d vecSeg = vecSeg1 - vecSeg0;
                vecPOC = vecSeg0 + fPOC * vecSeg / vecSeg.norm();                        
            }
        }
    }
    
    if (fT<=1.0)
    {
//         _p1->disableDynamics();
//         _p1->transform();
//         _p2->disableDynamics();
//         _p2->transform();
//         _p1->setVelocity(Vector2d(0.0,0.0));
//         _p2->setVelocity(Vector2d(0.0,0.0));
    } 
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two shapes (polygons) for collision
///
/// This method test all vertices of one polygon against the lines between
/// vertices of the other polygon. Therefore, it has to be called twice with
/// parameters being swapped to cover all collisions.
///
/// \param _pA1 Polygon 1, time t1
/// \param _pA0 Polygon 1, time t0
/// \param _pB1 Polygon 2, time t1
/// \param _pB0 Polygon 2, time t0
/// \param _p1 Object 1
/// \param _p2 Object 2
///
/// \return collision?
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CPolygon* _pA1, CPolygon* _pA0,
                             CPolygon* _pB1, CPolygon* _pB0,
                             CObject* _p1, CObject* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    VertexListType::const_iterator ciA01 = _pA1->getVertices().begin();
    VertexListType::const_iterator ciA00 = _pA0->getVertices().begin();
    
    VertexListType::const_iterator ciA11 = ciA01;
    VertexListType::const_iterator ciA10 = ciA00;
    ++ciA11;
    ++ciA10;
    
    Vector2d vecPOC;
    
    double fT = 2.0;
    
    while (ciA11 != _pA1->getVertices().end())
    {
        VertexListType::const_iterator ciB1 = _pB1->getVertices().begin();
        VertexListType::const_iterator ciB0 = _pB0->getVertices().begin();
        
        while (ciB1 != _pB1->getVertices().end())
        {
            CCollisionManager::PointLineContact Contact;
            Contact = this->testPointLine((*ciB1), (*ciB0), (*ciA01), (*ciA11), (*ciA00), (*ciA10));
            if ((Contact.fT >= 0.0) && (Contact.fT < fT))
            {
                fT = Contact.fT;
                vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
            }
            
            ++ciB1; ++ciB0;
            
            /// \todo PolygonType Loop!
        }
        ++ciA01; ++ciA00; ++ciA11; ++ciA10;
    }
    if (_pA1->getPolygonType() == PolygonType::LINE_LOOP)
    {
        ciA11 = _pA1->getVertices().begin();
        ciA10 = _pA1->getVertices().begin();
        VertexListType::const_iterator ciB1 = _pB1->getVertices().begin();
        VertexListType::const_iterator ciB0 = _pB0->getVertices().begin();
        
        while (ciB1 != _pB1->getVertices().end())
        {
            CCollisionManager::PointLineContact Contact;
            Contact = this->testPointLine((*ciB1), (*ciB0), (*ciA01), (*ciA11), (*ciA00), (*ciA10));
            if ((Contact.fT >= 0.0) && (Contact.fT < fT))
            {
                fT = Contact.fT;
                vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
            }
            
            ++ciB1, ++ciB0;
            
            /// \todo PolygonType Loop!
        }
    }
    
    if (fT<=1.0)
    {
//         _p1->disableDynamics();
//         _p1->transform();
//         _p2->disableDynamics();
//         _p2->transform();
//         _p1->setVelocity(Vector2d(0.0,0.0));
//         _p2->setVelocity(Vector2d(0.0,0.0));
    } 
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests a line against a circle
///
/// \param _vecLA1 Line, start point, time t1
/// \param _vecLB1 Line, end point, time t1
/// \param _vecLA0 Line, start point, time t0
/// \param _vecLB0 Line, end point, time t0
/// \param _pC1    Circle, time t1
/// \param _pC0    Circle, time t0
///
/// \return Time of contact
///
///////////////////////////////////////////////////////////////////////////////
double CCollisionManager::testLineCircle(const Vector2d& _vecLA1, const Vector2d& _vecLB1,
                                         const Vector2d& _vecLA0, const Vector2d& _vecLB0,
                                         const CCircle* const _pC1,
                                         const CCircle* const _pC0
                                        )
{
    METHOD_ENTRY("CCollisionManager::testLineCircle")
    
    Vector2d vecA0 = _pC0->getCenter();
    Vector2d vecA1 = _pC1->getCenter();
    
    Vector2d vecA = _vecLB0 - _vecLA0;
    Vector2d vecB = _vecLB1 - _vecLB0 - _vecLA1 + _vecLA0;
    Vector2d vecC = vecA0 - _vecLA0;
    Vector2d vecD = vecA1 - vecA0 - _vecLA1 + _vecLA0;
    
    double fLength = (_vecLA0 - _vecLB0).norm();
    double fRA     = _pC0->getRadius();    
    double fT      = -1.0;
    
    if (vecB.isZero())
    {
        double fD = vecA[0] * vecD[1] - vecA[1] * vecD[0];
        double fN = vecA[0] * vecC[1] - vecA[1] * vecC[0];
        
        if (!isApproxZero(fD))
        {
            double fT1 = (fLength * fRA - fN) / fD;
            if (fT1 >= 0.0)
                fT = fT1;
            double fT2 = (fLength * fRA + fN) / fD;
            if ((fT2 >= 0.0) && (fT2 < fT))
                fT = fT2;
        }
    }
    else
    {
        double fA  = vecB[0] * vecD[1] - vecB[1] * vecD[0];
        double fB  = vecB[0] * vecC[1] - vecB[1] * vecC[0] + 
                     vecA[0] * vecD[1] - vecA[1] * vecD[0];
        double fC1 = vecA[0] * vecC[1] - vecA[1] * vecC[0] -
                     fRA*fLength;
        double fC2 = vecC[0] * vecA[1] - vecC[1] * vecA[0] -
                     fRA*fLength;
        double fR1 = fB*fB - 4.0*fA*fC1;
        double fR2 = fB*fB + 4.0*fA*fC2;
        
        if (!isApproxZero(fA))
        {
            if (fR1 >= 0.0)
            {
                double fT1 = (-fB + sqrt(fR1)) / (2.0*fA);
                double fT2 = (-fB - sqrt(fR1)) / (2.0*fA);
            
                if (fT1 >= 0.0)
                    fT = fT1;
                if ((fT2 >= 0.0) && ( fT2 < fT))
                    fT = fT2;
            }
            if (fR2 >= 0.0)
            {
                double fT1 = (-fB + sqrt(fR2)) / (2.0*fA);
                double fT2 = (-fB - sqrt(fR2)) / (2.0*fA);
            
                if ((fT1 >= 0.0) && ( fT1 < fT))
                    fT = fT1;
                if ((fT2 >= 0.0) && ( fT2 < fT))
                    fT = fT2;
            }
        }
    }
    return fT;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests a point against a circle
///
/// \param _vecP1 Point, time t1
/// \param _vecP0 Point, time t0
/// \param _vecC1 Circle, time t1
/// \param _vecC0 Circle, time t0
///
/// \return Time of contact
///
///////////////////////////////////////////////////////////////////////////////
double CCollisionManager::testPointCircle(const Vector2d& _vecP1, const Vector2d& _vecP0,
                                          const CCircle* const _pC1,
                                          const CCircle* const _pC0)
{
    METHOD_ENTRY("CCollisionManager::testPointCircle")
    
    double fT = -1.0;
    
    Vector2d vecA = _vecP0 - _pC0->getCenter();
    Vector2d vecB = _vecP1 - _vecP0 - _pC1->getCenter() + _pC0->getCenter();
    
    double fA = vecB.dot(vecB);
    double fB = 2.0 * vecA.dot(vecB);
    double fC = vecA.dot(vecA) - _pC0->getRadius()*_pC0->getRadius();
    
    if (!isApproxZero(fA))
    {
        double fR = fB*fB - 4*fA*fC;
        if (fR >= 0.0)
        {
            double fT1 = (-fB + sqrt(fR)) / (2.0*fA);
            double fT2 = (-fB - sqrt(fR)) / (2.0*fA);
        
            if (fT1 >= 0.0)
            {
                fT = fT1;
            }
            if ((fT2 >= 0.0) && ( fT2 < fT))
            {
                fT = fT2;
            }
        }
    }
    return fT;
}
    

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests a point against a line
///
/// \param _vecP1  Point, time t1
/// \param _vecP0  Point, time t0
/// \param _vecLA1 Line, start point, time t1
/// \param _vecLB1 Line, end point, time t1
/// \param _vecLA0 Line, start point, time t0
/// \param _vecLB0 Line, end point, time t0
///
/// \return Time and relative segment position of contact
///
///////////////////////////////////////////////////////////////////////////////
CCollisionManager::PointLineContact CCollisionManager::testPointLine(
                                        const Vector2d& _vecP1,  const Vector2d& _vecP0,
                                        const Vector2d& _vecLA1, const Vector2d& _vecLB1,
                                        const Vector2d& _vecLA0, const Vector2d& _vecLB0)
{
    METHOD_ENTRY("CCollisionManager::testPointLine")
    
    double fAx = (_vecLA0 - _vecP0)[0];
    double fAy = (_vecLA0 - _vecP0)[1];
    double fBx = (_vecLB0 - _vecLA0)[0];
    double fBy = (_vecLB0 - _vecLA0)[1];
    double fCx = (_vecP1 - _vecP0 - _vecLA1 + _vecLA0)[0];
    double fCy = (_vecP1 - _vecP0 - _vecLA1 + _vecLA0)[1];
    double fDx = (_vecLB0 - _vecLA0 - _vecLB1 + _vecLA1)[0];
    double fDy = (_vecLB0 - _vecLA0 - _vecLB1 + _vecLA1)[1];
    
    CCollisionManager::PointLineContact Contact;
    Contact.fT = -1.0;
    Contact.fAlpha = -1.0;
    
    if (isApproxZero(fDx) && isApproxZero(fDy))
    {
        double fTmpA = fBx*fCy-fBy*fCx;
        double fTmpB = fAx*fCy-fAy*fCx;
        
        double fAlpha = - fTmpB / fTmpA;
        
        double fTmpT = -1.0;
        if ((fAlpha >= 0.0) && (fAlpha <= 1.0))
        {
            if (!isApproxZero(fCx))
            {
                fTmpT = (fAx+fAlpha*fBx) / (fCx);
                if (fTmpT >= 0.0)
                {
                    Contact.fT = fTmpT;
                    Contact.fAlpha = fAlpha;
                }
            }
            else
            {
                fTmpT = (fAy+fAlpha*fBy) / (fCy);
                if (fTmpT >= 0.0)
                {
                    Contact.fT = fTmpT;
                    Contact.fAlpha = fAlpha;
                }
            }
        }
    }
    else
    {
        double fTmpA = fBx*fDy-fBy*fDx;
        double fTmpB = fAx*fDy+fBx*fCy-fAy*fDx-fBy*fCx;
        double fTmpC = fAx*fCy-fAy*fCx;
        
        double fTmpR = fTmpB*fTmpB-4*fTmpA*fTmpC;
        double fAlpha1 = -1.0;
        double fAlpha2 = -1.0;
                
        if (!isApproxZero(fTmpR))
        {
            fAlpha1 = (-fTmpB + sqrt(fTmpR)) / (2*fTmpA);
            fAlpha2 = (-fTmpB - sqrt(fTmpR)) / (2*fTmpA);
        }
        
        double fTmpT1 = -1.0;
        double fTmpT2 = -1.0;
        if ((fAlpha1 >= 0.0) && (fAlpha1 <= 1.0))
        {
            fTmpT1 = (fAx+fAlpha1*fBx) / (fCx+fAlpha1*fDx);
        }
        if ((fAlpha2 >= 0.0) && (fAlpha2 <= 1.0))
        {
            fTmpT2 = (fAx+fAlpha2*fBx) / (fCx+fAlpha2*fDx);
        }
        if ((fTmpT1 >= 0.0))
        {
            Contact.fT = fTmpT1;
            Contact.fAlpha = fAlpha1;
        }
        if ((fTmpT2 >= 0.0) && (fTmpT2 < Contact.fT))
        {
            Contact.fT = fTmpT2;
            Contact.fAlpha = fAlpha2;
        }
    }
    
    return Contact;
}
