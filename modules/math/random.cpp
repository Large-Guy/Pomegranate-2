#include "random.h"

Random::Random() {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
}

Random::Random(long seed) {
    this->seed = seed;
}

float Random::randomFloat() {
    int x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    seed++;
    return (float)((x % 1000000) / 1000000.0);
}

float Random::randomFloat(float min, float max) {
    return randomFloat() * (max - min) + min;
}

int Random::randomInt(int min, int max) {
    return (int)randomFloat(min, max + 1);
}

bool Random::randomBool() {
    return randomInt(0, 1) == 1;
}