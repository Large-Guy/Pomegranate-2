#ifndef POMEGRANATEENGINE_CIRCLE_H
#define POMEGRANATEENGINE_CIRCLE_H
#include "vector2.h"
#include "ray2d.h"

struct Circle {
    Vector2 position;
    float radius;

    Circle();
    Circle(Vector2 position, float radius);
    Circle(float x, float y, float radius);
    Circle(const Circle& other);

    Circle& operator=(const Circle& other);

    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Circle& other) const;
    void cast(const Ray2D& ray, Ray2D::Hit& hit) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_CIRCLE_H
