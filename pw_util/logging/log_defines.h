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
/// \file       log_defines.h
/// \brief      Manages some macros for logging class "CLog"
///ain
/// \date       2009-06-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
/// \todo All macros wrapping logSeperator need LogSeverity-parameter
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_DEFINES_H
#define LOG_DEFINES_H

#include "conf_log.h"

// Test for version of log_conf.h
#ifndef LOGGING_VERSION
    #error LOGGING_VERSION not defined in conf_log.h
#elif LOGGING_VERSION!=2
    #error Wrong version of conf_log.h major
#endif

////////////////////////////////////////////////////////////////////////////////
///
/// \def DEBUG_MSG(a,b,c)
///         Macro wrapping debug output of logging class. One may use streams
///         for message b, for example:
///         DEBUG_MSG(Log, "Source", "Message" << "using" << "Streams", LOG_DOMAIN_NONE);
/// \def INFO_MSG(a,b,c)
///         Macro wrapping info output of logging class
/// \def NOTICE_MSG(a,b,c)
///         Macro wrapping notice output of logging class
/// \def WARNING_MSG(a,b,c)
///         Macro wrapping warning output of logging class
/// \def ERROR_MSG(a,b,c)
///         Macro wrapping error output of logging class
/// \def DEBUG_BLK(a)
///         Macro wrapping arbitary debug code
/// \def INFO_BLK(a)
///         Macro wrapping arbitary info code
/// \def NOTICE_BLK(a)
///         Macro wrapping arbitary notice code
/// \def WARNING_BLK(a)
///         Macro wrapping arbitary warning code
/// \def ERROR_BLK(a)
///         Macro wrapping arbitary error code
/// \def CTOR_CALL(a)
///         Macro simplifying log of domain: constructor call
/// \def DTOR_CALL(a)
///         Macro simplifying log of domain: destructor call
/// \def METHOD_ENTRY(a)
///         Macro simplifying log of domain: method entry
/// \def METHOD_EXIT(a)
///         Macro simplifying log of domain: method exit
/// \def MEM_ALLOC(a)
///         Macro simplifying log of domain: memory allocated
/// \def MEM_FREED(a)
///         Macro simplifying log of domain: memory freed
///
/// \def DOMAIN_MEMORY
///         Special define flag, indicating that "memory alloc" and "mem freed"
///         domains are both active.
/// \def DOMAIN_METHOD_HIERARCHY
///         Special define flag, indicating that "method entry" and "method exit"
///         domains are both active.
///
////////////////////////////////////////////////////////////////////////////////
#ifdef DOMAIN_NONE
    #define DOM_NONE(a)         {CLog::s_Dom = LOG_DOMAIN_NONE; a}
#else
    #define DOM_NONE(a)
#endif
#ifdef DOMAIN_METHOD_ENTRY
    #define DOM_MENT(a)         {CLog::s_Dom = LOG_DOMAIN_METHOD_ENTRY; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_MENT(a)
#endif
#ifdef DOMAIN_METHOD_EXIT
    #define DOM_MEXT(a)         {CLog::s_Dom = LOG_DOMAIN_METHOD_EXIT; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_MEXT(a)
#endif
#ifdef DOMAIN_CONSTRUCTOR
    #define DOM_CTOR(a)         {CLog::s_Dom = LOG_DOMAIN_CONSTRUCTOR; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_CTOR(a)
#endif
#ifdef DOMAIN_DESTRUCTOR
    #define DOM_DTOR(a)         {CLog::s_Dom = LOG_DOMAIN_DESTRUCTOR; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_DTOR(a)
#endif
#ifdef DOMAIN_MEMORY_ALLOCATED
    #define DOM_MEMA(a)         {CLog::s_Dom = LOG_DOMAIN_MEMORY_ALLOCATED; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_MEMA(a)
#endif
#ifdef DOMAIN_MEMORY_FREED
    #define DOM_MEMF(a)         {CLog::s_Dom = LOG_DOMAIN_MEMORY_FREED; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_MEMF(a)
#endif
#ifdef DOMAIN_DEV_LOGIC
    #define DOM_DEV(a)          {CLog::s_Dom = LOG_DOMAIN_DEV_LOGIC; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_DEV(a)
#endif
#ifdef DOMAIN_STATS
    #define DOM_STATS(a)        {CLog::s_Dom = LOG_DOMAIN_STATS; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_STATS(a)
#endif
#ifdef DOMAIN_VAR
    #define DOM_VAR(a)          {CLog::s_Dom = LOG_DOMAIN_VAR; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_VAR(a)
#endif
#ifdef DOMAIN_FILEIO
    #define DOM_FIO(a)          {CLog::s_Dom = LOG_DOMAIN_FILEIO; a CLog::s_Dom = LOG_DOMAIN_NONE;}
#else
    #define DOM_FIO(a)
#endif

