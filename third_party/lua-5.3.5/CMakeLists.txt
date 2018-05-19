file(GLOB LUA_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(lua ${LUA_SRC_FILES})
