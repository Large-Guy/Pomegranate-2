#version 410 core

out vec4 COLOR;

in vec3 FRAGMENT_COLOR;
in vec3 FRAGMENT_NORMAL;
in vec2 FRAGMENT_TEXCOORD;
in vec3 FRAGMENT_VIEW_DIR;

void main()
{
    COLOR = vec4(FRAGMENT_COLOR, 1.0);

    //Basic phong shading
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(FRAGMENT_NORMAL, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 ambient = vec3(0.1, 0.1, 0.1);

    COLOR = vec4(diffuse+ambient, 1.0) * COLOR;
}