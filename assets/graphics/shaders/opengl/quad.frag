#version 410 core

out vec4 COLOR;

//Texture
uniform sampler2D image;

in vec3 FRAGMENT_POSITION;
in vec3 FRAGMENT_COLOR;
in vec3 FRAGMENT_NORMAL;
in vec2 FRAGMENT_TEXCOORD;
in vec3 FRAGMENT_VIEW_DIR;

void main()
{
    vec3 color = texture(image, FRAGMENT_TEXCOORD).rgb;
    COLOR = vec4(color, 1.0);
}