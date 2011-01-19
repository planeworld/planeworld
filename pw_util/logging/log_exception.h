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
/// \file       log_exception.h
/// \brief      Prototype of class "LogException"
///
/// \date       2010-03-17
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef LOG_EXCEPTION_H
#define LOG_EXCEPTION_H

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Standard header --------------------------------------------------------//

//--- Misc. header -----------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for exception handling
///
////////////////////////////////////////////////////////////////////////////////
class LogException
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
               
        //--- Const Methods --------------------------------------------------//
        virtual void writeMessage() const;
        
        //--- Methods --------------------------------------------------------//
        
    private:
    
        //--- Methods --------------------------------------------------------//
        
        //--- Variables ------------------------------------------------------//
        std::string     m_strMsgSrc;    ///< Source of message
        std::string     m_strMsg;       ///< Message
        LogLevelType    m_MsgLevel;     ///< Loglevel of Message
        LogDomainType   m_MsgDom;       ///< Domain of Message

};

//--- Implementation is done here for inlining -------------------------------//

#endif
