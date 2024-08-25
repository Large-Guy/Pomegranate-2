#ifndef POMEGRANATEENGINE_VERTEX_H
#define POMEGRANATEENGINE_VERTEX_H
#include <math/vector3.h>
#include <math/vector2.h>
#include <core/serializable.h>

struct Vertex3D : public Serializable {
    Vector3 position = Vector3();
    Vector3 normal = Vector3();
    Vector3 color = Vector3();
    Vector2 texCoords = Vector2();
    Vertex3D(Vector3 position = {0.0f}, Vector3 normal = {0.0f}, Vector3 color = {1.0f}, Vector2 texCoords = {0.0f});

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};


#endif //POMEGRANATEENGINE_VERTEX_H
