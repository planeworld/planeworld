SET(THREADS_PREFER_PTHREAD_FLAG ON)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glm REQUIRED)

INCLUDE_DIRECTORIES (
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader_program.cpp
    gl_test.cpp
)

ADD_EXECUTABLE (gl_test ${SRCS})

TARGET_LINK_LIBRARIES (gl_test
    ${OPENGL_gl_LIBRARY}
    sfml-system
    sfml-window
    sfml-graphics
)

INSTALL (TARGETS gl_test
    RUNTIME DESTINATION bin
)
INSTALL (FILES shader.vert DESTINATION share/planeworld/data)
INSTALL (FILES shader.frag DESTINATION share/planeworld/data)