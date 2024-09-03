#include "rect.h"

Rect2D::Rect2D() {
    position = Vector2();
    size = Vector2();
}

Rect2D::Rect2D(Vector2 position, Vector2 size) {
    this->position = position;
    this->size = size;
}

Rect2D::Rect2D(float x, float y, float w, float h) {
    position = Vector2(x, y);
    size = Vector2(w, h);
}

Rect2D::Rect2D(const Rect2D &other) {
    position = other.position;
    size = other.size;
}

Rect2D &Rect2D::operator=(const Rect2D &other) {
    position = other.position;
    size = other.size;
    return *this;
}

Vector2 Rect2D::center() const {
    return this->position + (this->size * 0.5f);
}

Vector2 Rect2D::min() const {
    return this->position;
}

Vector2 Rect2D::max() const {
    return this->position + this->size;
}

bool Rect2D::contains(const Vector2& point) const {
    return point.x >= this->position.x && point.x <= this->position.x + this->size.x &&
           point.y >= this->position.y && point.y <= this->position.y + this->size.y;
}

bool Rect2D::intersects(const Rect2D &other) const {
    return this->position.x < other.position.x + other.size.x &&
           this->position.x + this->size.x > other.position.x &&
           this->position.y < other.position.y + other.size.y &&
           this->position.y + this->size.y > other.position.y;
}

void Rect2D::serialize(Archive &a) const {
    a << position << size;
}

void Rect2D::deserialize(Archive &a) {
    a >> &position >> &size;
}
