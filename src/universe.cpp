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
#include "namegenerator.h"
#include "universe.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CUniverse::CUniverse() : m_pStar(0),
                         m_pStarShape(0),
                         m_pStarVisuals(0),
                         m_pStarObjectVisuals(0),
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
    
    CNameGenerator StarNameGenerator(_nSeed);
    const int nBar=100;
    const int nNrOfStarTypes=7;
    
    m_StarSystems.reserve(_nNumberOfStars);
    
    std::mt19937 Generator;
    
    Generator.seed(_nSeed);
    
    // Density = m_nNrOfStars/(3.0*fSigma * 2.0*M_PI) = 0.4/30.857e15;
    double fSigma = _nNumberOfStars * 30.857e15 / (0.4*3.0*2.0*M_PI);
//     double fSigma = _nNumberOfStars * 30.857e8 / (0.4*3.0*2.0*M_PI);
    
    std::exponential_distribution<double>   ExponentialDistribution(3.5);
    std::normal_distribution<double>        NormalDistribution(0.0, fSigma);
    std::uniform_real_distribution<double>  UniformDistribution(0.0,2.0*M_PI);
    std::poisson_distribution<int>          PoissionDistribution(4);
    std::vector<int> vecNrOfPlanets;
    std::vector<int> vecNrOfStars(nNrOfStarTypes,0);

    int nNrOfPlanets = 0;
    
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
            Vector2d vecPosition(fDistance*std::sin(fAngle),fDistance*std::cos(fAngle));
            
            IUniverseScaled::separateCenterCell(vecPosition,vecCenter,vecCell);
            
            pStarSystem->setName(StarNameGenerator.getName());
            pStarSystem->setStarType(int(nNrOfStarTypes*fNumber));
            pStarSystem->setSeed(i);
            pStarSystem->setCenter(vecCenter);
            pStarSystem->setCell(vecCell);
            pStarSystem->setNumberOfPlanets(PoissionDistribution(Generator));
            nNrOfPlanets += pStarSystem->getNumberOfPlanets();
            
            INFO_MSG("Universe Generator", "System name: " << pStarSystem->getName())
            
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
            std::cout << std::string(double(vecNrOfStars[i]*nBar)/_nNumberOfStars,'#') << std::endl;
        }
        Log.logSeparator();
    )

    DOM_VAR(INFO_MSG("Universe generator", "Generated "<< nNrOfPlanets << " planets."))
    DEBUG(
        DEBUG_MSG("Universe generator", "Distribution of number of planet per star system: ")
        Log.logSeparator();
        
        for (int i=0; i<m_nNrOfPlanetsMax; ++i)
            std::cout << "Planets: " << i << ": " << std::string(double(vecNrOfPlanets[i]*nBar)/nNrOfPlanets,'#') << std::endl;
        
        Log.logSeparator();
    )
    
    // Reserve memory for star object
    m_pStar = new CRigidBody;
    m_pStar->setName("Procedurally_Generated_Star_Dummy");
    m_pStar->enableDynamics();
    m_pStar->enableGravitation();
    m_pStarShape = new CCircle;
    CDoubleBufferedShape* pShape = new CDoubleBufferedShape;
    pShape->buffer(m_pStarShape);
    m_pStar->getGeometry()->addShape(pShape);
    m_pStarVisuals = new CCircleVisuals(pShape);
    m_pStarObjectVisuals = new IObjectVisuals(m_pStar);
    
    MEM_ALLOC("pStar");
    MEM_ALLOC("pStarShape");
    MEM_ALLOC("pShape")
    MEM_ALLOC("pStarVisuals");
    MEM_ALLOC("pStarObjectVisuals");

    m_pStarObjectVisuals->addVisuals(m_pStarVisuals);

    m_Objects.push_back(m_pStar);
    m_Visuals.push_back(m_pStarObjectVisuals);
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
    
    for (std::vector<CStarSystem*>::const_iterator ci=_Universe.getStarSystems().begin();
                                                   ci!=_Universe.getStarSystems().end(); ++ci)
    {
        CStarSystem* pStarSystem = new CStarSystem();
        MEM_ALLOC("pStarSystem");
        
        *pStarSystem = *(*ci);
        m_StarSystems.push_back((pStarSystem));
    }

    // Hand over object information. Important: The original objects are kept,
    // the pointers are copied. This class passes responsibility of dynamically
    // allocated objects!
    m_pStar = _Universe.m_pStar;
    m_pStarShape = _Universe.m_pStarShape;
    m_pStarVisuals = _Universe.m_pStarVisuals;
    m_pStarObjectVisuals = _Universe.m_pStarObjectVisuals;
    m_Objects.push_back(_Universe.m_pStar);
    m_Visuals.push_back(_Universe.m_pStarObjectVisuals);
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
