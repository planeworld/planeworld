////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2009-2017 Torsten Büschenfeld
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
/// \file       log.h
/// \brief      Prototype of class "CLog"
///
/// \date       2009-06-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_H
#define LOG_H

//--- Program header ---------------------------------------------------------//
#include "log_defines.h"
#include "log_listener.h"

//--- Standard header --------------------------------------------------------//
#include <atomic>
#include <iostream>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>

//--- Misc header ------------------------------------------------------------//
#include "timer.h"

const unsigned short LOG_NOD = 11u;             ///< Number of Domains
const unsigned short LOG_COLSMAX_DEFAULT = 80u; ///< Default number for maximum columns
const bool LOG_COLOR = true;                    ///< Color logging
const bool LOG_NO_COLOR = false;                ///< Monochrom logging
const bool LOG_DYNSET_ON = true;                ///< Dynamic changes of loglevel/domain allowed
const bool LOG_DYNSET_OFF = false;              ///< Dynamic changes of loglevel/domain not allowed

/// Map of Log listeners (callbacks, observers)
typedef std::map<std::string, ILogListener*> LogListenersType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Logging class
///
/// This class receives messages from methods. Because of global use it is
/// implemented as a Meyers-Singleton. Even though the global instance is
/// automatically created within this file, a singleton/class form was chosen.
/// Hence, the class may be easily changed to use differently named or local
/// instances.
///
/// \todo Greater buffer for looped logentries.
///
////////////////////////////////////////////////////////////////////////////////
class CLog
{
    
    public:
        
        //--- Static variables -----------------------------------------------//
        static std::atomic<LogDomainType> s_Dom;

        //--- Destructor -----------------------------------------------------//
        ~CLog();

        //--- Static methods -------------------------------------------------//
        static CLog& getInstance();
        
        //--- Constant methods -----------------------------------------------//
        LogColourSchemeType stringToColourScheme(const std::string&) const;

        //--- Methods --------------------------------------------------------//
        void addListener(const std::string& _strListener, ILogListener* const _pListener);
        bool removeListener(const std::string& _strListener);
        
        void indent();
        void unindent();
        void log(const std::string&, const std::string&, const LogLevelType&,
                 const LogDomainType& = LOG_DOMAIN_NONE, const bool = false);
        void logSeparator(LogLevelType = LOG_LEVEL_INFO);
        void setBreak(const unsigned short&);
        void setDynSetting(const bool&);
        void setLoglevel(const LogLevelType&);
        void setDomain(const LogDomainType&);
        void unsetDomain(const LogDomainType&);
        void setColourScheme(const LogColourSchemeType);
        void progressBar(const std::string&, const int&, const int&, const int& _nBarSize=60);
        
        //--- Variables ------------------------------------------------------//
        std::recursive_mutex    m_Mutex;        ///< Mutex to lock writing to console
        
    private:
    
        //--- Variables ------------------------------------------------------//
        LogLevelType    m_LogLevel;             ///< The loglevel
        LogLevelType    m_LogLevelCompiled;     ///< Info about the loglevel given by macros
                
        bool            m_abDomain[LOG_NOD];    ///< Special flags indicating if domain should be logged
        bool            m_bDynSetting;
        bool            m_bLock;                ///< Locks output for progress bar
        bool            m_bPBarFirstCall;       ///< Progress bar starting
        bool            m_bPBarDone;            ///< Progress done
        CTimer          m_Timer;                ///< Timer for progress bar (ETA)
        double          m_fPreviousIterationTime;   ///< Stores the timer value from the last iteration
        double          m_fEstimationSmoothing;     ///< Strength of the smoothing for the estimated speed
        double          m_fEstimatedIterationTime;  ///< Estimated time for one iteration for the progress bar
        int             m_iProcessorCount;          ///< The number of available cpu cores
        
        #ifdef DOMAIN_MEMORY
            int                        m_nMemCounter;       ///< Counts memory (de)allocations
            std::map<std::string, int> m_MemCounterMap;     ///< Counts memony (de)allocations per class
        #endif
        #ifdef DOMAIN_METHOD_HIERARCHY
            int             m_nHierLevel;       ///< Level in method hierarchy
        #endif

        std::string     m_strMsgBufSrc;         ///< Message buffer for source
        std::string     m_strMsgBufMsg;         ///< Message buffer for message
        LogLevelType    m_MsgBufLevel;          ///< Message buffer for loglevel
        LogDomainType   m_MsgBufDom;            ///< Message buffer for domain
        unsigned int    m_nMsgCounter;          ///< Counts number of equal messages
        unsigned short  m_unColsMax;            ///< Maximum number of columns

        std::string     m_strColDefault;        ///< Color for normal text
        std::string     m_strColSender;         ///< Color for sender
        std::string     m_strColDebug;          ///< Color for debug text
        std::string     m_strColInfo;           ///< Color for info text
        std::string     m_strColNotice;         ///< Color for notice text
        std::string     m_strColWarning;        ///< Color for warning text
        std::string     m_strColError;          ///< Color for error text
        std::string     m_strColDom;            ///< Color for domain text
        std::string     m_strColRepetition;     ///< Color for log repetitions
        
        LogListenersType    m_LogListeners;     ///< List of listeners informed about log entries

        //--- Constructors ---------------------------------------------------//
        CLog();                                 ///< Empty constructor
        CLog(const CLog&);                      ///< Empty copy-constructor
        
