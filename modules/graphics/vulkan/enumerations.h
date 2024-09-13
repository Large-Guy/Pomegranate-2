#ifndef POMEGRANATEENGINE_ENUMERATIONS_H
#define POMEGRANATEENGINE_ENUMERATIONS_H
#include <vulkan/vulkan.h>

enum RenderMode {
    RENDER_MODE_FILL = VK_POLYGON_MODE_FILL,
    RENDER_MODE_WIRE_FRAME = VK_POLYGON_MODE_LINE,
    RENDER_MODE_POINTS = VK_POLYGON_MODE_POINT
};

enum CullMode {
    CULL_MODE_NONE = VK_CULL_MODE_NONE,
    CULL_MODE_FRONT = VK_CULL_MODE_FRONT_BIT,
    CULL_MODE_BACK = VK_CULL_MODE_BACK_BIT,
    CULL_MODE_BOTH = VK_CULL_MODE_FRONT_AND_BACK
};

enum BufferType {
    BUFFER_TYPE_NONE = 0,
    BUFFER_TYPE_VERTEX = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    BUFFER_TYPE_INDEX = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    BUFFER_TYPE_UNIFORM = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
};

struct RenderInfo {
    RenderMode renderMode = RENDER_MODE_FILL;
    CullMode cullMode = CULL_MODE_BACK;
};

#endif //POMEGRANATEENGINE_ENUMERATIONS_H
