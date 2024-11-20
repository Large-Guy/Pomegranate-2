#include "line3d.h"

Line3D::Line3D() {
    a = Vector3();
    b = Vector3();
}

Line3D::Line3D(Vector3 a, Vector3 b) {
    this->a = a;
    this->b = b;
}

Line3D::Line3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    a = Vector3(x1, y1, z1);
    b = Vector3(x2, y2, z2);
}

Line3D::Line3D(const Line3D &other) {
    a = other.a;
    b = other.b;
}

Line3D &Line3D::operator=(const Line3D &other) {
    a = other.a;
    b = other.b;
    return *this;
}

Vector3 Line3D::direction() const {
    return (b - a).normalize();
}

bool Line3D::contains(const Vector3 &point) const {
    return (point - a).cross(point - b).length() == 0.0f;
}

bool Line3D::intersects(const Line3D &other) const {
    return contains(other.a) || contains(other.b);
}

void Line3D::serialize(Archive &arc) const {
    arc << a << b;
}

void Line3D::deserialize(Archive &arc) {
    arc >> a >> b;
}