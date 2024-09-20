#ifndef POMEGRANATE_ENGINE_MATH_RECT_H
#define POMEGRANATE_ENGINE_MATH_RECT_H
#include "math/vector2.h"
#include "ray2d.h"
#include "shape.h"

struct Rectangle : public Shape {
    Vector2 position = Vector2();
    Vector2 size = Vector2();

    Rectangle();
    Rectangle(Vector2 position, Vector2 size);
    Rectangle(const Rectangle& other);

    Rectangle& operator=(const Rectangle& other);

    [[nodiscard]] Vector2 center() const;
    [[nodiscard]] Vector2 min() const;
    [[nodiscard]] Vector2 max() const;
    [[nodiscard]] bool contains(const Vector2& point) const override;
    [[nodiscard]] bool intersects(const Rectangle& other) const;
    [[nodiscard]] float area() const;
    void cast(const Ray2D& ray, Hit2D& hit) const override;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);
};

template<>
struct std::hash<Rectangle> {
    size_t operator()(const Rectangle& r) const
    {
        return hash<Vector2>()(r.position) ^ hash<Vector2>()(r.size);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_RECT_H
