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
/// \file       input_manager.cpp
/// \brief      Implementation of class "CInputManager"
///
/// \author     Torsten Büschenfeld (planeworld@bfeld.eu)
/// \date       2010-04-06
///
////////////////////////////////////////////////////////////////////////////////

#include "input_manager.h"

//--- Misc header ------------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Constructor
///
///////////////////////////////////////////////////////////////////////////////
CInputManager::CInputManager() : m_pWindow(nullptr),
                                 m_UIMode(UIModeType::WORLD)
{
    METHOD_ENTRY("CInputManager::CInputManager")
    CTOR_CALL("CInputManager::CInputManager")
    
    #ifdef PW_MULTITHREADING
        m_strModuleName = "Input Manager";
    #endif
    
    m_vecMouse = {0,0};
    m_vecMouseCenter = {0,0};
}

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Processes one input frame
///
/// \return Success
///
////////////////////////////////////////////////////////////////////////////////
bool CInputManager::processFrame()
{
    METHOD_ENTRY("CInputManager::processFrame")
    
    sf::Vector2i vecMouse = sf::Mouse::getPosition(*m_pWindow);
    m_vecMouse = m_vecMouseCenter-vecMouse;
    m_vecMouse.x = -m_vecMouse.x; // Horizontal movements to the left should be negative
    if (m_UIMode == UIModeType::WORLD)
        sf::Mouse::setPosition(m_vecMouseCenter,*m_pWindow);
    
    m_pComInterface->call<void, int, int>("mouse_set_cursor", vecMouse.x, vecMouse.y);

    //--- Handle events ---//
    sf::Event Event;
    while (m_pWindow->pollEvent(Event))
    {
        int nCamMainUID = m_pComInterface->call<int>("get_main_camera");
        
        switch (Event.type)
        {
            case sf::Event::Closed:
            {
                // End the program
                m_pComInterface->call<void>("quit");
                break;
            }
            case sf::Event::Resized:
            {
                // Adjust the viewport when the window is resized
                m_vecMouseCenter = sf::Vector2i(m_pWindow->getSize().x >> 1, m_pWindow->getSize().y >> 1);
                m_pComInterface->call<void,double,double>("win_main_resize_viewport", Event.size.width, Event.size.height);
                
                m_pComInterface->call<void,double,double>("e_resize", Event.size.width, Event.size.height);
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (Event.key.code == sf::Keyboard::Escape) m_pComInterface->call<void>("exit");
                    
                if (m_UIMode == UIModeType::UI)
                {
                    switch (Event.key.code)
                    {
                        case sf::Keyboard::F1:
                        {
                            m_pComInterface->call<void>("toggle_debug");
                            break;
                        }
                        case sf::Keyboard::BackSpace:
                        {
                            m_pComInterface->call<void>("com_console_backspace");
                            break;
                        }
                        case sf::Keyboard::Home:
                        {
                            m_UIMode = UIModeType::WORLD;
                            m_pComInterface->call<void>("mouse_cursor_off");
                            m_pComInterface->call<void>("com_console_off");
                            break;
                        }
                        case sf::Keyboard::Up:
                        {
                            m_pComInterface->call<void>("com_console_prev");
                            break;
                        }
                        case sf::Keyboard::Down:
                        {
                            m_pComInterface->call<void>("com_console_next");
                            break;
                        }
                        case sf::Keyboard::Return:
                        {
                            m_pComInterface->call<void>("com_console_execute");
                            break;
                        }
                        case sf::Keyboard::Tab:
                        {
                            m_pComInterface->call<void>("com_console_complement");
                            break;
                        }
                        case sf::Keyboard::U:
                        {
                            
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                            {
                                m_pComInterface->call<void>("uid_vis_toggle");
                            }
                            break;
                        }
                        case sf::Keyboard::W:
                        {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                            {
                                m_pComInterface->call<void>("win_show_all");
                            }
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
                            {
                                m_pComInterface->call<void>("win_hide_all");
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                else
                {
                    m_pComInterface->call<void, int>("e_key_pressed", Event.key.code);
                    
                    switch (Event.key.code)
                    {
                        case sf::Keyboard::Home:
                        {
                            m_UIMode = UIModeType::UI;
                            m_pComInterface->call<void>("mouse_cursor_on");
                            m_pComInterface->call<void>("com_console_on");
                            break;
                        }
                        case sf::Keyboard::F1:
                        {
                            m_pComInterface->call<void>("toggle_debug");
                            break;
                        }
                        case sf::Keyboard::U:
                        {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                            {
                                m_pComInterface->call<void>("uid_vis_toggle");
                            }
                            break;
                        }
                        case sf::Keyboard::W:
                        {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                            {
                                m_pComInterface->call<void>("win_show_all");
                            }
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
                            {
                                m_pComInterface->call<void>("win_hide_all");
                                
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (m_UIMode == UIModeType::UI)
                {
                    if (Event.mouseButton.button == sf::Mouse::Left)
                    {
                        m_pComInterface->call<void>("mouse_mbl_pressed");
                    }
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                if (m_UIMode == UIModeType::UI)
                {
                    if (Event.mouseButton.button == sf::Mouse::Left)
                    {
                        m_pComInterface->call<void>("mouse_mbl_released");
                    }
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (m_UIMode == UIModeType::WORLD && nCamMainUID != 0)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        double fZoom = m_pComInterface->call<double, int>("cam_get_zoom", nCamMainUID);
                        m_pComInterface->call<void, int, double, double>("cam_translate_by", nCamMainUID,
                                        0.2/2.0*double(m_vecMouse.x)/fZoom,
                                        0.2/2.0*double(m_vecMouse.y)/fZoom);
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        m_pComInterface->call<void, int, double>("cam_rotate_by", nCamMainUID, -double(m_vecMouse.x)*0.001);
                        m_pComInterface->call<void, int, double>("cam_zoom_by", nCamMainUID, 1.0+double(m_vecMouse.y)*0.001);
                        double fZoom = m_pComInterface->call<double, int>("cam_get_zoom", nCamMainUID);
                        if (fZoom < 1.0e-18)
                            m_pComInterface->call<void, int, double>("cam_zoom_to", nCamMainUID, 1.0e-18);
                        else if (fZoom > 1.0e3)
                            m_pComInterface->call<void, int, double>("cam_zoom_to", nCamMainUID, 1.0e3);
                    }
                }
                break;
            }
            case sf::Event::MouseWheelMoved:
            {
                if (nCamMainUID != 0)
                {
                    m_pComInterface->call<void, int, double>("cam_zoom_by", nCamMainUID, 1.0+double(Event.mouseWheel.delta)*0.1);
                    double fZoom = m_pComInterface->call<double, int>("cam_get_zoom", nCamMainUID);
                    if (fZoom < 1.0e-18)
                        m_pComInterface->call<void, int, double>("cam_zoom_to", nCamMainUID, 1.0e-18);
                    else if (fZoom > 1.0e3)
                        m_pComInterface->call<void, int, double>("cam_zoom_to", nCamMainUID, 1.0e3);
                }
                break;
            }
            case sf::Event::TextEntered:
            {
                if (m_UIMode == UIModeType::UI)
                {
                    if (Event.text.unicode > 31 && Event.text.unicode < 127)
                    {
                        m_pComInterface->call<void, std::string>("com_console_expand", sf::String(Event.text.unicode).toAnsiString());
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    m_pComInterface->callWriters("input");
    
    return true; 
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Initialise the command interface
///
///////////////////////////////////////////////////////////////////////////////
void CInputManager::myInitComInterface()
{
    METHOD_ENTRY("CInputManager::myInitComInterface")

    INFO_MSG("Input Manager", "Initialising com interace.")
    if (m_pComInterface != nullptr)
    {
        // Events
        m_pComInterface->registerEvent<int>("e_key_pressed",
                                        "Event, indicating that a key was pressed.",
                                        {{ParameterType::NONE, "No return value"},
                                         {ParameterType::INT, "Key Code"}},
                                        "system");
        m_pComInterface->registerEvent<double,double>("e_resize",
                                        "Event, indicating that the main window was resized.",
                                        {{ParameterType::NONE, "No return value"},
                                         {ParameterType::DOUBLE, "Size X"},
                                         {ParameterType::DOUBLE, "Size_Y"}},
                                        "system");
        
        // System package
        m_pComInterface->registerFunction("get_input_frequency",
                                          CCommand<double>([&]() -> double {return this->m_fFrequency;}),
                                          "Provides processing frequency of Input module.",
                                          {{ParameterType::DOUBLE, "Processing frequency of Input module"}},
                                           "system");
        m_pComInterface->registerFunction("key_is_pressed",
                                          CCommand<bool, int>([&](const int _nCode) -> bool
                                          {
                                              return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(_nCode));
                                          }),
                                          "Indicates if given key is pressed.",
                                          {{ParameterType::BOOL, "Is given key pressed?"},
                                           {ParameterType::INT, "Key code"}},
                                           "system");
        m_pComInterface->registerFunction("set_frequency_input",
                                          CCommand<void, double>([&](const double& _fFrequency)
                                          {
                                              this->setFrequency(_fFrequency);
                                          }),
                                          "Sets the frequency of the input thread.",
                                          {{ParameterType::NONE, "No return value"},
                                           {ParameterType::DOUBLE, "Frequency"}},
                                           "system", "input");
        m_pComInterface->registerFunction("toggle_ui_mode",
                                          CCommand<void>([&]()
                                          {
                                                if (m_UIMode == UIModeType::WORLD)
                                                {
                                                    m_UIMode = UIModeType::UI;
                                                    m_pComInterface->call<void>("mouse_cursor_on");
                                                }
                                                else
                                                {
                                                    m_UIMode = UIModeType::WORLD;
                                                    m_pComInterface->call<void>("mouse_cursor_off");
                                                }
                                                m_vecMouse.x=0;
                                                m_vecMouse.y=0;
                                          }),
                                          "Toggles user interface mode (UI, WORLD)",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "input");
    }
    else
    {
        WARNING_MSG("Input Manager", "Com interface not set, cannot register functions.")
    }
}
