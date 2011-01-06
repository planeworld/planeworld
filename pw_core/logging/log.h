/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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

//--- Standard header --------------------------------------------------------//
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

//--- Misc header ------------------------------------------------------------//
#include "timer.h"

/// Represents logging level
typedef enum 
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG 
} LogLevelType;

/// Represents logging domain
typedef enum 
{
    LOG_DOMAIN_NONE,
    LOG_DOMAIN_METHOD_ENTRY,
    LOG_DOMAIN_METHOD_EXIT,
    LOG_DOMAIN_CONSTRUCTOR,
    LOG_DOMAIN_DESTRUCTOR,
    LOG_DOMAIN_MEMORY_ALLOCATED,
    LOG_DOMAIN_MEMORY_FREED,
    LOG_DOMAIN_VAR,
    LOG_DOMAIN_FILEIO
} LogDomainType;

/// Represents logging level
typedef enum 
{
    LOG_COLOUR_SCHEME_MONOONBLACK,
    LOG_COLOUR_SCHEME_MONOONWHITE,
    LOG_COLOUR_SCHEME_ONBLACK,
    LOG_COLOUR_SCHEME_ONWHITE
} LogColourSchemeType;

const unsigned short LOG_NOD = 9u;              ///< Number of Domains
const unsigned short LOG_COLSMAX_DEFAULT = 80u; ///< Default number for maximum columns
const bool LOG_COLOR = true;                    ///< Color logging
const bool LOG_NO_COLOR = false;                ///< Monochrom logging
const bool LOG_DYNSET_ON = true;                ///< Dynamic changes of loglevel/domain allowed
const bool LOG_DYNSET_OFF = false;              ///< Dynamic changes of loglevel/domain not allowed

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
        static std::ostringstream   s_strStr;   ///< Used for streaming functionality in macros
        static LogDomainType        s_Dom;      ///< Used for domain handling in macros

        //--- Destructor -----------------------------------------------------//
        ~CLog();

        //--- Static methods -------------------------------------------------//
        static CLog& getInstance();
        
        //--- Constant methods -----------------------------------------------//
        const LogColourSchemeType stringToColourScheme(const std::string&) const;

        //--- Methods --------------------------------------------------------//
        void log(const std::string&, const std::string&, const LogLevelType&,
                const LogDomainType& = LOG_DOMAIN_NONE);
        void logSeparator(LogLevelType = LOG_LEVEL_INFO);
        void setBreak(const unsigned short&);
        void setDynSetting(const bool&);
        void setLoglevel(const LogLevelType&);
        void setDomain(const LogDomainType&);
        void unsetDomain(const LogDomainType&);
        void setColourScheme(const LogColourSchemeType);
        void progressBar(const int&, const int&, const int& _nBarSize=60);
        
    private:
    
        //--- Variables ------------------------------------------------------//
        LogLevelType    m_LogLevel;             ///< The loglevel
        LogLevelType    m_LogLevelCompiled;     ///< Info about the loglevel given by macros
                
        bool            m_abDomain[LOG_NOD];    ///< Special flags indicating if domain should be logged
        bool            m_bDynSetting;
        bool            m_bLock;                ///< Locks output for progress bar
        bool            m_bUnlock;              ///< Flags if p-bar already unlocked logging
        bool            m_bFirstCall;           ///< Progress bar starting
        CTimer          m_Timer;                ///< Timer for progress bar (ETA)
        
        #ifdef DOMAIN_MEMORY
            int             m_nMemCounter;      ///< Counts memory (de)allocations
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
        std::string     m_strColRepetition;     ///< Color for log repetitions


        //--- Constructors ---------------------------------------------------//
        CLog();                                 ///< Empty constructor
        CLog(const CLog&);                      ///< Empty copy-constructor
        
        //--- Operators ------------------------------------------------------//
        CLog& operator=(const CLog&);           ///< Empty operator=

        //--- Methods --------------------------------------------------------//
        std::string convLogDom2Str(const LogDomainType&);
        std::string convLogLev2Str(const LogLevelType&);

};

extern CLog& Log; ///< Global logging instance

// ////////////////////////////////////////////////////////////////////////////////
// ///
// /// \brief Base class for logging
// ///
// /// This class is the base class for all classes using logging. It just defines
// /// a constructor that initializes the meyers-singleton for the logging instance.
// /// 
// ////////////////////////////////////////////////////////////////////////////////
// class CLogBase
// {
//  public:
//      virtual ~CLogBase(){};
// 
//  protected:
//      //--- Protected constructor ------------------------------------------//
//      CLogBase():m_Log(CLog::getInstance())
//      {
//          CTOR_CALL(m_Log, "LogBase");
//      };
// 
//      //--- Protected variables --------------------------------------------//
//      CLog&   m_Log;                  ///< Instance of logging class
// };

//--- Implementation goes here for inline reasons ----------------------------//

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

    METHOD_EXIT("CLog::setBreak")
}

#endif
