set(POMEGRANATE_EVENTS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/event_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/event_manager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/question_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/question_manager.h
        ${CMAKE_CURRENT_SOURCE_DIR}/event_typedefs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/events.h
)

set(POMEGRANATE_EVENTS_INCLUDE_DIRS
        ../
)

add_library(PomegranateEvents ${POMEGRANATE_EVENTS_SOURCES})

target_include_directories(PomegranateEvents PUBLIC ${POMEGRANATE_EVENTS_INCLUDE_DIRS})

target_link_libraries(PomegranateEvents PomegranateCore)