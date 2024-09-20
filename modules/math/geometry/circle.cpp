#include "circle.h"

Circle::Circle() {
    position = Vector2();
    radius = 0.0f;
}

Circle::Circle(Vector2 position, float radius) {
    this->position = position;
    this->radius = radius;
}

Circle::Circle(float x, float y, float radius) {
    position = Vector2(x, y);
    this->radius = radius;
}

Circle::Circle(const Circle &other) {
    position = other.position;
    radius = other.radius;
}

Circle &Circle::operator=(const Circle &other) {
    position = other.position;
    radius = other.radius;
    return *this;
}

bool Circle::contains(const Vector2 &point) const {
    return (point - position).length() <= radius;
}

bool Circle::intersects(const Circle &other) const {
    return (position - other.position).length() <= radius + other.radius;
}

void Circle::cast(const Ray2D &ray, Ray2D::Hit &hit) const {
    hit.hit = false;

    Vector2 oc = ray.origin - position;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
        return;
    }

    float t = (-b - sqrt(discriminant)) / (2.0f * a);

    if (t < 0.0f) {
        return;
    }

    hit.hit = true;
    hit.point = ray.pointAt(t);
    hit.distance = t;
    hit.normal = (hit.point - position).normalized();
}

void Circle::serialize(Archive &a) const {
    a << position << radius;
}

void Circle::deserialize(Archive &a) {
    a >> position >> radius;
}
