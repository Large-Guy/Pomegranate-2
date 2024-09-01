set(POMEGRANATE_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.h
        ${CMAKE_CURRENT_SOURCE_DIR}/resource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/resource.h
        ${CMAKE_CURRENT_SOURCE_DIR}/range.h
        ${CMAKE_CURRENT_SOURCE_DIR}/range.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/list.h
        ${CMAKE_CURRENT_SOURCE_DIR}/hash_table.h
        ${CMAKE_CURRENT_SOURCE_DIR}/table.h
        ${CMAKE_CURRENT_SOURCE_DIR}/table.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string.h
        ${CMAKE_CURRENT_SOURCE_DIR}/string.cpp
)

set(POMEGRANATE_CORE_INCLUDE_DIRS
        ../
)

add_library(PomegranateCore ${POMEGRANATE_CORE_SOURCES})

target_include_directories(PomegranateCore PUBLIC ${POMEGRANATE_CORE_INCLUDE_DIRS})