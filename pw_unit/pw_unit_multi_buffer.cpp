////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016 Torsten Büschenfeld
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
/// \mainpage   Project Planeworld -- Unit Tests
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-08-06
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_unit_multi_buffer.cpp
/// \brief      Main program for unit test
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-08-06
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <cstdint>
#include <unordered_map>
#include <vector>

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "multi_buffer.h"

//--- Misc-Header ------------------------------------------------------------//

/// Data for unit tests
class CObject
{
    public: 
        std::string strName = "";
        std::uint32_t nVal = 0;
};

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Main function
///
/// This is the entrance point for program startup.
///
/// \param  argc Number of given arguments
/// \param  argv Array, storing the arguments
/// \return Exit code
///
///////////////////////////////////////////////////////////////////////////////
int main()
{
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    
    INFO_MSG("Unit test", "Starting unit test...")
    INDENT()
    
    CMultiBuffer<BUFFER_DOUBLE, CObject> DoubleBufferSingle;
    CMultiBuffer<BUFFER_DOUBLE, std::vector<CObject>, CObject> DoubleBufferUnary;
    CMultiBuffer<BUFFER_DOUBLE, std::unordered_map<std::string, CObject>, std::string, CObject> DoubleBufferBinary;
    
   
    CObject Obj0; Obj0.nVal=0; Obj0.strName="Obj0";
    CObject Obj1; Obj1.nVal=1; Obj1.strName="Obj1";
    CObject Obj2; Obj2.nVal=2; Obj2.strName="Obj2";
    CObject Obj3; Obj3.nVal=3; Obj3.strName="Obj3";

    INFO_MSG("Unit test", "Testing double buffer")
    
    INDENT()
    INFO_MSG("Unit test", "Insertion")
    
    DoubleBufferSingle.add(Obj0);
    
    DoubleBufferUnary.add(Obj0);
    DoubleBufferUnary.add(Obj1);
    DoubleBufferUnary.add(Obj2);
    DoubleBufferUnary.add(Obj3);
    
    DoubleBufferBinary.add(Obj0.strName, Obj0);
    DoubleBufferBinary.add(Obj1.strName, Obj1);
    DoubleBufferBinary.add(Obj2.strName, Obj2);
    DoubleBufferBinary.add(Obj3.strName, Obj3);
    
    if (DoubleBufferSingle.getBufferSize() != 2u)
    {
        ERROR_MSG("Unit test", "DoubleBufferSingle has wrong buffer size.")
        return EXIT_FAILURE;
    }
    if (DoubleBufferUnary.getBufferSize() != 2u)
    {
        ERROR_MSG("Unit test", "DoubleBufferUnary has wrong buffer size.")
        return EXIT_FAILURE;
    }
    if (DoubleBufferBinary.getBufferSize() != 2u)
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong buffer size.")
        return EXIT_FAILURE;
    }
    
    if (DoubleBufferSingle.getContainerSize() != 1u)
    {
        ERROR_MSG("Unit test", "DoubleBufferUnary has wrong container size.")
        return EXIT_FAILURE;
    }
    if (DoubleBufferUnary.getContainerSize() != 4u)
    {
        ERROR_MSG("Unit test", "DoubleBufferUnary has wrong container size.")
        return EXIT_FAILURE;
    }
    if (DoubleBufferBinary.getContainerSize() != 4u)
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong container size.")
        return EXIT_FAILURE;
    }
    
    
    INFO_MSG("Unit test", "Swapping")
    DoubleBufferSingle.swap(BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT);
    DoubleBufferUnary.swap(BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT);
    DoubleBufferBinary.swap(BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT);
    

    if ((DoubleBufferSingle.getBuffer(BUFFER_DOUBLE_FRONT)->nVal != 0u) ||
        (DoubleBufferSingle.getBuffer(BUFFER_DOUBLE_FRONT)->strName != "Obj0"))
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong entry.")
        return EXIT_FAILURE;
    }
    if ((DoubleBufferUnary.getBuffer(BUFFER_DOUBLE_FRONT)->at(0).nVal != 0u) ||
        (DoubleBufferUnary.getBuffer(BUFFER_DOUBLE_FRONT)->at(1).nVal != 1u) ||
        (DoubleBufferUnary.getBuffer(BUFFER_DOUBLE_FRONT)->at(2).nVal != 2u) ||
        (DoubleBufferUnary.getBuffer(BUFFER_DOUBLE_FRONT)->at(3).nVal != 3u))
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong entry.")
        return EXIT_FAILURE;
    }
    if ((DoubleBufferBinary.getBuffer(BUFFER_DOUBLE_FRONT)->at("Obj0").nVal != 0u) ||
        (DoubleBufferBinary.getBuffer(BUFFER_DOUBLE_FRONT)->at("Obj1").nVal != 1u) ||
        (DoubleBufferBinary.getBuffer(BUFFER_DOUBLE_FRONT)->at("Obj2").nVal != 2u) ||
        (DoubleBufferBinary.getBuffer(BUFFER_DOUBLE_FRONT)->at("Obj3").nVal != 3u))
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong entry.")
        return EXIT_FAILURE;
    }
    
    UNINDENT()
    UNINDENT()
    INFO_MSG("Unit test", "...done. Test successful.")
    return EXIT_SUCCESS;
}
