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

    std::list< IObject* >::const_iterator cj;
    
    for (std::list< IObject* >::const_iterator ci = m_DynamicObjects.begin();
        ci != m_DynamicObjects.end(); ++ci)
    {
        cj = ci;
        ++cj;
        while (cj != m_DynamicObjects.end())
        {
            // Test for overlapping depthlayers
            if (((*ci)->getDepths() & (*cj)->getDepths()) > 0)
            {

                // Test for overlapping bounding boxes
                if ((*ci)->getGeometry()->getBoundingBox().overlaps((*cj)->getGeometry()->getBoundingBox()))
                {
                    switch((*ci)->getObjectType())
                    {
                        case OBJECT_BODY:
                            this->test(static_cast<CBody*>((*ci)),static_cast<CBody*>((*cj)));
                    }
//                     CContact Contact;
//                     Contact = (*ci)->collidesWith((*cj));
//                     m_ContactList.push_back(Contact);
//                     m_ContactList.insert(m_ContactList.end(), Contacts.begin(),
//                                         Contacts.end());
                }
            }
            ++cj;
        }
//         for (std::list< CDebris* >::const_iterator cj = m_Debris.begin();
//             cj != m_Debris.end(); ++cj)
//         {
//             switch((*ci)->getObjectType())
//             {
//                 case OBJECT_BODY:
//                     this->test(static_cast<CBody*>((*ci)), (*cj));
//             }
//         }
//         m_Graphics.setColor(0.0, 0.8, 0.0);
//         m_Graphics.showVec((*ci)->getVelocity(), (*ci)->getCOM());
//         m_Graphics.setColor(1.0, 1.0, 1.0);
    }
    
    for (std::list< IObject* >::const_iterator ci = m_StaticObjects.begin();
        ci != m_StaticObjects.end(); ++ci)
    {
        for (std::list< CDebris* >::const_iterator cj = m_Debris.begin();
            cj != m_Debris.end(); ++cj)
        {
            switch((*ci)->getObjectType())
            {
                case OBJECT_BODY:
                    this->test(static_cast<CBody*>((*ci)), (*cj));
            }
        }
    }
    
    METHOD_EXIT("CCollisionManager::detectCollisions")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests object against debris
///
/// \param _p1 Object
/// \param _p2 Debris
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CBody* _p1, CDebris* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    std::list<IShape*>::const_iterator ci  = _p1->getGeometry()->getShapes()->begin();
    std::list<IShape*>::const_iterator ci0 = _p1->getGeometry()->getPrevShapes()->begin();
    
    while (ci != _p1->getGeometry()->getShapes()->end())
    {
        switch((*ci)->getShapeType())
        {
            case SHAPE_TERRAIN:
                this->test(static_cast<CTerrain*>((*ci)),_p2);
//             case SHAPE_CIRCLE:
//                 this->test(static_cast<CCircle*>((*ci)), static_cast<CCircle*>((*ci0)), _p1, _p2);
        }
        ++ci;
        ++ci0;
    }
    
    METHOD_EXIT("CCollisionManager::test")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests circle shape against debris
