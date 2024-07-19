#ifndef POMEGRANATEENGINE_VERTEX_H
#define POMEGRANATEENGINE_VERTEX_H
#include <vector3.h>
#include <vector2.h>
#include <serializable.h>

struct Vertex3D : public Serializable {
    Vector3 position;
    Vector3 normal;
    Vector3 color;
    Vector2 texCoords;
    Vertex3D(Vector3 position = {0.0f}, Vector3 normal = {0.0f}, Vector3 color = {1.0f}, Vector2 texCoords = {0.0f});

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_VERTEX_H
