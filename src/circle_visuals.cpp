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
    
    METHOD_EXIT("CCircleVisuals::CCircleVisuals")
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

    METHOD_EXIT("CCircleVisuals::~CCircleVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Circle
///
///////////////////////////////////////////////////////////////////////////////
void CCircleVisuals::draw() const
{
    METHOD_ENTRY("CCircleVisuals::draw()");

    double fAng = m_pCircle->getAngle();
    double fRad = m_pCircle->getRadius();
    Vector2d vecCenter = m_pCircle->getCenter();
//  double fAngInc =    2.0 * GRAPHICS_PI /
//                      ceil((100.0 * 2.0 * GRAPHICS_PI * m_fRadius * m_Graphics.getCamZoom()) /
//                      (GRAPHICS_RIGHT_DEFAULT-GRAPHICS_LEFT_DEFAULT));
    double fAngInc = 2.0 * GRAPHICS_PI /
                     ceil (
                         ( 2.0 * GRAPHICS_PI * fRad / m_Graphics.getResMPX() )
                         / m_Graphics.getDynPelSize() );

    // Limit the maximum and minimum amount of circle elements
    if ( fAngInc < 0.1*GRAPHICS_DEG2RAD ) fAngInc = 0.1*GRAPHICS_DEG2RAD;
    if ( fAngInc > 18.0*GRAPHICS_DEG2RAD ) fAngInc = 18.0*GRAPHICS_DEG2RAD;

    m_Graphics.beginLine(GRAPHICS_LINETYPE_LOOP, SHAPE_DEFAULT_DEPTH);

    while ( fAng <= 2*GRAPHICS_PI+m_pCircle->getAngle())
    {
        m_Graphics.addVertex(Vector2d(vecCenter[0]-std::sin(fAng)*fRad,
                                      vecCenter[1]+std::cos(fAng)*fRad));
        fAng += fAngInc;
    }

    m_Graphics.endLine();

    METHOD_EXIT("CCircleVisuals::draw()");
}
