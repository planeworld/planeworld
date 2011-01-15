PROJECT (planeworld)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(Boost REQUIRED COMPONENTS thread-mt)

INCLUDE_DIRECTORIES (
    ${SDL_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes
    ${CMAKE_HOME_DIRECTORY}/include
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/polyline.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/random_line.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/rectangle.cpp
    
    body.cpp
    bounding_box.cpp
    camera.cpp
    circle_visuals.cpp
    collision_manager.cpp
    engine_manager.cpp
    geometry.cpp
    graphics.cpp
    object.cpp
    parzival.cpp
    physics_manager.cpp
    planeworld.cpp
    pointmass.cpp
    polyline_visuals.cpp
    rectangle_visuals.cpp
    rigidbody.cpp
    spring.cpp
    spring_visuals.cpp
    visuals_manager.cpp
    xfig_loader.cpp
)

ADD_EXECUTABLE (planeworld ${SRCS})

TARGET_LINK_LIBRARIES (planeworld
    ${SDL_LIBRARY}
    ${LUA_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${Boost_LIBRARIES}
)

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
