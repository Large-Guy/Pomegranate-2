#include "sphere.h"

Sphere::Sphere() {
    position = Vector3();
    radius = 0.0f;
}

Sphere::Sphere(Vector3 position, float radius) {
    this->position = position;
    this->radius = radius;
}

Sphere::Sphere(float x, float y, float z, float radius) {
    position = Vector3(x, y, z);
    this->radius = radius;
}

Sphere::Sphere(const Sphere &other) {
    position = other.position;
    radius = other.radius;
}

Sphere &Sphere::operator=(const Sphere &other) {
    position = other.position;
    radius = other.radius;
    return *this;
}

bool Sphere::contains(const Vector3 &point) const {
    return (point - position).length() <= radius;
}

bool Sphere::intersects(const Sphere &other) const {
    return (position - other.position).length() <= radius + other.radius;
}

void Sphere::serialize(Archive &a) const {
    a << position << radius;
}

void Sphere::deserialize(Archive &a) {
    a >> position >> radius;
}