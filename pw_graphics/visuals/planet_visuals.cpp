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
/// \file       planet_visuals.cpp
/// \brief      Implementation of class "CPlanetVisuals"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2011-01-17
///
////////////////////////////////////////////////////////////////////////////////

#include "planet_visuals.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copy Constructor
///
/// \param _pPlanet Planet to attach when initialising 
///
///////////////////////////////////////////////////////////////////////////////
CPlanetVisuals::CPlanetVisuals(CPlanet* _pPlanet): m_pPlanet(_pPlanet)
{
    METHOD_ENTRY("CPlanetVisuals::CPlanetVisuals")
    CTOR_CALL("CPlanetVisuals::CPlanetVisuals")
    
    METHOD_EXIT("CPlanetVisuals::CPlanetVisuals")
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor
///
///////////////////////////////////////////////////////////////////////////////
CPlanetVisuals::~CPlanetVisuals()
{
    METHOD_ENTRY("CPlanetVisuals::~CPlanetVisuals()");
    DTOR_CALL("CPlanetVisuals::~CPlanetVisuals()");

    METHOD_EXIT("CPlanetVisuals::~CPlanetVisuals()");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Draw the Planet
///
/// \param _pCamera Active camera for drawing visuals
///
///////////////////////////////////////////////////////////////////////////////
void CPlanetVisuals::draw(const CCamera* const _pCamera) const
{
    METHOD_ENTRY("CPlanetVisuals::draw()");

    double   fRad      = m_pPlanet->getRadius();
    double   fHeight   = m_pPlanet->getHeight();
    double   fPAng     = m_pPlanet->getAngle();
    int      nSeed     = m_pPlanet->getSeed();
    double   fSmooth   = m_pPlanet->getSmoothness();
    Vector2d vecDist   = m_pPlanet->getCenter()-_pCamera->getCenter();
    Vector2d vecCenter = m_pPlanet->getCenter()-_pCamera->getCenter();
    
    if (vecDist.norm() > fRad-_pCamera->getBoundingCircleRadius())
    {
        Vector2d    vecEx(1.0, 0.0);
        double      fAng;    
        double      fAngEnd;
        LineType    LineT;

        double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / vecDist.norm()));
        if (isnan(fAlpha))
        {
            fAng = 0.0;
            fAngEnd = 2.0*M_PI;
            LineT = GRAPHICS_LINETYPE_LOOP;
        }
        else
        {
            double fAng0 = std::acos((- vecDist.dot(vecEx)) / vecDist.norm());
            
            if (vecDist[1] > 0.0) fAng0 = 2.0*M_PI - fAng0;
            
            fAng = fAng0-fAlpha;
            fAngEnd = fAng0+fAlpha;
            LineT = GRAPHICS_LINETYPE_STRIP;
        }

        double fInc = m_pPlanet->getGroundResolution() / fRad;
        
        // Subsample planet surface when zooming out.
        if (_pCamera->getZoom()*m_pPlanet->getGroundResolution() <= 1.0)
            fInc /= _pCamera->getZoom()*m_pPlanet->getGroundResolution();
        
        // Snap angle to ground resolution grid to avoid flickering.
        // If angle is started at arbitrary position, aliasing causes flickering when zooming
        // or moving, since height is always sampled at different positions.
        fAng -= ((fAng/fInc)-floor(fAng/fInc))*fInc;
        fAngEnd += ((fAng/fInc)-floor(fAng/fInc))*fInc;
        
        if (fAngEnd < fAng) std::swap<double>(fAng, fAngEnd);
               
        m_Graphics.beginLine(LineT, SHAPE_DEFAULT_DEPTH);

            while ( fAng <= fAngEnd)
            {
                double fHght = m_pPlanet->getSurface().GetValue(std::cos(fAng-fPAng)*fRad,
                                                                std::sin(fAng-fPAng)*fRad,0.0);
                
                m_Graphics.addVertex(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
                                              vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));
//                 m_Graphics.dot(Vector2d(vecCenter[0]+std::cos(fAng)*(fRad+fHght*fHeight),
//                                               vecCenter[1]+std::sin(fAng)*(fRad+fHght*fHeight)));

//                 if (_pCamera->getZoom() <= 1.0)
//                     fAng += fInc / _pCamera->getZoom();
//                 else
                    fAng += fInc;
            }

        m_Graphics.endLine();
    }

    METHOD_EXIT("CPlanetVisuals::draw()");
}

// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Draw the Planet
// ///
// /// \param _pCamera Active camera for drawing visuals
// ///
// ///////////////////////////////////////////////////////////////////////////////
// void CPlanetVisuals::draw(const CCamera* const _pCamera) const
// {
//     METHOD_ENTRY("CPlanetVisuals::draw()");
// 
//     double   fAng      = m_pPlanet->getAngle();
//     double   fRad      = m_pPlanet->getRadius();
//     double   fHeight   = m_pPlanet->getHeight();
//     int      nNrOSP    = m_pPlanet->getNrOfSamplingPoints();
//     int      nSeed     = m_pPlanet->getSeed();
//     double   fSmooth   = m_pPlanet->getSmoothness();
//     Vector2d vecCenter = m_pPlanet->getCenter()-_pCamera->getOrigin();
// 
// //     double fAngInc = 2.0 * GRAPHICS_PI /
// //                      ceil (
// //                          ( 2.0 * GRAPHICS_PI * fRad / m_Graphics.getResMPX() )
// //                          / m_Graphics.getDynPelSize() );
// //     srand(nSeed);
//     
//     // Limit the maximum and minimum amount of planet elements
// //     if ( fAngInc < 0.1*GRAPHICS_DEG2RAD ) fAngInc = 0.1*GRAPHICS_DEG2RAD;
// //     if ( fAngInc > 18.0*GRAPHICS_DEG2RAD ) fAngInc = 18.0*GRAPHICS_DEG2RAD;
// 
//     m_Graphics.beginLine(GRAPHICS_LINETYPE_LOOP, SHAPE_DEFAULT_DEPTH);
// 
//     int nAng=0;
//     while ( fAng <= 2*GRAPHICS_PI+m_pPlanet->getAngle())
//     {
//         int m_nRandom = rand()%int(fHeight);
//         double fH = 0.0;
//         double fFac     = 1.0;
//         double fFreq    = 0.01;
//         double fFreqFac = 1.93926598;
//         double fPers    = 0.6;
//         
//         for (int i=0; i<20; ++i)
//         {
//             int    nX1 = floor(fFreq * nAng);
//             int    nX2 = nX1+1;
//             double fX  = fFreq*nAng - nX1;
//             double fTmp1 = hashDouble(nX1)/2.0+0.5;
//             double fTmp2 = hashDouble(nX2)/2.0+0.5;
//             
//             fH += this->interpolate(fTmp1, fTmp2, fX) * fFac;
//             
//             fFreq   *= fFreqFac;
//             fFac    *= fPers;
//         }
//  
//         m_Graphics.addVertex(Vector2d(vecCenter[0]-std::sin(fAng)*(fRad+fH*fHeight),
//                                       vecCenter[1]+std::cos(fAng)*(fRad+fH*fHeight)));
// //         m_Graphics.addVertex(Vector2d(vecCenter[0]+fAng*30-100.0,
// //                                       vecCenter[1]+fRad+fH*10.0));
// //         m_Graphics.dot(Vector2d(vecCenter[0]-std::sin(fAng)*(fRad+fH*4000.0),
// //                                       vecCenter[1]+std::cos(fAng)*(fRad+fH*4000.0)));
//         fAng += 0.0003141;
//         ++nAng;
//     }
// 
//     m_Graphics.endLine();
// 
// //     std::cout << nAng << std::endl;
//     
//     METHOD_EXIT("CPlanetVisuals::draw()");
// }

///////////////////////////////////////////////////////////////////////////////
///
/// \brief 32-bit hash function
///
/// \param _nV Value
///
/// \return Hash value
///
///////////////////////////////////////////////////////////////////////////////
const uint32_t CPlanetVisuals::hashUInt32(const uint32_t _nV) const
{
    METHOD_ENTRY("CPlanetVisuals::hashUInt32")

    uint32_t nV = _nV;

    nV = (nV ^ 61) ^ (nV >> 16);
    nV = nV + (nV << 3);
    nV = nV ^ (nV >> 4);
    nV = nV * 0x27d4eb2d;
    nV = nV ^ (nV >> 15);
    
//     if (nV < 100000)
//     std::cout << nV << std::endl;
    
    METHOD_EXIT("CPlanetVisuals::hashUInt32")
    return nV;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Hash function
///
/// \param _nV Value
///
/// \return Hash value between -1 and 1
///
///////////////////////////////////////////////////////////////////////////////
const double CPlanetVisuals::hashDouble(const int _nV) const
{
    METHOD_ENTRY("CPlanetVisuals::hashDouble")

    double fResult;
    int    nTmp;
   
    nTmp = (_nV << 13) ^_nV;
    fResult = 1 - double((nTmp * (nTmp * nTmp * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824;
    
    METHOD_EXIT("CPlanetVisuals::hashDouble")
    return fResult;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Interpolation
///
/// \param _fV1 Value 1
/// \param _fV2 Value 2
/// \param _fX  X fraction
///
/// \return Interpolated value
///
///////////////////////////////////////////////////////////////////////////////
const double CPlanetVisuals::interpolate(const double& _fV1,
                                         const double& _fV2,
                                         const double& _fX) const
{
    METHOD_ENTRY("CPlanetVisuals::interpolate")

    double fResult = (1.0 - std::cos(_fX * 3.1415927)) * 0.5;

    fResult = _fV1*(1.0-fResult) + _fV2*fResult;
    
    METHOD_EXIT("CPlanetVisuals::interpolate")
    return fResult;
}
