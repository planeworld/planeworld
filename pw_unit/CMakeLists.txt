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

SET(SRCS_UID
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures/unique_id.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    pw_unit_uid.cpp
)

ADD_EXECUTABLE (pw_unit_uid ${SRCS_UID})

TARGET_LINK_LIBRARIES (pw_unit_uid
    sfml-system
    sfml-window
    sfml-graphics
    Threads::Threads
)

INSTALL (TARGETS pw_unit_uid
    RUNTIME DESTINATION bin
)
