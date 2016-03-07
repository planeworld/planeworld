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
/// \file       circle_visuals.cpp
/// \brief      Implementation of class "CCircleVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-08
///
////////////////////////////////////////////////////////////////////////////////

#include "circle_visuals.h"

#include "circle.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pCircle Circle to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CCircleVisuals::CCircleVisuals(CDoubleBufferedShape* const _pCircle)
{
    METHOD_ENTRY("CCircleVisuals::CCircleVisuals")
    CTOR_CALL("CCircleVisuals::CCircleVisuals")
    
    this->attachTo(_pCircle);
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
/// \brief Clone a circular shape visual
///
/// \param _pShape Shape the visuals are attached to
///
/// \return Clone of circular shape visuals
///
///////////////////////////////////////////////////////////////////////////////
IVisuals* CCircleVisuals::clone(CDoubleBufferedShape* const _pShape) const
{
    METHOD_ENTRY("CCircleVisuals::clone");
    
    CCircleVisuals* pClone = new CCircleVisuals(_pShape);
    MEM_ALLOC("CCircleVisuals")
    
    return pClone;
}
    
///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Circle
///
/// \param _pCamera Active camera for drawing visuals
/// \param _pObject Corresponding object
///
///////////////////////////////////////////////////////////////////////////////
void CCircleVisuals::draw(CCamera* const _pCamera,
                          const IObject* const _pObject) const
{
    METHOD_ENTRY("CCircleVisuals::draw()");
    
    double   fRad      = static_cast<CCircle*>(m_pDBShape->getShapeCur())->getRadius();
    Vector2d vecCenter = static_cast<CCircle*>(m_pDBShape->getShapeCur())->getCenter() - _pCamera->getCenter()/* +
                         IUniverseScaled::cellToDouble(_pObject->getCell() - _pCamera->getCell())*/;

                         if ((vecCenter.norm() <= fRad+_pCamera->getBoundingCircleRadius()) &&
        (vecCenter.norm() >  fRad-_pCamera->getBoundingCircleRadius())
       )
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        LineType    LineT;
        
        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecCenter.norm()));
        if (std::isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = 2.0*M_PI;
            LineT = LineType::GRAPHICS_LINETYPE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
            
            if (vecCenter[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = LineType::GRAPHICS_LINETYPE_STRIP;
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

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Input stream for game state information
///
/// \param _is  Source stream
///
/// \return Remaining stream with game state information
///
////////////////////////////////////////////////////////////////////////////////
std::istream& CCircleVisuals::myStreamIn(std::istream& _is)
{
    METHOD_ENTRY("CCircleVisuals::myStreamIn")
    return _is;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Output stream for game state information
///
/// \param _os  Source stream
///
/// \return Stream with game state information of CCircleVisuals instance
///
////////////////////////////////////////////////////////////////////////////////
std::ostream& CCircleVisuals::myStreamOut(std::ostream& _os)
{
    METHOD_ENTRY("CCircleVisuals::myStreamOut")
    return _os;
}
