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
/// \file       log_defines.h
/// \brief      Manages some macros for logging class "CLog"
///
/// \date       2009-06-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
/// \todo All macros wrapping logSeperator need LogSeverity-parameter
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_DEFINES_H
#define LOG_DEFINES_H

#include "conf_log.h"

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
/// \def DEBUG(a)
///         Macro wrapping arbitary debug code
/// \def INFO(a)
///         Macro wrapping arbitary info code
/// \def NOTICE(a)
///         Macro wrapping arbitary notice code
/// \def WARNING(a)
///         Macro wrapping arbitary warning code
/// \def ERROR(a)
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
    #define DEBUG_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_DEBUG, CLog::s_Dom);
    #define DEBUG(a)                a
    #define INFO_MSG(a,b)           CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_INFO, CLog::s_Dom);
    #define INFO(a)                 a
    #define NOTICE_MSG(a,b)         CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_NOTICE, CLog::s_Dom);
    #define NOTICE(a)               a
    #define WARNING_MSG(a,b)        CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_WARNING, CLog::s_Dom);
    #define WARNING(a)              a
    #define ERROR_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_ERROR, CLog::s_Dom);
    #define ERROR(a)                a
    #define CTOR_CALL(a)            DOM_CTOR(Log.log("Constructor called", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
    #define DTOR_CALL(a)            DOM_DTOR(Log.log("Destructor called", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
    #define METHOD_ENTRY(a)         DOM_MENT(Log.log("Method entry", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
    #define METHOD_EXIT(a)          DOM_MEXT(Log.log("Method exit", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
    #define MEM_ALLOC(a)            DOM_MEMA(Log.log("Memory allocated", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
    #define MEM_FREED(a)            DOM_MEMF(Log.log("Memory freed", a, LOG_LEVEL_DEBUG, CLog::s_Dom);)
#endif

#ifdef LOGLEVEL_INFO
    #define DEBUG_MSG(a,b)
    #define DEBUG(a)
    #define INFO_MSG(a,b)           CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_INFO, CLog::s_Dom);
    #define INFO(a)                 a
    #define NOTICE_MSG(a,b)         CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_NOTICE, CLog::s_Dom);
    #define NOTICE(a)               a
    #define WARNING_MSG(a,b)        CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_WARNING, CLog::s_Dom);
    #define WARNING(a)              a
    #define ERROR_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_ERROR, CLog::s_Dom);
    #define ERROR(a)                a

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
#endif

#ifdef LOGLEVEL_NOTICE
    #define DEBUG_MSG(a,b)
    #define DEBUG(a)
    #define INFO_MSG(a,b)
    #define INFO(a)
    #define NOTICE_MSG(a,b)         CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_NOTICE, CLog::s_Dom);
    #define NOTICE(a)               a
    #define WARNING_MSG(a,b)        CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_WARNING, CLog::s_Dom);
    #define WARNING(a)              a
    #define ERROR_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_ERROR, CLog::s_Dom);
    #define ERROR(a)                a

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
#endif

#ifdef LOGLEVEL_WARNING
    #define DEBUG_MSG(a,b)
    #define DEBUG(a)
    #define INFO_MSG(a,b)
    #define INFO(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE(a)
    #define WARNING_MSG(a,b)        CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_WARNING, CLog::s_Dom);
    #define WARNING(a)              a
    #define ERROR_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_ERROR, CLog::s_Dom);
    #define ERROR(a)                a

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
#endif

#ifdef LOGLEVEL_ERROR
    #define DEBUG_MSG(a,b)
    #define DEBUG(a)
    #define INFO_MSG(a,b)
    #define INFO(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE(a)
    #define WARNING_MSG(a,b)
    #define WARNING(a)
    #define ERROR_MSG(a,b)          CLog::s_strStr.str(""); \
                                    CLog::s_strStr << b; \
                                    Log.log(a, CLog::s_strStr.str(), LOG_LEVEL_ERROR, CLog::s_Dom);
    #define ERROR(a)                a

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
#endif

#ifdef LOGLEVEL_NONE
    #define DEBUG_MSG(a,b)
    #define DEBUG(a)
    #define INFO_MSG(a,b)
    #define INFO(a)
    #define NOTICE_MSG(a,b)
    #define NOTICE(a)
    #define WARNING_MSG(a,b)
    #define WARNING(a)
    #define ERROR_MSG(a,b)
    #define ERROR(a)

    #define CTOR_CALL(a)
    #define DTOR_CALL(a)
    #define METHOD_ENTRY(a)
    #define METHOD_EXIT(a)
    #define MEM_ALLOC(a)
    #define MEM_FREED(a)
#endif

// Be sure not to compile certain domains, although loglevel debug may be active
#ifndef DOMAIN_CONSTRUCTOR
    #undef CTOR_CALL
    #define CTOR_CALL(a)
#endif
#ifndef DOMAIN_DESTRUCTOR
    #undef DTOR_CALL
    #define DTOR_CALL(a)
#endif
#ifndef DOMAIN_METHOD_ENTRY
    #undef METHOD_ENTRY
    #define METHOD_ENTRY(a)
#endif
#ifndef DOMAIN_METHOD_EXIT
    #undef METHOD_EXIT
    #define METHOD_EXIT(a)
#endif
#ifndef DOMAIN_MEMORY_ALLOCATED
    #undef MEM_ALLOC
    #define MEM_ALLOC(a)
#endif
#ifndef DOMAIN_MEMORY_FREED
    #undef MEM_FREED
    #define MEM_FREED(a)
#endif

// Additional macro for memory management
#ifdef DOMAIN_MEMORY_ALLOCATED
    #define DOMAIN_MEMORY
#endif
#ifndef DOMAIN_MEMORY_FREED
    #undef DOMAIN_MEMORY
#endif

// Additional macro for method hierarchy
#ifdef DOMAIN_METHOD_ENTRY
    #define DOMAIN_METHOD_HIERARCHY
#endif
#ifndef DOMAIN_METHOD_EXIT
    #undef DOMAIN_METHOD_HIERARCHY
#endif

#endif