///
/// \param _pC1 Circle shape, time t1
/// \param _pC0 Circle shape, time t0
/// \param _p1 Object 1
/// \param _p2 Debris
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCircle* _pC1, CCircle* _pC0, CBody* _p1, CDebris* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    boost::circular_buffer<Vector2d>* pPositions (_p2->getPositions());
    boost::circular_buffer<Vector2d>* pVelocities(_p2->getVelocities());
    boost::circular_buffer<Vector2d>* pPreviousPositions (_p2->getPreviousPositions());
    boost::circular_buffer<Vector2d>* pPreviousVelocities(_p2->getPreviousVelocities());
    boost::circular_buffer<Vector2d>::iterator itPos = pPositions->begin();
    boost::circular_buffer<Vector2d>::iterator itVel = pVelocities->begin();
    boost::circular_buffer<Vector2d>::iterator itPosP = pPreviousPositions->begin();
    boost::circular_buffer<Vector2d>::iterator itVelP = pPreviousVelocities->begin();
    
    Vector2d vecPOC;
    Vector2d vecC0 = _pC0->getCenter();
    Vector2d vecC1 = _pC1->getCenter();
    double   fR0   = _pC0->getRadius();

    while (itPos != pPositions->end())
    {
//         if (_p1->getGeometry()->getBoundingBox().isInside((*itPos)))
        {
            double   fT   = 2.0;
            Vector2d vecA = (*itPosP) - vecC0;
            Vector2d vecB = (*itPos) - (*itPosP) - vecC1 + vecC0;
            
            double fA = vecB.dot(vecB);
            double fB = 2 * vecA.dot(vecB);
            double fC = vecA.dot(vecA) - fR0*fR0;
            
            if (fA != 0.0)
            {
                double fR = fB*fB - 4*fA*fC;
                if (fR >= 0.0)
                {
                    double fT1 = (-fB + sqrt(fR)) / (2*fA);
                    double fT2 = (-fB - sqrt(fR)) / (2*fA);
                
                    if ((fT1 >= 0.0) && ( fT1 < fT))
                    {
                        fT = fT1;
                        vecPOC = (*itPos) + fT * ((*itPos) - (*itPosP));
                    }
                    if ((fT2 >= 0.0) && ( fT2 < fT))
                    {
                        fT = fT2;
                        vecPOC = (*itPos) + fT * ((*itPos) - (*itPosP));
                    }
                }
                if (fT<=1.0)
                {
                    (*itPos) = vecPOC+((*itPosP)-vecPOC)*0.1;
                    (*itVel) = -(*itVel);
                }
            }
        }
        ++itPos;
        ++itPosP;
        ++itVel;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests terrain shape against debris
///
/// The test of current debris position with the object's bounding box does not
/// prevent tunneling. Debris is meant to be a less accurate but fast and
/// physically plausible particle class.
///
/// \todo Build temporary bounding box for moving debris to have a correct
///       broad phase detection.
///
/// \param _p1 Terrain shape
/// \param _p2 Debris
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CTerrain* _p1, CDebris* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    boost::circular_buffer<Vector2d>* pPositions (_p2->getPositions());
    boost::circular_buffer<Vector2d>* pVelocities(_p2->getVelocities());
    boost::circular_buffer<Vector2d>* pPreviousPositions (_p2->getPreviousPositions());
    boost::circular_buffer<Vector2d>* pPreviousVelocities(_p2->getPreviousVelocities());
    boost::circular_buffer<Vector2d>::iterator itPos = pPositions->begin();
    boost::circular_buffer<Vector2d>::iterator itVel = pVelocities->begin();
    boost::circular_buffer<Vector2d>::iterator itPosP = pPreviousPositions->begin();
    boost::circular_buffer<Vector2d>::iterator itVelP = pPreviousVelocities->begin();
    
    double fInc = _p1->getGroundResolution();

    while (itPos != pPositions->end())
    {
        // Simple broad phase collision detection, does _not_ prevent tunneling.
//         if (_p1->getBoundingBox().isInside((*itPos)))
        {
            Vector2d vecPOC;
            Vector2d vecTmp;
            double fT = 2.0;
            
            double fTerrainLeft  =  _p1->getCenter()[0]-_p1->getWidth()*0.5;
            double fTerrainRight =  _p1->getCenter()[0]+_p1->getWidth()*0.5;
            
            double fDebrisLeft;
            double fDebrisRight;
            if ((*itPos)[0] < ((*itPosP)[0]))
            {
                fDebrisLeft  = (*itPos )[0];
                fDebrisRight = (*itPosP)[0];
            }
            else
            {
                fDebrisLeft  = (*itPosP)[0];
                fDebrisRight = (*itPos )[0];
            }
            
            if (fDebrisLeft > fTerrainLeft)
                fTerrainLeft = fDebrisLeft;
            if (fDebrisRight < fTerrainRight)
                fTerrainRight = fDebrisRight;
                
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
                double fCx = (*itPosP)[0]-fX0;
                double fCy = (*itPosP)[1]-fY0;
                double fDx = ((*itPos) - (*itPosP))[0];
                double fDy = ((*itPos) - (*itPosP))[1];

                double fTmpA = fAx*fCy-fAy*fCx;
                double fTmpB = fAx*fDy-fAy*fDx;
                
                double fTmpT = -(fTmpA / fTmpB);
                
                if (fTmpB != 0)
                {
                    if ((fTmpT >= 0.0) && (fTmpT <= 1.0))
                    {
                        if (fTmpT < fT)
                        {
                            vecTmp = (*itPosP) + fTmpT*((*itPos)-(*itPosP));
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
                double fPosAngle = std::atan2((*itPos)[1]-(*itPosP)[1], (*itPos)[0]-(*itPosP)[0]);
                
                Vector2d vecNewVelOrth;
                Vector2d vecNewVelTang;
                
                vecNewVelTang = Vector2d(std::cos(fSegAngle),
                                         std::sin(fSegAngle));
                vecNewVelOrth = Vector2d(std::cos(fSegAngle+M_PI*0.5),
                                         std::sin(fSegAngle+M_PI*0.5));
                
                double fTang = std::cos(fSegAngle-fPosAngle)*0.5;
                double fOrth = std::sin(fSegAngle-fPosAngle)*0.5;
                double fDamping = sqrt(fTang*fTang+fOrth*fOrth);
                
                double fNorm = (*itVel).norm();
                (*itVel)[0] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[0];
                (*itVel)[1] = ((fOrth*vecNewVelOrth+fTang*vecNewVelTang).normalized() * fDamping * fNorm)[1];
                (*itPos)=vecPOC+((*itPosP)-vecPOC)/((*itPosP)-vecPOC).norm()*0.01;
             
            }
        }
        ++itPos; ++itVel; ++itPosP; ++itVelP;
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
void CCollisionManager::test(CBody* _p1, CBody* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")

    std::list<IShape*>::const_iterator ci  = _p1->getGeometry()->getShapes()->begin();
    std::list<IShape*>::const_iterator ci0 = _p1->getGeometry()->getPrevShapes()->begin();
    std::list<IShape*>::const_iterator cj;
    std::list<IShape*>::const_iterator cj0;
    
    while (ci != _p1->getGeometry()->getShapes()->end())
    {
        cj  = _p2->getGeometry()->getShapes()->begin();
        cj0 = _p2->getGeometry()->getPrevShapes()->begin();
        while (cj != _p2->getGeometry()->getShapes()->end())
        {
            switch((*ci)->getShapeType())
            {
                case SHAPE_CIRCLE:
                    switch((*cj)->getShapeType())
                    {
                        case SHAPE_CIRCLE:
                            CCircle* pCircA1;
                            CCircle* pCircB1;
                            CCircle* pCircA0;
                            CCircle* pCircB0;
                            pCircA1 = static_cast<CCircle*>((*ci));
                            pCircB1 = static_cast<CCircle*>((*cj));
                            pCircA0 = static_cast<CCircle*>((*ci0));
                            pCircB0 = static_cast<CCircle*>((*cj0));
                            this->test(pCircA1, pCircA0, pCircB1, pCircB0, _p1, _p2);
                            break;
                        case SHAPE_PLANET:
//                             this->getSurfaceOfInterest();
                            break;
                        case SHAPE_POLYLINE:
                            CCircle* pCirc1;
                            CCircle* pCirc0;
                            CPolyLine* pPoly1;
                            CPolyLine* pPoly0;
                            pCirc1 = static_cast<CCircle*>((*ci));
                            pCirc0 = static_cast<CCircle*>((*ci0));
                            pPoly1 = static_cast<CPolyLine*>((*cj));
                            pPoly0 = static_cast<CPolyLine*>((*cj0));
                            this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        case SHAPE_RECTANGLE:
// //                             this->test((*ci),(*cj));
                            break;
                    }
                    break;
                case SHAPE_PLANET:
                    switch((*cj)->getShapeType())
                    {
                        case SHAPE_CIRCLE:
//                             CCircle* pCirc1;
//                             CCircle* pCirc0;
//                             CPolyLine* pPoly1;
//                             CPolyLine* pPoly0;
//                             pCirc1 = static_cast<CCircle*>((*cj));
//                             pCirc0 = static_cast<CCircle*>((*cj0));
//                             pPoly1 = static_cast<CPolyLine*>((*ci));
//                             pPoly0 = static_cast<CPolyLine*>((*ci0));
//                             this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        case SHAPE_PLANET:
//                             this->getSurfaceOfInterest();
                            break;
                        case SHAPE_POLYLINE:
//                             CPolyLine* pPolyA1;
//                             CPolyLine* pPolyB1;
//                             CPolyLine* pPolyA0;
//                             CPolyLine* pPolyB0;
//                             pPolyA1 = static_cast<CPolyLine*>((*ci));
//                             pPolyB1 = static_cast<CPolyLine*>((*cj));
//                             pPolyA0 = static_cast<CPolyLine*>((*ci0));
//                             pPolyB0 = static_cast<CPolyLine*>((*cj0));
//                             this->test(pPolyA1, pPolyA0, pPolyB1, pPolyB0, _p1, _p2);
//                             this->test(pPolyB1, pPolyB0, pPolyA1, pPolyA0, _p2, _p1);
                            break;
                        case SHAPE_RECTANGLE:
//     //                             this->test((*ci),(*cj));
                            break;
                    }
                    break;
                case SHAPE_POLYLINE:
                    switch((*cj)->getShapeType())
                    {
                        case SHAPE_CIRCLE:
                        {
                            CCircle* pCirc1;
                            CCircle* pCirc0;
                            CPolyLine* pPoly1;
                            CPolyLine* pPoly0;
                            pCirc1 = static_cast<CCircle*>((*cj));
                            pCirc0 = static_cast<CCircle*>((*cj0));
                            pPoly1 = static_cast<CPolyLine*>((*ci));
                            pPoly0 = static_cast<CPolyLine*>((*ci0));
                            this->test(pCirc1, pCirc0, pPoly1, pPoly0, _p1, _p2);
                            break;
                        }
                        case SHAPE_PLANET:
                        {
//                             CPlanet* pPlanet1;
//                             CPlanet* pPlanet0;
//                             CPolyLine* pPoly1;
//                             CPolyLine* pPoly0;
//                             pPlanet1 = static_cast<CPlanet*>((*cj));
//                             pPlanet0 = static_cast<CPlanet*>((*cj0));
//                             pPoly1 = static_cast<CPolyLine*>((*ci));
//                             pPoly0 = static_cast<CPolyLine*>((*ci0));
//                             this->getSurfaceOfInterest();
                            break;
                        }
                        case SHAPE_POLYLINE:
                        {
                            CPolyLine* pPolyA1;
                            CPolyLine* pPolyB1;
                            CPolyLine* pPolyA0;
                            CPolyLine* pPolyB0;
                            pPolyA1 = static_cast<CPolyLine*>((*ci));
                            pPolyB1 = static_cast<CPolyLine*>((*cj));
                            pPolyA0 = static_cast<CPolyLine*>((*ci0));
                            pPolyB0 = static_cast<CPolyLine*>((*cj0));
                            this->test(pPolyA1, pPolyA0, pPolyB1, pPolyB0, _p1, _p2);
                            this->test(pPolyB1, pPolyB0, pPolyA1, pPolyA0, _p2, _p1);
                            break;
                        }
                        case SHAPE_RECTANGLE:
// //                             this->test((*ci),(*cj));
                            break;
                    }
                    break;
                case SHAPE_RECTANGLE:
                    switch((*cj)->getShapeType())
                    {
                        case SHAPE_CIRCLE:
// //                             this->test((*cj),(*ci));
                            break;
                        case SHAPE_PLANET:
//                             this->getSurfaceOfInterest();
                            break;
                        case SHAPE_POLYLINE:
// //                             this->test((*cj),(*ci));
                            break;
                        case SHAPE_RECTANGLE:
// //                             this->test((*ci),(*cj));
                            break;
                    }
                    break;
            }
            ++cj;
            ++cj0;
        }
        ++ci;
        ++ci0;
    }
        
    METHOD_EXIT("CCollisionManager::test")
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
//         LineType    LineT;
// 
//         double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecDist.norm()));
//         if (isnan(fAlpha))
//         {
//             fAng = 0.0;
//             fAngEnd = 2.0*M_PI;
//             LineT = GRAPHICS_LINETYPE_LOOP;
//         }
//         else
//         {
//             double fAng0 = std::acos((- vecDist.dot(vecEx)) / vecDist.norm());
//             
//             if (vecDist[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
//             
//             fAng = fAng0-fAlpha;
//             fAngEnd = fAng0+fAlpha;
//             LineT = GRAPHICS_LINETYPE_STRIP;
//         }
//         
//         if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
//         
//         double fInc = m_pPlanet->getGroundResolution() / m_pPlanet->getRadius();
//         
//         m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);
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
    
    METHOD_EXIT("CCollisionManager::getSurfaceOfInterest")
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
                             CBody* _p1, CBody* _p2)
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
        fR = fRA-fRB;
    }
    
    Vector2d vecA = vecPA0-vecPB0;
    Vector2d vecB = (vecPA1 - vecPA0) - (vecPB1 - vecPB0);
    
    double fT = 2.0;
    
    if (vecB.squaredNorm() != 0.0)
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
        // Colliding objects
//         m_Graphics.setColor(0.0, 0.8, 0.0);
//         m_Graphics.rect(_p1->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p1->getGeometry().getBoundingBox().getUpperRight());
//         m_Graphics.rect(_p2->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p2->getGeometry().getBoundingBox().getUpperRight());
//         
//         // Point of collision
//         m_Graphics.setColor(0.8, 0.0, 0.0);
//         m_Graphics.dot(vecPA0+(vecPB0-vecPA0)/sqrt((vecPB0-vecPA0).dot((vecPB0-vecPA0)))*fRA);
//         m_Graphics.setColor(1.0, 1.0, 1.0);
        
        _p1->disableDynamics();
        _p1->transform();
        _p2->disableDynamics();
        _p2->transform();
//         _p1->setVelocity(-_p1->getVelocity());
//         _p2->setVelocity(-_p2->getVelocity());
    }
    
    METHOD_EXIT("CCollisionManager::test")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two shapes (circles and polylines) for collision
///
/// \param _pA1 Circle, time t1
/// \param _pA0 Circle, time t0
/// \param _pB1 Polyline, time t1
/// \param _pB0 Polyline, time t0
/// \param _p1 Object 1
/// \param _p2 Object 2
///
/// \return collision?
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCircle* _pA1, CCircle* _pA0,
                             CPolyLine* _pB1, CPolyLine* _pB0,
                             CBody* _p1, CBody* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    Vector2d vecA1 = _pA1->getCenter();
    Vector2d vecA0 = _pA0->getCenter();
    double fRA = _pA0->getRadius();
    
    std::list<Vector2d>::const_iterator ciB01 = _pB1->getVertices().begin();
    std::list<Vector2d>::const_iterator ciB00 = _pB0->getVertices().begin();
    
    std::list<Vector2d>::const_iterator ciB11 = ciB01;
    std::list<Vector2d>::const_iterator ciB10 = ciB00;
    ++ciB11;
    ++ciB10;
    
    Vector2d vecPOC;
    
    double fT = 2.0;
    
    while (ciB10 != _pB0->getVertices().end())
    {
        double fLength = ((*ciB10) - (*ciB00)).norm();
                
        //-------------------------------------------
        // Test every point for collsion with circle
        //-------------------------------------------
        {
            Vector2d vecA = (*ciB00) - vecA0;
            Vector2d vecB = (*ciB01) - (*ciB00) - vecA1 + vecA0;
            
            double fA = vecB.dot(vecB);
            double fB = 2 * vecA.dot(vecB);
            double fC = vecA.dot(vecA) - fRA*fRA;
            
            if (fA != 0.0)
            {
                double fR = fB*fB - 4*fA*fC;
                if (fR >= 0.0)
                {
                    double fT1 = (-fB + sqrt(fR)) / (2*fA);
                    double fT2 = (-fB - sqrt(fR)) / (2*fA);
                
                    if ((fT1 >= 0.0) && ( fT1 < fT))
                    {
                        fT = fT1;
                        vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
                    }
                    if ((fT2 >= 0.0) && ( fT2 < fT))
                    {
                        fT = fT2;
                        vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
                    }
                }
            }
        }
        //------------------------------------------
        // Test every line for collsion with circle
        //------------------------------------------
        {
            Vector2d vecA = (*ciB10) - (*ciB00);
            Vector2d vecB = (*ciB11) - (*ciB10) - (*ciB01) + (*ciB00);
            Vector2d vecC = vecA0 - (*ciB00);
            Vector2d vecD = vecA1 - vecA0 - (*ciB01) + (*ciB00);
            
            double fTTmp = 2.0;
            
            if (vecB.isZero())
            {
                double fD = vecA[0] * vecD[1] - vecA[1] * vecD[0];
                double fN = vecA[0] * vecC[1] - vecA[1] * vecC[0];
                
                if (fD != 0)
                {
                    double fT1 = (fLength * fRA - fN) / fD;
                    if ((fT1 >= 0.0) && (fT1 < fTTmp))
                        fTTmp = fT1;
                    double fT2 = (fLength * fRA + fN) / fD;
                    if ((fT2 >= 0.0) && (fT2 < fTTmp))
                        fTTmp = fT2;
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
                double fR1 = fB*fB - 4*fA*fC1;
                double fR2 = fB*fB + 4*fA*fC2;
                
                if (fA != 0.0)
                {
                    if (fR1 >= 0.0)
                    {
                        double fT1 = (-fB + sqrt(fR1)) / (2*fA);
                        double fT2 = (-fB - sqrt(fR1)) / (2*fA);
                    
                        if ((fT1 >= 0.0) && ( fT1 < fTTmp))
                            fTTmp = fT1;
                        if ((fT2 >= 0.0) && ( fT2 < fTTmp))
                            fTTmp = fT2;
                    }
                    if (fR2 >= 0.0)
                    {
                        double fT1 = (-fB + sqrt(fR2)) / (2*fA);
                        double fT2 = (-fB - sqrt(fR2)) / (2*fA);
                    
                        if ((fT1 >= 0.0) && ( fT1 < fTTmp))
                            fTTmp = fT1;
                        if ((fT2 >= 0.0) && ( fT2 < fTTmp))
                            fTTmp = fT2;
                    }
                }
            }
            if (fTTmp < fT)
            {
                Vector2d vecSeg0 = (*ciB00) + fTTmp*((*ciB01) - (*ciB00));
                Vector2d vecSeg1 = (*ciB10) + fTTmp*((*ciB11) - (*ciB10));
                double fPOC = (vecSeg1 - vecSeg0).dot(
                            (vecA0 + fTTmp*(vecA1-vecA0)) - vecSeg0
                            ) / fLength;
                if ((fPOC >= 0.0) && (fPOC <= fLength))
                {
                    fT = fTTmp;
                    Vector2d vecSeg = vecSeg1 - vecSeg0;
                    vecPOC = vecSeg0 + fPOC * vecSeg / vecSeg.norm();                        
                }
            }
//             double fPOC =
//                  (((*ciB10) + fTTmp*((*ciB11) - (*ciB10))) - 
//                   ((*ciB00) + fTTmp*((*ciB01) - (*ciB00)))).dot(
//                   (  vecA0  + fTTmp*(  vecA1  -   vecA0)) - 
//                   ((*ciB00) + fTTmp*((*ciB01) - (*ciB00)))) / fLength;
//             if ((fPOC >= 0.0) && (fPOC <= fLength))
//             {
//                 fT = fTTmp;
//                 vecPOC =   (*ciB00) + fTTmp*((*ciB01) - (*ciB00)) + fPOC *
//                          (
//                           ((*ciB10) + fTTmp*((*ciB11) - (*ciB10))) - 
//                           ((*ciB00) + fTTmp*((*ciB01) - (*ciB00)))
//                          ) /
//                          (
//                           ((*ciB10) + fTTmp*((*ciB11) - (*ciB10))) - 
//                           ((*ciB00) + fTTmp*((*ciB01) - (*ciB00)))
//                          ).norm();
//             }
        }
        
        ++ciB00;
        ++ciB10;
        ++ciB01;
        ++ciB11;
    }
    if (_pB0->getLineType() == GRAPHICS_LINETYPE_LOOP)
    {
        ciB11 = _pB1->getVertices().begin();
        ciB10 = _pB0->getVertices().begin();
        
        double fLength = ((*ciB10) - (*ciB00)).norm();
                
        //-------------------------------------------
        // Test every point for collsion with circle
        //-------------------------------------------
        {
            Vector2d vecA = (*ciB00) - vecA0;
            Vector2d vecB = (*ciB01) - (*ciB00) - vecA1 + vecA0;
            
            double fA = vecB.dot(vecB);
            double fB = 2 * vecA.dot(vecB);
            double fC = vecA.dot(vecA) - fRA*fRA;
            
            if (fA != 0.0)
            {
                double fR = fB*fB - 4*fA*fC;
                if (fR >= 0.0)
                {
                    double fT1 = (-fB + sqrt(fR)) / (2*fA);
                    double fT2 = (-fB - sqrt(fR)) / (2*fA);
                
                    if ((fT1 >= 0.0) && ( fT1 < fT))
                    {
                        fT = fT1;
                        vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
                    }
                    if ((fT2 >= 0.0) && ( fT2 < fT))
                    {
                        fT = fT2;
                        vecPOC = (*ciB01) + fT * ((*ciB01) - (*ciB00));
                    }
                }
            }
        }
        //------------------------------------------
        // Test every line for collsion with circle
        //------------------------------------------
        {
            Vector2d vecA = (*ciB10) - (*ciB00);
            Vector2d vecB = (*ciB11) - (*ciB10) - (*ciB01) + (*ciB00);
            Vector2d vecC = vecA0 - (*ciB00);
            Vector2d vecD = vecA1 - vecA0 - (*ciB01) + (*ciB00);
            
            double fTTmp = 2.0;
            
            if (vecB.isZero())
            {
                double fD = vecA[0] * vecD[1] - vecA[1] * vecD[0];
                double fN = vecA[0] * vecC[1] - vecA[1] * vecC[0];
                
                if (fD != 0)
                {
                    double fT1 = (fLength * fRA - fN) / fD;
                    if ((fT1 >= 0.0) && (fT1 < fTTmp))
                        fTTmp = fT1;
                    double fT2 = (fLength * fRA + fN) / fD;
                    if ((fT2 >= 0.0) && (fT2 < fTTmp))
                        fTTmp = fT2;
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
                double fR1 = fB*fB - 4*fA*fC1;
                double fR2 = fB*fB + 4*fA*fC2;
                
                if (fA != 0.0)
                {
                    if (fR1 >= 0.0)
                    {
                        double fT1 = (-fB + sqrt(fR1)) / (2*fA);
                        double fT2 = (-fB - sqrt(fR1)) / (2*fA);
                    
                        if ((fT1 >= 0.0) && ( fT1 < fTTmp))
                            fTTmp = fT1;
                        if ((fT2 >= 0.0) && ( fT2 < fTTmp))
                            fTTmp = fT2;
                    }
                    if (fR2 >= 0.0)
                    {
                        double fT1 = (-fB + sqrt(fR2)) / (2*fA);
                        double fT2 = (-fB - sqrt(fR2)) / (2*fA);
                    
                        if ((fT1 >= 0.0) && ( fT1 < fTTmp))
                            fTTmp = fT1;
                        if ((fT2 >= 0.0) && ( fT2 < fTTmp))
                            fTTmp = fT2;
                    }
                }
            }
            if (fTTmp < fT)
            {
                Vector2d vecSeg0 = (*ciB00) + fTTmp*((*ciB01) - (*ciB00));
                Vector2d vecSeg1 = (*ciB10) + fTTmp*((*ciB11) - (*ciB10));
                double fPOC = (vecSeg1 - vecSeg0).dot(
                            (vecA0 + fTTmp*(vecA1-vecA0)) - vecSeg0
                            ) / fLength;
                if ((fPOC >= 0.0) && (fPOC <= fLength))
                {
                    fT = fTTmp;
                    Vector2d vecSeg = vecSeg1 - vecSeg0;
                    vecPOC = vecSeg0 + fPOC * vecSeg / vecSeg.norm();                        
                }
            }
        }
    }
    
    if (fT<=1.0)
    {
        // Colliding objects
//         m_Graphics.setColor(0.0, 0.8, 0.0);
//         m_Graphics.rect(_p1->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p1->getGeometry().getBoundingBox().getUpperRight());
//         m_Graphics.rect(_p2->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p2->getGeometry().getBoundingBox().getUpperRight());
// 
//         // Point of collision
//         m_Graphics.setColor(0.8, 0.0, 0.0);
//         m_Graphics.dot(vecPOC);
//         m_Graphics.setColor(1.0, 1.0, 1.0);

        _p1->disableDynamics();
        _p1->transform();;
        _p2->disableDynamics();
        _p2->transform();;
    } 
        
    METHOD_EXIT("CCollisionManager::test")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Tests two shapes (polylines) for collision
///
/// \param _pA1 PolyLine 1, time t1
/// \param _pA0 PolyLine 1, time t0
/// \param _pB1 PolyLine 2, time t1
/// \param _pB0 PolyLine 2, time t0
/// \param _p1 Object 1
/// \param _p2 Object 2
///
/// \return collision?
///
///////////////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CPolyLine* _pA1, CPolyLine* _pA0,
                             CPolyLine* _pB1, CPolyLine* _pB0,
                             CBody* _p1, CBody* _p2)
{
    METHOD_ENTRY("CCollisionManager::test")
    
    std::list<Vector2d>::const_iterator ciA01 = _pA1->getVertices().begin();
    std::list<Vector2d>::const_iterator ciA00 = _pA0->getVertices().begin();
    
    std::list<Vector2d>::const_iterator ciA11 = ciA01;
    std::list<Vector2d>::const_iterator ciA10 = ciA00;
    ++ciA11;
    ++ciA10;
    
    Vector2d vecPOC;
    
    double fT = 2.0;
    
    while (ciA11 != _pA1->getVertices().end())
    {
        std::list<Vector2d>::const_iterator ciB1 = _pB1->getVertices().begin();
        std::list<Vector2d>::const_iterator ciB0 = _pB0->getVertices().begin();
        
        while (ciB1 != _pB1->getVertices().end())
        {
            double fAx = ((*ciA00) - (*ciB0))[0];
            double fAy = ((*ciA00) - (*ciB0))[1];
            double fBx = ((*ciA10) - (*ciA00))[0];
            double fBy = ((*ciA10) - (*ciA00))[1];
            double fCx = ((*ciB1) - (*ciB0) - (*ciA01) + (*ciA00))[0];
            double fCy = ((*ciB1) - (*ciB0) - (*ciA01) + (*ciA00))[1];
            double fDx = ((*ciA10) - (*ciA00) - (*ciA11) + (*ciA01))[0];
            double fDy = ((*ciA10) - (*ciA00) - (*ciA11) + (*ciA01))[1];

            if ((fDx == 0.0) && (fDy == 0.0))
            {
                double fTmpA = fBx*fCy-fBy*fCx;
                double fTmpB = fAx*fCy-fAy*fCx;
                
                double fAlpha = - fTmpB / fTmpA;
                
                double fTmpT = -1.0;
                if ((fAlpha >= 0.0) && (fAlpha <= 1.0))
                {
                    /// \todo Optimise here and add similar code segments!
                    /// \todo Implement cases for fCx==0!
                    if (fCx != 0.0)
                        fTmpT = (fAx+fAlpha*fBx) / (fCx);
                    if ((fTmpT >= 0.0) && (fTmpT < fT))
                    {
                        fT = fTmpT;
                        vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
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
                
                if (fTmpR != 0)
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
                if ((fTmpT1 >= 0.0) && (fTmpT1 < fT))
                {
                    fT = fTmpT1;
                    vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
                }
                if ((fTmpT2 >= 0.0) && (fTmpT2 < fT))
                {
                    fT = fTmpT2;
                    vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
                }
            }
            
            ++ciB1; ++ciB0;
        }
        /// \todo LINETYPE_LOOP must be tested here to for _pB1
        ++ciA01; ++ciA00; ++ciA11; ++ciA10;
    }
    if (_pA1->getLineType() == GRAPHICS_LINETYPE_LOOP)
    {
        ciA11 = _pA1->getVertices().begin();
        ciA10 = _pA1->getVertices().begin();
        std::list<Vector2d>::const_iterator ciB1 = _pB1->getVertices().begin();
        std::list<Vector2d>::const_iterator ciB0 = _pB0->getVertices().begin();
        
        while (ciB1 != _pB1->getVertices().end())
        {
            double fAx = ((*ciA00) - (*ciB0))[0];
            double fAy = ((*ciA00) - (*ciB0))[1];
            double fBx = ((*ciA10) - (*ciA00))[0];
            double fBy = ((*ciA10) - (*ciA00))[1];
            double fCx = ((*ciB1) - (*ciB0) - (*ciA01) + (*ciA00))[0];
            double fCy = ((*ciB1) - (*ciB0) - (*ciA01) + (*ciA00))[1];
            double fDx = ((*ciA10) - (*ciA00) - (*ciA11) + (*ciA01))[0];
            double fDy = ((*ciA10) - (*ciA00) - (*ciA11) + (*ciA01))[1];

            if ((fDx == 0.0) && (fDy == 0.0))
            {
                double fTmpA = fBx*fCy-fBy*fCx;
                double fTmpB = fAx*fCy-fAy*fCx;
                
                double fAlpha = - fTmpB / fTmpA;
                
                double fTmpT = -1.0;
                if ((fAlpha >= 0.0) && (fAlpha <= 1.0))
                {
                    if (fCx != 0.0)
                        fTmpT = (fAx+fAlpha*fBx) / (fCx);
                    if ((fTmpT >= 0.0) && (fTmpT < fT))
                    {
                        fT = fTmpT;
                        vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
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
                
                if (fTmpR != 0)
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
                if ((fTmpT1 >= 0.0) && (fTmpT1 < fT))
                {
                    fT = fTmpT1;
                    vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
                }
                if ((fTmpT2 >= 0.0) && (fTmpT2 < fT))
                {
                    fT = fTmpT2;
                    vecPOC = (*ciB0) + fT * ((*ciB1)-(*ciB0));
                }
            }
            
            ++ciB1, ++ciB0;
        }
        /// \todo LINETYPE_LOOP must be tested here to for _pB1
    }
    
    if (fT<=1.0)
    {
//         // Colliding objects
//         m_Graphics.setColor(0.0, 0.8, 0.0);
//         m_Graphics.rect(_p1->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p1->getGeometry().getBoundingBox().getUpperRight());
//         m_Graphics.rect(_p2->getGeometry().getBoundingBox().getLowerLeft(),
//                         _p2->getGeometry().getBoundingBox().getUpperRight());
// 
//         // Point of collision
//         m_Graphics.setColor(0.8, 0.0, 0.0);
//         m_Graphics.dot(vecPOC);
//         m_Graphics.setColor(1.0, 1.0, 1.0);
        
        _p1->disableDynamics();
        _p1->transform();;
        _p2->disableDynamics();
        _p2->transform();;
    } 
            
    METHOD_EXIT("CCollisionManager::test")
}

