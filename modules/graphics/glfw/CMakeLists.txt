set(POMEGRANATE_GRAPHICS_GLFW_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/window.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphics_core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graphics_core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/glad.c
)

set(POMEGRANATE_GRAPHICS_GLFW_INCLUDE_DIRS
        .
)

add_library(PomegranateGraphicsGLFW ${POMEGRANATE_GRAPHICS_GLFW_SOURCES})

target_include_directories(PomegranateGraphicsGLFW PUBLIC ${POMEGRANATE_GRAPHICS_GLFW_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


target_link_libraries(PomegranateGraphicsGLFW glfw OpenGL::GL PomegranateMath)