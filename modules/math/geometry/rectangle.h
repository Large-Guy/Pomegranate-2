#ifndef POMEGRANATE_ENGINE_MATH_RECT_H
#define POMEGRANATE_ENGINE_MATH_RECT_H
#include "vector2.h"
#include "ray2d.h"

struct Rectangle {
    Vector2 position = Vector2();
    Vector2 size = Vector2();

    Rectangle();
    Rectangle(Vector2 position, Vector2 size);
    Rectangle(const Rectangle& other);

    Rectangle& operator=(const Rectangle& other);

    [[nodiscard]] Vector2 center() const;
    [[nodiscard]] Vector2 min() const;
    [[nodiscard]] Vector2 max() const;
    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Rectangle& other) const;
    void cast(const Ray2D& ray, Ray2D::Hit& hit) const;

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
