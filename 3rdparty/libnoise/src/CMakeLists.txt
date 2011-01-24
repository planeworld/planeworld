INCLUDE_DIRECTORIES (
    ${CMAKE_HOME_DIRECTORY}/src/model
    ${CMAKE_HOME_DIRECTORY}/src/module
    ${CMAKE_HOME_DIRECTORY}/src
)

SET(HDRS
    ${CMAKE_HOME_DIRECTORY}/src/basictypes.h
    ${CMAKE_HOME_DIRECTORY}/src/exception.h
    ${CMAKE_HOME_DIRECTORY}/src/interp.h
    ${CMAKE_HOME_DIRECTORY}/src/latlon.h
    ${CMAKE_HOME_DIRECTORY}/src/mathconsts.h
    ${CMAKE_HOME_DIRECTORY}/src/misc.h
    ${CMAKE_HOME_DIRECTORY}/src/noisegen.h
    ${CMAKE_HOME_DIRECTORY}/src/noise.h
    ${CMAKE_HOME_DIRECTORY}/src/vectortable.h
)
SET(MODEL_HDRS
    ${CMAKE_HOME_DIRECTORY}/src/model/cylinder.h
    ${CMAKE_HOME_DIRECTORY}/src/model/model.h
    ${CMAKE_HOME_DIRECTORY}/src/model/sphere.h
    ${CMAKE_HOME_DIRECTORY}/src/model/line.h 
    ${CMAKE_HOME_DIRECTORY}/src/model/plane.h 
)
SET(MODULE_HDRS
    ${CMAKE_HOME_DIRECTORY}/src/module/abs.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/add.h
    ${CMAKE_HOME_DIRECTORY}/src/module/billow.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/blend.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/cache.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/checkerboard.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/clamp.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/const.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/curve.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/cylinders.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/displace.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/exponent.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/invert.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/max.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/min.h
    ${CMAKE_HOME_DIRECTORY}/src/module/module.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/modulebase.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/multiply.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/perlin.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/power.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/ridgedmulti.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/rotatepoint.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/scalebias.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/scalepoint.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/select.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/spheres.h
    ${CMAKE_HOME_DIRECTORY}/src/module/terrace.h
    ${CMAKE_HOME_DIRECTORY}/src/module/translatepoint.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/turbulence.h 
    ${CMAKE_HOME_DIRECTORY}/src/module/voronoi.h
)

SET(SRCS
    ${CMAKE_HOME_DIRECTORY}/src/latlon.cpp
    ${CMAKE_HOME_DIRECTORY}/src/noisegen.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/model/cylinder.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/model/line.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/model/plane.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/model/sphere.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/abs.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/add.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/billow.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/blend.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/cache.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/checkerboard.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/clamp.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/const.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/curve.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/cylinders.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/displace.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/exponent.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/invert.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/max.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/min.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/modulebase.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/multiply.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/perlin.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/power.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/ridgedmulti.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/rotatepoint.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/scalebias.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/scalepoint.cpp
    ${CMAKE_HOME_DIRECTORY}/src/module/select.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/spheres.cpp
    ${CMAKE_HOME_DIRECTORY}/src/module/terrace.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/translatepoint.cpp 
    ${CMAKE_HOME_DIRECTORY}/src/module/turbulence.cpp
    ${CMAKE_HOME_DIRECTORY}/src/module/voronoi.cpp 
)

ADD_LIBRARY (noise SHARED ${SRCS})

INSTALL (TARGETS noise
        LIBRARY DESTINATION lib
)

INSTALL (FILES ${HDRS} DESTINATION include/noise)
INSTALL (FILES ${MODEL_HDRS} DESTINATION include/noise/model)
INSTALL (FILES ${MODULE_HDRS} DESTINATION include/noise/module)
