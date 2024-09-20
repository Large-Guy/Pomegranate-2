#ifndef POMEGRANATEENGINE_SPHERE_H
#define POMEGRANATEENGINE_SPHERE_H
#include "vector3.h"

struct Sphere {
    Vector3 position;
    float radius;

    Sphere();
    Sphere(Vector3 position, float radius);
    Sphere(float x, float y, float z, float radius);
    Sphere(const Sphere& other);

    Sphere& operator=(const Sphere& other);

    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Sphere& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_SPHERE_H
