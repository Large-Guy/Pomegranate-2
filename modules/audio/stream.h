#ifndef POMEGRANATEENGINE_STREAM_H
#define POMEGRANATEENGINE_STREAM_H

#include "core.h"
#include<portaudio/portaudio.h>

class Stream {
    PaStream *stream;
    Function customCallback;
public:
    struct CallbackInfo {
        float* input;
        float* output;
        unsigned long frameCount;
        float time;
        float frameDeltaTime;
        int sampleRate;
        int channels;
        void* userData;
    };

    Stream();
    ~Stream();

    void start();
    void stop();

    Function& setCustomCallback(Function callback);
    Function& getCustomCallback();
};


#endif //POMEGRANATEENGINE_STREAM_H
