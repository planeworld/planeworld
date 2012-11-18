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

#include "universe.h"

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
///
///////////////////////////////////////////////////////////////////////////////
void CUniverse::generate(const int& _nSeed)
{
    METHOD_ENTRY("CUniverse::generate")
    
    const int nBar=100;
    const int nNrOfStars=20000;
    const int nNrOfStarTypes=7;
    
    m_StarSystems.reserve(nNrOfStars);
    
    std::mt19937 Generator;
    
    Generator.seed(_nSeed);
    
    std::exponential_distribution<double>   ExponentialDistribution(3.5);
    std::normal_distribution<double>        NormalDistribution(0.0, 1.0e5);
    std::uniform_real_distribution<double>  UniformDistribution(0.0,2.0*M_PI);
    std::poisson_distribution<int>          PoissionDistribution(3.0);
    std::vector<int> vecNrOfPlanets(20,0);
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    // Create a globular cluster
    for (int i=0; i<nNrOfStars; ++i)
    {
        double fNumber = ExponentialDistribution(Generator);
        if (fNumber<1.0)
        {
            ++vecNrOfStars[int(nNrOfStarTypes*fNumber)];
            
            CStarSystem* pStarSystem = new CStarSystem();
            MEM_ALLOC("pStarSystem");

            double fDistance = NormalDistribution(Generator);
            double fAngle    = UniformDistribution(Generator);
            Vector2d vecLocation;
            
            vecLocation[0] = fDistance*std::sin(fAngle);
            vecLocation[1] = fDistance*std::cos(fAngle);
            
            pStarSystem->setStarType(int(nNrOfStarTypes*fNumber));
            pStarSystem->setLocation(vecLocation);
            pStarSystem->setNumberOfPlanets(PoissionDistribution(Generator));
            
            m_StarSystems.push_back(pStarSystem);
            
            ++vecNrOfPlanets[pStarSystem->getNumberOfPlanets()];
        }
    }

    INFO_MSG("Universe generator", "Generated " << nNrOfStars << " Stars. Distribution of spectral classes: ")
    Log.logSeparator();
    
    for (int i=0; i<nNrOfStarTypes; ++i) {
        std::cout << "Class " << this->starClassToString(i) << ": ";
        std::cout << std::string(vecNrOfStars[i]*nBar/nNrOfStars,'#') << std::endl;
    }
    Log.logSeparator();

    int nNrOfPlanets = 0;
    for (int i=0; i<20; ++i)
        nNrOfPlanets += vecNrOfPlanets[i]*i;
    
    INFO_MSG("Universe generator", "Generated "<< nNrOfPlanets << " planets. Distribution of number of planet per star system: ")
    Log.logSeparator();
    
    for (int i=0; i<10; ++i)
        std::cout << "Planets: " << i << ": " << std::string(vecNrOfPlanets[i]*nBar/nNrOfStars,'#') << std::endl;
    
    Log.logSeparator();
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
