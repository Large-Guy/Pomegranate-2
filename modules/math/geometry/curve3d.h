#ifndef POMEGRANATEENGINE_CURVE3D_H
#define POMEGRANATEENGINE_CURVE3D_H
#include "vector3.h"
#include "core/list.h"

struct Curve3D {
    List<Vector3> points;

    Curve3D();
    Curve3D(const List<Vector3>& points);
    Curve3D(const Curve3D& other);

    Curve3D& operator=(const Curve3D& other);

    [[nodiscard]] Vector3 bezier(float t) const;
    [[nodiscard]] float linearLength() const;
    [[nodiscard]] float bezierLength() const;

    void serialize(Archive& arc) const;
    void deserialize(Archive& arc);
};


#endif //POMEGRANATEENGINE_CURVE3D_H
