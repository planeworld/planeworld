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
/// \brief Outputs data of UID internal structures
///
/// \param  _strAction String, explaining which operation was done with UID
///
///////////////////////////////////////////////////////////////////////////////
void outputInternalUIDData(const std::string& _strAction)
{
    METHOD_ENTRY("outputInternalUIDData")
    INFO_BLK(
        std::cout << _strAction << std::endl;
        std::cout << "  Unused UIDs: ";
        for (auto UnusedUIDs : CUniqueID::getUnusedUIDs())
            std::cout << UnusedUIDs << " ";
        std::cout << std::endl;
        std::cout << "  Referenced UIDs [val]:  ";
        for (auto ReferencedUIDs : CUniqueID::getReferencedUIDs())
            std::cout <<  ReferencedUIDs.first << " ";
        std::cout << std::endl;
        std::cout << "  Referenced UIDs [refs]: ";
        for (auto ReferencedUIDs : CUniqueID::getReferencedUIDs())
            std::cout <<  ReferencedUIDs.second << " ";
        std::cout << std::endl;
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
       
    CUniqueID UID0;
    outputInternalUIDData("1x Constructor");
    if (UID0.getValue() != 1u)
    {
        ERROR_MSG("Unit test", "Initial uid value not 1.")
        return EXIT_FAILURE;
    }
    if (UID0.getName() != "UID_1")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid0=" << UID0.getName() << ")")
        return EXIT_FAILURE;
    }
    // UID 1,2 in local scope
    {
        CUniqueID UID1;
        CUniqueID UID2;
        outputInternalUIDData("2x Constructor");
        if (UID2.getValue() != 3u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid2=" << UID2.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID2.getName() != "UID_3")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid2=" << UID2.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    outputInternalUIDData("2x Destructor");
    
    // UID 1,2 should be used, since they were destroyed when exiting scope
    CUniqueID UID3;
    CUniqueID UID4;
    outputInternalUIDData("2x Constructor");
    if (UID3.getValue() == 2u)
    {
        if (UID3.getName() != "UID_2")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid3=" << UID3.getName() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getValue() != 3u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid4=" << UID4.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getName() != "UID_3")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid4=" << UID4.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    else if (UID3.getValue() == 3u)
    {
        if (UID3.getName() != "UID_3")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid3=" << UID3.getName() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getValue() != 2u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid4=" << UID4.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID4.getName() != "UID_2")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid4=" << UID4.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    else
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid3=" << UID3.getValue() << ")")
        return EXIT_FAILURE;
    }
    {
        // Counting should go on, UID 5 should be set to 4
        CUniqueID UID5;
        outputInternalUIDData("1x Constructor");
        if (UID5.getValue() != 4u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid5=" << UID5.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID5.getName() != "UID_4")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid5=" << UID5.getName() << ")")
            return EXIT_FAILURE;
        }
        // Copy constructor should copy the UID, but start reference counting
        {
            CUniqueID UID6(UID5);
            outputInternalUIDData("1x Copy constructor");
            if (UID6.getValue() != 4u)
            {
                ERROR_MSG("Unit test", "Incorrect uid value (uid6=" << UID6.getValue() << ")")
                return EXIT_FAILURE;
            }
            if (UID6.getName() != "UID_4")
            {
                ERROR_MSG("Unit test", "Incorrect default string (uid6=" << UID6.getName() << ")")
                return EXIT_FAILURE;
            }
            CUniqueID UID7(UID6);
            outputInternalUIDData("1x Copy constructor");
            if (UID7.getValue() != 4u)
            {
                ERROR_MSG("Unit test", "Incorrect uid value (uid7=" << UID7.getValue() << ")")
                return EXIT_FAILURE;
            }
            if (UID7.getName() != "UID_4")
            {
                ERROR_MSG("Unit test", "Incorrect default string (uid7=" << UID7.getName() << ")")
                return EXIT_FAILURE;
            }
        }
        outputInternalUIDData("2x Destructor");
        
        CUniqueID UID8;
        outputInternalUIDData("1x Constructor");
        if (UID8.getValue() != 5u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid8=" << UID8.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID8.getName() != "UID_5")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid8=" << UID8.getName() << ")")
            return EXIT_FAILURE;
        }
        CUniqueID UID9; 
        outputInternalUIDData("1x Constructor");
        UID9 = UID8;
        outputInternalUIDData("1x Copy assignment operator");
        if (UID9.getValue() != 5u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid9=" << UID9.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID8.getName() != "UID_5")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid9=" << UID9.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    outputInternalUIDData("2x Destructor");
    // UIDs 3, 4, and 5 should be free by now
    CUniqueID UID10;
    outputInternalUIDData("1x Constructor");
    if (UID10.getValue() != 6u)
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid10=" << UID10.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID10.getName() != "UID_6")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid10=" << UID10.getName() << ")")
        return EXIT_FAILURE;
    }
    CUniqueID UID11;
    outputInternalUIDData("1x Constructor");
    if (UID11.getValue() != 5u)
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid11=" << UID11.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID11.getName() != "UID_5")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid11=" << UID11.getName() << ")")
        return EXIT_FAILURE;
    }
    CUniqueID UID12;
    outputInternalUIDData("1x Constructor");
    if (UID12.getValue() != 4u)
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid12=" << UID12.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID12.getName() != "UID_4")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid12=" << UID12.getName() << ")")
        return EXIT_FAILURE;
    }
    // Test setting of new IDs
    {
        CUniqueID UID13;
        outputInternalUIDData("1x Constructor");
        UID12.setNewID();
        outputInternalUIDData("1x New ID");
        if (UID12.getValue() != 8u)
        {
            ERROR_MSG("Unit test", "Incorrect uid value (uid12=" << UID12.getValue() << ")")
            return EXIT_FAILURE;
        }
        if (UID12.getName() != "UID_8")
        {
            ERROR_MSG("Unit test", "Incorrect default string (uid12=" << UID12.getName() << ")")
            return EXIT_FAILURE;
        }
    }
    outputInternalUIDData("1x Destructor");
    UID12.setNewID();
    outputInternalUIDData("1x New ID");
    if (UID12.getValue() != 4u)
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid12=" << UID12.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID12.getName() != "UID_4")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid12=" << UID12.getName() << ")")
        return EXIT_FAILURE;
    }
    UID12.setNewID();
    outputInternalUIDData("1x New ID");
    if (UID12.getValue() != 7u)
    {
        ERROR_MSG("Unit test", "Incorrect uid value (uid12=" << UID12.getValue() << ")")
        return EXIT_FAILURE;
    }
    if (UID12.getName() != "UID_7")
    {
        ERROR_MSG("Unit test", "Incorrect default string (uid12=" << UID12.getName() << ")")
        return EXIT_FAILURE;
    }
    
 
    INFO_MSG("Unit test", "...done. Test successful.")
    return EXIT_SUCCESS;
}
