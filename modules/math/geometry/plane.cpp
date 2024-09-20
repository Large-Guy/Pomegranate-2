#include "plane.h"

Plane::Plane() {
    normal = Vector3(0.0f, 1.0f, 0.0f);
}

Plane::Plane(Vector3 normal) {
    this->normal = normal;
}

Plane::Plane(float x, float y, float z) {
    normal = Vector3(x, y, z);
}

Plane::Plane(const Plane &other) {
    normal = other.normal;
}

Plane &Plane::operator=(const Plane &other) {
    normal = other.normal;
    return *this;
}

float Plane::distance(const Vector3 &point) const {
    return normal.dot(point);
}

bool Plane::contains(const Vector3 &point) const {
    return distance(point) == 0.0f;
}

bool Plane::intersects(const Plane &other) const {
    return normal.cross(other.normal).length() == 0.0f;
}

void Plane::serialize(Archive &arc) const {
    arc << normal;
}

void Plane::deserialize(Archive &arc) {
    arc >> normal;
}