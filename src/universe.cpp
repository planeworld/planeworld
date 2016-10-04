////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2012-016 Torsten Büschenfeld
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
/// \file       universe.cpp
/// \brief      Implementation of class "CUniverse"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-11
///
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <random>

#include "engine_common.h"
#include "namegenerator.h"
#include "universe.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CUniverse::CUniverse() : m_pStar(0),
                         m_pStarShape(0),
                         m_nNrOfPlanetsMax(0)
{
    METHOD_ENTRY("CUniverse::CUniverse");
    CTOR_CALL("CUniverse::CUniverse");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, freeing memory
///
///////////////////////////////////////////////////////////////////////////////
CUniverse::~CUniverse()
{
    METHOD_ENTRY("CUniverse::~CUniverse");
    DTOR_CALL("CUniverse::~CUniverse");
    
    for (auto i=0u; i<m_StarSystems.size(); ++i)
    {
        delete m_StarSystems[i];
        m_StarSystems[i] = 0;
        MEM_FREED("CStarSystem");
    }
    m_StarSystems.clear();
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Procudurally generates a universe based on a given seed.
///
/// \param _nSeed Initial seed for procedural universe generation
/// \param _nNumberOfStars Number of stars for this universe
///
///////////////////////////////////////////////////////////////////////////////
void CUniverse::generate(const int& _nSeed, const int& _nNumberOfStars)
{
    METHOD_ENTRY("CUniverse::generate")
    
    CNameGenerator StarNameGenerator(_nSeed);
    const int nNrOfStarTypes=7;
    
    m_StarSystems.reserve(_nNumberOfStars);
    
    std::mt19937 Generator;
    
    Generator.seed(_nSeed);
    
    double fDistanceAvg = 33.e15; // Average distance of stars (~3.5 ly)
    double fLimit = std::sqrt(double(_nNumberOfStars)) * fDistanceAvg;
    
    CKinematicsState::setWorldLimit(fLimit, fLimit);
    
    std::exponential_distribution<double>   ExponentialDistribution(3.5);
    std::uniform_real_distribution<double>  UniformDistribution(-fLimit,fLimit);
    std::poisson_distribution<int>          PoissionDistribution(4);
    std::vector<int> vecNrOfPlanets;
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    int nNrOfPlanets = 0;
    
    double fMin = 10000.0;
    double fMax = 10000.0;
    
    // Create a star field
    for (int i=0; i<_nNumberOfStars; ++i)
    {
        double fNumber = ExponentialDistribution(Generator);
        
        // Calculate temperature of star
        double fTemp = (fNumber * 50000.0) + 2000.0 -
                        ExponentialDistribution(Generator)*500.0;
        if (fTemp < 100.0) fTemp = 100.0;
    
        // Calculate stellar class of star
        std::uint8_t nStellarClass = static_cast<int>(nNrOfStarTypes*fNumber);
        if (nStellarClass >= nNrOfStarTypes) nStellarClass = nNrOfStarTypes - 1;
        DOM_STATS(DEBUG(++vecNrOfStars[nStellarClass];))
        
        
            
            
            CStarSystem* pStarSystem = new CStarSystem();
            MEM_ALLOC("CStarSystem");

            Vector2i vecCell;
            Vector2d vecOrigin;
            Vector2d vecPosition(UniformDistribution(Generator), UniformDistribution(Generator));
            
            IUniverseScaled::separateCenterCell(vecPosition,vecOrigin,vecCell);
            
            pStarSystem->Star().setName(StarNameGenerator.getName());
            pStarSystem->Star().setStarType(int(nNrOfStarTypes*fNumber));
            pStarSystem->Star().setOrigin(vecOrigin);
            pStarSystem->Star().setRadius((0.5+7.0*fNumber)*SOLAR_RADIUS);
            pStarSystem->setSeed(i);
            pStarSystem->setCell(vecCell);
            pStarSystem->setNumberOfPlanets(PoissionDistribution(Generator));
            nNrOfPlanets += pStarSystem->getNumberOfPlanets();
            
            // Store the maximum number of planets
            if (pStarSystem->getNumberOfPlanets() > m_nNrOfPlanetsMax)
            {
                m_nNrOfPlanetsMax = pStarSystem->getNumberOfPlanets();
                vecNrOfPlanets.resize(m_nNrOfPlanetsMax+1,0);
            }
            
            m_StarSystems.push_back(pStarSystem);
            
            DOM_STATS(DEBUG(++vecNrOfPlanets[pStarSystem->getNumberOfPlanets()];))
    }
    
    DOM_STATS(
    INFO_MSG("Universe generator", "Generated " << m_StarSystems.size() << " Stars.")
    DEBUG(
        DEBUG_MSG("Universe generator", "Distribution of spectral classes: ")
        Log.logSeparator();
        
        int nMax = 0;
        for (int i=0; i<nNrOfStarTypes; ++i)
        {
            if (vecNrOfStars[i] > nMax) nMax = vecNrOfStars[i];
        }
        for (int i=0; i<nNrOfStarTypes; ++i)
        {
            std::cout << "Class " << this->starClassToString(i) << ": ";
            std::cout << std::string(double(vecNrOfStars[i])/double(nMax)*60.0,'#') << std::endl;
        }
        Log.logSeparator();
    )

    INFO_MSG("Universe generator", "Generated "<< nNrOfPlanets << " planets.")
    DEBUG(
        DEBUG_MSG("Universe generator", "Distribution of number of planets per star system: ")
        Log.logSeparator();
        
        int nMax = 0;
        for (int i=0; i<m_nNrOfPlanetsMax; ++i)
            if (vecNrOfPlanets[i] > nMax) nMax = vecNrOfPlanets[i];
        for (int i=0; i<m_nNrOfPlanetsMax; ++i)
            std::cout << "Planets: " << i << ": " << std::string(double(vecNrOfPlanets[i])/double(nMax) * 60.0,'#') << std::endl;
        
        Log.logSeparator();
    ))
    
    // Reserve memory for star object
//     m_pStar = new CRigidBody;
//     m_pStar->setName("Procedurally_Generated_Star_Dummy");
//     m_pStar->enableDynamics();
//     m_pStar->enableGravitation();
//     m_pStarShape = new CCircle;
//     CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
//     pShape->buffer(m_pStarShape);
//     m_pStar->getGeometry()->addShape(pShape);
//     m_pStarVisuals = new CCircleVisuals(pShape);
//     m_pStarObjectVisuals = new IObjectVisuals(m_pStar);
// 
//     MEM_ALLOC("CRigidBody");
//     MEM_ALLOC("CCircle");
//     MEM_ALLOC("CDoubleBufferedShape")
//     MEM_ALLOC("CCircleVisuals");
//     MEM_ALLOC("IObjectVisuals");
// 
//     /// \bug Local variables are not freed
//     
//     m_pStarObjectVisuals->addVisuals(m_pStarVisuals);
//     
// //     m_pDataStorage->addObject(m_pStar);
// //     m_pDataStorage->addObjectVisuals(m_pStarObjectVisuals);
// 
//     m_Objects.push_back(m_pStar);
//     m_Visuals.push_back(m_pStarObjectVisuals);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Copies the full content of a given universe
///
/// \param _Universe The universe to be copied
///
///////////////////////////////////////////////////////////////////////////////
void CUniverse::clone(const CUniverse& _Universe)
{
    m_nNrOfPlanetsMax = _Universe.m_nNrOfPlanetsMax;
    
    if (_Universe.getStarSystems().size() > 0)
    {
        for (std::vector<CStarSystem*>::const_iterator ci=_Universe.getStarSystems().begin();
                                                    ci!=_Universe.getStarSystems().end(); ++ci)
        {
            CStarSystem* pStarSystem = new CStarSystem();
            MEM_ALLOC("CStarSystem");
            
            *pStarSystem = *(*ci);
            m_StarSystems.push_back((pStarSystem));
        }

        // Hand over object information. Important: The original objects are kept,
        // the pointers are copied. This class passes responsibility of dynamically
        // allocated objects!
        m_pStar = _Universe.m_pStar;
        m_pStarShape = _Universe.m_pStarShape;
        m_Objects.push_back(_Universe.m_pStar);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns a string that identifies the star class.
///
/// \param _nClass Number identifying star class
///
///////////////////////////////////////////////////////////////////////////////
const std::string CUniverse::starClassToString(const int& _nClass) const
{
    METHOD_ENTRY("CUniverse::starClassToString")
    
    switch (_nClass)
    {
        case STAR_CLASS_M: return "M"; break;
        case STAR_CLASS_K: return "K"; break;
        case STAR_CLASS_G: return "G"; break;
        case STAR_CLASS_F: return "F"; break;
        case STAR_CLASS_A: return "A"; break;
        case STAR_CLASS_B: return "B"; break;
        case STAR_CLASS_O: return "O"; break;
        default: return "Unknown";
    }
}
