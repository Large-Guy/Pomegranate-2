#ifndef POMEGRANATEENGINE_SHAPE_H
#define POMEGRANATEENGINE_SHAPE_H
struct Ray2D;
struct Hit2D;
#include "math/vector2.h"

struct Shape {
    virtual ~Shape();

    [[nodiscard]] virtual bool contains(const Vector2& point) const;
    virtual void cast(const Ray2D& ray, Hit2D& hit) const;
    virtual void serialize(Archive& a) const;
    virtual void deserialize(Archive& a);
};


#endif //POMEGRANATEENGINE_SHAPE_H
