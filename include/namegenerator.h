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
/// \file       namegenerator.h
/// \brief      Prototype of class "CNameGenerator"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2013-09-16
///
////////////////////////////////////////////////////////////////////////////////

#ifndef NAME_GENERATOR_H
#define NAME_GENERATOR_H

/// Characters of the alphabet
const char ALPHABET[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

const int NAME_GENERATOR_LENGTH_MIN  =  3; ///< Minimum length for generated names
const int NAME_GENERATOR_LENGTH_MAX  =  9; ///< Maximum length for generated names
const int NAME_GENERATOR_LENGTH_MEAN =  5; ///< Mean length for generated names

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for procedural creation of names.
///
////////////////////////////////////////////////////////////////////////////////
class CNameGenerator
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CNameGenerator();
        CNameGenerator(const int&);
        
        //--- Methods --------------------------------------------------------//
        const std::string getName();
        
    private:
    
        //--- Variables ------------------------------------------------------//
        std::mt19937    m_Generator; ///< Random number Generator
};

#endif // CNAMEGENERATOR_H
