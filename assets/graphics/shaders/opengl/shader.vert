#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fColor;

void main()
{
    gl_Position = projection * view * model * vec4(position.xyz, 1.0f);
    fColor = color;
}