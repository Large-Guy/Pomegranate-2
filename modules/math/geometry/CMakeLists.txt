set(POMEGRANATE_MATH_GEOMETRY_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/rectangle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rectangle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/circle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/circle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cube.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cube.h
        ${CMAKE_CURRENT_SOURCE_DIR}/geometry.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sphere.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sphere.h
        ${CMAKE_CURRENT_SOURCE_DIR}/segment2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/segment2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/segment3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/segment3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ray2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ray2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/triangle2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/triangle2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/triangle3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/triangle3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/line2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/line2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/line3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/line3d.h
        plane.cpp
        plane.h
        curve2d.cpp
        curve2d.h
        curve3d.cpp
        curve3d.h
)

set(POMEGRANATE_MATH_GEOMETRY_INCLUDE_DIRS
        ../
)

add_library(PomegranateGeometry ${POMEGRANATE_MATH_GEOMETRY_SOURCES})

target_include_directories(PomegranateGeometry PUBLIC ${POMEGRANATE_MATH_GEOMETRY_INCLUDE_DIRS})

target_link_libraries(PomegranateGeometry PomegranateCore PomegranateMath)