////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2016-2016 Torsten Büschenfeld
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
/// \file       com_console.h
/// \brief      Prototype of class "CComConsole"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2016-10-26
///
////////////////////////////////////////////////////////////////////////////////

#ifndef COM_CONSOLE_H
#define COM_CONSOLE_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "log.h"
#include "circular_buffer.h"
#include "com_interface_user.h"

//--- Misc header ------------------------------------------------------------//


/// Command queue
typedef CCircularBuffer<std::string> CommandBufferType;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class providing an command console
///
////////////////////////////////////////////////////////////////////////////////
class CComConsole : public IComInterfaceUser
{
    
    public:
        
        //--- Constructor/Destructor -----------------------------------------//
        CComConsole();
        
        //--- Constant Methods -----------------------------------------------//
        const CommandBufferType&    getCommands() const;
        const std::string&          getCurrentCommand() const;
        const CommandBufferType&    getReturnValues() const;
        
        //--- Methods --------------------------------------------------------//
        void addCommand(const std::string& _strCom);
        void execute();
        void nextCommand();
        void prevCommand();
        void setCurrentCommand(const std::string&);
                
        //--- friends --------------------------------------------------------//
        friend std::istream& operator>>(std::istream&, CComConsole&);
        friend std::ostream& operator<<(std::ostream&, CComConsole&);
        
    private:
        
        CommandBufferType   m_CommandBuffer; ///< Command buffer
        CommandBufferType   m_RetValBuffer;  ///< Return value buffer
        std::string         m_strRet;        ///< Return value of last command
        std::string         m_strCurrent;    ///< Currently selected/entered command
        int                 m_nICurrent;     ///< Index of currently selected command
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns command buffer
///
/// \return Command buffer
///
////////////////////////////////////////////////////////////////////////////////
inline const CommandBufferType& CComConsole::getCommands() const
{
    METHOD_ENTRY("CComConsole::getCommands")
    return m_CommandBuffer;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns current command string
///
/// The current command returned by this method might either be a command from
/// command buffer history or the currently entered string.
///
/// \return Current command string
///
////////////////////////////////////////////////////////////////////////////////
inline const std::string& CComConsole::getCurrentCommand() const
{
    METHOD_ENTRY("CComConsole::getCurrentCommand")
    return m_strCurrent;
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Returns return value buffer of functions that were executed
///
/// \return Return value buffer of functions that were executed
///
////////////////////////////////////////////////////////////////////////////////
inline const CommandBufferType& CComConsole::getReturnValues() const
{
    METHOD_ENTRY("CComConsole::getReturnValue")
    return m_RetValBuffer;
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
inline void CComConsole::setCurrentCommand(const std::string& _strCurrent)
{
    METHOD_ENTRY("CComConsole::setCurrentCommand")
    m_strCurrent = _strCurrent;
}

#endif // COM_CONSOLE_H
