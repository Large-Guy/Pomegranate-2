#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include "vector2.h"
#include "vector3.h"
#include <vulkan/vulkan.h>
#include <array>

struct Vertex2D {
    Vec2 position = {0.0,0.0};
    Vec2 texCoord = {0.0,0.0};
    Vec3 color = {1.0,1.0,1.0};

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription,3> getAttributeDescriptions();
};


#endif //POMEGRANATEENGINE_VERTEX2D_H
