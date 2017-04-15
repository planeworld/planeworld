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
                             m_nICurrent(0),
                             m_State(ConsoleStateType::PACKAGE_COMPLETION),
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
    m_State = ConsoleStateType::PACKAGE_COMPLETION;
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
        switch (m_State)
        {
            case ConsoleStateType::PACKAGE_COMPLETION: 
            {
                std::string strPW = "pw";
                if (strPW.find(m_strFind) != std::string::npos)
                {
                    m_strCurrent = "pw.";
                    m_State = ConsoleStateType::DOMAIN_COMPLETION;
                    m_strFind = "";
                }   
                break;
            }
            case ConsoleStateType::DOMAIN_COMPLETION:
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
                            m_strDomain = m_strPart;
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
            case ConsoleStateType::FUNCTION_COMPLETION:
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
            default:
                break;
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
/// \brief Expands the current command by given string.
///
/// This method expands the internal command by the given string sequence. This
/// is needed for external input which isn't aware of or shouldn't keep track
/// of the command string.
///
/// \param _strS Expansion von the current command
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::expandCommand(const std::string& _strS)
{
    METHOD_ENTRY("CComConsole::expandCommand")
    
    m_strCurrent += _strS;
    
    // Find last "." indicating a state change
    auto PosPkg = m_strCurrent.find_last_of('.');
    
    if (PosPkg != std::string::npos)
    {
        // Currently entered string too short to fit pw?
        if (PosPkg < 2)
        {
            m_State = ConsoleStateType::NO_COMPLETION;
        }
        else
        {
            // Was the domain already entered? Further search for pw
            if (m_strCurrent.substr(PosPkg-2, 2) != "pw")
            {
                auto PosDom = m_strCurrent.substr(0, PosPkg).find_last_of('.');
                if (PosDom != std::string::npos)
                {
                    if (m_strCurrent.substr(PosDom-2, 2) == "pw")
                    {
                        m_State = ConsoleStateType::FUNCTION_COMPLETION;
                        m_strFind = m_strCurrent.substr(PosPkg+1);
                    }
                }
            }
            else
            {
                m_State = ConsoleStateType::DOMAIN_COMPLETION;
                m_strFind = m_strCurrent.substr(PosPkg+1);
                m_strDomain = m_strFind;
            }
        }
    }
    else
    {
        m_strFind = m_strCurrent;
        m_State = ConsoleStateType::PACKAGE_COMPLETION;
    }
    m_strFindLast = "";
    m_strPart = "";
    m_bFirstFind = true;
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
/// \brief Remove last character of command string
///
////////////////////////////////////////////////////////////////////////////////
void CComConsole::backspace()
{
    METHOD_ENTRY("CComConsole::backspace")
    
    if (!m_strCurrent.empty())
    {
        m_strCurrent.pop_back();
    }
}
