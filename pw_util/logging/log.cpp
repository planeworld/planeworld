////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2016 Torsten Büschenfeld
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
/// \file       log.cpp
/// \brief      Implementation of class "CLog"
///
/// \date       2009-06-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#include "log.h"

#ifdef __linux__
	#include <sys/ioctl.h>
#endif

std::ostringstream  CLog::s_strStr; ///< Used for streaming functionality in macros
LogDomainType       CLog::s_Dom = LOG_DOMAIN_NONE;    ///< Used for domain handling in macros
CLog& Log=CLog::getInstance();

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Destructor, closes logfile
///
/// \bug Wrong memory information if loglevel is changed dynamically
///
///////////////////////////////////////////////////////////////////////////////
CLog::~CLog()
{
    METHOD_ENTRY("CLog::~CLog");
    DTOR_CALL("CLog::~CLog");
    
    #ifdef DOMAIN_MEMORY
        // The memory domain is given by the enclosing macro
        if (m_nMemCounter > 0)
        {
            NOTICE_MSG ("Logging", "The next message results from debug information. A lower loglevel won't display it.")
            WARNING_MSG("Logging", "There may be memory leaks, please check: " << m_nMemCounter)
            
                std::cout << "\n";
                std::map<std::string,int>::const_iterator ci = m_MemCounterMap.begin();
                while (ci != m_MemCounterMap.end())
                {
                    if (ci->second != 0) std::cout << m_strColWarning;
                    std::cout << "    " << (*ci).first << ": " << (*ci).second << m_strColDefault << std::endl;
                    ++ci;
                }
        }
        if (m_nMemCounter < 0)
        {
            NOTICE_MSG ("Logging", "The next message results from debug information. A lower loglevel won't display it.")
            WARNING_MSG("Logging", "Maybe more memory freed (" << -m_nMemCounter << " frees) than allocated, please check.")
            
                std::cout << "\n";
                std::map<std::string,int>::const_iterator ci = m_MemCounterMap.begin();
                while (ci != m_MemCounterMap.end())
                {
                    if (ci->second != 0) std::cout << m_strColWarning;
                    std::cout << "    " << (*ci).first << ": " << (*ci).second << m_strColDefault << std::endl;
                    ++ci;
                }
        }
    #endif
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns instance of meyers-singleton
///
/// This method returns a reference to logging instance
///
///////////////////////////////////////////////////////////////////////////////
CLog& CLog::getInstance()
{
    static CLog Instance;
    
    // !!! Do not log the getInstance method, since there is no reference yet !!!
    //  METHOD_ENTRY("CLog::getInstance");
    
    return (Instance);
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Converts a string to LogColourSchemeType
///
/// \param _strScheme Colour scheme string to convert
///
/// \return Colour scheme
///
///////////////////////////////////////////////////////////////////////////////
LogColourSchemeType CLog::stringToColourScheme(const std::string& _strScheme) const
{
    METHOD_ENTRY("CLog::stringToColourScheme");
    
    if (_strScheme == "default")
    {
        return LOG_COLOUR_SCHEME_DEFAULT;
    }
    else if (_strScheme == "mono_on_white")
    {
        return LOG_COLOUR_SCHEME_MONOONWHITE;
    }
    else if (_strScheme == "mono_on_black")
    {
        return LOG_COLOUR_SCHEME_MONOONBLACK;
    }
    else if (_strScheme == "on_black")
    {
        return LOG_COLOUR_SCHEME_ONBLACK;
    }
    else if (_strScheme == "on_white")
    {
        return LOG_COLOUR_SCHEME_ONWHITE;
    }
    else
    {
        return LOG_COLOUR_SCHEME_MONOONWHITE;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief logs messages depending on state and loglevel
///
/// This method logs messages depending on their state an global loglevel
///
/// \param _strSrc Message source
/// \param _strMessage Message
/// \param _Level State of message
/// \param _Domain Domain the message should be associated with
///
/// \bug Domains appear in logfile, even if loglevel is below DEBUG
/// \todo Added domain to output.
///
///////////////////////////////////////////////////////////////////////////////
void CLog::log( const std::string& _strSrc, const std::string& _strMessage,
                const LogLevelType& _Level, const LogDomainType& _Domain)
{
    // !!! Do not log the logging method, this action will never stop !!!
    // METHOD_ENTRY("CLog::log");

    if (!m_bLock)
    {
        m_Mutex.lock();

        std::string strDomFlag;
    
        // Messages to be displayed
        if (((_Level <= m_LogLevel) && (m_abDomain[_Domain] == true)) ||
             (_Level == LOG_LEVEL_ERROR))
        {
            #ifdef DOMAIN_MEMORY
                if (_Domain == LOG_DOMAIN_MEMORY_ALLOCATED)
                {
                    ++m_nMemCounter;
                    if (m_MemCounterMap.find(_strMessage) != m_MemCounterMap.end())
                        m_MemCounterMap[_strMessage] += 1;
                    else
                        m_MemCounterMap[_strMessage] =  1;
                }
                if (_Domain == LOG_DOMAIN_MEMORY_FREED)
                {
                    --m_nMemCounter;
                    if (m_MemCounterMap.find(_strMessage) != m_MemCounterMap.end())
                        m_MemCounterMap[_strMessage] -= 1;
                    else
                        m_MemCounterMap[_strMessage] = -1;
                }
            #endif
            #ifdef DOMAIN_METHOD_HIERARCHY
                if (_Domain == LOG_DOMAIN_METHOD_EXIT)
                {
                    this->unindent();
                }
            #endif
            if ((m_strMsgBufSrc == _strSrc) && (m_strMsgBufMsg == _strMessage) &&
                (m_MsgBufLevel == _Level) && (m_MsgBufDom == _Domain))
            {
                ++m_nMsgCounter;
            }
            else
            {
                if (m_nMsgCounter != 1)
                {
                    std::cout << m_strColRepetition << "--- Last message repeated " << m_nMsgCounter << " times ---" << m_strColDefault << std::endl;
                    
                    m_nMsgCounter = 1u;
                }
    
                // Split up string if to long, carriage return
                std::string strMessage = _strMessage;
                std::string strTmp = _strMessage;
                unsigned short unLengthMax = m_unColsMax;
                
                #ifdef DOMAIN_METHOD_HIERARCHY 
                    unsigned short unIndent = _strSrc.size() + 26 + m_nHierLevel*2;
                #else
                    unsigned short unIndent = _strSrc.size() + 26;
                #endif
                std::string strIndent(unIndent, ' ');
    
                if ((unLengthMax - unIndent) < 1) unLengthMax=unIndent+1;
    
                // If newline is found, output seems formatted -> newline
                if (strMessage.find('\n',0) != std::string::npos)
                {
    //              std::string strSeperation(unLengthMax, '-');
    //              strMessage = "\n"+strSeperation+"\n"+strMessage+"\n"+strSeperation;
                    strMessage = "\n"+strMessage;
                    unIndent = 0;
                }
                // Otherwise use programmer defined break
                else if (strMessage.size() + unIndent  > unLengthMax)
                {
                    strMessage = strTmp.substr(0,unLengthMax-unIndent)+'\n'+strIndent;
                    strTmp = strTmp.substr(unLengthMax-unIndent);
                    // Cut leading whitespaces
                    while (*(strTmp.begin()) == ' ')
                        strTmp.erase(strTmp.begin());
                    while (strTmp.size() > static_cast<unsigned int>(unLengthMax-unIndent))
                    {
                        strMessage += strTmp.substr(0,unLengthMax-unIndent)+'\n'+strIndent;
                        strTmp = strTmp.substr(unLengthMax-unIndent);
                        // Cut leading whitespaces
                        while (*(strTmp.begin()) == ' ')
                            strTmp.erase(strTmp.begin());
                    }
                    strMessage += strTmp;
                }
    
                switch(_Level)
                {
                    case LOG_LEVEL_NONE:
                        break;
                    case LOG_LEVEL_ERROR:
                        std::cout << m_strColError << std::left << std::setw(14) <<  "[error]";
                        std::cout << m_strColDom << std::left << std::setw(10) << "[" + convLogDom2Str(_Domain) + "]";
                        #ifdef DOMAIN_METHOD_HIERARCHY
                            for (int i=0; i<m_nHierLevel; ++i)
                                std::cerr << "  ";
                        #endif
                        std::cerr << m_strColSender << 
                        _strSrc << ": " << m_strColDefault << strMessage << std::endl;
                        break;
                    case LOG_LEVEL_WARNING:
                        std::cout << m_strColWarning << std::left << std::setw(14) <<  "[warning]";
                        std::cout << m_strColDom << std::left << std::setw(10) << "[" + convLogDom2Str(_Domain) + "]";
                        #ifdef DOMAIN_METHOD_HIERARCHY
                            for (int i=0; i<m_nHierLevel; ++i)
                                std::cout << "  ";
                        #endif
                        std::cout << m_strColSender << \
                        _strSrc << ": " << m_strColDefault << strMessage << std::endl;
                        break;
                    case LOG_LEVEL_NOTICE:
                        std::cout << m_strColNotice << std::left << std::setw(14) <<  "[notice]";
                        std::cout << m_strColDom << std::left << std::setw(10) << "[" + convLogDom2Str(_Domain) + "]";
                        #ifdef DOMAIN_METHOD_HIERARCHY
                            for (int i=0; i<m_nHierLevel; ++i)
                                std::cout << "  ";
                        #endif
                        std::cout << m_strColSender << \
                        _strSrc << ": " << m_strColDefault << strMessage << std::endl;
                        break;
                    case LOG_LEVEL_INFO:
                        std::cout << m_strColInfo << std::left << std::setw(14) <<  "[info]";
                        std::cout << m_strColDom << std::left << std::setw(10) << "[" + convLogDom2Str(_Domain) + "]";
                        #ifdef DOMAIN_METHOD_HIERARCHY
                            for (int i=0; i<m_nHierLevel; ++i)
                                std::cout << "  ";
                        #endif
                        std::cout << m_strColSender << \
                        _strSrc << ": " << m_strColDefault << strMessage << std::endl;
                        break;
                    case LOG_LEVEL_DEBUG:
                        std::cout << m_strColDebug << std::left << std::setw(14) <<  "[debug]";
                        std::cout << m_strColDom << std::left << std::setw(10) << "[" + convLogDom2Str(_Domain) + "]";
                        #ifdef DOMAIN_METHOD_HIERARCHY
                            for (int i=0; i<m_nHierLevel; ++i)
                                std::cout << m_strColDefault << "  ";
                        #endif
                        std::cout << m_strColSender << \
                        _strSrc << ": " << m_strColDefault << strMessage << std::endl;
                        break;
                }
            }
            #ifdef DOMAIN_METHOD_HIERARCHY
                if (_Domain == LOG_DOMAIN_METHOD_ENTRY)
                {
                    this->indent();
                }
            #endif
        }
        // Store the last message
        m_strMsgBufSrc = _strSrc;
        m_strMsgBufMsg = _strMessage;
        m_MsgBufLevel = _Level;
        m_MsgBufDom = _Domain;
        
        m_Mutex.unlock();
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Inserts a separator
///
/// This method inserts a separator mainly to separate common logging from
/// logging messages
///
///////////////////////////////////////////////////////////////////////////////
void CLog::logSeparator(LogLevelType _Level)
{
    // Method entry isn't really nice here
    // METHOD_ENTRY("CLog::logSeparator(LogSevType)");

    if (!m_bLock)
    {
        switch(_Level)
        {
            case LOG_LEVEL_NONE:
                break;
            case LOG_LEVEL_ERROR:
                std::cerr << m_strColError << "[error]    ----------" << \
                m_strColDefault << std::endl;
                break;
            case LOG_LEVEL_WARNING:
                #ifdef LOGLEVEL_ERROR
                #else
                    std::cout << m_strColWarning << "[warning]  ----------" << \
                    m_strColDefault << std::endl;
                #endif
                break;
            case LOG_LEVEL_NOTICE:
                #ifdef LOGLEVEL_ERROR
                #else
                #ifdef LOGLEVEL_WARNING
                #else
                    std::cout << m_strColNotice << "[notice]   ----------" << \
                    m_strColDefault << std::endl;
                #endif
                #endif
                break;
            case LOG_LEVEL_INFO:
                #ifdef LOGLEVEL_ERROR
                #else
                #ifdef LOGLEVEL_WARNING
                #else
                #ifdef LOGLEVEL_NOTICE
                #else
                    std::cout << m_strColInfo << "[info]     ----------" << \
                    m_strColDefault << std::endl;
                #endif
                #endif
                #endif
                break;
            case LOG_LEVEL_DEBUG:
                #ifdef LOGLEVEL_ERROR
                #else
                #ifdef LOGLEVEL_WARNING
                #else
                #ifdef LOGLEVEL_NOTICE
                #else
                #ifdef LOGLEVEL_INFO
                #else
                    std::cout << m_strColDebug << "[debug]    ----------" << \
                    m_strColDefault << std::endl;
                #endif
                #endif
                #endif
                #endif
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Selects, whether dynamic setting of loglevel/domain is done or not.
///
/// \param _bDynSet Dynamic log setting on/off
///
///////////////////////////////////////////////////////////////////////////////
void CLog::setDynSetting(const bool& _bDynSet)
{
    METHOD_ENTRY("CLog::setDynSetting");

    m_bDynSetting = _bDynSet;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the new loglevel
///
/// This method sets the new loglevel. All according messages will be displayed
/// from now on
///
/// \param _Loglevel Domain to be set
///
///////////////////////////////////////////////////////////////////////////////
void CLog::setLoglevel(const LogLevelType& _Loglevel)
{
    METHOD_ENTRY("CLog::setLoglevel");

    if (m_bDynSetting)
    {
        if (_Loglevel > m_LogLevelCompiled)
        {
            NOTICE_MSG("Logging", "Loglevel "+convLogLev2Str(_Loglevel)+
            " not compiled, using "+convLogLev2Str(m_LogLevelCompiled))
            m_LogLevel = m_LogLevelCompiled;
        }
        else
        {
            if (_Loglevel < m_LogLevel)
            {
                // Dynmically calling loglevel might used for loops to avoid
                // message flooding. Hence, this shouldn't be done here.
                // DEBUG_MSG("Logging", "Dynamically setting loglevel "+convLogLev2Str(_Loglevel))
                #ifdef DOMAIN_MEMORY
                if (m_LogLevel == LOG_LEVEL_DEBUG)
                    m_nMemCounter = 0;
                #endif
                m_LogLevel = _Loglevel;
            }
            else if (_Loglevel > m_LogLevel)
            {
                #ifdef DOMAIN_MEMORY
                if (_Loglevel == LOG_LEVEL_DEBUG)
                    m_nMemCounter = 0;
                #endif
                m_LogLevel = _Loglevel;
                
                // Dynmically calling loglevel might used for loops to avoid
                // message flooding. Hence, this shouldn't be done here.
                // DEBUG_MSG("Logging", "Dynamically setting loglevel "+convLogLev2Str(_Loglevel))
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the specified domain true
///
/// This method sets the value "true" for the given domain. The logging
/// information for this domain will be -- depending on loglevel -- considered
///
/// \param _Domain Domain to be set
///
///////////////////////////////////////////////////////////////////////////////
void CLog::setDomain(const LogDomainType& _Domain)
{
    METHOD_ENTRY("CLog::setDomain");

    if (m_bDynSetting)
    {
        m_abDomain[_Domain] = true;
        DEBUG_MSG("Logging", "Set domain "+convLogDom2Str(_Domain))
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Unsets the specified domain
///
/// This method sets the value "false" for the given domain. The logging
/// information for this domain won't be considered, indepent of loglevel
///
/// \param _Domain Domain to be unset
///
///////////////////////////////////////////////////////////////////////////////
void CLog::unsetDomain(const LogDomainType& _Domain)
{
    if (m_bDynSetting)
    {
        if (_Domain != LOG_DOMAIN_METHOD_ENTRY)
        {
            METHOD_ENTRY("CLog::unsetDomain(const LogDomainType&)");
        }
    
        m_abDomain[_Domain] = false;
        DEBUG_MSG("Logging", "Unset domain "+convLogDom2Str(_Domain))
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets scheme for colouring of output
///
/// \param _bColourScheme Defines how the output is colored.
///
///////////////////////////////////////////////////////////////////////////////
void CLog::setColourScheme(const LogColourSchemeType _ColourScheme)
{
    METHOD_ENTRY("CLog::setColourScheme");
    
    switch(_ColourScheme)
    {
        case LOG_COLOUR_SCHEME_DEFAULT:
        {
            m_strColDefault = "";
            m_strColSender = "";
            m_strColDebug = "";
            m_strColInfo = "";
            m_strColNotice = "";
            m_strColWarning = "";
            m_strColError = "";
            m_strColDom = "";
            m_strColRepetition = "";
            break;
        }
        case LOG_COLOUR_SCHEME_MONOONBLACK:
        {
            m_strColDefault = "\033[0;37m";
            m_strColSender = "";
            m_strColDebug = "";
            m_strColInfo = "";
            m_strColNotice = "";
            m_strColWarning = "";
            m_strColError = "";
            m_strColDom = "";
            m_strColRepetition = "";
            break;
        }
        case LOG_COLOUR_SCHEME_MONOONWHITE:
        {
            m_strColDefault = "\033[0;30m";
            m_strColSender = "";
            m_strColDebug = "";
            m_strColInfo = "";
            m_strColNotice = "";
            m_strColWarning = "";
            m_strColError = "";
            m_strColDom = "";
            m_strColRepetition = "";
            break;
        }
        case LOG_COLOUR_SCHEME_ONBLACK:
        {
            m_strColDefault = "\033[0;37m";
            m_strColSender = "\033[1;37m";
            m_strColDebug = "\033[1;32m";
            m_strColInfo = "\033[0;32m";
            m_strColNotice = "\033[1;33m";
            m_strColWarning = "\033[1;31m";
            m_strColError = "\033[0;31m";
            m_strColDom = "\033[0;36m";
            m_strColRepetition = "\033[1;34m";
            break;
        }
        case LOG_COLOUR_SCHEME_ONWHITE:
        {
            m_strColDefault = "\33[0;30m";
            m_strColSender = "\33[1;30m";
            m_strColDebug = "\033[1;32m";
            m_strColInfo = "\033[0;32m";
            m_strColNotice = "\033[1;33m";
            m_strColWarning = "\033[1;31m";
            m_strColError = "\033[0;31m";
            m_strColDom = "\033[0;36m";
            m_strColRepetition = "\033[1;34m";
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Shows a progress bar in the console
///
/// \param _strMsg Message for progress bar
/// \param _nI Current loop index
/// \param _nLoopSize Size of loop = maximum loop index + 1
/// \param _nBarSize Size of progress bar (ascii blocks)
///
///////////////////////////////////////////////////////////////////////////////
void CLog::progressBar(const std::string& _strMsg, const int& _nI,
                       const int& _nLoopSize, const int& _nBarSize)
{
//     METHOD_ENTRY("CLog::progressBar");

    if (m_bPBarFirstCall == true)
    {
        m_bPBarFirstCall = false;
        m_bPBarDone = false;
        m_Timer.stop();
        m_Timer.start();
        m_fPreviousIterationTime = m_Timer.getSplitTime();
        m_fEstimatedIterationTime = 0.0;
        INFO_MSG("Processing started", _strMsg)
    }

    if (_nI == _nLoopSize-1)
    {
        m_bPBarFirstCall = true;
    }
    
    if (!m_bPBarDone)
    {
        if (_nI != 0 && _nI % m_iProcessorCount == 0)
        {
            double fCurrentIterationTime = m_Timer.getSplitTime();
            double fIterationTime = (fCurrentIterationTime - m_fPreviousIterationTime) / m_iProcessorCount;
            
            if (_nI == m_iProcessorCount)
                m_fEstimatedIterationTime = fIterationTime;
            else
                m_fEstimatedIterationTime = m_fEstimationSmoothing * m_fEstimatedIterationTime + (1.0 - m_fEstimationSmoothing) * fIterationTime;
            
            m_fPreviousIterationTime = fCurrentIterationTime;
        }
        
        // Done with progress bar
        if (_nI==_nLoopSize-1)
        {
            #ifdef LOG_LOCKING_ON
                m_bLock = false;
            #endif
            std::cout << "\r";
            for (int i=0; i<m_unColsMax; ++i)
            {
                std::cout << " ";
            }
            std::cout << "\r" << m_strColDefault;
//             std::cout.flush();

            double fDuration = m_Timer.getSplitTime();
            
            std::string strUnit = "s              ";
            if (fDuration > 60.0)
            {
                strUnit = "min            ";
                fDuration /= 60.0;
                if (fDuration > 60.0)
                {
                    strUnit = "h               ";
                    fDuration /= 60.0;
                    if (fDuration > 24.0)
                    {
                        strUnit = "day(s)        ";
                        fDuration /= 24.0;
                    }
                }
            }

            INFO_MSG("Processing finished", "Duration: " << std::setprecision(2) <<
                    fDuration << strUnit << std::setprecision(5))
            m_bPBarDone = true;
        }
        else if (_nI % (_nLoopSize/_nBarSize+1) == 0)
        {
            std::cout << m_strColRepetition << "[progress] ";
            for (int i=0; i<(_nBarSize*_nI)/_nLoopSize; ++i)
            {
                std::cout << "#";
            }
            for (int i=0; i<_nBarSize-(_nBarSize*_nI)/_nLoopSize-1; ++i)
            {
                std::cout << "=";
            }
            

            #ifdef LOG_LOCKING_ON
                m_bLock = true;
            #endif

            //double fETE = (m_Timer.getSplitTime() / _nI * (_nLoopSize-_nI));
            
            double fETE = (_nLoopSize - _nI) * m_fEstimatedIterationTime;
            
            std::string strUnit = "s              ";
            if (fETE > 60.0)
            {
                strUnit = "min            ";
                fETE /= 60.0;
                if (fETE > 60.0)
                {
                    strUnit = "h               ";
                    fETE /= 60.0;
                    if (fETE > 24.0)
                    {
                        strUnit = "day(s)        ";
                        fETE /= 24.0;
                    }
                }
            }

            std::cout << " ETE: " << std::setprecision(2) << fETE << strUnit << "\r";
            std::cout << std::setprecision(5);
            std::cout.flush();
            
        }
    }

    std::cout << m_strColDefault;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Private constructor
///
///////////////////////////////////////////////////////////////////////////////
CLog::CLog():   m_bDynSetting(LOG_DYNSET_ON),
                m_bLock(false),
                m_bPBarFirstCall(true),
                m_bPBarDone(false),
                m_fEstimationSmoothing(0.75),
                m_iProcessorCount(std::thread::hardware_concurrency()),
                m_unColsMax(LOG_COLSMAX_DEFAULT),
                m_strColDefault(""),
                m_strColSender(""),
                m_strColDebug(""),
                m_strColInfo(""),
                m_strColNotice(""),
                m_strColWarning(""),
                m_strColError(""),
                m_strColDom(""),
                m_strColRepetition("")
{
    #ifdef DOMAIN_MEMORY
        m_nMemCounter = 0;
    #endif
    #ifdef DOMAIN_METHOD_HIERARCHY
        m_nHierLevel = 0;
    #endif

    #ifdef LOGLEVEL_DEBUG
        m_LogLevelCompiled=LOG_LEVEL_DEBUG;
        m_LogLevel=LOG_LEVEL_DEBUG;
    #endif
    #ifdef LOGLEVEL_INFO
        m_LogLevelCompiled=LOG_LEVEL_INFO;
        m_LogLevel=LOG_LEVEL_INFO;
    #endif
    #ifdef LOGLEVEL_NOTICE
        m_LogLevelCompiled=LOG_LEVEL_NOTICE;
        m_LogLevel=LOG_LEVEL_NOTICE;
    #endif
    #ifdef LOGLEVEL_WARNING
        m_LogLevelCompiled=LOG_LEVEL_WARNING;
        m_LogLevel=LOG_LEVEL_WARNING;
    #endif
    #ifdef LOGLEVEL_ERROR
        m_LogLevelCompiled=LOG_LEVEL_ERROR;
        m_LogLevel=LOG_LEVEL_ERROR;
    #endif
    #ifdef LOGLEVEL_NONE
        m_LogLevelCompiled=LOG_LEVEL_NONE;
        m_LogLevel=LOG_LEVEL_NONE;
    #endif


    #ifdef DOMAIN_NONE
        m_abDomain[LOG_DOMAIN_NONE] = true;
    #else
        m_abDomain[LOG_DOMAIN_NONE] = false;
    #endif
    #ifdef DOMAIN_METHOD_ENTRY
        m_abDomain[LOG_DOMAIN_METHOD_ENTRY] = true;
    #else
        m_abDomain[LOG_DOMAIN_METHOD_ENTRY] = false;
    #endif
    #ifdef DOMAIN_METHOD_EXIT
        m_abDomain[LOG_DOMAIN_METHOD_EXIT] = true;
    #else
        m_abDomain[LOG_DOMAIN_METHOD_EXIT] = false;
    #endif
    #ifdef DOMAIN_CONSTRUCTOR
        m_abDomain[LOG_DOMAIN_CONSTRUCTOR] = true;
    #else
        m_abDomain[LOG_DOMAIN_CONSTRUCTOR] = false;
    #endif
    #ifdef DOMAIN_DESTRUCTOR
        m_abDomain[LOG_DOMAIN_DESTRUCTOR] = true;
    #else
        m_abDomain[LOG_DOMAIN_DESTRUCTOR] = false;
    #endif
    #ifdef DOMAIN_MEMORY_ALLOCATED
        m_abDomain[LOG_DOMAIN_MEMORY_ALLOCATED] = true;
    #else
        m_abDomain[LOG_DOMAIN_MEMORY_ALLOCATED] = false;
    #endif
    #ifdef DOMAIN_MEMORY_FREED
        m_abDomain[LOG_DOMAIN_MEMORY_FREED] = true;
    #else
        m_abDomain[LOG_DOMAIN_MEMORY_FREED] = false;
    #endif
    #ifdef DOMAIN_STATS
        m_abDomain[LOG_DOMAIN_STATS] = true;
    #else
        m_abDomain[LOG_DOMAIN_STATS] = false;
    #endif
    #ifdef DOMAIN_VAR
        m_abDomain[LOG_DOMAIN_VAR] = true;
    #else
        m_abDomain[LOG_DOMAIN_VAR] = false;
    #endif
    #ifdef DOMAIN_FILEIO
        m_abDomain[LOG_DOMAIN_FILEIO] = true;
    #else
        m_abDomain[LOG_DOMAIN_FILEIO] = false;
    #endif
    
    // No previous message, Dom and Sev are not relvant
    m_strMsgBufSrc = "";
    m_strMsgBufMsg = "";
    m_nMsgCounter = 1u;

    // Entry appears late, because just now the Logging class is initialized.
    // The entry domain is needed for correct indenting when appropriate
    // domains an debuglevel is used.
//  METHOD_ENTRY("CLog::CLog");

//  CTOR_CALL("CLog::CLog");
    
	#ifdef __linux__
		// Request the terminal width from the system
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		m_unColsMax = w.ws_col;
	#else
		m_unColsMax = 80u;
	#endif
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Converts domain to according string.
///
/// \param _LogDomain Domain that should be converted
///
///////////////////////////////////////////////////////////////////////////////
std::string CLog::convLogDom2Str(const LogDomainType& _LogDomain)
{
    // METHOD_ENTRY("CLog::convLogDom2Str");

    std::string strOut;

    switch(_LogDomain)
    {
        case LOG_DOMAIN_NONE:
            strOut  = "";
            break;
        case LOG_DOMAIN_METHOD_ENTRY:
            strOut  = "calls";
            break;
        case LOG_DOMAIN_METHOD_EXIT:
            strOut  = "calls";
            break;
        case LOG_DOMAIN_CONSTRUCTOR:
            strOut  = "obj";
            break;
        case LOG_DOMAIN_DESTRUCTOR:
            strOut  = "obj";
            break;
        case LOG_DOMAIN_MEMORY_ALLOCATED:
            strOut  = "mem";
            break;
        case LOG_DOMAIN_MEMORY_FREED:
            strOut  = "mem";
            break;
        case LOG_DOMAIN_STATS:
            strOut  = "stats";
            break;
        case LOG_DOMAIN_VAR:
            strOut  = "var";
            break;
        case LOG_DOMAIN_FILEIO:
            strOut  = "file_io";
            break;
    }

    return strOut;
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Converts loglevel to according string.
///
/// \param _Loglevel Loglevel that should be converted
///
///////////////////////////////////////////////////////////////////////////////
std::string CLog::convLogLev2Str(const LogLevelType& _Loglevel)
{
    METHOD_ENTRY("CLog::convLogLev2Str");

    std::string strOut;

    switch(_Loglevel)
    {
        case LOG_LEVEL_NONE:
            strOut  = "LOG_LEVEL_NONE";
            break;
        case LOG_LEVEL_ERROR:
            strOut  = "LOG_LEVEL_ERROR";
            break;
        case LOG_LEVEL_WARNING:
            strOut  = "LOG_LEVEL_WARNING";
            break;
        case LOG_LEVEL_NOTICE:
            strOut  = "LOG_LEVEL_NOTICE";
            break;
        case LOG_LEVEL_INFO:
            strOut  = "LOG_LEVEL_INFO";
            break;
        case LOG_LEVEL_DEBUG:
            strOut  = "LOG_LEVEL_DEBUG";
            break;
    }

    return strOut;
}
