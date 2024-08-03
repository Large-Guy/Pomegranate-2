#include "rect.h"

Rect::Rect() {
    position = Vector2();
    size = Vector2();
}

Rect::Rect(Vector2 position, Vector2 size) {
    this->position = position;
    this->size = size;
}

Rect::Rect(float x, float y, float w, float h) {
    position = Vector2(x, y);
    size = Vector2(w, h);
}

Vector2 Rect::center() const {
    return this->position + (this->size * 0.5f);
}

Vector2 Rect::min() const {
    return this->position;
}

Vector2 Rect::max() const {
    return this->position + this->size;
}

bool Rect::contains(const Vector2& point) const {
    return point.x >= this->position.x && point.x <= this->position.x + this->size.x &&
           point.y >= this->position.y && point.y <= this->position.y + this->size.y;
}

bool Rect::intersects(const Rect &other) const {
    return this->position.x < other.position.x + other.size.x &&
           this->position.x + this->size.x > other.position.x &&
           this->position.y < other.position.y + other.size.y &&
           this->position.y + this->size.y > other.position.y;
}

void Rect::serialize(Archive &a) const {
    a << position << size;
}

void Rect::deserialize(Archive &a) {
    a >> &position >> &size;
}
