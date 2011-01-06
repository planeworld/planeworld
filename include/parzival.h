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
/// \file       parzival.h
/// \brief      Prototype of class "CParzival"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////
#ifndef PARZIVAL_H
#define PARZIVAL_H

//--- Standard header --------------------------------------------------------//
// #include <iostream>
#include <fstream>
// #include <sstream>
// #include <string>

//--- Program header ---------------------------------------------------------//
#include "log.h"

//--- Misc header ------------------------------------------------------------//

//--- Constants --------------------------------------------------------------//
const char              PARZIVAL_COMMENT_CHAR_DEFAULT = ';';
const unsigned short    PARZIVAL_MAX_COLUMNS = 256;

////////////////////////////////////////////////////////////////////////////////
///
/// \class CParzival 
///
/// \brief Wrapper class for parsing plain textfiles
///
/// This classes purpose is to provide easy access to filestreams concerning
/// plain textfiles. Therefore it implements simple methods like "readint" to
/// simplify the use of configfiles with respect to c++ standard.
///
////////////////////////////////////////////////////////////////////////////////
class CParzival
{

    public:
        //--- Constructor/Destructor -----------------------------------------//
        CParzival(){}
        CParzival(const std::string&);
        ~CParzival();
        
        //--- Methods --------------------------------------------------------//
        bool                create(const std::string&);
        bool                open(const std::string&);
        const double        readDouble();
        const int           readInt();
        const std::string   readString();
        const std::string   readLine();
        void                close();
        void                goHead();
        void                goNext();
        void                writeInt(const int&);
        void                writeString(const std::string&);
        void                setComment(const char);
        std::fstream&       getStream();
    
    private:

        char                m_chComment;        ///< Character used to indicate comment 
        std::fstream        m_FStream;          ///< Filestream
        std::string         m_strFilename;      ///< Name of file
};


//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Set the character that initiates a comment
///
/// \param _chComment Character used for comments
///
////////////////////////////////////////////////////////////////////////////////
inline void CParzival::setComment(const char _chComment)
{
    METHOD_ENTRY("CParzival::setComment(const char)");

    m_chComment = _chComment;

    METHOD_EXIT("CParzival::setComment(const char)");
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Set the character that initiates a comment
///
////////////////////////////////////////////////////////////////////////////////
inline std::fstream& CParzival::getStream()
{
    METHOD_ENTRY("CParzival::getStream()");

    METHOD_EXIT("CParzival::getStream()");
    return m_FStream;
}

#endif
