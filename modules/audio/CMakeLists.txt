set(POMEGRANATE_AUDIO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/audio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sample.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core.h

)

set(POMEGRANATE_AUDIO_INCLUDE_DIRS
        ../
        ${CMAKE_CURRENT_SOURCE_DIR}/external/
)

add_library(PomegranateAudio ${POMEGRANATE_AUDIO_SOURCES})

target_include_directories(PomegranateAudio PUBLIC ${POMEGRANATE_AUDIO_INCLUDE_DIRS} ${POMEGRANATE_MATH_INCLUDE_DIRS} ${POMEGRANATE_CORE_INCLUDE_DIRS})

# we will be using port audio because it is cross platform, bare bones and easy to use
if (APPLE)
    set(PORTAUDIO_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/external/portaudio/libportaudio.dylib) # Adjust if needed
endif (APPLE)

target_link_libraries(PomegranateAudio ${PORTAUDIO_LIBRARIES} PomegranateMath PomegranateCore)