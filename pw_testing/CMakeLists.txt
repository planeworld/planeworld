PROJECT (planeworld)

FIND_PACKAGE(SDL REQUIRED)

INCLUDE_DIRECTORIES (
    ${SDL_INCLUDE_DIR}
    ${CMAKE_HOME_DIRECTORY}/include
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging/log.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_core/logging/timer.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/circle.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/polyline.cpp
    ${CMAKE_HOME_DIRECTORY}/pw_graphics/shapes/rectangle.cpp

    ${CMAKE_HOME_DIRECTORY}/src/adams_bashforth_integrator.cpp
    ${CMAKE_HOME_DIRECTORY}/src/adams_moulton_integrator.cpp
    ${CMAKE_HOME_DIRECTORY}/src/bounding_box.cpp
    ${CMAKE_HOME_DIRECTORY}/src/body.cpp
    ${CMAKE_HOME_DIRECTORY}/src/circle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/src/collision_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/src/engine_manager.cpp
    ${CMAKE_HOME_DIRECTORY}/src/euler_integrator.cpp
    ${CMAKE_HOME_DIRECTORY}/src/geometry.cpp
    ${CMAKE_HOME_DIRECTORY}/src/graphics.cpp
    ${CMAKE_HOME_DIRECTORY}/src/object.cpp
    ${CMAKE_HOME_DIRECTORY}/src/polyline_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/src/rectangle_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/src/rigidbody.cpp
    ${CMAKE_HOME_DIRECTORY}/src/spring.cpp
    ${CMAKE_HOME_DIRECTORY}/src/spring_visuals.cpp
    ${CMAKE_HOME_DIRECTORY}/src/visuals_manager.cpp

    integration_test.cpp
)

ADD_EXECUTABLE (integration_test ${SRCS})

# ADD_DEPENDENCIES(integration_test liblog.so)

TARGET_LINK_LIBRARIES (integration_test
    ${SDL_LIBRARY}
    ${OPENGL_gl_LIBRARY}
#   liblog.so
)

INSTALL (TARGETS integration_test
    RUNTIME DESTINATION bin
)
