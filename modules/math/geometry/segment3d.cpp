#include "segment3d.h"

Segment3D::Segment3D() {
    start = Vector3();
    end = Vector3();
}

Segment3D::Segment3D(Vector3 start, Vector3 end) {
    this->start = start;
    this->end = end;
}

Segment3D::Segment3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    start = Vector3(x1, y1, z1);
    end = Vector3(x2, y2, z2);
}

Segment3D::Segment3D(const Segment3D &other) {
    start = other.start;
    end = other.end;
}

Segment3D &Segment3D::operator=(const Segment3D &other) {
    start = other.start;
    end = other.end;
    return *this;
}

Vector3 Segment3D::direction() const {
    return end - start;
}

Vector3 Segment3D::center() const {
    return (start + end) / 2.0f;
}

float Segment3D::length() const {
    return (end - start).length();
}

Vector3 Segment3D::pointAt(float t) const {
    return start + direction() * t;
}

bool Segment3D::contains(const Vector3 &point) const {
    return (point - start).length() + (point - end).length() <= length();
}

bool Segment3D::intersects(const Segment3D &other) const {
    Vector3 a = direction();
    Vector3 b = other.direction();
    Vector3 c = other.start - start;
    float d = a.cross(b).length();
    if (d == 0.0f) {
        return false;
    }
    float t = c.cross(b).length() / d;
    float u = c.cross(a).length() / d;
    return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
}

void Segment3D::serialize(Archive &a) const {
    a << start << end;
}

void Segment3D::deserialize(Archive &a) {
    a >> start >> end;
}