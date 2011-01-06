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
/// \file       parzival.cpp 
/// \brief      implementation of class "CParzival"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2009-10-18
///
////////////////////////////////////////////////////////////////////////////////

#include "parzival.h"

//////////////////////////////////////////////////////////////////////////////
///
/// \brief  Constructor, directly opens file
///
////////////////////////////////////////////////////////////////////////////////
CParzival::CParzival(const std::string& strFilename) :
                        m_chComment(PARZIVAL_COMMENT_CHAR_DEFAULT)
{
    METHOD_ENTRY("CParzival::CParzival")
    CTOR_CALL("FileIO")
    DOM_FIO(
        DEBUG_MSG("FileIO", "Directly open file by constructor, calling method open().")
    )
    open(strFilename);

    METHOD_EXIT("CParzival::CParzival")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Destructor
///
/// Normally the destructor is responsible for closing the filestream.
///
////////////////////////////////////////////////////////////////////////////////
CParzival::~CParzival()
{
    METHOD_ENTRY("CParzival::~CParzival")

    DTOR_CALL("FileIO");
    if (m_FStream.is_open() == true)
    {
        m_FStream.close();
        DOM_FIO(
            DEBUG_MSG("FileIO","File closed by destructor.")
        )
    }

    METHOD_EXIT("CParzival::~CParzival")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Reads characters from file, interpreting them as an double value
/// 
/// \return Double value read from file
///
////////////////////////////////////////////////////////////////////////////////
const double CParzival::readDouble()
{
    METHOD_ENTRY("CParzival::readDouble")

    double fValue = 0.0;

    goNext();
    m_FStream >> fValue;

    METHOD_EXIT("CParzival::readDouble")
    return fValue;
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Reads characters from file, interpreting them as an integer value
/// 
/// \return Integer value read from file
///
////////////////////////////////////////////////////////////////////////////////
const int CParzival::readInt()
{
    METHOD_ENTRY("CParzival::readInt")

    int nValue = 0;

    goNext();
    m_FStream >> nValue;

    METHOD_EXIT("CParzival::readInt")
    return nValue;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Reads a string from file
/// 
/// \return String read from file
///
////////////////////////////////////////////////////////////////////////////////
const std::string CParzival::readString()
{
    METHOD_ENTRY("CParzival::readString")

    std::string str;

    goNext();
    m_FStream >> str;

    METHOD_EXIT("CParzival::read")
    return str;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Reads one line from current position
/// 
/// \return Line, stored in std::string
///
////////////////////////////////////////////////////////////////////////////////
const std::string CParzival::readLine()
{
    METHOD_ENTRY("CParzival::readLine()")

    std::string strLine;
    char        chLine[PARZIVAL_MAX_COLUMNS];

    m_FStream.getline(chLine, PARZIVAL_MAX_COLUMNS);
    strLine = chLine;

    METHOD_EXIT("CParzival::readLine")
    return strLine;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Formats and integer value an writes it as characters to textfile
/// 
/// \return Integer value to write to file
///
////////////////////////////////////////////////////////////////////////////////
void CParzival::writeInt(const int& nValue)
{
    METHOD_ENTRY("CParzival::writeInt")

    m_FStream << nValue;

    METHOD_EXIT("CParzival::writeInt")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Formats a string and writes it to textfile
/// 
/// \return String to write to file
///
////////////////////////////////////////////////////////////////////////////////
void CParzival::writeString(const std::string& _str)
{
    METHOD_ENTRY("CParzival::writeString")

    m_FStream << _str;

    METHOD_EXIT("CParzival::writeString")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Create a file for input and output
///
/// This method creates a file for further operations. If it could not be opened
/// the failure bits are resetted to enable opening another file.
/// If the stream is already open, a warning is thrown and the new file will be
/// opened after closing the old one.
///
/// \param strFilename File to be opened for further operations
///
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
bool CParzival::create(const std::string& strFilename)
{
    METHOD_ENTRY("CParzival::create")

    m_strFilename = strFilename;

    // Close an already open stream
    if (m_FStream.is_open() == true)
    {
        m_FStream.close();
        DOM_FIO(
        WARNING_MSG("FileIO", "Warning, there's already an open filestream for this object... closing."))
    }
    
    m_FStream.open(strFilename.c_str(),std::ios::out);
    if (!m_FStream)
    {
        ERROR_MSG("FileIO", "File " + strFilename + " could not be created.")
        m_FStream.clear();
        METHOD_EXIT("CParzival::create")
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("FileIO", strFilename + " succesfully created."))
        METHOD_EXIT("CParzival::create")
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Open a specific file for input and output
///
/// This method opens a file for further operations. If it could not be opened
/// the failure bits are resetted to enable opening another file.
/// If the stream is already open, a warning is thrown and the new file will be
/// opened after closing the old one.
///
/// \param strFilename File to be opened for further operations
///
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
bool CParzival::open(const std::string& strFilename)
{
    METHOD_ENTRY("CParzival::open")

    m_strFilename = strFilename;

    // Close an already open stream
    if (m_FStream.is_open() == true)
    {
        m_FStream.close();
        DOM_FIO(
        WARNING_MSG("FileIO", "Warning, there's already an open filestream for this object... closing."))
    }
    
    m_FStream.open(strFilename.c_str());
    if (!m_FStream)
    {
        ERROR_MSG("FileIO", "File " + strFilename + " could not be opened.")
        m_FStream.clear();
        METHOD_EXIT("CParzival::open")
        return false;
    }
    else
    {
        DOM_FIO(DEBUG_MSG("FileIO", strFilename + " succesfully opened."))
        METHOD_EXIT("CParzival::open")
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Close the file
///
/// This method closes the file. It is not neccessary to call this method
/// unless you need to change the associated file or just need it temporarely.
/// Otherwise closing is done by the destructor.
///
////////////////////////////////////////////////////////////////////////////////
void CParzival::close()
{
    METHOD_ENTRY("CParzival::close")

    // Only close an open stream
    if (m_FStream.is_open() == true)
    {
        m_FStream.close();
        DOM_FIO(DEBUG_MSG("FileIO", m_strFilename + " closed."))
    }

    METHOD_EXIT("CParzival::close")
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Go to the beginning of the file
///
////////////////////////////////////////////////////////////////////////////////
void CParzival::goHead()
{
    METHOD_ENTRY("CParzival::goHead")

    m_FStream.seekp(0, std::ios::beg);

    METHOD_EXIT("CParzival::goHead")
}


////////////////////////////////////////////////////////////////////////////////
///
/// \brief Go to the next position that is not a comment
///
////////////////////////////////////////////////////////////////////////////////
void CParzival::goNext()
{
    METHOD_ENTRY("CParzival::goNext")

    char ch='t';

    m_FStream.get(ch);

    // Ignore whitespaces, newlines and comments
    while ((!m_FStream.eof()) && ((ch==m_chComment) || (ch=='\n') || (ch=='\t') || (ch==' ')))
    {
        // Ignore rest of the line if commented
        if (ch==m_chComment)
        {
            m_FStream.ignore(PARZIVAL_MAX_COLUMNS, '\n');
        }
        m_FStream.get(ch);
    }
    m_FStream.putback(ch);

    METHOD_EXIT("CParzival::goNext")
}
