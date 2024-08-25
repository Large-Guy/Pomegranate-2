set(POMEGRANATE_MATH_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2i.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector2i.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vector3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vector4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vector4.h
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix2x2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix2x2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix3x3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix3x3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix4x4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix4x4.h
        ${CMAKE_CURRENT_SOURCE_DIR}/noise1d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/noise1d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/noise2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/noise2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/rect.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rect.h
        ${CMAKE_CURRENT_SOURCE_DIR}/pmath.h
)

set(POMEGRANATE_MATH_INCLUDE_DIRS
        ../
)

add_library(PomegranateMath ${POMEGRANATE_MATH_SOURCES})

target_include_directories(PomegranateMath PUBLIC ${POMEGRANATE_MATH_INCLUDE_DIRS})

target_link_libraries(PomegranateMath PomegranateCore)