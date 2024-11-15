#include "stream.h"

static int callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    static double time = 0.0;

    Stream::CallbackInfo info{
        .input = (float *)inputBuffer,
        .output = (float *)outputBuffer,
        .frameCount = framesPerBuffer,
        .time = time,
        .frameDeltaTime = 1.0 / 44100.0,
        .sampleRate = 44100,
        .channels = 2,
        .userData = userData,
        .frame = 0
    };

    auto *stream = (Stream *)userData;

    for (int i = 0; i < framesPerBuffer; ++i) {
        time += info.frameDeltaTime;
        info.frame = i;
        info.time = time;
        stream->getCustomCallback().call<void>(info);
    }

    return paContinue;
}

Stream::Stream() {
    stream = nullptr;
}

Stream::~Stream() {
    stop();
}

void Stream::start() {
    if (stream == nullptr) {
        Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, callback, this);
        Pa_StartStream(stream);
    }
}

void Stream::stop() {
    if (stream != nullptr) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
    }
}

Function& Stream::setCustomCallback(Function callback) {
    customCallback = callback;
    return customCallback;
}

Function& Stream::getCustomCallback() {
    return customCallback;
}