#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
#include <ecs/ecs.h>
#include <math/math.h>

struct Color : public Reflectable {
    Vector4 color;
    Color();
    Color(const Vector4& color);
    Color(const Color& other);
    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_COLOR_H
