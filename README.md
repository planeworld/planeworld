| branch | state |
|--------|-------|
| master | [![Build Status](https://travis-ci.org/planeworld/planeworld.svg?branch=master)](https://travis-ci.org/planeworld/planeworld) ![Last commit](https://img.shields.io/github/last-commit/planeworld/planeworld.svg?branch=master) ![Code size](https://img.shields.io/github/languages/code-size/planeworld/planeworld.svg?branch=master) |
| dev    | [![Build Status](https://travis-ci.org/planeworld/planeworld.svg?branch=dev)](https://travis-ci.org/planeworld/planeworld) ![Last commit](https://img.shields.io/github/last-commit/planeworld/planeworld.svg?branch=dev) ![Code size](https://img.shields.io/github/languages/code-size/planeworld/planeworld.svg?branch=dev) |

![license](https://img.shields.io/github/license/planeworld/planeworld.svg)

planeworld
==========

planeworld is a 2D simulation engine and a passion project. The main goal of its development is best explained by the following examplary scenerio: Start a rocket from a plantery surface with simulated physics, such as rigid body dynamics, fuel system, planetary atmosphere, etc. Reach another celestial body, like a moon, using realistic trajectory planning, incorporating n-Body dynamics for the star system. Land on the target body and drive around with a rover, which is also simulated with springs and dampers, suspension, tire flex and more. 
The engine should allow for (Lua) scripting and remote missions with simulated communication and patchable onboard computers.

Features
--------
* Universe
    * n-Body dynamics (done)
    * Procedurally generated planetary surfaces using noise functions (done)
    * Simulation of planetary atmosphere (done)
    * Different surface types, biomes,... (wip)
    * Procedurally generated universe (wip)
* Components
    * Thrusters (done)
    * Fuel system (planned)
    * Hydraulics (planned)
    * Control units (planned)
* Physics
    * Basic dynamics (done)
    * Automatic online calculation of inertia and center of mass (done)
    * Particle system and emitters (done)
    * Springs and Dampers (wip)
    * Continuous collision detection (wip)
    * Hard connections between objects (wip)
    * Constraint-based physics with several different joints (planned)
    * Rope physics (planned)
    * 2D tire physics (planned)
    * Drag and lift (planned)
    * Data communication (planned)
* Graphics
    * OpenGL Core Profile (done)
    * Font manager (done)
    * Simple deferred-like rendering (done)
    * Different render targets, render-to-texture (done)
    * Different light sources, such as thrusters, headlights, stars (wip)
    * Shadow mapping (planned)
* UI
    * Simple Window management (done)
    * Command console (done)
    * Virtual cameras (done)
    * Further widgets (planned)
* System
    * Multi-threading (done)
    * Command interface with thread safe command queues (done)
    * Scripting API for command interface (done)
    * Separation of simulation and visualisation (done)
    * Handle manager (wip)
    * Client-server architecture (planned)

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
