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
/// \file       universe.cpp
/// \brief      Implementation of class "CUniverse"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2012-11-11
///
////////////////////////////////////////////////////////////////////////////////

#include <random>

#include "engine_common.h"
#include "universe.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CUniverse::CUniverse() :m_nNrOfPlanetsMax(0)
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
    
    for (int i=0; i<m_StarSystems.size(); ++i)
    {
        delete m_StarSystems[i];
        m_StarSystems[i] = 0;
        MEM_FREED("pStarSystem");
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
    
    const int nBar=100;
    const int nNrOfStarTypes=7;
    
    m_StarSystems.reserve(_nNumberOfStars);
    
    std::mt19937 Generator;
    
    Generator.seed(_nSeed);
    
    // Density = m_nNrOfStars/(3.0*fSigma * 2.0*M_PI) = 0.4/30.857e15;
    double fSigma = _nNumberOfStars * 30.857e15 / (0.4*3.0*2.0*M_PI);
    
    std::exponential_distribution<double>   ExponentialDistribution(3.5);
    std::normal_distribution<double>        NormalDistribution(0.0, fSigma);
    std::uniform_real_distribution<double>  UniformDistribution(0.0,2.0*M_PI);
    std::poisson_distribution<int>          PoissionDistribution(4);
    std::vector<int> vecNrOfPlanets;
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    // Create a globular cluster
    for (int i=0; i<_nNumberOfStars; ++i)
    {
        double fNumber = ExponentialDistribution(Generator);
        if (fNumber<1.0)
        {
            ++vecNrOfStars[int(nNrOfStarTypes*fNumber)];
            
            CStarSystem* pStarSystem = new CStarSystem();
            MEM_ALLOC("pStarSystem");

            double fDistance = NormalDistribution(Generator);
            double fAngle    = UniformDistribution(Generator);
            Vector2i vecCell;
            Vector2d vecCenter;
            
            vecCell[0] = static_cast<int>(fDistance*std::sin(fAngle)/DEFAULT_CELL_SIZE_2);
            vecCell[1] = static_cast<int>(fDistance*std::cos(fAngle)/DEFAULT_CELL_SIZE_2);
            
            vecCenter[0] = fDistance*std::sin(fAngle)-DEFAULT_CELL_SIZE_2*vecCell[0];
            vecCenter[1] = fDistance*std::cos(fAngle)-DEFAULT_CELL_SIZE_2*vecCell[1];
            
            pStarSystem->setStarType(int(nNrOfStarTypes*fNumber));
            pStarSystem->setCenter(vecCenter);
            pStarSystem->setCell(vecCell);
            pStarSystem->setNumberOfPlanets(PoissionDistribution(Generator));
            
            // Store the maximum number of planets
            if (pStarSystem->getNumberOfPlanets() > m_nNrOfPlanetsMax)
            {
                m_nNrOfPlanetsMax = pStarSystem->getNumberOfPlanets();
                vecNrOfPlanets.resize(m_nNrOfPlanetsMax,0);
            }
            
            m_StarSystems.push_back(pStarSystem);
            
            ++vecNrOfPlanets[pStarSystem->getNumberOfPlanets()];
        }
    }

    DOM_VAR(INFO_MSG("Universe generator", "Generated " << m_StarSystems.size() << " Stars."))
    DEBUG(
        DEBUG_MSG("Universe generator", "Distribution of spectral classes: ")
        Log.logSeparator();
        
        for (int i=0; i<nNrOfStarTypes; ++i) {
            std::cout << "Class " << this->starClassToString(i) << ": ";
            std::cout << std::string(vecNrOfStars[i]*nBar/_nNumberOfStars,'#') << std::endl;
        }
        Log.logSeparator();
    )

    int nNrOfPlanets = 0;
    for (int i=0; i<=m_nNrOfPlanetsMax; ++i)
        nNrOfPlanets += vecNrOfPlanets[i]*i;
    
    DOM_VAR(INFO_MSG("Universe generator", "Generated "<< nNrOfPlanets << " planets."))
    DEBUG(
        DEBUG_MSG("Universe generator", "Distribution of number of planet per star system: ")
        Log.logSeparator();
        
        for (int i=0; i<=m_nNrOfPlanetsMax; ++i)
            std::cout << "Planets: " << i << ": " << std::string(vecNrOfPlanets[i]*nBar/_nNumberOfStars,'#') << std::endl;
        
        Log.logSeparator();
    )
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
    for (std::vector<CStarSystem*>::const_iterator ci=_Universe.getStarSystems().begin();
                                                   ci!=_Universe.getStarSystems().end(); ++ci)
    {
        CStarSystem* pStarSystem = new CStarSystem();
        MEM_ALLOC("pStarSystem");
        
        *pStarSystem = *(*ci);
        m_StarSystems.push_back((pStarSystem));
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
    }
}
