#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include <array>
#include <math/math.h>
#include "enumerations.h"

struct Vertex2D {
    Vector2 position = {0.0,0.0};
    Vector2 texCoord = {0.0,0.0};
    Vector3 color = {1.0,1.0,1.0};

    static VertexBindingInfo getBindingInfo();
    static std::vector<VertexAttributeInfo> getAttributeInfo();

    void serialize(Archive& archive);
    void deserialize(Archive& archive);
};


#endif //POMEGRANATEENGINE_VERTEX2D_H
