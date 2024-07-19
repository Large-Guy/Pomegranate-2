#include "vertex3d.h"

Vertex3D::Vertex3D(Vector3 position, Vector3 normal, Vector3 color, Vector2 texCoords) {
    this->position = position;
    this->normal = normal;
    this->color = color;
    this->texCoords = texCoords;
}