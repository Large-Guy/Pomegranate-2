#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
#include<ecs/ecs.h>
#include <math/math.h>
#include "hierarchy.h"

struct Transform2D : public Reflectable
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
    Transform2D(const Transform2D& other);
    static Vector2 getPosition(Entity& entity);
    static Vector2 getScale(Entity& entity);
    static float getRotation(Entity& entity);
    static Matrix3x3 getMatrix(Entity& entity);
    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};

#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM2D_H
