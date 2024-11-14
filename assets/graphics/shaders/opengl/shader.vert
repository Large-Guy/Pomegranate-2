#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 color;

out vec3 fColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    fColor = color;
}