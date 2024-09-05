set(POMEGRANATE_GRAPHICS_VULKAN_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/graphics.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/window.h
        ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
)

set(POMEGRANATE_GRAPHICS_VULKAN_INCLUDE_DIRS
        ../
)

add_library(PomegranateVulkan ${POMEGRANATE_GRAPHICS_VULKAN_SOURCES})

target_include_directories(PomegranateVulkan PUBLIC ${POMEGRANATE_GRAPHICS_VULKAN_INCLUDE_DIRS} ${POMEGRANATE_MATH_INCLUDE_DIRS} ${POMEGRANATE_CORE_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)


target_link_libraries(PomegranateVulkan glfw Vulkan::Vulkan PomegranateMath PomegranateCore)