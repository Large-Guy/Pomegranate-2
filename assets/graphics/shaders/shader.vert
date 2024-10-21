#version 450

layout(binding = 0) uniform Perspective {
    mat4 model;
    mat4 view;
    mat4 projection;
} perspective;

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec2 TEXCOORD;
layout(location = 2) in vec3 NORMAL;
layout(location = 3) in vec3 COLOR;

layout(location = 0) out vec3 FRAG_POSITION;
layout(location = 1) out vec2 FRAG_TEXCOORD;
layout(location = 2) out vec3 FRAG_COLOR;

void main() {
    gl_Position = /*perspective.projection * perspective.view * perspective.model * */vec4(POSITION, 1.0);
    FRAG_POSITION = POSITION;
    FRAG_TEXCOORD = TEXCOORD;
    FRAG_COLOR = COLOR;
}