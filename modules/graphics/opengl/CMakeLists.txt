set(POMEGRANATE_GRAPHICS_OPEN_GL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/graphics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/window.h
        ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/enumerations.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/enumerations.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mesh.h
)

set(POMEGRANATE_GRAPHICS_OPEN_GL_GLAD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/glad/glad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/KHR/khrplatform.h
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
)

set(POMEGRANATE_GRAPHICS_OPEN_GL_INCLUDE_DIRS
        ../
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

add_library(PomegranateGL ${POMEGRANATE_GRAPHICS_OPEN_GL_SOURCES} ${POMEGRANATE_GRAPHICS_OPEN_GL_GLAD_SOURCES})

target_include_directories(PomegranateGL PUBLIC ${POMEGRANATE_GRAPHICS_OPEN_GL_INCLUDE_DIRS} ${POMEGRANATE_MATH_INCLUDE_DIRS} ${POMEGRANATE_CORE_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)


target_link_libraries(PomegranateGL glfw Vulkan::Vulkan PomegranateMath PomegranateCore)