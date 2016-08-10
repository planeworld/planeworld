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
    
    INFO_MSG("Triple buffer [unit test]", "Starting unit test...")
       
//     CTripleBuffer<std::unordered_map<std::string, CObject>, std::string, CObject> TripleBufferByName;
//     CTripleBuffer<std::vector<CObject>, CObject> TripleBufferByValue;
//    
//     CObject Obj0; Obj0.nVal=0; Obj0.strName="Obj0";
//     CObject Obj1; Obj1.nVal=1; Obj1.strName="Obj1";
//     CObject Obj2; Obj2.nVal=2; Obj2.strName="Obj2";
//     CObject Obj3; Obj3.nVal=3; Obj3.strName="Obj3";
//     
//     std::vector<CObject> TestVector;
//     TestVector.push_back(Obj0);
//     TestVector.push_back(Obj1);
//     
// //     TripleBufferByValue.buffer(TestVector);
//     TripleBufferByValue.add(Obj2);
//     TripleBufferByValue.add(Obj3);
//     
//     std::unordered_map<std::string, CObject> TestMap;
//     TestMap.insert(std::pair<std::string, CObject>(Obj0.strName, Obj0));
//     TestMap.insert(std::pair<std::string, CObject>(Obj1.strName, Obj1));
//     
// //     TripleBufferByName.buffer(TestMap);
//     TripleBufferByName.add(Obj2.strName, Obj2);
//     TripleBufferByName.add(Obj3.strName, Obj3);
//     
//     if (TripleBufferByName.getFront()->size() != 4u)
//     {
//         ERROR_MSG("Unit test", "TripleBufferByName has wrong size.")
//         return EXIT_FAILURE;
//     }
//     if (TripleBufferByValue.getFront()->size() != 4u)
//     {
//         ERROR_MSG("Unit test", "TripleBufferByValue has wrong size.")
//         return EXIT_FAILURE;
//     }
// 
//     TripleBufferByName.swapBack();
//     TripleBufferByName.swapFront();
//     
//     TripleBufferByValue.swapBack();
//     TripleBufferByValue.swapFront();
//     
//     if ((TripleBufferByName.getFront()->at("Obj0").nVal != 0u) ||
//         (TripleBufferByName.getFront()->at("Obj1").nVal != 1u) ||
//         (TripleBufferByName.getFront()->at("Obj2").nVal != 2u) ||
//         (TripleBufferByName.getFront()->at("Obj3").nVal != 3u))
//     {
//         ERROR_MSG("Unit test", "TripleBufferByName has wrong entry.")
//         return EXIT_FAILURE;
//     }
//     
//     if ((TripleBufferByValue.getFront()->operator[](0).nVal != 0u) ||
//         (TripleBufferByValue.getFront()->operator[](1).nVal != 1u) ||
//         (TripleBufferByValue.getFront()->operator[](2).nVal != 2u) ||
//         (TripleBufferByValue.getFront()->operator[](3).nVal != 3u))
//     {
//         ERROR_MSG("Unit test", "TripleBufferByName has wrong entry.")
//         return EXIT_FAILURE;
//     }
  
    INFO_MSG("Triple buffer [unit test]", "...done. Test successful.")
    return EXIT_SUCCESS;
}
