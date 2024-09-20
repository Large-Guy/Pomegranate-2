#include "cube.h"

Cube::Cube() {
    position = Vector3();
    size = Vector3();
}

Cube::Cube(Vector3 position, Vector3 size) {
    this->position = position;
    this->size = size;
}

Cube::Cube(float x, float y, float z, float w, float h, float d) {
    position = Vector3(x, y, z);
    size = Vector3(w, h, d);
}

Cube &Cube::operator=(const Cube &other) {
    position = other.position;
    size = other.size;
    return *this;
}

Vector3 Cube::center() const {
    return this->position + (this->size * 0.5f);
}

Vector3 Cube::min() const {
    return this->position;
}

Vector3 Cube::max() const {
    return this->position + this->size;
}

bool Cube::contains(const Vector3 &point) const {
    return point.x >= this->position.x && point.x <= this->position.x + this->size.x &&
           point.y >= this->position.y && point.y <= this->position.y + this->size.y &&
           point.z >= this->position.z && point.z <= this->position.z + this->size.z;
}

bool Cube::intersects(const Cube &other) const {
    return this->position.x < other.position.x + other.size.x &&
           this->position.x + this->size.x > other.position.x &&
           this->position.y < other.position.y + other.size.y &&
           this->position.y + this->size.y > other.position.y &&
           this->position.z < other.position.z + other.size.z &&
           this->position.z + this->size.z > other.position.z;
}

void Cube::serialize(Archive &a) const {
    a << position << size;
}

void Cube::deserialize(Archive &a) {
    a >> position >> size;
}
