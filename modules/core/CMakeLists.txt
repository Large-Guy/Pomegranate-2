set(POMEGRANATE_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/asset.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/asset.h
        ${CMAKE_CURRENT_SOURCE_DIR}/asset_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/asset_manager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/range.h
        ${CMAKE_CURRENT_SOURCE_DIR}/range.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/list.h
        ${CMAKE_CURRENT_SOURCE_DIR}/hash_table.h
        ${CMAKE_CURRENT_SOURCE_DIR}/table.h
        ${CMAKE_CURRENT_SOURCE_DIR}/table.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/debug.h
        ${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/file.h
        ${CMAKE_CURRENT_SOURCE_DIR}/file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/directory.h
        ${CMAKE_CURRENT_SOURCE_DIR}/directory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/reflectable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/reflectable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.h
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.cpp
)

set(POMEGRANATE_CORE_INCLUDE_DIRS
        ../
)

add_library(PomegranateCore ${POMEGRANATE_CORE_SOURCES})

target_include_directories(PomegranateCore PUBLIC ${POMEGRANATE_CORE_INCLUDE_DIRS})