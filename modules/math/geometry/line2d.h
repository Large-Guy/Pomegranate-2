#ifndef POMEGRANATEENGINE_LINE2D_H
#define POMEGRANATEENGINE_LINE2D_H
#include "vector2.h"
#include "ray2d.h"

//This represents a 2D line that extends infinitely in both directions.
struct Line2D {
    Vector2 a;
    Vector2 b;

    Line2D();
    Line2D(Vector2 a, Vector2 b);
    Line2D(float x1, float y1, float x2, float y2);
    Line2D(const Line2D& other);

    Line2D& operator=(const Line2D& other);

    [[nodiscard]] Vector2 direction() const;
    [[nodiscard]] Vector2 normal() const;
    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Line2D& other) const;
    void cast(const Ray2D& ray, Ray2D::Hit& hit) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_LINE2D_H
