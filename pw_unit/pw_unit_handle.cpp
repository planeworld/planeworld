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
#include <iomanip>
#include <string>

//--- Program header ---------------------------------------------------------//
#include "conf_pw.h"
#include "log.h"
#include "handle.h"

//--- Misc-Header ------------------------------------------------------------//

//--- Constants --------------------------------------------------------------//
static constexpr int SHOW_HEADER = 1;
static constexpr int SHOW_FOOTER = 2;
static constexpr int INDENTION_L1 = 4;
static constexpr int INDENTION_L2 = 2*INDENTION_L1;
static constexpr int WIDTH_COL_01 = 10;
static constexpr int WIDTH_COL_02 = 10;
static constexpr int WIDTH_COL_03 = 10;
static constexpr int WIDTH_COL_04 = 10;
static constexpr int WIDTH_COL_05 = 14;

#define PW_UNIT_CHECK(a) if ((a) == false) \
                         { \
                             ERROR_MSG("Unit Test", "... interupted. Test not successful.") \
                             return EXIT_FAILURE; \
                         }

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Indents output by given value using whitespaces
///
/// \param  _nIndention Number of columns to indent (space)
///
////////////////////////////////////////////////////////////////////////////////
void indent(const int _nIndention)
{
    for (auto i=0; i<_nIndention; ++i) std::cout << " ";
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Outputs data of internal handle structures
///
/// \param  _strAction String, explaining which operation was done with handle
///
////////////////////////////////////////////////////////////////////////////////
void outputInternalHandleData(const std::string& _strAction)
{
    METHOD_ENTRY("outputInternalHandleData")
    
    std::cout <<  _strAction << "\n" << std::endl;
    INFO_BLK(
        indent(INDENTION_L1);
        std::cout << "Current handle map:" << std::endl;
        indent(INDENTION_L1);
        std::cout << "===================\n" << std::endl;
        indent(INDENTION_L2);
        std::cout << std::setw(WIDTH_COL_01) << "INDEX" << " | "
                  << std::setw(WIDTH_COL_02) << "COUNTER" << " | "
                  << std::setw(WIDTH_COL_03) << "STATE" << " | "
                  << std::setw(WIDTH_COL_05) << "POINTER"
                  << std::endl;
                  
        indent(INDENTION_L2);
        for (auto i=0u; i<WIDTH_COL_01+WIDTH_COL_02+WIDTH_COL_03+WIDTH_COL_05 + 9; ++i)
        {
            std::cout << "-";
        }
        std::cout << std::endl;
        
        for (auto Handle : *CHandleBase::getHandleMap())
        {
            indent(INDENTION_L2);
            if (Handle.ID.C.Index != 0u)
            {
                std::cout << std::setw(WIDTH_COL_01) << Handle.ID.C.Index << " | "
                          << std::setw(WIDTH_COL_02) << Handle.ID.C.Counter << " | "
                          << std::setw(WIDTH_COL_03) << "valid" << " | "
                          << std::setw(WIDTH_COL_05) << Handle.pEntry
                          << std::endl;
            }
            else
            {
                std::cout << std::setw(WIDTH_COL_01) << Handle.ID.C.Index << " | "
                          << std::setw(WIDTH_COL_02) << Handle.ID.C.Counter << " | "
                          << std::setw(WIDTH_COL_03) << "invalid" << " | "
                          << std::setw(WIDTH_COL_05) << Handle.pEntry
                          << std::endl;
            }
        }
        
        std::cout << std::endl;
        
        indent(INDENTION_L1);
        std::cout << "Currently free handle indices: " << CHandleBase::getFreeHandles()->size() << std::endl;
        indent(INDENTION_L1);
        std::cout << "================================\n" << std::endl;
        indent(INDENTION_L2);
        if (CHandleBase::getFreeHandles()->size() > 0)
        {
            for (auto FreeHandle : *CHandleBase::getFreeHandles())
                std::cout << FreeHandle << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    )
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Outputs data of handle
///
/// \param _h Handle
/// \param _nHeaderFooter Print header and/or footer
///
/// \return Handle valid?
///
///////////////////////////////////////////////////////////////////////////////
template<class T>
bool outputHandleData(const CHandle<T>& _h, const int _nHeaderFooter = 0)
{
    METHOD_ENTRY("outputHandleData")
    INFO_BLK(
        if (_nHeaderFooter & SHOW_HEADER)
        {
            
            indent(INDENTION_L1);
            std::cout << "Currently active handles:" << std::endl;
            indent(INDENTION_L1);
            std::cout << "=========================" << std::endl;
            std::cout << std::endl;
            indent(INDENTION_L2);
            std::cout << std::setw(WIDTH_COL_01) << "INDEX" << " | "
                      << std::setw(WIDTH_COL_02) << "COUNTER" << " | "
                      << std::setw(WIDTH_COL_03) << "STATE" << " | "
                      << std::setw(WIDTH_COL_04) << "CONTENT"
                      << std::endl;
            indent(INDENTION_L2);
            for (auto i=0u; i<WIDTH_COL_01+WIDTH_COL_02+WIDTH_COL_03+WIDTH_COL_04 + 9; ++i)
            {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
        
        indent(INDENTION_L2);
        if (_h.isValid())
        {
            std::cout << std::setw(WIDTH_COL_01) << _h.ID().C.Index << " | "
                      << std::setw(WIDTH_COL_02) << _h.ID().C.Counter << " | "
                      << std::setw(WIDTH_COL_03) << "valid" << " | "
                      << std::setw(WIDTH_COL_04) << *_h
                      << std::endl;
        }
        else
        {
            std::cout << std::setw(WIDTH_COL_01) << _h.ID().C.Index << " | "
                      << std::setw(WIDTH_COL_02) << _h.ID().C.Counter << " | "
                      << std::setw(WIDTH_COL_03) << "invalid" << " | "
                      << std::setw(WIDTH_COL_04) << "-"
                      << std::endl;
        }
        if (_nHeaderFooter & SHOW_FOOTER)
        {
            std::cout << std::endl;
        }
    )
    return _h.isValid();
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
       
    int nOne = 1;
    double fTwo = 2.19;
    std::string strThree("three");
    int* pOne = &nOne;
    double* pTwo = &fTwo;
    std::string* pThree = &strThree;
    
    CHandle<int> hOne(pOne);
    outputInternalHandleData("Created handle <int> " + std::to_string(hOne.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER | SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == true);
    
    CHandle<int> hCopy;
    hCopy = hOne;
    outputInternalHandleData("Copied handle <int> " + std::to_string(hOne.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == true);
    PW_UNIT_CHECK(hCopy.isValid() == true);
    
    CHandle<double> hTwo(pTwo);
    outputInternalHandleData("Created handle <double> " + std::to_string(hTwo.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == true);
    PW_UNIT_CHECK(hCopy.isValid() == true);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    
    CHandle<std::string> hThree(pThree);
    outputInternalHandleData("Created handle <std::string> " + std::to_string(hThree.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo);
    outputHandleData<std::string>(hThree, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == true);
    PW_UNIT_CHECK(hCopy.isValid() == true);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    PW_UNIT_CHECK(hThree.isValid() == true);
    
    hOne.remove();
    outputInternalHandleData("Removed handle <int> " + std::to_string(hOne.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo);
    outputHandleData<std::string>(hThree, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == false);
    PW_UNIT_CHECK(hCopy.isValid() == false);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    PW_UNIT_CHECK(hThree.isValid() == true);
    
    double f1 = 23.479;
    double* pf1 = &f1;;
    CHandle<double> hFour(pf1);
    outputInternalHandleData("Created handle <double> " + std::to_string(hFour.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo);
    outputHandleData<std::string>(hThree);
    outputHandleData<double>(hFour, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == false);
    PW_UNIT_CHECK(hCopy.isValid() == false);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    PW_UNIT_CHECK(hThree.isValid() == true);
    PW_UNIT_CHECK(hFour.isValid() == true);
    
    CHandle<std::string> hCopyStr;
    hCopyStr = hThree;
    outputInternalHandleData("Copied handle <std::string> " + std::to_string(hThree.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo);
    outputHandleData<std::string>(hThree);
    outputHandleData<std::string>(hCopyStr);
    outputHandleData<double>(hFour, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == false);
    PW_UNIT_CHECK(hCopy.isValid() == false);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    PW_UNIT_CHECK(hThree.isValid() == true);
    PW_UNIT_CHECK(hCopyStr.isValid() == true);
    PW_UNIT_CHECK(hFour.isValid() == true);
    
    std::string strUpdate("update");
    std::string* pstrUpdate = &strUpdate;
    hThree.update(pstrUpdate);
    outputInternalHandleData("Updated handle <std::string> " + std::to_string(hThree.ID().C.Index));
    outputHandleData<int>(hOne, SHOW_HEADER);
    outputHandleData<int>(hCopy);
    outputHandleData<double>(hTwo);
    outputHandleData<std::string>(hThree);
    outputHandleData<std::string>(hCopyStr);
    outputHandleData<double>(hFour, SHOW_FOOTER);
    PW_UNIT_CHECK(hOne.isValid() == false);
    PW_UNIT_CHECK(hCopy.isValid() == false);
    PW_UNIT_CHECK(hTwo.isValid() == true);
    PW_UNIT_CHECK(hThree.isValid() == true);
    PW_UNIT_CHECK(hCopyStr.isValid() == false);
    PW_UNIT_CHECK(hFour.isValid() == true);
    
                
    INFO_MSG("Unit test", "...done. Test successful.")
    return EXIT_SUCCESS;
}
