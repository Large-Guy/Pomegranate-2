#ifndef POMEGRANATEENGINE_CURVE2D_H
#define POMEGRANATEENGINE_CURVE2D_H
#include "vector2.h"
#include "core/list.h"
#include "ray2d.h"

struct Curve2D {
    List<Vector2> points;

    Curve2D();
    Curve2D(const List<Vector2>& points);
    Curve2D(const Curve2D& other);

    Curve2D& operator=(const Curve2D& other);

    [[nodiscard]] Vector2 bezier(float t) const;
    [[nodiscard]] float linearLength() const;
    [[nodiscard]] float bezierLength() const;
    void cast(const Ray2D& ray, Ray2D::Hit& hit) const;

    void serialize(Archive& arc) const;
    void deserialize(Archive& arc);
};


#endif //POMEGRANATEENGINE_CURVE2D_H
