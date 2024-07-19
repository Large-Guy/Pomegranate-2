#include "vertex2d.h"

Vertex2D::Vertex2D(Vector2 position, Vector3 color, Vector2 texCoords) {
    this->position = position;
    this->color = color;
    this->texCoords = texCoords;
}

void Vertex2D::serialize(Archive &a) const {
    a << position << color << texCoords;
}

void Vertex2D::deserialize(Archive &a) {
    a >> &position >> &color >> &texCoords;
}
