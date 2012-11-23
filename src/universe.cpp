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
CUniverse::CUniverse() : m_nNrOfStars(UNIVERSE_NR_OF_STARS_DEFAULT)
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
///
///////////////////////////////////////////////////////////////////////////////
void CUniverse::generate(const int& _nSeed)
{
    METHOD_ENTRY("CUniverse::generate")
    
    const int nBar=100;
    const int nNrOfStarTypes=7;
    
    m_StarSystems.reserve(m_nNrOfStars);
    
    std::mt19937 Generator;
    
    Generator.seed(_nSeed);
    
    // Density = m_nNrOfStars/(3.0*fSigma * 2.0*M_PI) = 0.4/30.857e15;
    double fSigma = m_nNrOfStars * 30.857e15 / (0.4*3.0*2.0*M_PI);
    
    std::exponential_distribution<double>   ExponentialDistribution(3.5);
    std::normal_distribution<double>        NormalDistribution(0.0, fSigma);
    std::uniform_real_distribution<double>  UniformDistribution(0.0,2.0*M_PI);
    std::poisson_distribution<int>          PoissionDistribution(3.0);
    std::vector<int> vecNrOfPlanets(20,0);
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    // Create a globular cluster
    for (int i=0; i<m_nNrOfStars; ++i)
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
            
            vecCell[0] = static_cast<int>(fDistance*std::sin(fAngle)/DEFAULT_CELL_SIZE);
            vecCell[1] = static_cast<int>(fDistance*std::cos(fAngle)/DEFAULT_CELL_SIZE);
            
            vecCenter[0] = fDistance*std::sin(fAngle)-vecCell[0]*DEFAULT_CELL_SIZE;
            vecCenter[1] = fDistance*std::cos(fAngle)-vecCell[1]*DEFAULT_CELL_SIZE;
            
            pStarSystem->setStarType(int(nNrOfStarTypes*fNumber));
            pStarSystem->setCenter(vecCenter);
            pStarSystem->setCell(vecCell);
            pStarSystem->setNumberOfPlanets(PoissionDistribution(Generator));
            
            m_StarSystems.push_back(pStarSystem);
            
            ++vecNrOfPlanets[pStarSystem->getNumberOfPlanets()];
        }
    }

    INFO_MSG("Universe generator", "Generated " << m_StarSystems.size() << " Stars. Distribution of spectral classes: ")
    Log.logSeparator();
    
    for (int i=0; i<nNrOfStarTypes; ++i) {
        std::cout << "Class " << this->starClassToString(i) << ": ";
        std::cout << std::string(vecNrOfStars[i]*nBar/m_nNrOfStars,'#') << std::endl;
    }
    Log.logSeparator();

    int nNrOfPlanets = 0;
    for (int i=0; i<20; ++i)
        nNrOfPlanets += vecNrOfPlanets[i]*i;
    
    INFO_MSG("Universe generator", "Generated "<< nNrOfPlanets << " planets. Distribution of number of planet per star system: ")
    Log.logSeparator();
    
    for (int i=0; i<10; ++i)
        std::cout << "Planets: " << i << ": " << std::string(vecNrOfPlanets[i]*nBar/m_nNrOfStars,'#') << std::endl;
    
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
