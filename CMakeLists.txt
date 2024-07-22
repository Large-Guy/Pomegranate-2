cmake_minimum_required(VERSION 3.5)
project(PomegranateEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the modules

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/math)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/ecs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/graphics/glfw)


set(SOURCE_FILES
    ${SOURCE_DIR}/main.cpp
)

# Set include directories
include_directories(${INCLUDE_DIR})

# Set the executable
add_executable(PomegranateEngine ${SOURCE_FILES})

# Link the modules
target_link_libraries(PomegranateEngine PomegranateCore)
target_link_libraries(PomegranateEngine PomegranateMath)
target_link_libraries(PomegranateEngine PomegranateECS)
target_link_libraries(PomegranateEngine PomegranateGraphicsGLFW)



# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
