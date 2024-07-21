#version 460 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D TEXTURE;
void main()
{
    FragColor.xy = TexCoords;
    FragColor.zw = vec2(0.0, 1.0);
    FragColor = texture(TEXTURE, TexCoords);
}