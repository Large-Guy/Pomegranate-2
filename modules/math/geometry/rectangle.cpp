#include "rectangle.h"

Rectangle::Rectangle() {
    position = Vector2();
    size = Vector2();
}

Rectangle::Rectangle(Vector2 position, Vector2 size) {
    this->position = position;
    this->size = size;
}

Rectangle::Rectangle(const Rectangle &other) {
    position = other.position;
    size = other.size;
}

Rectangle &Rectangle::operator=(const Rectangle &other) {
    position = other.position;
    size = other.size;
    return *this;
}

Vector2 Rectangle::center() const {
    return this->position + (this->size * 0.5f);
}

Vector2 Rectangle::min() const {
    return this->position;
}

Vector2 Rectangle::max() const {
    return this->position + this->size;
}

bool Rectangle::contains(const Vector2& point) const {
    return point.x >= this->position.x && point.x <= this->position.x + this->size.x &&
           point.y >= this->position.y && point.y <= this->position.y + this->size.y;
}

bool Rectangle::intersects(const Rectangle &other) const {
    return this->position.x < other.position.x + other.size.x &&
           this->position.x + this->size.x > other.position.x &&
           this->position.y < other.position.y + other.size.y &&
           this->position.y + this->size.y > other.position.y;
}

float Rectangle::area() const {
    return this->size.x * this->size.y;
}

void Rectangle::cast(const Ray2D &ray, Hit2D &hit) const {
    hit.hit = false;

    float tmin = (this->position.x - ray.origin.x) / ray.direction.x;
    float tmax = (this->position.x + this->size.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) {
        std::swap(tmin, tmax);
    }

    float tymin = (this->position.y - ray.origin.y) / ray.direction.y;
    float tymax = (this->position.y + this->size.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) {
        std::swap(tymin, tymax);
    }

    if (tmin > tymax || tymin > tmax) {
        return;
    }

    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    hit.hit = true;
    hit.point = ray.pointAt(tmin);
    hit.distance = tmin;
    hit.normal = Vector2();
}

void Rectangle::serialize(Archive &a) const {
    a << position << size;
}

void Rectangle::deserialize(Archive &a) {
    a >> position >> size;
}
