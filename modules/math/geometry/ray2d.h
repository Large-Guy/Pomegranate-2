#ifndef POMEGRANATEENGINE_RAY2D_H
#define POMEGRANATEENGINE_RAY2D_H
#include "math/vector2.h"
#include "shape.h"

//Ray casting
struct Hit2D {
    bool hit;
    Vector2 point;
    float distance;
    Vector2 normal;
};

struct Ray2D {

    Vector2 origin;
    Vector2 direction;

    Ray2D();
    Ray2D(Vector2 origin, Vector2 direction);
    Ray2D(float x1, float y1, float x2, float y2);
    Ray2D(const Ray2D& other);

    Ray2D& operator=(const Ray2D& other);

    [[nodiscard]] Vector2 pointAt(float t) const;
    [[nodiscard]] bool contains(const Vector2& point) const;
    [[nodiscard]] bool intersects(const Ray2D& other) const;

    void serialize(Archive& a) const;
    void deserialize(Archive& a);

    //RayCasting
    Hit2D cast(Shape* shape) const {
        Hit2D hit{};

        shape->cast(*this, hit);

        return hit;
    }
};


#endif //POMEGRANATEENGINE_RAY2D_H
