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
                                 m_bMouseCursorVisible(false),
                                 m_strConsoleCommand(""),
                                 m_bConsoleMode(false)
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
////////////////////////////////////////////////////////////////////////////////
void CInputManager::processFrame()
{
    METHOD_ENTRY("CInputManager::processFrame")
    
    m_vecMouse = m_vecMouseCenter-sf::Mouse::getPosition(*m_pWindow);
    m_vecMouse.x = -m_vecMouse.x; // Horizontal movements to the left should be negative
    if (!m_bMouseCursorVisible)
        sf::Mouse::setPosition(m_vecMouseCenter,*m_pWindow);

    //--- Handle events ---//
    sf::Event Event;
    while (m_pWindow->pollEvent(Event))
    {
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
                m_pComInterface->call<void,double,double>("win_resize", Event.size.width, Event.size.height);
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (Event.key.code == sf::Keyboard::Home)
                {
                    m_strConsoleCommand = "";
                    m_bConsoleMode ^= true;
                    m_pComInterface->call<void>("toggle_console_mode");
                    INFO_MSG("Main","Console mode toggled")
                    break;
                }
                if (m_bConsoleMode)
                {
                    switch (Event.key.code)
                    {
                        case sf::Keyboard::BackSpace:
                        {
                            if (!m_strConsoleCommand.empty())
                            {
                                m_strConsoleCommand.pop_back();
                            }
                            m_pComConsole->setCurrentCommand(m_strConsoleCommand);
                            break;
                        }
                        case sf::Keyboard::Home:
                        {
                            m_strConsoleCommand = "";
                            m_bConsoleMode ^= true;
                            m_pComInterface->call<void>("toggle_console_mode");
                            INFO_MSG("Main","Console mode toggled")
                            break;
                        }
                        case sf::Keyboard::Up:
                        {
                            m_pComConsole->prevCommand();
                            m_strConsoleCommand = m_pComConsole->getCurrentCommand();
                            break;
                        }
                        case sf::Keyboard::Down:
                        {
                            m_pComConsole->nextCommand();
                            m_strConsoleCommand = m_pComConsole->getCurrentCommand();
                            break;
                        }
                        case sf::Keyboard::Return:
                        {
                            m_strConsoleCommand = "";
                            m_pComConsole->execute();
                            break;
                        }
                        default:
                            break;
                    }
                }
                else
                {
                    switch (Event.key.code)
                    {
                        case sf::Keyboard::Escape:
                        {
                            m_pComInterface->call<void>("quit");
                            break;
                        }
                        case sf::Keyboard::Num0:
                        {
                            m_pComInterface->call<void>("toggle_timers");
                            break;
                        }
                        case sf::Keyboard::Num1:
                        {
                            m_pComInterface->call<void,int>("toggle_timer", 1);
                            break;
                        }
                        case sf::Keyboard::Num2:
                        {
                            m_pComInterface->call<void,int>("toggle_timer", 2);
                            break;
                        }
                        case sf::Keyboard::Num3:
                        {
                            m_pComInterface->call<void,int>("toggle_timer", 3);
                            break;
                        }
                        case sf::Keyboard::Add:
                        case sf::Keyboard::A:
                        {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                                m_pComInterface->call<void, bool>("accelerate_time", true);
                            else
                                m_pComInterface->call<void, bool>("accelerate_time", false);
                            break;
                        }
                        case sf::Keyboard::Subtract:
                        case sf::Keyboard::Dash:
                        case sf::Keyboard::D:
                        {
                            m_pComInterface->call<void>("decelerate_time");
                            break;
                        }
                        case sf::Keyboard::Return:
                        {
                            m_pComInterface->call<void>("reset_time");
                            break;
                        }
                        case sf::Keyboard::C:
                        {
                            m_pComInterface->call<void>("cam_cycle");
                            break;
                        }
                        case sf::Keyboard::B:
                        {
                            m_pComInterface->call<void>("toggle_bboxes");
                            break;
                        }
                        case sf::Keyboard::G:
                        {
                            m_pComInterface->call<void>("toggle_grid");
                            break;
                        }
                        case sf::Keyboard::K:
                        {
                            m_pComInterface->call<void>("toggle_kin_states");
                            break;
                        }
                        case sf::Keyboard::L:
                        {
                            /// \todo Really check if physics thread is paused before saving the simulation
                            m_pComInterface->call<void>("pause");
//                             GameStateManager.load();
//                             pCamera=(*VisualsDataStorage.getCamerasByName().cbegin()).second;
                            m_pComInterface->call<void>("toggle_pause");
                            break;
                        }
                        case sf::Keyboard::M:
                        {
                            m_pComInterface->call<void>("toggle_com");
                            break;
                        }
                        case sf::Keyboard::N:
                        {
                            m_pComInterface->call<void>("toggle_names");
                            break;
                        }
                        case sf::Keyboard::P:
                        {
                            m_pComInterface->call<void>("toggle_pause");
                            break;
                        }
                        case sf::Keyboard::S:
                        {
                            /// \todo Really check if physics thread is paused before saving the simulation
                            m_pComInterface->call<void>("pause");
//                             GameStateManager.save();
                            m_pComInterface->call<void>("toggle_pause");
                            break;
                        }
                        case sf::Keyboard::Space:
                        {
                            m_pComInterface->call<void>("process_one_frame");
                            break;
                        }
                        case sf::Keyboard::T:
                        {
//                             pVisualsManager->toggleVisualisations(VISUALS_OBJECT_TRAJECTORIES);
                            break;
                        }
                        case sf::Keyboard::V:
                        {
//                             bGraphicsOn ^= 1;
                            m_bMouseCursorVisible ^= 1;
                            m_pWindow->setMouseCursorVisible(m_bMouseCursorVisible);
                            m_vecMouse.x=0;
                            m_vecMouse.y=0;
//                             
//                             if (bGraphicsOn)
//                             {
//                                 INFO_MSG("Main", "Graphics reactivated.")
//                             }
//                             else
//                             {
//                                 INFO_MSG("Main", "Graphics deactivated, simulation still running...")
//                             }
                            break;
                        }
                        default:
                            break;
                    }
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
//                 if (bGraphicsOn)
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        double fZoom = m_pComInterface->call<double>("cam_get_zoom");
                        m_pComInterface->call<void,double,double>("cam_translate_by",
                                        0.2/2.0*double(m_vecMouse.x)/fZoom,
                                        0.2/2.0*double(m_vecMouse.y)/fZoom);
//                                         0.2/GRAPHICS_PX_PER_METER*double(m_vecMouse.x)/fZoom,
//                                         0.2/GRAPHICS_PX_PER_METER*double(m_vecMouse.y)/fZoom);
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        m_pComInterface->call<void, double>("cam_rotate_by", -double(m_vecMouse.x)*0.001);
                        m_pComInterface->call<void, double>("cam_zoom_by",1.0+double(m_vecMouse.y)*0.001);
                        double fZoom = m_pComInterface->call<double>("cam_get_zoom");
                        if (fZoom < 1.0e-18)
                            m_pComInterface->call<void,double>("cam_zoom_to",1.0e-18);
                        else if (fZoom > 1.0e3)
                            m_pComInterface->call<void,double>("cam_zoom_to",1.0e3);
                    }
                    break;
                }
            }
            case sf::Event::MouseWheelMoved:
