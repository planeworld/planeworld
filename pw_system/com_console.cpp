////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2017 Torsten Büschenfeld
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
                             m_strDomain(""),
                             m_strFind(""),
                             m_bFirstFind(true),
                             m_nState(0),
                             m_nICurrent(0),
                             m_ConsoleMode(ConsoleModeType::COM)
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
    m_strFind = "";
    m_strFindLast = "";
    m_strPart = "";
    m_bFirstFind = true;
    m_nICurrent = m_CommandBuffer.size();
    m_nState=0;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Complements the currently given command (see tab completion)
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::complementCommand()
{
    METHOD_ENTRY("CComConsole::complementCommand")

    if (m_ConsoleMode == ConsoleModeType::LUA)
    {
        switch (m_nState)
        {
            case 0: 
            {
                m_strCurrent = "pw.";
                m_nState = 1;
                break;
            }
            case 1:
            {
                auto itDom = m_pComInterface->getDomains()->begin();
                auto i = 0u;
                //--------------------------------------------------------
                // Worst case search range is roughly twice the functions
                // container size, since the first find might be next to 
                // the end of the container. After that, the second search
                // has to cover the whole container again.
                //--------------------------------------------------------
                while (i++ < m_pComInterface->getDomains()->size()*2)
                {
                    if (!m_bFirstFind)
                    {
                        if (*itDom == m_strFindLast)
                        {
                            m_bFirstFind = true;
                        }
                    }
                    else
                    {
                        if (itDom->find(m_strFind) != std::string::npos)
                        {
                            if (m_strFindLast != "")
                                m_strCurrent.erase(m_strCurrent.end() - m_strFindLast.length(), m_strCurrent.end());
                            else
                                m_strCurrent.erase(m_strCurrent.end() - m_strFind.length(), m_strCurrent.end());
                            m_strPart = *itDom;
                            m_strFindLast = m_strPart;
                            m_strCurrent += m_strPart;
                            m_bFirstFind = false;
                            break;
                        }
                    }
                    if (++itDom == m_pComInterface->getDomains()->end())
                        itDom =  m_pComInterface->getDomains()->begin();
                }
                break;
            }
            case 2:
            {
                auto itCom = m_pComInterface->getFunctions()->begin();
                auto i = 0u;
                //-----------------------
                // Search range see above
                //-----------------------
                while (i++ < m_pComInterface->getFunctions()->size()*2)
                {
                    if (!m_bFirstFind)
                    {
                        if (itCom->first == m_strFindLast)
                        {
                            m_bFirstFind = true;
                        }
                    }
                    else
                    {
                        if ((itCom->first.find(m_strFind) != std::string::npos) && 
                            (m_strDomain == m_pComInterface->getDomainsByFunction()->at(itCom->first)))
                        {
                            if (m_strFindLast != "")
                                m_strCurrent.erase(m_strCurrent.end() - m_strFindLast.length(), m_strCurrent.end());
                            else
                                m_strCurrent.erase(m_strCurrent.end() - m_strFind.length(), m_strCurrent.end());
                            m_strPart = itCom->first;
                            m_strFindLast = m_strPart;
                            m_strCurrent += m_strPart;
                            m_bFirstFind = false;
                            break;
                        }
                    }
                    if (++itCom == m_pComInterface->getFunctions()->end())
                        itCom =  m_pComInterface->getFunctions()->begin();
                }
                break;
            }
        }
    }
    else
    {
        auto itCom = m_pComInterface->getFunctions()->begin();
        auto i = 0u;
        //-----------------------
        // Search range see above
        //-----------------------
        while (i++ < m_pComInterface->getFunctions()->size()*2)
        {
            if (m_strFindLast != "")
            {
                if (itCom->first == m_strFindLast)
                {
                    m_strFindLast = "";
                }
            }
            else
            {
                if (itCom->first.find(m_strFind) != std::string::npos)
                {
                    m_strCurrent = itCom->first;
                    m_strFindLast = m_strCurrent;
                    break;
                }
            }
            if (++itCom == m_pComInterface->getFunctions()->end())
                  itCom =  m_pComInterface->getFunctions()->begin();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Executes the current command, using the com interface
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::execute()
{
    METHOD_ENTRY("CComConsole::execute")
    
    if (m_ConsoleMode == ConsoleModeType::LUA)
    {
        m_pComInterface->call<void, std::string>("execute_lua", m_strCurrent);
    }
    else
    {
        try
        {
            m_strRet = m_pComInterface->call(m_strCurrent);
        }
        catch (CComInterfaceException ComIntEx)
        {
            m_strRet = ComIntEx.getMessage();
        }
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
        if (++m_nICurrent >= static_cast<int>(m_CommandBuffer.size())) m_nICurrent = 0;
        m_strCurrent = m_CommandBuffer[m_nICurrent];
        
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
        if (--m_nICurrent < 0) m_nICurrent = m_CommandBuffer.size()-1;
        m_strCurrent = m_CommandBuffer[m_nICurrent];
        
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Externally sets the currently active command
///
/// The currently active command might originate from sources suche like
/// keyboard input.
///
/// \param _strCurrent Currently active command string
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::setCurrentCommand(const std::string& _strCurrent)
{
    METHOD_ENTRY("CComConsole::setCurrentCommand")
    
    m_strCurrent = _strCurrent;
    auto Pos = _strCurrent.find(".");
    if (Pos != std::string::npos)
    {
        m_strFind = _strCurrent.substr(Pos+1);
        if (m_nState == 1) m_strDomain = m_strFind;
        m_nState = 1;
        Pos = m_strFind.find(".");
        if (Pos != std::string::npos)
        {
            if (m_strDomain.back() == '.') m_strDomain.pop_back();
            m_strFind = m_strFind.substr(Pos+1);
            m_nState = 2;
        }
    }
    else
    {
        m_strFind = _strCurrent;
        m_nState = 0;
    }
    m_strFindLast = "";
    m_strPart = "";
    m_bFirstFind = true;
}
