////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
/// \mainpage   Project Planeworld -- Evaluation Tests
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-03-07
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
/// \file       pw_eval_multithreading.cpp
/// \brief      Main program for evaluation test
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-03-07
///
////////////////////////////////////////////////////////////////////////////////

//--- Standard header --------------------------------------------------------//
#include <atomic>
#include <chrono>
#include <thread>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc-Header ------------------------------------------------------------//

volatile int     g_nTest;
volatile double  g_fTest;
std::atomic_int  g_nTestAtomic;

std::atomic_bool g_bClean;
std::atomic_bool g_bExit;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Increments global variable
///
///////////////////////////////////////////////////////////////////////////////
void inc()
{
    METHOD_ENTRY("inc")
    while (!g_bExit)
    {
        g_nTest++;
        g_fTest += 0.00072387350902;
        if (g_nTest == 1000000000) g_bExit = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Increments global variable, atomic access
///
///////////////////////////////////////////////////////////////////////////////
void incAtomic()
{
    METHOD_ENTRY("incAtomic")
    while (!g_bExit)
    {
        g_nTestAtomic++;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Read global variable
///
///////////////////////////////////////////////////////////////////////////////
void read()
{
    METHOD_ENTRY("read")
    
    int    nTestLast = 0;
    double fTestLast = 0.0;
    
    while (!g_bExit)
    {
        int    nTest = g_nTest;
        double fTest = g_fTest;
        if (nTest < nTestLast || fTest < fTestLast) g_bClean = false;
        //         std::cout << "Raw:    " << g_nTest << " " << g_fTest << std::endl;
        //         std::this_thread::sleep_for(std::chrono::microseconds(10));
        nTestLast = nTest;
        fTestLast = fTest;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Read global variable, atomic access
///
///////////////////////////////////////////////////////////////////////////////
void readAtomic()
{
    METHOD_ENTRY("readAtomic")
    while (!g_bExit)
    {
        //         std::cout << "Atomic: " << g_nTestAtomic.load() << std::endl;
        //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Main function
///
/// This is the entrance point for program startup.
///
/// \return Exit code
///
///////////////////////////////////////////////////////////////////////////////
int main()
{
    Log.setColourScheme(LOG_COLOUR_SCHEME_ONBLACK);
    g_bClean = true;
    g_bExit = false;
    g_nTest = 0;
    g_fTest = 0.0;
    g_nTestAtomic.store(0);
    
    std::thread IncrementThread(inc);
    std::thread IncrementAtomicThread(incAtomic);
    std::thread ReadThread(read);
    std::thread ReadAtomicThread(readAtomic);
    
    IncrementThread.join();
    IncrementAtomicThread.join();
    ReadThread.join();
    ReadAtomicThread.join();
    
    if (g_bClean)
    {
        INFO_MSG("Multithreading Evaluation", "Passed.")
        return EXIT_SUCCESS;
    }
    else
    {
        ERROR_MSG("Multithreading Evaluation", "Failed. Invalid values.")
        return EXIT_FAILURE;
    }
}
