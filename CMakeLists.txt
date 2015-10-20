PROJECT (planeworld)
cmake_minimum_required(VERSION 3.1)

ADD_SUBDIRECTORY(pw_util)
ADD_SUBDIRECTORY(src)

SET(CMAKE_CXX_FLAGS "-std=c++11 -mtune=native" CACHE STRING "" FORCE)

INSTALL (DIRECTORY ./data DESTINATION share/planeworld/)
INSTALL (FILES SETENV DESTINATION .)
