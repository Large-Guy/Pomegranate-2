#ifndef POMEGRANATEENGINE_NOISE2D_H
#define POMEGRANATEENGINE_NOISE2D_H
#include <cmath>
#include <algorithm>
#include "vector2.h"

class Noise2D {
private:
    static inline Vector2 sin(const Vector2& uv) {
        return Vector2(std::sin(uv.x), std::sin(uv.y));
    }
    static inline Vector2 fract(const Vector2& uv) {
        return Vector2(uv.x - std::floor(uv.x), uv.y - std::floor(uv.y));
    }
    static inline Vector2 smoothstep(const Vector2& uv) {
        return Vector2(
                uv.x * uv.x * (3.0f - 2.0f * uv.x),
                uv.y * uv.y * (3.0f - 2.0f * uv.y)
        );
    }
    static inline Vector2 mix(const Vector2& a, const Vector2& b, float t) {
        return Vector2(a.x * (1.0f - t) + b.x * t, a.y * (1.0f - t) + b.y * t);
    }
    static inline float mix(float a, float b, float t) {
        return (1.0f - t) * a + t * b;
    }
    static inline Vector2 floor(const Vector2& uv) {
        return Vector2(std::floor(uv.x), std::floor(uv.y));
    }
    static inline float dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }
    static inline Vector2 random(Vector2 uv) {
        uv = {dot(uv,Vector2(127.1f,311.7f)),
              dot(uv,Vector2(269.5f,183.3f))
        };
        return Vector2(-1.0f) + fract(sin(uv) * 43758.5453f) * 2.0f;
    }
public:
    int seed = 0;
    float sample(Vector2 u);
};


#endif //POMEGRANATEENGINE_NOISE2D_H
