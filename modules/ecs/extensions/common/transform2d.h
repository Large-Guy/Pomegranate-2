#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
#include<ecs/ecs.h>
#include <math/math.h>

struct Transform2D : public Component
{
private:
    bool _initalized;
    Matrix3x3 _matrix;
    Vector2 _position;
    Vector2 _scale;
    float _rotation;
public:
    Vector2 position;
    Vector2 scale;
    float rotation;
    Transform2D();
    Transform2D(const Vector2& position, const Vector2& scale, float rotation);
    [[nodiscard]] Matrix3x3 matrix();
};

#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
