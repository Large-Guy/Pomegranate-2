#include "core.h"

Audio Audio::_instance{};

Audio* Audio::getInstance() {
    return &_instance;
}

Audio::Audio() {
    PaError err = Pa_Initialize();
    if(err != paNoError) {
        // Handle error
        Debug::Log::error("Failed to initialize PortAudio");
        return;
    }

    Debug::Log::pass("PortAudio initialized");
}

Audio::~Audio() {
    Pa_Terminate();
}