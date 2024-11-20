#include "triangle3d.h"

Triangle3D::Triangle3D() {
    a = Vector3();
    b = Vector3();
    c = Vector3();
}

Triangle3D::Triangle3D(Vector3 a, Vector3 b, Vector3 c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Triangle3D::Triangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    a = Vector3(x1, y1, z1);
    b = Vector3(x2, y2, z2);
    c = Vector3(x3, y3, z3);
}

Triangle3D::Triangle3D(const Triangle3D &other) {
    a = other.a;
    b = other.b;
    c = other.c;
}

Triangle3D &Triangle3D::operator=(const Triangle3D &other) {
    a = other.a;
    b = other.b;
    c = other.c;
    return *this;
}

Vector3 Triangle3D::normal() const {
    return (b - a).cross(c - a).normalize();
}

Vector3 Triangle3D::center() const {
    return (a + b + c) / 3.0f;
}

float Triangle3D::area() const {
    return (b - a).cross(c - a).length() / 2.0f;
}

bool Triangle3D::contains(const Vector3 &point) const {
    Vector3 n = normal();
    Vector3 ab = b - a;
    Vector3 bc = c - b;
    Vector3 ca = a - c;
    Vector3 ap = point - a;
    Vector3 bp = point - b;
    Vector3 cp = point - c;
    return n.dot(ab.cross(ap)) >= 0.0f && n.dot(bc.cross(bp)) >= 0.0f && n.dot(ca.cross(cp)) >= 0.0f;
}

bool Triangle3D::intersects(const Triangle3D &other) const {
    return contains(other.a) || contains(other.b) || contains(other.c);
}

void Triangle3D::serialize(Archive &arc) const {
    arc << a << b << c;
}

void Triangle3D::deserialize(Archive &arc) {
    arc >> a >> b >> c;
}