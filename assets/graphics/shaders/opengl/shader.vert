#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 color;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fColor;

void main()
{
    vec3 pos = vec3(position.x, position.y + (sin(position.x+time) + sin(position.z+time))*0.2, position.z);
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0f);
    fColor = normal;
}