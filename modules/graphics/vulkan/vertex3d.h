#ifndef POMEGRANATEENGINE_VERTEX3D_H
#define POMEGRANATEENGINE_VERTEX3D_H
#include <vulkan/vulkan.h>
#include <array>
#include <math/math.h>

struct Vertex3D {
    Vector3 position = {0.0,0.0,0.0};
    Vector2 texCoord = {0.0,0.0};
    Vector3 normal = {0.0,0.0,0.0};
    Vector3 color = {1.0,1.0,1.0};

    static VkVertexInputBindingDescription getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

    void serialize(Archive& archive);
    void deserialize(Archive& archive);
};


#endif //POMEGRANATEENGINE_VERTEX3D_H