        //--- Operators ------------------------------------------------------//
        CLog& operator=(const CLog&);           ///< Empty operator=
};

//--- Enum parser ------------------------------------------------------------//

static std::unordered_map<LogDomainType, std::string> s_LogDomainTypeToStringMap = {
    {LOG_DOMAIN_NONE, ""},
    {LOG_DOMAIN_METHOD_ENTRY, "calls"},
    {LOG_DOMAIN_METHOD_EXIT, "calls"},
    {LOG_DOMAIN_CONSTRUCTOR, "obj"},
    {LOG_DOMAIN_DESTRUCTOR, "obj"},
    {LOG_DOMAIN_MEMORY_ALLOCATED, "mem"},
    {LOG_DOMAIN_MEMORY_FREED, "mem"},
    {LOG_DOMAIN_DEV_LOGIC, "dev"},
    {LOG_DOMAIN_STATS, "stats"},
    {LOG_DOMAIN_VAR, "var"},
    {LOG_DOMAIN_FILEIO, "file_io"}
}; ///< Map from LogDomainType to string

static std::unordered_map<LogLevelType, std::string> s_LogLevelTypeToStringMap = {
    {LOG_LEVEL_NONE, "LOG_LEVEL_NONE"},
    {LOG_LEVEL_ERROR, "LOG_LEVEL_ERROR"},
    {LOG_LEVEL_WARNING, "LOG_LEVEL_WARNING"},
    {LOG_LEVEL_NOTICE, "LOG_LEVEL_NOTICE"},
    {LOG_LEVEL_INFO, "LOG_LEVEL_INFO"},
    {LOG_LEVEL_DEBUG, "LOG_LEVEL_DEBUG"}
}; ///< Map from LogLevelType to string

extern CLog& Log; ///< Global logging instance

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Helper class for method entry/exit
///
/// This class is automatically used by METHOD_ENTRY and METHOD_EXIT.
/// Entry and Exit messages are automatically created when the constructor and
/// destructor are called. This guarantuees, that the Exit message is created
/// even in case of multiple exit points and unhandled exceptions.
///
////////////////////////////////////////////////////////////////////////////////
class CLogMethodHelper
{
    public:
        
      CLogMethodHelper(const std::string& _strMethodname, const bool _bNoListener = false)
      {
          m_strMethodname = _strMethodname;
          m_bNoListener = _bNoListener;
          Log.log("Method entry", m_strMethodname, LOG_LEVEL_DEBUG, LOG_DOMAIN_METHOD_ENTRY, m_bNoListener);
          CLog::s_Dom = LOG_DOMAIN_NONE;
      }
      
      ~CLogMethodHelper()
      {
          Log.log("Method exit", m_strMethodname, LOG_LEVEL_DEBUG, LOG_DOMAIN_METHOD_EXIT, m_bNoListener);
          CLog::s_Dom = LOG_DOMAIN_NONE;
      }
    
    private:
        
        //--- Variables [private] --------------------------------------------//
        std::string m_strMethodname; ///< Name of method that was entered
        bool        m_bNoListener;   ///< Call listeners of logging function?
};

//--- Implementation goes here for inline reasons ----------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Add log listener (callback, observer) to map of listeners
///
/// \param _strListener Name of listener to be added
/// \param _pListener Listener to be added
///
////////////////////////////////////////////////////////////////////////////////
inline void CLog::addListener(const std::string& _strListener, ILogListener* const _pListener)
{
    METHOD_ENTRY("CLog::addListener")
    m_LogListeners.insert({_strListener,_pListener});
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Remove log listener (callback, observer) from map of listeners
///
/// \param _strListener Name of listener to be removed
///
/// \return Success?
///
////////////////////////////////////////////////////////////////////////////////
inline bool CLog::removeListener(const std::string& _strListener)
{
    METHOD_ENTRY("CLog::removeListener")
    
    DOM_DEV(
        auto nRes = m_LogListeners.erase(_strListener);
        if (nRes == 0)
        {
            ERROR_MSG("Log", "Listener <" << _strListener << "> unknown, cannot remove.")
            return false;
        }
        return true;
    )
    m_LogListeners.erase(_strListener);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Set number of columns
///
/// \param _unCols Number of columns
///
////////////////////////////////////////////////////////////////////////////////
inline void CLog::setBreak(const unsigned short& _unCols)
{
    METHOD_ENTRY("CLog::setBreak")

    if (_unCols < m_unColsMax)
    {
        DOM_VAR(DEBUG_MSG("Logging", "Max. number of columns: " << _unCols))
        m_unColsMax = _unCols;
    }
    else
    {
        m_unColsMax = _unCols;
        DOM_VAR(DEBUG_MSG("Logging", "Max. number of columns: " << _unCols))
    }
}

#ifdef DOMAIN_METHOD_HIERARCHY
////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Indent output
///
////////////////////////////////////////////////////////////////////////////////
inline void CLog::indent()
{
    // METHOD_ENTRY("CLog::indent")
    ++m_nHierLevel;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Unindent output
///
////////////////////////////////////////////////////////////////////////////////
inline void CLog::unindent()
{
    // METHOD_ENTRY("CLog::unindent")
    if (m_nHierLevel > 0)
        --m_nHierLevel;
    else
    {
        NOTICE_MSG("Logging", "Something went wrong with indention.")
    }
}
#endif

#endif
