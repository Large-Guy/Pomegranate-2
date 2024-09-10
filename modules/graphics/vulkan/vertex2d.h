#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include "vector2.h"
#include "vector3.h"
#include <vulkan/vulkan.h>

struct Vertex2D {
    Vec2 position;
    Vec2 texCoord;
    Vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static VkVertexInputAttributeDescription* getAttributeDescriptions();
};


#endif //POMEGRANATEENGINE_VERTEX2D_H
