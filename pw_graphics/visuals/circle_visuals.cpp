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
/// \file       circle_visuals.cpp
/// \brief      Implementation of class "CCircleVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "circle_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pCircle Circle to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CCircleVisuals::CCircleVisuals(CCircle* _pCircle): m_pCircle(_pCircle)
{
    METHOD_ENTRY("CCircleVisuals::CCircleVisuals")
    CTOR_CALL("CCircleVisuals::CCircleVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CCircleVisuals::~CCircleVisuals()
{
    METHOD_ENTRY("CCircleVisuals::~CCircleVisuals()");
    DTOR_CALL("CCircleVisuals::~CCircleVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Circle
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CCircleVisuals::draw(const CCamera* const _pCamera,
                          const IObject* const _pObject) const
{
    METHOD_ENTRY("CCircleVisuals::draw()");
    
    double   fRad      = m_pCircle->getRadius();
    double   fPAng     = m_pCircle->getAngle();
    Vector2d vecCenter = m_pCircle->getCenter() - _pCamera->getCenter() +
                         IUniverseScaled::cellToDouble(_pObject->getCell() - _pCamera->getCell());
    
    if ((vecCenter.norm() <= fRad+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  fRad-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        LineType    LineT;
        
        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = 2.0*M_PI;
            LineT = GRAPHICS_LINETYPE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = GRAPHICS_LINETYPE_STRIP;
        }

        double fInc = CIRCLE_DEFAULT_RESOLUTION * m_Graphics.getResMPX() / fRad; 
        
        if (fInc > 2.0*M_PI / CIRCLE_MINIMUM_SEGMENTS) fInc = 2.0*M_PI / CIRCLE_MINIMUM_SEGMENTS;
        fAngEnd += fInc;
        
        if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
        
        m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);

            while ( fAng < fAngEnd)
            {
                m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*fRad,
                                              vecCenter[1]+std::sin(fAng)*fRad));
                fAng += fInc;
            }
        m_Graphics.endLine();
    }
}
