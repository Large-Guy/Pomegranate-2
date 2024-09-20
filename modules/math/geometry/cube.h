#ifndef POMEGRANATEENGINE_CUBE_H
#define POMEGRANATEENGINE_CUBE_H
#include "math/vector3.h"

struct Cube {
    Vector3 position;
    Vector3 size;

    Cube();
    Cube(Vector3 position, Vector3 size);
    Cube(float x, float y, float z, float w, float h, float d);

    Cube& operator=(const Cube& other);

    [[nodiscard]] Vector3 center() const;
    [[nodiscard]] Vector3 min() const;
    [[nodiscard]] Vector3 max() const;
    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Cube& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_CUBE_H
