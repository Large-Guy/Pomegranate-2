#include "line2d.h"

Line2D::Line2D() {
    this->a = Vector2();
    this->b = Vector2();
}

Line2D::Line2D(Vector2 a, Vector2 b) {
    this->a = a;
    this->b = b;
}

Line2D::Line2D(float x1, float y1, float x2, float y2) {
    this->a = Vector2(x1, y1);
    this->b = Vector2(x2, y2);
}

Line2D::Line2D(const Line2D &other) {
    this->a = other.a;
    this->b = other.b;
}

Line2D &Line2D::operator=(const Line2D &other) {
    this->a = other.a;
    this->b = other.b;
    return *this;
}

Vector2 Line2D::direction() const {
    return (b - a).normalize();
}

Vector2 Line2D::normal() const {
    return Vector2(-(b - a).y, (b - a).x).normalize();
}

bool Line2D::contains(const Vector2 &point) const {
    return (point - a).length() + (point - b).length() == (b - a).length();
}

bool Line2D::intersects(const Line2D &other) const {
    return contains(other.a) || contains(other.b);
}

void Line2D::cast(const Ray2D &ray, Hit2D &hit) const {
    hit.hit = false;

    Vector2 lineDir = b - a;
    Vector2 rayDir = ray.direction;

    float determinant = lineDir.x * rayDir.y - lineDir.y * rayDir.x;

    if (fabsf(determinant) < 1e-6f) {
        return;
    }

    Vector2 originDiff = ray.origin - a;
    float s = (originDiff.x * lineDir.y - originDiff.y * lineDir.x) / determinant;

    if (s >= 0) {
        hit.hit = true;
        hit.point = ray.origin + ray.direction * s;
        hit.distance = s;
        //Calculate the normal of the line and adjust based on the ray dir later
        Vector2 d = normal();
        if (d.dot(rayDir) > 0) {
            d = d * -1;
        }
        hit.normal = d;
    }
}

void Line2D::serialize(Archive &arc) const {
    arc << a << b;
}

void Line2D::deserialize(Archive &arc) {
    arc >> a >> b;
}