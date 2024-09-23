set(POMEGRANATE_LUA_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/lua_script.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lua_script.h
        ${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/debug.h
)

set(POMEGRANATE_LUA_INCLUDE_DIRS
        ../
)

add_library(PomegranateLua ${POMEGRANATE_LUA_SOURCES})

target_include_directories(PomegranateLua PUBLIC ${POMEGRANATE_LUA_INCLUDE_DIRS})

target_link_libraries(PomegranateLua PomegranateCore)

# You need to put luajit library in external/luajit
target_link_libraries(PomegranateLua ${CMAKE_CURRENT_SOURCE_DIR}/external/luajit/src/libluajit.a)