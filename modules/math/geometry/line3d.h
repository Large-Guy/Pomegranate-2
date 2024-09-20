#ifndef POMEGRANATEENGINE_LINE3D_H
#define POMEGRANATEENGINE_LINE3D_H
#include "math/vector3.h"

struct Line3D {
    Vector3 a;
    Vector3 b;

    Line3D();
    Line3D(Vector3 a, Vector3 b);
    Line3D(float x1, float y1, float z1, float x2, float y2, float z2);
    Line3D(const Line3D& other);

    Line3D& operator=(const Line3D& other);

    [[nodiscard]] Vector3 direction() const;
    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Line3D& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_LINE3D_H
