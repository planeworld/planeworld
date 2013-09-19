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
/// \file       namegenerator.cpp
/// \brief      Implementation of class "CNameGenerator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2013-09-16
///
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <random>

#include "namegenerator.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CNameGenerator::CNameGenerator()
{
    m_Generator.seed(1);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor, setting the seed for random name generation.
///
/// \param _nSeed Seed for random name generation
///
///////////////////////////////////////////////////////////////////////////////
CNameGenerator::CNameGenerator(const int& _nSeed)
{
    m_Generator.seed(_nSeed);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Return a random name from generator.
///
/// \return Random name
///
///////////////////////////////////////////////////////////////////////////////
const std::string CNameGenerator::getName()
{
    std::uniform_int_distribution<int>  CharDistribution(0,25);
    std::poisson_distribution<int>      LengthDistribution(NAME_GENERATOR_LENGTH_MEAN);
    
    int nMode = 0;
    
    std::string strOut("");
    
    int nLength = LengthDistribution(m_Generator);
    while (nLength > NAME_GENERATOR_LENGTH_MAX || nLength < NAME_GENERATOR_LENGTH_MIN) nLength = LengthDistribution(m_Generator);
       
    while (nLength-- > 0)
    {
        int nChar = CharDistribution(m_Generator);
        
        switch (nChar)
        {
            case 0:
            case 4:
            case 8:
            case 14:
            case 20:
              nMode = 1;
              strOut += ALPHABET[nChar];
              break;
            default:
              if (nMode == 2)
              {
                  if (nChar == 18)
                    strOut += ALPHABET[nChar];
                  else
                    nLength++;
              }
              else
              {
                  strOut += ALPHABET[nChar];
                  nMode = 2;
              }
        }
    }

    // First character to upper case
    std::transform(strOut.begin(), strOut.begin()+1,strOut.begin(), ::toupper);
    
    return strOut;
}

