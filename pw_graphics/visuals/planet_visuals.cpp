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
        
//     m_Surface.SetSeed(5);
//     m_Surface.SetFrequency(0.000006);
//     m_Surface.SetLacunarity(13.9346918734);
//     m_Surface.SetOctaveCount(3);
//     m_Surface.SetNoiseQuality(QUALITY_BEST);
    
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
    int      nNrOSP    = m_pPlanet->getNrOfSamplingPoints();
    int      nSeed     = m_pPlanet->getSeed();
    double   fSmooth   = m_pPlanet->getSmoothness();
    Vector2d vecCenter = m_pPlanet->getCenter()-_pCamera->getPosition();
//     module::Select Surface = m_pPlanet->getSurface();
    
    module::RidgedMulti mountainTerrain;
    mountainTerrain.SetFrequency(0.000025);
    mountainTerrain.SetLacunarity(1.9737346);
    mountainTerrain.SetNoiseQuality(QUALITY_BEST);
    mountainTerrain.SetOctaveCount(20);

    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency (0.00001);
    baseFlatTerrain.SetLacunarity(1.793947);
    baseFlatTerrain.SetNoiseQuality(QUALITY_BEST);

    module::ScaleBias flatTerrain;
    flatTerrain.SetSourceModule (0, baseFlatTerrain);
    flatTerrain.SetScale (0.25);
    flatTerrain.SetBias (-0.75);
    
    module::Perlin terrainType;
    terrainType.SetFrequency (0.000001);
    terrainType.SetPersistence (0.25);
    terrainType.SetLacunarity(2.12358986);
    terrainType.SetNoiseQuality(QUALITY_BEST);

    module::Select Surface;
    Surface.SetSourceModule (0, flatTerrain);
    Surface.SetSourceModule (1, mountainTerrain);
    Surface.SetControlModule (terrainType);
    Surface.SetBounds (0.0, 100000.0);
    Surface.SetEdgeFalloff (0.125);
        
    double fAlpha = fabs(std::asin(_pCamera->getBoundingCircleRadius() / 
                                   vecCenter.norm()));
                                   
    Vector2d vecEx(1.0, 0.0);
    double fAng;    
    double fAngEnd;
    
    if (std::isnan(fAlpha))
    {
        fAng = m_pPlanet->getAngle();
        fAngEnd = 2.0*M_PI + m_pPlanet->getAngle();
    }
    else
    {
        double fAng0 = std::acos((- vecCenter.dot(vecEx)) / vecCenter.norm());
        fAng = fAng0-fAlpha + m_pPlanet->getAngle()-M_PI*0.5;
        fAngEnd = fAng0+fAlpha + m_pPlanet->getAngle()-M_PI*0.5;
    }
    
    m_Graphics.beginLine(GRAPHICS_LINETYPE_STRIP, SHAPE_DEFAULT_DEPTH);

        while ( fAng <= fAngEnd)
        {
            m_Graphics.addVertex(Vector2d(vecCenter[0]-std::sin(fAng)*(fRad+Surface.GetValue(std::sin(fAng)*fRad, std::cos(fAng)*fRad,0.0)*fHeight),
                                          vecCenter[1]+std::cos(fAng)*(fRad+Surface.GetValue(std::sin(fAng)*fRad, std::cos(fAng)*fRad,0.0)*fHeight)));
//                 m_Graphics.dot(Vector2d(vecCenter[0]-std::sin(fAng)*(fRad+Surface.GetValue(std::sin(fAng)*fRad, std::cos(fAng)*fRad,0.0)*fHeight),
//                                                 vecCenter[1]+std::cos(fAng)*(fRad+Surface.GetValue(std::sin(fAng)*fRad, std::cos(fAng)*fRad,0.0)*fHeight)));
            fAng += 2.0*M_PI * 2.0e-8 / _pCamera->getZoom();
        }

    m_Graphics.endLine();

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
//     Vector2d vecCenter = m_pPlanet->getCenter()-_pCamera->getPosition();
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
