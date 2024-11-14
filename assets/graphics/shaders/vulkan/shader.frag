#version 450

layout(location = 0) in vec3 FRAG_POSITION;
layout(location = 1) in vec2 FRAG_TEXCOORD;
layout(location = 2) in vec3 FRAG_COLOR;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(FRAG_COLOR, 1.0);
}