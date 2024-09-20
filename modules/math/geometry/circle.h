#ifndef POMEGRANATEENGINE_CIRCLE_H
#define POMEGRANATEENGINE_CIRCLE_H
#include "math/vector2.h"
#include "ray2d.h"
#include "shape.h"

struct Circle : Shape {
    Vector2 position;
    float radius;

    Circle();
    Circle(Vector2 position, float radius);
    Circle(float x, float y, float radius);
    Circle(const Circle& other);

    Circle& operator=(const Circle& other);

    [[nodiscard]] float area() const;
    [[nodiscard]] bool contains(const Vector2& point) const override;
    [[nodiscard]] bool intersects(const Circle& other) const;
    void cast(const Ray2D& ray, Hit2D& hit) const override;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_CIRCLE_H
