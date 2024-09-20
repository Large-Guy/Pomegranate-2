#ifndef POMEGRANATEENGINE_SEGMENT2D_H
#define POMEGRANATEENGINE_SEGMENT2D_H
#include "math/vector2.h"
#include "ray2d.h"
#include "cmath"
#include "shape.h"

struct Segment2D : public Shape {
    Vector2 start;
    Vector2 end;

    Segment2D();
    Segment2D(Vector2 start, Vector2 end);
    Segment2D(float x1, float y1, float x2, float y2);
    Segment2D(const Segment2D& other);

    Segment2D& operator=(const Segment2D& other);

    [[nodiscard]] Vector2 direction() const;
    [[nodiscard]] Vector2 center() const;
    [[nodiscard]] float length() const;
    [[nodiscard]] Vector2 normal() const;
    [[nodiscard]] Vector2 pointAt(float t) const;
    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Segment2D& other) const;
    void cast(const Ray2D& ray, Hit2D& hit) const override;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_SEGMENT2D_H
