#ifndef POMEGRANATEENGINE_AUDIO_CORE_H
#define POMEGRANATEENGINE_AUDIO_CORE_H

#include<portaudio/portaudio.h>
#include<core/core.h>
#include<cmath>
#include<vector>
#include"sample.h"

struct Sound {
    AudioSample* sample;
    size_t position;
    float volume;
    bool loop;
};

struct AudioData {
    Sound* sounds;
    size_t* numSounds;
};

class Audio {
public:
    static Audio _instance;
public:
    Audio();
    ~Audio();

    static Audio* getInstance();
};

#endif //POMEGRANATEENGINE_CORE_H
