#ifndef POMEGRANATE_ENGINE_MATH_RECT_H
#define POMEGRANATE_ENGINE_MATH_RECT_H
#include "vector2.h"

struct Rect : public Serializable {
    Vector2 position = Vector2();
    Vector2 size = Vector2();

    Rect();
    Rect(Vector2 position, Vector2 size);
    Rect(float x, float y, float w, float h);
    Rect(const Rect& other);

    Rect& operator=(const Rect& other);

    [[nodiscard]] Vector2 center() const;
    [[nodiscard]] Vector2 min() const;
    [[nodiscard]] Vector2 max() const;
    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Rect& other) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<Rect> {
    size_t operator()(const Rect& r) const
    {
        return hash<Vector2>()(r.position) ^ hash<Vector2>()(r.size);
    }
};

#endif //POMEGRANATE_ENGINE_MATH_RECT_H
