SET(CMAKE_CXX_FLAGS "-std=c++14 -mtune=native -Wall -pedantic" CACHE STRING "")

PROJECT (planeworld)
cmake_minimum_required(VERSION 3.1)

ADD_SUBDIRECTORY(pw_util)
ADD_SUBDIRECTORY(src)


INSTALL (DIRECTORY ./data DESTINATION share/planeworld)
#INSTALL (FILES SETENV DESTINATION .)
