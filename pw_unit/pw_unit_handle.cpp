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
/// \date       2019-01-12
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_unit_handle.cpp
/// \brief      Main program for unit test
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2019-01-12
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "handle.h"

//--- Misc-Header ------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Outputs data of internal handle structures
///
/// \param  _strAction String, explaining which operation was done with handle
///
///////////////////////////////////////////////////////////////////////////////
void outputInternalHandleData(const std::string& _strAction)
{
    METHOD_ENTRY("outputInternalUIDData")
    INFO_BLK(
//         std::cout << _strAction << std::endl;
//         std::cout << "  Unused UIDs: ";
//         for (auto UnusedUIDs : CUID::getUnusedUIDs())
//             std::cout << UnusedUIDs << " ";
//         std::cout << std::endl;
//         std::cout << "  Referenced UIDs [val]:  ";
//         for (auto ReferencedUIDs : CUID::getReferencedUIDs())
//             std::cout <<  ReferencedUIDs.first << " ";
//         std::cout << std::endl;
//         std::cout << "  Referenced UIDs [refs]: ";
//         for (auto ReferencedUIDs : CUID::getReferencedUIDs())
//             std::cout <<  ReferencedUIDs.second << " ";
//         std::cout << std::endl;
    )
}

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
       
    
    
 
    INFO_MSG("Unit test", "...done. Test successful.")
    return EXIT_SUCCESS;
}
