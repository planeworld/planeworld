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
/// \file       com_console.cpp
/// \brief      Implementation of class "CComConsole"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-10-28
///
////////////////////////////////////////////////////////////////////////////////

#include "com_console.h"

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CComConsole::CComConsole() : m_strRet(""),
                             m_strCurrent(""),
                             m_nICurrent(0)
{
    METHOD_ENTRY("CComConsole::CComConsole")
    DTOR_CALL("CComConsole::CComConsole")

    m_CommandBuffer.reserve(100);
    m_RetValBuffer.reserve(100);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Adds a command to command buffer.
///
/// \param _strCom Command to be added to command buffer.
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::addCommand(const std::string& _strCom)
{
    METHOD_ENTRY("CComConsole::addCommand")
    
    m_CommandBuffer.push_back(_strCom);
    m_RetValBuffer.push_back(m_strRet);
    m_strCurrent = "";
    m_nICurrent = m_CommandBuffer.size()-1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Executes the current command, using the com interface
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::execute()
{
    METHOD_ENTRY("CComConsole::execute")
    
    try
    {
        m_strRet = m_pComInterface->call(m_strCurrent);
    }
    catch (CComInterfaceException ComIntEx)
    {
        m_strRet = ComIntEx.getMessage();
    }
    this->addCommand(m_strCurrent);
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Cycle through command buffer, get next command
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::nextCommand()
{
    METHOD_ENTRY("CComConsole::nextCommand")
    
    if (m_CommandBuffer.size() > 0)
    {
        m_strCurrent = m_CommandBuffer[m_nICurrent];
        if (++m_nICurrent == static_cast<int>(m_CommandBuffer.size())) m_nICurrent = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Cycle through command buffer, get previous command
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::prevCommand()
{
    METHOD_ENTRY("CComConsole::prevCommand")
    
    if (m_CommandBuffer.size() > 0)
    {
        m_strCurrent = m_CommandBuffer[m_nICurrent];
        if (--m_nICurrent < 0) m_nICurrent = m_CommandBuffer.size()-1;
    }
}
