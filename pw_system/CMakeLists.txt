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
IF (UNIX)
    FIND_PACKAGE(X11 REQUIRED)
    INCLUDE_DIRECTORIES (${X11_INCLUDE_DIR})
ENDIF()

INCLUDE_DIRECTORIES (
    ${EIGEN3_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${NOISE2D_INCLUDE_DIR}
    ${SFML_INCLUDE_DIR}
    ${CMAKE_HOME_DIRECTORY}/3rdparty/ConcurrentQueue
    ${CMAKE_HOME_DIRECTORY}/3rdparty/sol2
    ${CMAKE_HOME_DIRECTORY}/3rdparty/stb_truetype
    ${CMAKE_HOME_DIRECTORY}/pw_io
    ${CMAKE_HOME_DIRECTORY}/pw_io/import
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/window_management
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core
    ${CMAKE_HOME_DIRECTORY}/pw_physics/components
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects
    ${CMAKE_HOME_DIRECTORY}/pw_physics/universe
    ${CMAKE_HOME_DIRECTORY}/pw_system
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging
    ${CMAKE_HOME_DIRECTORY}/pw_util/math
    ${CMAKE_HOME_DIRECTORY}/pw_util/pcg
    ${CMAKE_HOME_DIRECTORY}/pw_unit
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/font_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/render_target.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader_program.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/text.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/camera.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/uid_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/visuals_data_storage.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/visuals/visuals_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/window_management/widget_console.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/window_management/widget_text.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/window_management/window.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/window_management/win_frame_user.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/game_state_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/input_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/parzival.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xfig_loader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_io/import/xml_importer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/collision_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/debris_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/kinematics_state.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/objects_emitter.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/physics_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/core/sim_timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/components/thruster.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/bounding_box.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/geometry.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/planet.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/polygon.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/shape.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/geometry/terrain.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/joints/spring.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/debris.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/objects/object.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/universe/star_system.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_physics/universe/universe.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_system/com_console.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_system/com_interface.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_system/lua_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_system/planeworld.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_system/thread_module.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures/unique_id.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures/world_data_storage.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/pcg/namegenerator.cpp
)

ADD_EXECUTABLE (planeworld ${SRCS})

TARGET_LINK_LIBRARIES (planeworld
    ${OPENGL_gl_LIBRARY}
    ${LUA_LIBRARIES}
    ${X11LIBRARIES}
    noise2d
    pugixml
    sfml-system
    sfml-window
    sfml-graphics
    Threads::Threads
)
IF (UNIX)
    TARGET_LINK_LIBRARIES (planeworld
        ${X11_X11_LIB})
ENDIF()

INSTALL (TARGETS planeworld
    RUNTIME DESTINATION bin
)
