#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
uniform vec2 SCREEN_RESOLUTION;
uniform mat3x3 MODEL_MATRIX;
uniform mat3x3 VIEW_MATRIX;
uniform float Z_INDEX;
out vec2 TexCoords;
void main()
{
    float aspect = SCREEN_RESOLUTION.x / SCREEN_RESOLUTION.y;
    
    
    vec2 pos = aPos;
    pos = (VIEW_MATRIX * (MODEL_MATRIX * vec3(pos, 1.0))).xy;
    
    gl_Position = vec4(pos.x, pos.y, Z_INDEX, 1.0);
    TexCoords = aTexCoords;
}