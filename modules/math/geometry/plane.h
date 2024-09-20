#ifndef POMEGRANATEENGINE_PLANE_H
#define POMEGRANATEENGINE_PLANE_H
#include "vector3.h"

struct Plane {
    Vector3 normal;

    Plane();
    Plane(Vector3 normal);
    Plane(float x, float y, float z);
    Plane(const Plane& other);

    Plane& operator=(const Plane& other);

    [[nodiscard]] float distance(const Vector3& point) const;
    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Plane& other) const;

    void serialize(Archive& arc) const;
    void deserialize(Archive& arc);
};


#endif //POMEGRANATEENGINE_PLANE_H
