SET(CMAKE_CXX_FLAGS "-std=c++14 -mtune=native -Wall -Wextra -pedantic" CACHE STRING "")

PROJECT (planeworld)
cmake_minimum_required(VERSION 3.1)

ADD_SUBDIRECTORY(pw_util)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(testing)


INSTALL (DIRECTORY ./data DESTINATION share/planeworld)
#INSTALL (FILES SETENV DESTINATION .)
