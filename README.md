![Last commit](https://img.shields.io/github/last-commit/planeworld/planeworld.svg)
![Code size](https://img.shields.io/github/languages/code-size/planeworld/planeworld.svg)
![license](https://img.shields.io/github/license/planeworld/planeworld.svg)

| branch | state |
|--------|-------|
| master | [![Build Status](https://travis-ci.org/planeworld/planeworld.svg?branch=master)](https://travis-ci.org/planeworld/planeworld) |
| dev    | [![Build Status](https://travis-ci.org/planeworld/planeworld.svg?branch=dev)](https://travis-ci.org/planeworld/planeworld)

planeworld
==========

planeworld is a 2D physics engine.

Dependencies
------------
* [eigen](http://eigen.tuxfamily.org/)
* [glm](https://glm.g-truc.net/)
* [Lua](https://www.lua.org/)
* [SFML](https://www.sfml-dev.org/)
* [libnoise2d](https://github.com/planeworld/libnoise2d/)

Installation
------------

    > git clone https://github.com/planeworld/planeworld.git
    > mkdir planeworld/build
    > cd planeworld/build
    > cmake ../
    > ccmake .
    (To configure compile and install details)
    > make install


Usage
-----

    > planeworld example.lua
    
Default Commands (see pw_input_default.lua)

| Key/Mouse                | Description                     |
|--------------------------|---------------------------------|
| 0                        | Toggle timers on/off            |
| 1                        | Toggle timer 1 start/stop       |
| 2                        | Toggle timer 2 start/stop       |
| 3                        | Toggle timer 3 start/stop       |
| f                        | Toggle fullscreen               |
| g                        | Show grid                       |
| b                        | Show bounding boxes             |
| k                        | Show kinematics states          |
| c                        | Cycle through cameras           |
| l                        | Load game state                 |
| m                        | Show center of mass (COM)       |
| n                        | Show names                      |
| s                        | Save game state                 |
| t                        | Show object trajectories        |
| u and LCTRL              | Toggle UID visuals on/off       |
| w and LCTRL              | Show all windows                |
| w and LALT               | Hide all windows                |
| Home                     | Toggle command console          |
| TAB                      | Tab completion in console mode  |
| F2                       | Toggle debug info               |
| F3                       | Toggle debug render             |
| ESC                      | Quit                            |
| + / a                    | Accelerate time (only key-pad +)|
| + / a  and CTRL          | ... and allow timestep increment|
| - / d                    | Decelarate time                 |
| RETURN/ENTER             | Reset time                      |     
| p                        | Pause physics                   |
| SPACE                    | Stepwise physics (when paused)  |
| Mouse-Wheel              | Zoom                            |
| Right-Mouse + Move Up    | Zoom in                         |
| Right-Mouse + Move Down  | Zoom out                        |
| Right-Mouse + Move Left  | Rotate clockwise                |
| Right-Mouse + Move Right | Rotate counterclockwise         |
| Left-Mouse + Move        | Translate                       |
