planeworld
==========

planeworld is a 2D physics engine.

Installation
------------

### libnoise2d
> git clone path/to/libnoise.git
> mkdir libnoise/build
> cd libnoise/build
> cmake ../
> ccmake .
Configure install path (e.g. ~/bin/)
> make install

### planeworld
> git clone path/to/planeworld.git
> cp planeworld/SETENV ~/bin/
> source ~/bin/SETENV
> mkdir planeworld/build
> cd planeworld/build
> cmake ../
> ccmake .
Configure install path (e.g. ~/bin/)
> make install


Usage
-----

> planeworld ~/bin/share/planeworld/data/solar_system.xml

| Key/Mouse                | Description                     |
|--------------------------|---------------------------------|
| g                        | Show grid                       |
| b                        | Show bounding boxes             |
| k                        | Show kinematics states          |
| n                        | Show names                      |
| t                        | Show object trajectories        |
| ESC                      | Quit                            |
| +                        | Accelerate time (only key-pad +)|
| -                        | Decelarate time                 |
| RETURN/ENTER             | Reset time                      |     
| p                        | Pause physics                   |
| Mouse-Wheel              | Zoom                            |
| Right-Mouse + Move Up    | Zoom in                         |
| Right-Mouse + Move Down  | Zoom out                        |
| Right-Mouse + Move Left  | Rotate clockwise                |
| Right-Mouse + Move Right | Rotate counterclockwise         |
| Left-Mouse + Move        | Translate                       |
