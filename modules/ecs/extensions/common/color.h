#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
#include <ecs/ecs.h>
#include <math/math.h>

struct Color : public Component {
    Vector4 color;
    Color();
    Color(const Vector4& color);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
