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

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(POSITION, 1.0);
    fragColor = COLOR;
}