#ifndef POMEGRANATEENGINE_LINE2D_H
#define POMEGRANATEENGINE_LINE2D_H
#include "math/vector2.h"
#include "ray2d.h"
#include "shape.h"

struct Line2D : Shape {
    Vector2 a;
    Vector2 b;

    Line2D();
    Line2D(Vector2 a, Vector2 b);
    Line2D(float x1, float y1, float x2, float y2);
    Line2D(const Line2D& other);

    Line2D& operator=(const Line2D& other);

    [[nodiscard]] Vector2 direction() const;
    [[nodiscard]] Vector2 normal() const;
    [[nodiscard]] bool contains(const Vector2& point) const override;
    [[nodiscard]] bool intersects(const Line2D& other) const;
    void cast(const Ray2D& ray, Hit2D& hit) const override;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_LINE2D_H
