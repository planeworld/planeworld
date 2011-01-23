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
    ${CMAKE_HOME_DIRECTORY}/pw_io/import
    ${CMAKE_HOME_DIRECTORY}/pw_util/
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging
    ${CMAKE_HOME_DIRECTORY}/pw_util/math
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects
    ${CMAKE_HOME_DIRECTORY}/include
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/circle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/planet_visuals.cpp    
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/polyline_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/rectangle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/spring_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xfig_loader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/geometry.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/planet.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/polyline.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/random_line.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/rectangle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints/spring.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/body.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/object.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/pointmass.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/rigidbody.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
        
    bounding_box.cpp
    camera.cpp
    collision_manager.cpp
    engine_manager.cpp
    parzival.cpp
    physics_manager.cpp
    planeworld.cpp
    visuals_manager.cpp
)

ADD_EXECUTABLE (planeworld ${SRCS})

TARGET_LINK_LIBRARIES (planeworld
    ${SDL_LIBRARY}
    ${LUA_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${Boost_LIBRARIES}
    noise
)

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