#ifdef LOGLEVEL_DEBUG
    #define DEBUG_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_DEBUG, CLog::s_Dom); \
                                    }
    #define DEBUG_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define INFO_MSG(a,b)           {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_INFO, CLog::s_Dom); \
                                    }
    #define INFO_BLK(a)             {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define NOTICE_MSG(a,b)         {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_NOTICE, CLog::s_Dom); \
                                    }
    #define NOTICE_BLK(a)           {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define WARNING_MSG(a,b)        {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_WARNING, CLog::s_Dom); \
                                    }
    #define WARNING_BLK(a)          {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define ERROR_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_ERROR, CLog::s_Dom); \
                                    }
    #define ERROR_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define CTOR_CALL(a)            DOM_CTOR( \
                                    std::ostringstream oss(""); \
                                    oss << a; \
                                    Log.log("Constructor called", oss.str(), LOG_LEVEL_DEBUG, LOG_DOMAIN_CONSTRUCTOR);)
    #define DTOR_CALL(a)            DOM_DTOR( \
                                    std::ostringstream oss(""); \
                                    oss << a; \
                                    Log.log("Destructor called", oss.str(), LOG_LEVEL_DEBUG, LOG_DOMAIN_DESTRUCTOR);)
    #define METHOD_ENTRY(a)         DOM_MENT(CLogMethodHelper ___LOGGING_ENTRY_EXIT_(a);)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)            DOM_MEMA( \
                                    std::ostringstream oss(""); \
                                    oss << a; \
                                    Log.log("Memory allocated", oss.str(), LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_ALLOCATED);)
    #define MEM_FREED(a)            DOM_MEMF( \
                                    std::ostringstream oss(""); \
                                    oss << a; \
                                    Log.log("Memory freed", oss.str(), LOG_LEVEL_DEBUG, LOG_DOMAIN_MEMORY_FREED);)
    #define LOGIC_CHECK(a)          DOM_DEV(a)
#endif

#ifdef LOGLEVEL_INFO
    #define DEBUG_MSG(a,b)
    #define DEBUG_BLK(a)
    #define INFO_MSG(a,b)           {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_INFO, CLog::s_Dom); \
                                    }
    #define INFO_BLK(a)             {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define NOTICE_MSG(a,b)         {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_NOTICE, CLog::s_Dom); \
                                    }
    #define NOTICE_BLK(a)           {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define WARNING_MSG(a,b)        {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_WARNING, CLog::s_Dom); \
                                    }
    #define WARNING_BLK(a)          {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define ERROR_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_ERROR, CLog::s_Dom); \
                                    }
    #define ERROR_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
    #define LOGIC_CHECK(a)
#endif

#ifdef LOGLEVEL_NOTICE
    #define DEBUG_MSG(a,b)
    #define DEBUG_BLK(a)
    #define INFO_MSG(a,b)
    #define INFO_BLK(a)
    #define NOTICE_MSG(a,b)         {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_NOTICE, CLog::s_Dom); \
                                    }
    #define NOTICE_BLK(a)           {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define WARNING_MSG(a,b)        {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_WARNING, CLog::s_Dom); \
                                    }
    #define WARNING_BLK(a)          {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define ERROR_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_ERROR, CLog::s_Dom); \
                                    }
    #define ERROR_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
    #define LOGIC_CHECK(a)
#endif

#ifdef LOGLEVEL_WARNING
    #define DEBUG_MSG(a,b)
    #define DEBUG_BLK(a)
    #define INFO_MSG(a,b)
    #define INFO_BLK(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE_BLK(a)
    #define WARNING_MSG(a,b)        {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_WARNING, CLog::s_Dom); \
                                    }
    #define WARNING_BLK(a)          {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}
    #define ERROR_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_ERROR, CLog::s_Dom); \
                                    }
    #define ERROR_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
    #define LOGIC_CHECK(a)
#endif

#ifdef LOGLEVEL_ERROR
    #define DEBUG_MSG(a,b)
    #define DEBUG_BLK(a)
    #define INFO_MSG(a,b)
    #define INFO_BLK(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE_BLK(a)
    #define WARNING_MSG(a,b)
    #define WARNING_BLK(a)
    #define ERROR_MSG(a,b)          {\
                                    std::ostringstream oss(""); \
                                    oss << b; \
                                    Log.log(a, oss.str(), LOG_LEVEL_ERROR, CLog::s_Dom); \
                                    }
    #define ERROR_BLK(a)            {std::lock_guard<std::recursive_mutex> lock(Log.m_Mutex); a}

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
    #define LOGIC_CHECK(a)
#endif

#ifdef LOGLEVEL_NONE
    #define DEBUG_MSG(a,b)
    #define DEBUG_BLK(a)
    #define INFO_MSG(a,b)
    #define INFO_BLK(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE_BLK(a)
    #define WARNING_MSG(a,b)
    #define WARNING_BLK(a)
    #define ERROR_MSG(a,b)
    #define ERROR_BLK(a)

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
    #define LOGIC_CHECK(a)
#endif

// Additional macro for memory management
#ifdef DOMAIN_MEMORY_ALLOCATED
    #define DOMAIN_MEMORY
#endif
#ifndef DOMAIN_MEMORY_FREED
    #undef DOMAIN_MEMORY
#endif

// Additional macro for method hierarchy
#ifdef OUTPUT_INDENTION
    #define DOMAIN_METHOD_HIERARCHY
    #define INDENT() Log.indent();
    #define UNINDENT() Log.unindent();
#endif
#ifndef OUTPUT_INDENTION
    #undef DOMAIN_METHOD_HIERARCHY
    #define INDENT()
    #define UNINDENT()
    
#endif

#endif
