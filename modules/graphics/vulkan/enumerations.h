#ifndef POMEGRANATEENGINE_ENUMERATIONS_H
#define POMEGRANATEENGINE_ENUMERATIONS_H
#include <vulkan/vulkan.h>

enum RenderMode {
    Fill = VK_POLYGON_MODE_FILL,
    WireFrame = VK_POLYGON_MODE_LINE,
    Points = VK_POLYGON_MODE_POINT
};

enum CullMode {
    None = VK_CULL_MODE_NONE,
    Front = VK_CULL_MODE_FRONT_BIT,
    Back = VK_CULL_MODE_BACK_BIT,
    Both = VK_CULL_MODE_FRONT_AND_BACK
};

enum BufferType {
    VertexBuffer = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    IndexBuffer = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    UniformBuffer = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
};

struct RenderInfo {
    RenderMode renderMode = Fill;
    CullMode cullMode = Back;
};

#endif //POMEGRANATEENGINE_ENUMERATIONS_H
