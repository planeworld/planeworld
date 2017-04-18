////////////////////////////////////////////////////////////////////////////////
//
// This file is part of planeworld, a 2D simulation of physics and much more.
// Copyright (C) 2017 Torsten Büschenfeld
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
/// \file       input_manager.h
/// \brief      Prototype of class "CInputManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2017-01-25
///
////////////////////////////////////////////////////////////////////////////////

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

//--- Standard header --------------------------------------------------------//

//--- Program header ---------------------------------------------------------//
#include "com_console.h"
#include "com_interface_provider.h"
#include "thread_module.h"

//--- Misc header ------------------------------------------------------------//
#include <SFML/Graphics.hpp>

const double INPUT_DEFAULT_FREQUENCY = 120.0;   ///< Default frequency for input update

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Class for managing the input (mouse, keyboard)
///
////////////////////////////////////////////////////////////////////////////////
class CInputManager : public IComInterfaceProvider,
                      public IThreadModule
{
    
    public:

        //--- Constructor/Destructor -----------------------------------------//
        CInputManager();
        
        //--- Constant Methods -----------------------------------------------//
                
        //--- Methods --------------------------------------------------------//
        void processFrame();
        void setWindow(sf::RenderWindow* const _pWindow);
        
    private:

        //--- Constant methods [private] -------------------------------------//
        
        //--- Methods [private] ----------------------------------------------//
        void myInitComInterface();
        
        //--- Variables [private] --------------------------------------------//
        sf::RenderWindow*   m_pWindow;          ///< Window with input focus
        
        sf::Vector2i    m_vecMouse;             ///< Current mouse position
        sf::Vector2i    m_vecMouseCenter;       ///< Mouse position at window center
        bool            m_bMouseCursorVisible;  ///< Toggles visibilty of mouse cursor
        
        
        bool            m_bConsoleMode;         ///< Toggles com console mode
};

//--- Implementation is done here for inline optimisation --------------------//

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Set window with input focus
///
/// \param _pWindow Window with input focus
///
////////////////////////////////////////////////////////////////////////////////
inline void CInputManager::setWindow(sf::RenderWindow* const _pWindow)
{
    METHOD_ENTRY("CInputManager::setWindow")
    m_pWindow = _pWindow;
    m_vecMouseCenter = sf::Vector2i(m_pWindow->getSize().x >> 1, m_pWindow->getSize().y >> 1);
}

#endif
