#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include <vector3.h>
#include <vector2.h>
#include <serializable.h>

struct Vertex2D : public Serializable {
    Vector2 position = Vector2();
    Vector3 color = Vector3();
    Vector2 texCoords = Vector2();
    Vertex2D(Vector2 position = {0.0f}, Vector3 color = {1.0f}, Vector2 texCoords = {0.0f});

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};



#endif //POMEGRANATEENGINE_VERTEX2D_H
