set(POMEGRANATE_ECS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/entity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/archetype.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/archetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ecs_typedefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/component.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ecs.h
)

set(POMEGRANATE_ECS_INCLUDE_DIRS
        ../
)

add_library(PomegranateECS ${POMEGRANATE_ECS_SOURCES})

target_include_directories(PomegranateECS PUBLIC ${POMEGRANATE_ECS_INCLUDE_DIRS})

target_link_libraries(PomegranateECS PomegranateCore)