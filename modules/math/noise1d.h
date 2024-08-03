#ifndef POMEGRANATEENGINE_NOISE1D_H
#define POMEGRANATEENGINE_NOISE1D_H
#include <cmath>
#include <algorithm>

class Noise1D {
private:
    inline static float random(float u) {
        return std::fmod(std::sin(u)*43758.5453f, 1.0f);
    }
    inline static float mix(float a, float b, float t) {
        return (1.0f-t)*a + t*b;
    }
    inline static float smoothstep(float t) {
        return t * t * (3.0f - 2.0f * t);
    }
public:
    int seed = 0;
    float sample(float u);
};


#endif //POMEGRANATEENGINE_NOISE1D_H
