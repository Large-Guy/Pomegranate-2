#include "segment2d.h"

Segment2D::Segment2D() {
    start = Vector2();
    end = Vector2();
}

Segment2D::Segment2D(Vector2 start, Vector2 end) {
    this->start = start;
    this->end = end;
}

Segment2D::Segment2D(float x1, float y1, float x2, float y2) {
    start = Vector2(x1, y1);
    end = Vector2(x2, y2);
}

Segment2D::Segment2D(const Segment2D &other) {
    start = other.start;
    end = other.end;
}

Segment2D &Segment2D::operator=(const Segment2D &other) {
    start = other.start;
    end = other.end;
    return *this;
}

Vector2 Segment2D::direction() const {
    return end - start;
}

Vector2 Segment2D::center() const {
    return (start + end) / 2.0f;
}

float Segment2D::length() const {
    return (end - start).length();
}

Vector2 Segment2D::pointAt(float t) const {
    return start + direction() * t;
}

bool Segment2D::contains(const Vector2 &point) const {
    return (point - start).length() + (point - end).length() == (start - end).length();
}

bool Segment2D::intersects(const Segment2D &other) const {
    return contains(other.start) || contains(other.end);
}

void Segment2D::cast(const Ray2D &ray, Ray2D::Hit &hit) const {
    hit.hit = false;

    Vector2 segDir = end - start;
    Vector2 rayDir = ray.direction;

    float determinant = segDir.x * rayDir.y - segDir.y * rayDir.x;

    if (fabs(determinant) < 1e-6f) {
        return;
    }

    Vector2 originDiff = ray.origin - start;
    float t = (originDiff.x * rayDir.y - originDiff.y * rayDir.x) / determinant;
    float s = (originDiff.x * segDir.y - originDiff.y * segDir.x) / determinant;

    if (s >= 0 && t >= 0 && t <= 1) {
        hit.hit = true;
        hit.point = ray.origin + ray.direction * s;
        hit.distance = s;
    }
}

void Segment2D::serialize(Archive &a) const {
    a << start << end;
}

void Segment2D::deserialize(Archive &a) {
    a >> start >> end;
}