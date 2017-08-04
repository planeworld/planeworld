SET(THREADS_PREFER_PTHREAD_FLAG ON)

FIND_PACKAGE(OpenGL REQUIRED)
# FIND_PACKAGE(glm REQUIRED)

INCLUDE_DIRECTORIES (
    ${OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_HOME_DIRECTORY}/3rdparty/stb_truetype
    ${CMAKE_HOME_DIRECTORY}/include
    ${CMAKE_HOME_DIRECTORY}/pw_util/data_structures
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging
    ${CMAKE_HOME_DIRECTORY}/pw_util/math
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core
    ${CMAKE_HOME_DIRECTORY}/pw_system
)

SET(BUFFERS_SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader_program.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/render_target.cpp
    pw_gl_test_buffers.cpp
)

SET(FONT_RENDERING_SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/font_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader_program.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/render_target.cpp
    pw_gl_test_font_rendering.cpp
)

SET(RENDER_TO_TEXTURE_SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_util/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/shader_program.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/core/render_target.cpp
    pw_gl_test_render_to_texture.cpp
)

ADD_EXECUTABLE (pw_gl_test_buffers ${BUFFERS_SRCS})
ADD_EXECUTABLE (pw_gl_test_font_rendering ${FONT_RENDERING_SRCS})
ADD_EXECUTABLE (pw_gl_test_render_to_texture ${RENDER_TO_TEXTURE_SRCS})

TARGET_LINK_LIBRARIES (pw_gl_test_buffers
    ${OPENGL_gl_LIBRARY}
    sfml-system
    sfml-window
    sfml-graphics
)

TARGET_LINK_LIBRARIES (pw_gl_test_font_rendering
    ${OPENGL_gl_LIBRARY}
    sfml-system
    sfml-window
    sfml-graphics
)

TARGET_LINK_LIBRARIES (pw_gl_test_render_to_texture
    ${OPENGL_gl_LIBRARY}
    sfml-system
    sfml-window
    sfml-graphics
)

INSTALL (TARGETS
    pw_gl_test_buffers
    pw_gl_test_font_rendering
    pw_gl_test_render_to_texture
    RUNTIME DESTINATION bin
)
INSTALL (FILES font.vert DESTINATION share/planeworld/data)
INSTALL (FILES font.frag DESTINATION share/planeworld/data)
INSTALL (FILES shader.vert DESTINATION share/planeworld/data)
INSTALL (FILES shader.frag DESTINATION share/planeworld/data)
INSTALL (FILES render_to_screen.frag DESTINATION share/planeworld/data)
INSTALL (FILES render_to_screen.vert DESTINATION share/planeworld/data)
