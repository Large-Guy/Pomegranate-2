set(POMEGRANATE_ECS_EXTENSIONS_COMMON_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/transform2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/transform2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/transform3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/transform3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/name.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/name.h
        ${CMAKE_CURRENT_SOURCE_DIR}/color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/color.h
        ${CMAKE_CURRENT_SOURCE_DIR}/hierarchy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hierarchy.h
)

add_library(PomegranateECSExtensionsCommon ${POMEGRANATE_ECS_EXTENSIONS_COMMON_SOURCES})

target_include_directories(PomegranateECSExtensionsCommon PUBLIC ${POMEGRANATE_ECS_EXTENSIONS_COMMON_INCLUDE_DIRS})

target_link_libraries(PomegranateECSExtensionsCommon PomegranateCore)