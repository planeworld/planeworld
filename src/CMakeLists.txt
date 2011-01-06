PROJECT (planeworld)

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)

INCLUDE_DIRECTORIES (
    ${SDL_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
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
    
    adams_bashforth_integrator.cpp
    adams_moulton_integrator.cpp
    body.cpp
    bounding_box.cpp
    circle_visuals.cpp
    collision_manager.cpp
    engine_manager.cpp
    euler_integrator.cpp
    geometry.cpp
    graphics.cpp
    object.cpp
    parzival.cpp
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
)

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
