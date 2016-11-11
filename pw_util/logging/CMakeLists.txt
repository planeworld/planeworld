INCLUDE_DIRECTORIES(.)

SET(HDRS
    conf_log.h
    log.h
    log_defines.h
    timer.h
)

SET(SRCS
    log.cpp
    timer.cpp
)

ADD_LIBRARY (log SHARED ${SRCS})

IF(WIN32)
    INSTALL (TARGETS log
        RUNTIME DESTINATION lib/planeworld)
ELSE()
    INSTALL (TARGETS log
        LIBRARY DESTINATION lib/planeworld)
ENDIF()

#INSTALL (FILES ${HDRS} DESTINATION include/planeworld)
