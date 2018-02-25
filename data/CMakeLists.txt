FIND_PACKAGE(Lua REQUIRED)

INSTALL (DIRECTORY ./shader DESTINATION share/planeworld)
INSTALL (DIRECTORY ./fonts DESTINATION share/planeworld)
INSTALL (FILES ./lua/pw_input_default.lua DESTINATION share/lua/${LUA_VERSION_MAJOR}.${LUA_VERSION_MINOR}/planeworld)
