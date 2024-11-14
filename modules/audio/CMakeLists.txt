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
)

add_library(PomegranateAudio ${POMEGRANATE_AUDIO_SOURCES})

target_include_directories(PomegranateAudio PUBLIC ${POMEGRANATE_AUDIO_INCLUDE_DIRS} ${POMEGRANATE_MATH_INCLUDE_DIRS} ${POMEGRANATE_CORE_INCLUDE_DIRS})

# we will be using port audio because it is cross platform, bare bones and easy to use
set(PORTAUDIO_INCLUDE_DIRS "/usr/include")
set(PORTAUDIO_LIBRARIES "/usr/lib/x86_64-linux-gnu/libportaudio.so") # Adjust if needed

target_link_libraries(PomegranateAudio ${PORTAUDIO_LIBRARIES} PomegranateMath PomegranateCore)