#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include "vector2.h"
#include "vector3.h"
#include <vulkan/vulkan.h>
#include <array>

struct Vertex2D {
    Vec2 position;
    Vec2 texCoord;
    Vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription,3> getAttributeDescriptions();
};


#endif //POMEGRANATEENGINE_VERTEX2D_H
