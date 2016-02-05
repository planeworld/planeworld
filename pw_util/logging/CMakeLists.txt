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

INSTALL (TARGETS log
        LIBRARY DESTINATION lib/planeworld
)

#INSTALL (FILES ${HDRS} DESTINATION include/planeworld)
