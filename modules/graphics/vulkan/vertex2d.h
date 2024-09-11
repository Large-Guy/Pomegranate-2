#ifndef POMEGRANATEENGINE_VERTEX2D_H
#define POMEGRANATEENGINE_VERTEX2D_H
#include <vulkan/vulkan.h>
#include <array>
#include <math/math.h>

struct Vertex2D {
    Vector2 position = {0.0,0.0};
    Vector2 texCoord = {0.0,0.0};
    Vector3 color = {1.0,1.0,1.0};

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription,3> getAttributeDescriptions();

    void serialize(Archive& archive);
    void deserialize(Archive& archive);
};


#endif //POMEGRANATEENGINE_VERTEX2D_H
