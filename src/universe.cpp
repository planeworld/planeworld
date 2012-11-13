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
/// \brief Procudurally generates a universe based on a given seed.
///
/// \param _nSeed Initial seed for procedural universe generation
///
///////////////////////////////////////////////////////////////////////////////
void CUniverse::generate(const int& _nSeed)
{
    METHOD_ENTRY("CUniverse::generate")
    
    const int nBar=100;
    const int nNrOfStars=10000000;
    const int nNrOfStarTypes=7;
    
    std::default_random_engine Generator;
    
    std::exponential_distribution<double> ExponentialDistribution(3.5);
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    for (int i=0; i<nNrOfStars; ++i) {
        double fNumber = ExponentialDistribution(Generator);
        if (fNumber<1.0) ++vecNrOfStars[int(nNrOfStarTypes*fNumber)];
    }

    INFO_MSG("Universe generator", "Generated " << nNrOfStars << " Stars. Distribution of spectral classes: ")
    Log.logSeparator();
    
    for (int i=0; i<nNrOfStarTypes; ++i) {
        std::cout << "Class " << this->starClassToString(i) << ": ";
        std::cout << std::string(vecNrOfStars[i]*nBar/nNrOfStars,'#') << std::endl;
    }
    Log.logSeparator();

    std::poisson_distribution<int> PoissionDistribution(4.0);
    std::vector<int> vecNrOfPlanets(20,0);
    
    for (int i=0; i<nNrOfStars; ++i)
    {
        ++vecNrOfPlanets[PoissionDistribution(Generator)];
    }
    
    INFO_MSG("Universe generator", "Generated planets. Distribution of number of planet per star system: ")
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
        case 0: return "M"; break;
        case 1: return "K"; break;
        case 2: return "G"; break;
        case 3: return "F"; break;
        case 4: return "A"; break;
        case 5: return "B"; break;
        case 6: return "O"; break;
    }
}
