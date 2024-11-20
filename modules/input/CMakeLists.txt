set(POMEGRANATE_INPUT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/input.h
        ${CMAKE_CURRENT_SOURCE_DIR}/enumerations.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/enumerations.h
        ${CMAKE_CURRENT_SOURCE_DIR}/button.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/button.h
        ${CMAKE_CURRENT_SOURCE_DIR}/axis.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/axis.h
        ${CMAKE_CURRENT_SOURCE_DIR}/keyboard.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/keyboard.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mouse.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mouse.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gamepad.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gamepad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/alias.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/input_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/input_manager.h
)

set(POMEGRANATE_INPUT_INCLUDE_DIRS
        ../
)

add_library(PomegranateInput ${POMEGRANATE_INPUT_SOURCES})

target_include_directories(PomegranateInput PUBLIC ${POMEGRANATE_INPUT_INCLUDE_DIRS} ${POMEGRANATE_GRAPHICS_SOURCES})

find_package(glfw3 REQUIRED)

target_link_libraries(PomegranateInput glfw PomegranateMath PomegranateCore PomegranateGraphics)