#ifndef POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM3D_H
#define POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM3D_H
#include<ecs/ecs.h>
#include <math/math.h>
#include "hierarchy.h"

struct Transform3D : public Reflectable
{
    Vector3 position;
    Vector3 scale;
    Vector3 rotation;
    Transform3D();
    Transform3D(const Vector3& position, const Vector3& scale, const Vector3& rotation);
    Transform3D(const Transform3D& other);

    Matrix4x4 getLocalMatrix() const;

    static Vector3 getPosition(Entity& entity);
    static Vector3 getScale(Entity& entity);
    static Vector3 getRotation(Entity& entity);
    static Matrix4x4 getMatrix(Entity& entity);
    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATE_ENGINE_ECS_EXTENSIONS_COMMON_TRANSFORM3D_H
