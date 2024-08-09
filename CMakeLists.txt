cmake_minimum_required(VERSION 3.5)
project(PomegranateEngine)

add_compile_options( -Wreturn-type)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the modules

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/math)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/ecs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/graphics/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/events)

set(SOURCE_FILES
    ${SOURCE_DIR}/main.cpp
)

set(OBJECT_SOURCE_FILES
    ${SOURCE_DIR}/components/tile_map.h
    ${SOURCE_DIR}/components/tile_map.cpp
    ${SOURCE_DIR}/components/tile_set.h
    ${SOURCE_DIR}/components/tile_set.cpp
    ${SOURCE_DIR}/components/tile.h
    ${SOURCE_DIR}/components/tile.cpp
    ${SOURCE_DIR}/components/transform.h
    ${SOURCE_DIR}/components/transform.cpp
)

set(SYSTEM_SOURCE_FILES
    ${SOURCE_DIR}/systems/tile_map.h
    ${SOURCE_DIR}/systems/tile_map.cpp
)

# Set include directories
include_directories(${INCLUDE_DIR})

# Set the executable
add_executable(PomegranateEngine ${SOURCE_FILES} ${OBJECT_SOURCE_FILES} ${SYSTEM_SOURCE_FILES})

# Link the modules
target_link_libraries(PomegranateEngine PomegranateCore)
target_link_libraries(PomegranateEngine PomegranateMath)
target_link_libraries(PomegranateEngine PomegranateECS)
target_link_libraries(PomegranateEngine PomegranateGraphicsGLFW)
target_link_libraries(PomegranateEngine PomegranateEvents)

# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
