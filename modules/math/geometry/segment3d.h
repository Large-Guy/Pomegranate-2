#ifndef POMEGRANATEENGINE_SEGMENT3D_H
#define POMEGRANATEENGINE_SEGMENT3D_H
#include "math/vector3.h"

struct Segment3D {
    Vector3 start;
    Vector3 end;

    Segment3D();
    Segment3D(Vector3 start, Vector3 end);
    Segment3D(float x1, float y1, float z1, float x2, float y2, float z2);
    Segment3D(const Segment3D& other);

    Segment3D& operator=(const Segment3D& other);

    [[nodiscard]] Vector3 direction() const;
    [[nodiscard]] Vector3 center() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector3 pointAt(float t) const;
    [[nodiscard]] bool contains(const Vector3& point) const;
    [[nodiscard]] bool intersects(const Segment3D& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_SEGMENT3D_H
