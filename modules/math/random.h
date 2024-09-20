#ifndef POMEGRANATEENGINE_RANDOM_H
#define POMEGRANATEENGINE_RANDOM_H
#include <chrono>

struct Random {
    long seed;
    Random();
    Random(long seed);

    float randomFloat(); // 0.0f to 1.0f
    float randomFloat(float min, float max);
    int randomInt(int min, int max);
    bool randomBool();
};


#endif //POMEGRANATEENGINE_RANDOM_H
