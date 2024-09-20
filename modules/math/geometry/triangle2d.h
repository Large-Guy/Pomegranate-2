#ifndef POMEGRANATEENGINE_TRIANGLE2D_H
#define POMEGRANATEENGINE_TRIANGLE2D_H
#include "math/vector2.h"
#include "ray2d.h"
#include "segment2d.h"
#include "shape.h"

struct Triangle2D : public Shape {
    Vector2 a;
    Vector2 b;
    Vector2 c;

    Triangle2D();
    Triangle2D(Vector2 a, Vector2 b, Vector2 c);
    Triangle2D(float x1, float y1, float x2, float y2, float x3, float y3);
    Triangle2D(const Triangle2D& other);

    Triangle2D& operator=(const Triangle2D& other);

    [[nodiscard]] float area() const;
    [[nodiscard]] Vector2 center() const;
    [[nodiscard]] bool contains(const Vector2& point) const override;
    [[nodiscard]] bool intersects(const Triangle2D& other) const;
    void cast(const Ray2D& ray, Hit2D& hit) const override;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_TRIANGLE2D_H
