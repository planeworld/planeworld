////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2019 Torsten Büschenfeld
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
    
    CMultiBuffer<BUFFER_DOUBLE, int> DoubleBufferSingle;
    CMultiBuffer<BUFFER_DOUBLE, std::vector<int>, int> DoubleBufferUnary;
    CMultiBuffer<BUFFER_DOUBLE, std::unordered_map<std::string, int>, std::string, int> DoubleBufferBinary;
    
   
    int n0=0; std::string str0="zero";
    int n1=1; std::string str1="one";
    int n2=2; std::string str2="two";
    int n3=3; std::string str3="three";

    INFO_MSG("Unit test", "Testing double buffer")
    
    INDENT()
    INFO_MSG("Unit test", "Insertion")
    
    DoubleBufferSingle.add(n0);
    
    DoubleBufferUnary.add(n0);
    DoubleBufferUnary.add(n1);
    DoubleBufferUnary.add(n2);
    DoubleBufferUnary.add(n3);
    
    DoubleBufferBinary.add(str0, n0);
    DoubleBufferBinary.add(str1, n1);
    DoubleBufferBinary.add(str2, n2);
    DoubleBufferBinary.add(str3, n3);
    
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
    DoubleBufferSingle.swap<BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT>();
    DoubleBufferUnary.swap<BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT>();
    DoubleBufferBinary.swap<BUFFER_DOUBLE_BACK, BUFFER_DOUBLE_FRONT>();
    

    if (*DoubleBufferSingle.getBuffer<BUFFER_DOUBLE_FRONT>() != 0)
    {
        ERROR_MSG("Unit test", "DoubleBufferSingle has wrong entry.")
        return EXIT_FAILURE;
    }
    if ((DoubleBufferUnary.getBuffer<BUFFER_DOUBLE_FRONT>()->at(0) != 0) ||
        (DoubleBufferUnary.getBuffer<BUFFER_DOUBLE_FRONT>()->at(1) != 1) ||
        (DoubleBufferUnary.getBuffer<BUFFER_DOUBLE_FRONT>()->at(2) != 2) ||
        (DoubleBufferUnary.getBuffer<BUFFER_DOUBLE_FRONT>()->at(3) != 3))
    {
        ERROR_MSG("Unit test", "DoubleBufferUnary has wrong entry.")
        return EXIT_FAILURE;
    }
    if ((DoubleBufferBinary.getBuffer<BUFFER_DOUBLE_FRONT>()->at("zero") != 0) ||
        (DoubleBufferBinary.getBuffer<BUFFER_DOUBLE_FRONT>()->at("one") != 1) ||
        (DoubleBufferBinary.getBuffer<BUFFER_DOUBLE_FRONT>()->at("two") != 2) ||
        (DoubleBufferBinary.getBuffer<BUFFER_DOUBLE_FRONT>()->at("three") != 3))
    {
        ERROR_MSG("Unit test", "DoubleBufferBinary has wrong entry.")
        return EXIT_FAILURE;
    }
    
    UNINDENT()
    UNINDENT()
    INFO_MSG("Unit test", "...done. Test successful.")
    return EXIT_SUCCESS;
}
