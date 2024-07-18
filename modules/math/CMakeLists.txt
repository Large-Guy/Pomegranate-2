set(POMEGRANATE_MATH_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2.h
)

set(POMEGRANATE_MATH_INCLUDE_DIRS
        .
)

add_library(PomegranateMath ${POMEGRANATE_MATH_SOURCES})

target_include_directories(PomegranateMath PUBLIC ${POMEGRANATE_MATH_INCLUDE_DIRS})

target_link_libraries(PomegranateMath PomegranateCore)