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
/// \date       2016-06-15
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_unit_uid.cpp
/// \brief      Main program for unit test
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-06-15
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "unique_id.h"

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
    
    INFO_MSG("Unique ID [unit test]", "Starting unit test...")
       
    CUniqueID UID0;
    if (UID0.getValue() != 0u)
    {
        ERROR_MSG("Unique ID [unit test]", "Initial uid value not 0.")
        return EXIT_FAILURE;
    }
    if (UID0.getName() != "UID_0")
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid0=" << UID0.getName() << ")")
        return EXIT_FAILURE;
    }
    // UID 1,2 in local scope
    {
        CUniqueID UID1;
        CUniqueID UID2;
        if (UID2.getValue() != 2u)
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid2=" << UID2.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID2.getName() != "UID_2")
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid2=" << UID2.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    // UID 1,2 should be used, since they were destroyed when exiting scope
    CUniqueID UID3;
    CUniqueID UID4;
    if (UID3.getValue() == 1u)
    {
        if (UID3.getName() != "UID_1")
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid3=" << UID3.getName() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getValue() != 2u)
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid4=" << UID4.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getName() != "UID_2")
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid4=" << UID4.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    else if (UID3.getValue() == 2u)
    {
        if (UID3.getName() != "UID_2")
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid3=" << UID3.getName() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getValue() != 1u)
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid4=" << UID4.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getName() != "UID_1")
        {
            ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid4=" << UID4.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    else
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid3=" << UID3.getValue() << ")")
        return EXIT_FAILURE;
    }
    
    // Counting should go on, UID 5 should be set to 3
    CUniqueID UID5;
    if (UID5.getValue() != 3u)
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid5=" << UID5.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID5.getName() != "UID_3")
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid5=" << UID5.getName() << ")")
        return EXIT_FAILURE;
    }
    // Copy constructor should create a new uid, ignoring the given one
    CUniqueID UID6(UID5);
    if (UID6.getValue() != 4u)
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect uid value (uid6=" << UID6.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID6.getName() != "UID_4")
    {
        ERROR_MSG("Unique ID [unit test]", "Incorrect default string (uid6=" << UID6.getName() << ")")
        return EXIT_FAILURE;
    }
  
    INFO_MSG("Unique ID [unit test]", "...done. Test successful.")
    return EXIT_SUCCESS;
}
