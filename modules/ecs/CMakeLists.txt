option(ENABLE_ECS_EXTENSIONS "Enable ECS extensions" ON)

set(POMEGRANATE_ECS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/entity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/archetype.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/archetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ecs_typedefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/component.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.h
        ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
)

if(ENABLE_ECS_EXTENSIONS)
set(POMEGRANATE_ECS_SOURCES ${POMEGRANATE_ECS_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/transform2d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/transform2d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/transform3d.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/transform3d.h
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/name.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/name.h
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/color.h
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/hierarchy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/extensions/common/hierarchy.h
)
endif ()

set(POMEGRANATE_ECS_INCLUDE_DIRS
        ../
)

add_library(PomegranateECS ${POMEGRANATE_ECS_SOURCES})

target_include_directories(PomegranateECS PUBLIC ${POMEGRANATE_ECS_INCLUDE_DIRS})

target_link_libraries(PomegranateECS PomegranateCore)