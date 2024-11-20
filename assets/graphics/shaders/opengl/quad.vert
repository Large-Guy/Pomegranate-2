#version 410 core
layout(location = 0) in vec3 VERTEX;
layout(location = 1) in vec2 VERTEX_TEXCOORD;
layout(location = 2) in vec3 VERTEX_NORMAL;
layout(location = 3) in vec3 VERTEX_COLOR;

out vec3 FRAGMENT_POSITION;
out vec3 FRAGMENT_COLOR;
out vec3 FRAGMENT_NORMAL;
out vec2 FRAGMENT_TEXCOORD;
out vec3 FRAGMENT_VIEW_DIR;

void main()
{
    gl_Position = vec4(VERTEX, 1.0);
    FRAGMENT_POSITION = VERTEX;
    FRAGMENT_COLOR = VERTEX_COLOR;
    FRAGMENT_NORMAL = VERTEX_NORMAL;
    FRAGMENT_TEXCOORD = VERTEX_TEXCOORD;
    FRAGMENT_VIEW_DIR = normalize(-VERTEX);
}
