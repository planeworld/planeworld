////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2018 Torsten Büschenfeld
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
/// \file       serializer_basic.h
/// \brief      Prototype of class "CSerializerBasic"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2018-04-26
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZER_BASIC_H
#define SERIALIZER_BASIC_H

//--- Standard header --------------------------------------------------------//
#include <fstream>

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "serializer.h"

//--- Misc header ------------------------------------------------------------//


//--- Forward declarations ---------------------------------------------------//

using namespace Eigen;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class implementing a basic serializer
///
////////////////////////////////////////////////////////////////////////////////
class CSerializerBasic : public ISerializer
{

    public:
   
        //--- Constructor/Destructor -----------------------------------------//
        CSerializerBasic() : m_strFilename("")
        {
            METHOD_ENTRY("CSerializerBasic::CSerializerBasic")
            CTOR_CALL("CSerializerBasic::CSerializerBasic")
        }
        
        ~CSerializerBasic() override
        {
            METHOD_ENTRY("CSerializerBasic::~CSerializerBasic")
            DTOR_CALL("CSerializerBasic::~CSerializerBasic")
            
            // Only close an open stream
            if (m_Stream.is_open())
            {
                m_Stream.close();
                DOM_FIO(DEBUG_MSG("Gamestate Manager", m_strFilename + " closed."))
            }
        }
        

        //--- Methods --------------------------------------------------------//
        bool setFilename(const std::string& _strFilename)
        {
            METHOD_ENTRY("CSerializerBasic::setFilename")
            m_strFilename = _strFilename;

            m_Stream.open(_strFilename);
            if (!m_Stream)
            {
                DOM_FIO(ERROR_MSG("Serializer", "File " + _strFilename + " could not be created."))
                m_Stream.clear();
                return false;
            }
            else
            {
                DOM_FIO(DEBUG_MSG("Serializer", _strFilename + " succesfully created."))
                /// \todo Use std::numeric_limits to set precision
                m_Stream << std::setprecision(17) << std::setw(25) << "Test: Serializer should fill this. " << std::endl;
                return true;
            }
        }
        
        void serialize(const std::string& _strDescr) override
        {
            m_Stream << "--- " << _strDescr << " ---" << std::endl;
        }
        void serialize(const std::string& _strDescr, bool _bB) override
        {
            m_Stream << "bool: " << _strDescr << " = " << _bB << std::endl;
        }
        void serialize(const std::string& _strDescr, double _fD) override
        {
            m_Stream << "double: " << _strDescr << " = " << _fD << std::endl;
        }
        void serialize(const std::string& _strDescr, int _nI) override
        {
            m_Stream << "int: " << _strDescr << " = " << _nI << std::endl;
        }
        void serialize(const std::string& _strDescr, unsigned int _unI) override
        {
            m_Stream << "unsigned int: " << _strDescr << " = " << _unI << std::endl;
        }
        void serialize(const std::string& _strDescr, std::size_t _nI) override
        {
            m_Stream << "size_t: " << _strDescr << " = " << _nI << std::endl;
        }
        void serialize(const std::string& _strDescr, const std::string& _strS) override
        {
            m_Stream << "string: " << _strDescr << " = " << _strS << std::endl;
        }
        void serialize(const std::string& _strDescr, const Vector2d& _vecV) override
        {
            m_Stream << "vector2d: " << _strDescr << " = " << _vecV[0] << ", " << _vecV[1] << std::endl;
        }
        void serialize(const std::string& _strDescr, const Vector2i& _vecV) override
        {
            m_Stream << "vector2i: " << _strDescr << " = " << _vecV[0] << ", " << _vecV[1] << std::endl;
        }
        
    protected:
        
        std::ofstream   m_Stream; ///< Output stream of data
        std::string     m_strFilename; ///< Filename to store data
        
};

//--- Implementation is done here for inline optimisation --------------------//

#endif // SERIALIZER_BASIC_H
