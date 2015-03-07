PROJECT (planeworld)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)
# FIND_PACKAGE(Boost)

INCLUDE_DIRECTORIES (
    ${Boost_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${CMAKE_HOME_DIRECTORY}/3rdparty/pugixml/src
    ${CMAKE_HOME_DIRECTORY}/pw_io/import
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging
    ${CMAKE_HOME_DIRECTORY}/pw_util/math
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core
    ${CMAKE_HOME_DIRECTORY}/pw_physics/components
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects
    ${CMAKE_HOME_DIRECTORY}/pw_unit
    ${CMAKE_HOME_DIRECTORY}/include
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/3rdparty/pugixml/src/pugixml.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/circle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/debris_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/object_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/planet_visuals.cpp    
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/polyline_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/spring_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/terrain_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/visuals_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xfig_loader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xml_importer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/debris_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/objects_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/physics_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/components/thruster.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/double_buffered_shape.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/geometry.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/planet.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/polyline.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/terrain.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints/spring.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/body.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/debris.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/object.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/pointmass.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/rigidbody.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
        
    bounding_box.cpp
    camera.cpp
    collision_manager.cpp
    hooker.cpp
    namegenerator.cpp
    parzival.cpp
    planeworld.cpp
    star_system.cpp
    universe.cpp
    world_data_storage.cpp
    
)

SET(SRCS_STARMAP_UT
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    
    ${CMAKE_HOME_DIRECTORY}/pw_unit/star_map_ut.cpp
    
    star_system.cpp
    universe.cpp
)

SET(SRCS_UNIVERSE_UT
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    
    ${CMAKE_HOME_DIRECTORY}/pw_unit/universe_ut.cpp
    
    star_system.cpp
    universe.cpp
)

ADD_EXECUTABLE (planeworld ${SRCS})
#ADD_EXECUTABLE (starmap_ut ${SRCS_STARMAP_UT})
#ADD_EXECUTABLE (universe_ut ${SRCS_UNIVERSE_UT})

TARGET_LINK_LIBRARIES (planeworld
    ${OPENGL_gl_LIBRARY}
    ${LUA_LIBRARY}
    noise2d
    sfml-system
    sfml-window
    sfml-graphics
)

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
