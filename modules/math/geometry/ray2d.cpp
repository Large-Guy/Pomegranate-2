#include "ray2d.h"

Ray2D::Ray2D() {
    origin = Vector2();
    direction = Vector2();
}

Ray2D::Ray2D(Vector2 origin, Vector2 direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray2D::Ray2D(float x1, float y1, float x2, float y2) {
    origin = Vector2(x1, y1);
    direction = Vector2(x2, y2);
}

Ray2D::Ray2D(const Ray2D &other) {
    origin = other.origin;
    direction = other.direction;
}

Ray2D &Ray2D::operator=(const Ray2D &other) {
    origin = other.origin;
    direction = other.direction;
    return *this;
}

Vector2 Ray2D::pointAt(float t) const {
    return origin + direction * t;
}

bool Ray2D::contains(const Vector2 &point) const {
    return (point - origin).length() + (point - (origin + direction)).length() == direction.length();
}

bool Ray2D::intersects(const Ray2D &other) const {
    return contains(other.origin) || contains(other.origin + other.direction);
}

void Ray2D::serialize(Archive &a) const {
    a << origin << direction;
}

void Ray2D::deserialize(Archive &a) {
    a >> origin >> direction;
}