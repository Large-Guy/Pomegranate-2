#include "vertex3d.h"

void Vertex3D::serialize(Archive &archive) {
    archive << position;
    /*archive << texCoord;
    archive << normal;
    archive << color;*/
}

void Vertex3D::deserialize(Archive &archive) {
    archive >> position;
    /*archive >> texCoord;
    archive >> normal;
    archive >> color;*/
}

VertexBindingInfo Vertex3D::getBindingInfo() {
    VertexBindingInfo bindingInfo{};
    bindingInfo.binding = 0;
    bindingInfo.stride = sizeof(Vertex3D);
    bindingInfo.offset = 0;
    bindingInfo.inputRate = InputRate::INPUT_RATE_VERTEX;

    return bindingInfo;
}

std::vector<VertexAttributeInfo> Vertex3D::getAttributeInfo() {
    std::vector<VertexAttributeInfo> attributeDescriptions(4);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].type = AttributeFormat::ATTRIBUTE_TYPE_FLOAT3;
    attributeDescriptions[0].offset = offsetof(Vertex3D, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].type = AttributeFormat::ATTRIBUTE_TYPE_FLOAT2;
    attributeDescriptions[1].offset = offsetof(Vertex3D, texCoord);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].type = AttributeFormat::ATTRIBUTE_TYPE_FLOAT3;
    attributeDescriptions[2].offset = offsetof(Vertex3D, normal);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].type = AttributeFormat::ATTRIBUTE_TYPE_FLOAT3;
    attributeDescriptions[3].offset = offsetof(Vertex3D, color);

    return attributeDescriptions;
}