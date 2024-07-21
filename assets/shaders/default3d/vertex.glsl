#version 460 core
layout (location = 0) in vec3 aPos;
uniform vec2 SCREEN_RESOLUTION;
void main()
{
    float aspect = SCREEN_RESOLUTION.x / SCREEN_RESOLUTION.y;
    gl_Position = vec4(aPos.x / aspect, aPos.y, aPos.z, 1.0);
}