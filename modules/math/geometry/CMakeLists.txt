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
        ${CMAKE_CURRENT_SOURCE_DIR}/plane.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/plane.h
        ${CMAKE_CURRENT_SOURCE_DIR}/curve2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/curve2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/curve3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/curve3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/shape.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/shape.h
)

add_library(PomegranateMathGeometry ${POMEGRANATE_MATH_GEOMETRY_SOURCES})

target_include_directories(PomegranateMathGeometry PUBLIC ${POMEGRANATE_MATH_GEOMETRY_INCLUDE_DIRS})

target_link_libraries(PomegranateMathGeometry PomegranateCore PomegranateMath)