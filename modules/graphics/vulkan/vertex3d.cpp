#include "vertex3d.h"

VkVertexInputBindingDescription Vertex3D::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex3D);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> Vertex3D::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex3D, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex3D, texCoord);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex3D, normal);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(Vertex3D, color);

    return attributeDescriptions;
}

void Vertex3D::serialize(Archive &archive) {
    archive << position;
    archive << texCoord;
    archive << normal;
    archive << color;
}

void Vertex3D::deserialize(Archive &archive) {
    archive >> position;
    archive >> texCoord;
    archive >> normal;
    archive >> color;
}