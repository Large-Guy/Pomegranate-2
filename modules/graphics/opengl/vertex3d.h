#ifndef POMEGRANATEENGINE_VERTEX3D_H
#define POMEGRANATEENGINE_VERTEX3D_H
#include <array>
#include <math/math.h>
#include "enumerations.h"

struct Vertex3D {
    Vector3 position = {0.0,0.0,0.0};
    Vector2 texCoord = {0.0,0.0};
    Vector3 normal = {0.0,0.0,0.0};
    Vector3 color = {1.0,1.0,1.0};

    static VertexBindingInfo getBindingInfo();
    static std::vector<VertexAttributeInfo> getAttributeInfo();

    void serialize(Archive& archive);
    void deserialize(Archive& archive);

    bool operator==(const Vertex3D& other) const {
        return position == other.position && texCoord == other.texCoord && normal == other.normal && color == other.color;
    }
};


#endif //POMEGRANATEENGINE_VERTEX3D_H
