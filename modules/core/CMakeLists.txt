set(POMEGRANATE_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serializable.h
)

set(POMEGRANATE_CORE_INCLUDE_DIRS
        .
)

add_library(PomegranateCore ${POMEGRANATE_CORE_SOURCES})

target_include_directories(PomegranateCore PUBLIC ${POMEGRANATE_CORE_INCLUDE_DIRS})