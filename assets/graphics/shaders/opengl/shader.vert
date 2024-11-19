#version 410 core
layout(location = 0) in vec3 VERTEX;
layout(location = 1) in vec2 VERTEX_TEXCOORD;
layout(location = 2) in vec3 VERTEX_NORMAL;
layout(location = 3) in vec3 VERTEX_COLOR;

uniform float time;uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FRAGMENT_POSITION;
out vec3 FRAGMENT_COLOR;
out vec3 FRAGMENT_NORMAL;
out vec2 FRAGMENT_TEXCOORD;
out vec3 FRAGMENT_VIEW_DIR;

void main()
{
    //Extract the camera position from the view matrix
    vec3 cameraPosition = vec3(inverse(view)[3]);

    vec3 pos = vec3(VERTEX.x, VERTEX.y, VERTEX.z);
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0f);
    FRAGMENT_COLOR = VERTEX_COLOR;
    FRAGMENT_NORMAL = VERTEX_NORMAL;
    FRAGMENT_TEXCOORD = VERTEX_TEXCOORD;
    FRAGMENT_VIEW_DIR = normalize(cameraPosition - pos);
    FRAGMENT_POSITION = pos;
}
