#include "noise1d.h"

float Noise1D::sample(float u) {
    u += seed;

    // Find unit cell and fractional part
    int i = static_cast<int>(std::floor(u));
    float f = u - i;

    // Generate random values for unit cell corners
    float a = random(i);
    float b = random(i + 1);

    // Cubic interpolation
    float t = smoothstep(f);

    // Linear interpolation
    return mix(a, b, t);
}
