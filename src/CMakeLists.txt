SET(THREADS_PREFER_PTHREAD_FLAG ON)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Lua REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(OpenMP REQUIRED)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ENDIF()

INCLUDE_DIRECTORIES (
    ${LUA_INCLUDE_DIR}
    ${CMAKE_HOME_DIRECTORY}/3rdparty/pugixml/src
    ${CMAKE_HOME_DIRECTORY}/pw_io
    ${CMAKE_HOME_DIRECTORY}/pw_io/import
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures
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
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/circle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/debris_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/debris_visuals_thruster.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/object_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/planet_visuals.cpp    
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/polygon_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/spring_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/terrain_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/visuals_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/game_state_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xfig_loader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xml_importer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/debris_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/objects_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/physics_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/sim_timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/components/thruster.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/double_buffered_shape.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/geometry.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/planet.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/polygon.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/shape.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/terrain.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints/spring.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/debris.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/object.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
        
    bounding_box.cpp
    camera.cpp
    collision_manager.cpp
    kinematics_state.cpp
    namegenerator.cpp
    parzival.cpp
    planeworld.cpp
    star_system.cpp
    unique_id.cpp
    universe.cpp
    world_data_storage.cpp
    
)

ADD_EXECUTABLE (planeworld ${SRCS})

TARGET_LINK_LIBRARIES (planeworld
    ${OPENGL_gl_LIBRARY}
    ${LUA_LIBRARIES}
    noise2d
    pugixml
    sfml-system
    sfml-window
    sfml-graphics
    Threads::Threads
)

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
