#ifndef POMEGRANATEENGINE_TRIANGLE3D_H
#define POMEGRANATEENGINE_TRIANGLE3D_H
#include "math/vector3.h"

class Triangle3D {
public:
    Vector3 a;
    Vector3 b;
    Vector3 c;

    Triangle3D();
    Triangle3D(Vector3 a, Vector3 b, Vector3 c);
    Triangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    Triangle3D(const Triangle3D& other);

    Triangle3D& operator=(const Triangle3D& other);

    [[nodiscard]] Vector3 normal() const;
    [[nodiscard]] Vector3 center() const;
    [[nodiscard]] float area() const;
    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Triangle3D& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_TRIANGLE3D_H
