#include "noise2d.h"

float Noise2D::sample(Vector2 uv) {
    uv = uv + seed;
    Vector2 uv_index = floor(uv);
    Vector2 uv_fract = fract(uv);

    Vector2 blur = smoothstep(uv_fract);

    return mix( mix( dot( random(uv_index ), uv_fract ),
                     dot( random(uv_index + Vector2(1.0,0.0) ), uv_fract - Vector2(1.0,0.0) ), blur.x),
                mix( dot( random(uv_index + Vector2(0.0,1.0) ), uv_fract - Vector2(0.0,1.0) ),
                     dot( random(uv_index + Vector2(1.0,1.0) ), uv_fract - Vector2(1.0,1.0) ), blur.x), blur.y) + 0.5f;
}