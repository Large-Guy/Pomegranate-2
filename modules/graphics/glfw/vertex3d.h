#ifndef POMEGRANATEENGINE_VERTEX_H
#define POMEGRANATEENGINE_VERTEX_H
#include <vector3.h>
#include <vector2.h>

struct Vertex3D {
    Vector3 position;
    Vector3 normal;
    Vector3 color;
    Vector2 texCoords;
    Vertex3D(Vector3 position, Vector3 normal = {0.0f}, Vector3 color = {1.0f}, Vector2 texCoords = {0.0f});
};


#endif //POMEGRANATEENGINE_VERTEX_H
