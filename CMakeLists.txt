PROJECT (planeworld)
cmake_minimum_required(VERSION 2.6)

ADD_SUBDIRECTORY(pw_core)
ADD_SUBDIRECTORY(pw_graphics)
ADD_SUBDIRECTORY(src)

INSTALL (DIRECTORY ./data DESTINATION share/planeworld/)
# install (DIRECTORY ./doc DESTINATION . )