//                 if (bGraphicsOn)
                {
                    m_pComInterface->call<void, double>("cam_zoom_by",1.0+double(Event.mouseWheel.delta)*0.1);
                    double fZoom = m_pComInterface->call<double>("cam_get_zoom");
                    if (fZoom < 1.0e-18)
                        m_pComInterface->call<void,double>("cam_zoom_to",1.0e-18);
                    else if (fZoom > 1.0e3)
                        m_pComInterface->call<void,double>("cam_zoom_to",1.0e3);
                }
            case sf::Event::TextEntered:
            {
                if (m_bConsoleMode)
                {
                    if (Event.text.unicode > 31 && Event.text.unicode < 127)
                    {
                        m_strConsoleCommand += static_cast<char>(Event.text.unicode);
                        m_pComConsole->setCurrentCommand(m_strConsoleCommand);
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    m_pComInterface->callWriters("input");
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
        // System package
        m_pComInterface->registerFunction("get_input_frequency",
                                          CCommand<double>([&]() -> double {return this->m_fFrequency;}),
                                          "Provides processing frequency of Input module.",
                                          {{ParameterType::DOUBLE, "Processing frequency of Input module"}},
                                           "system");
        m_pComInterface->registerFunction("toggle_mouse_cursor",
                                          CCommand<void>([&]()
                                          {
                                              m_bMouseCursorVisible ^= 1;
                                              m_pWindow->setMouseCursorVisible(m_bMouseCursorVisible);
                                              m_vecMouse.x=0;
                                              m_vecMouse.y=0;
                                          }),
                                          "Toggles visibility of mouse cursor.",
                                          {{ParameterType::NONE, "No return value"}},
                                           "system", "input");
    }
    else
    {
        WARNING_MSG("Input Manager", "Com interface not set, cannot register functions.")
    }
}
